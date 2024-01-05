#include <iostream>
#include <mysql/mysql.h>

const char *host = "127.0.0.1";
const char *user = "root";
const char *password = "1111";
const char *database_name = "cplusplus";
const int port = 3306;

struct Student {
    int id;
    const char *name;
    const char *sex;
};

int main(int argc, char **argv)
{
    MYSQL *con = mysql_init(NULL);

    mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");

    if(!mysql_real_connect(con, host, user, password, database_name, port, NULL, 0)) {
        fprintf(stderr, "failed to connect to database: Error: %s\n", mysql_error(con));
        return -1;
    }

    Student stu;
    stu.id = 2;
    stu.name = "xiaohong";
    stu.sex = "g";

    char sql[1024];
    // 这里必须使用''进行引用
    sprintf(sql, "insert into student(id, name, sex) values(%d, '%s', '%s')", stu.id, stu.name, stu.sex);

    if(mysql_query(con, sql)) {
        fprintf(stderr, "Failed to insert data: Error:%s\n", mysql_error(con));
        return -1;
    }

    return 0;
}
