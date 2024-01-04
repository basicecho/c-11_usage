#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadsGuard {
public:
    ThreadsGuard(std::vector<std::thread> &threads)
        :m_threads(threads) {}
    ~ThreadsGuard()
    {
        for(size_t i = 0; i < m_threads.size(); i++) {
            if(m_threads[i].joinable())
                m_threads[i].join();
        }
    }

private:
    ThreadsGuard(ThreadsGuard &&) = delete;
    ThreadsGuard(const ThreadsGuard &) = delete;
    ThreadsGuard &operator=(ThreadsGuard &&) = delete;
    ThreadsGuard &operator=(const ThreadsGuard &) = delete;

private:
    std::vector<std::thread>& m_threads;
};

class ThreadPool {
public:
    typedef std::function<void()> task_type;

public:
    explicit ThreadPool(int n = 0):
        m_stop(false),
        m_threadsGuard(m_threads) // 自己给自己赋值？
    {
        int nthreads = n;
        if(nthreads <= 0) {
            nthreads = std::thread::hardware_concurrency();
            nthreads = (nthreads == 0 ? 2 : nthreads);
        }

        for(size_t i = 0; i < nthreads; i++) {
            m_threads.emplace_back(std::thread([this] {
                while(!m_stop.load(std::memory_order_acquire)) {
                    task_type task;
                    {
                        std::unique_lock<std::mutex> lock(this->m_mtx);
                        this->m_cond.wait(lock, [this]{
                            return m_stop.load(std::memory_order_acquire) || !this->m_tasks.empty();
                        });
                        if(m_stop.load(std::memory_order_acquire))
                            return ;
                        task = std::move(this->m_tasks.front());
                        this->m_tasks.pop();
                    }
                    task();
                }
            }));
        }
    }

    template<class Function, class ...Args>
    std::future<typename std::result_of<Function(Args...)>::type> add(Function &&fcn, Args &&...args)
    {
        typedef typename std::result_of<Function(Args...)>::type return_type;
        typedef std::packaged_task<return_type()> task;

        std::shared_ptr<task> t = std::make_shared<task>(std::bind(std::forward<Function>(fcn), std::forward<Args>(args)...));
        std::future<return_type> result = t->get_future();

        {
            std::lock_guard<std::mutex> lock(m_mtx);
            if(m_stop.load(std::memory_order_acquire))
                throw std::runtime_error("thread pool has stopped");
            m_tasks.emplace([t]{(*t)();});
        }
        m_cond.notify_one();

        return result;
    }

private:
    std::atomic<bool> m_stop;
    std::mutex m_mtx;
    std::condition_variable m_cond;

    std::queue<task_type> m_tasks;
    std::vector<std::thread> m_threads;
    ThreadsGuard m_threadsGuard;
};

#endif