#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cgic207/cgic.h"
#include "/usr/include/mysql/mysql.h"

MYSQL_RES *executeQuery(char *sql)
{
    MYSQL *mysql = mysql_init(NULL);
    if (!mysql_real_connect(mysql, "localhost", "root", "123456", "zhanghaoliang", 0, NULL, 0))
    {
        goto error;
    }
    if (mysql_query(mysql, "set names utf8")) //返回1 是代表失败
    {
        goto error;
    }
    if (mysql_query(mysql, sql))
    {
        goto error;
    }
    MYSQL_RES *result = mysql_store_result(mysql);
    if (result == NULL)
        return NULL;
    mysql_close(mysql);
    return result;
error:
    mysql_close(mysql);
    return NULL;
exit:
    mysql_close(mysql);
    return NULL;
}

void PrintErrorGet(char *msg, char *username, char *password)
{
    cgiHeaderContentType("text/html;charset=utf-8");
    fprintf(cgiOut, "<html><head><title>注册失败</title></head><body>");
    fprintf(cgiOut, "<form action='http://127.0.0.1:4000/cgiGet.cgi' method='GET'>");
    fprintf(cgiOut, "<table>");
    fprintf(cgiOut, "<tr>");
    fprintf(cgiOut, "<td>用户名:</td><td><input type='text' name='username' placeholder='用户名' value='%s'></td></tr>", username);
    fprintf(cgiOut, "<tr><td>密码:</td><td><input type='password' name='password' placeholder='密码' value='%s'></td></tr>", password);
    fprintf(cgiOut, "<tr><td><input type='submit' name='get' value='注册'></td></tr>");
    fprintf(cgiOut, "<font color='red'>%s</font>", msg);
    fprintf(cgiOut, "</table></form></body></html>");
}
int cgiMain(void)
{
    char Username[256] = {0};
    char Password[256] = {0};
    if (cgiFormString("username", Username, sizeof(Username)) != cgiFormSuccess)
    {
        PrintErrorGet("用户名没有填写", Username, Password);
        return 0;
    }
    if (cgiFormString("password", Password, sizeof(Password)) != cgiFormSuccess)
    {
        PrintErrorGet("密码没有填写", Username, Password);
        return 0;
    }

    char sql[256] = {0};
    sprintf(sql, "select *from T_Users where Username='%s';", Username);
    MYSQL_RES *res = executeQuery(sql);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row != NULL)
    {
        PrintErrorGet("该用户名已经被其他用户注册过了", Username, Password);
        mysql_free_result(res);
        return 0;
    }
    else
    {
        bzero(sql, sizeof(sql));
        sprintf(sql, "insert into T_Users(Username,Password) values('%s','%s');", Username, Password);
        MYSQL_RES *result = executeQuery(sql);
        if (res == NULL)
        {
            PrintErrorGet("数据库执行sql语句出错", Username, Password);
            return 0;
        }
        else
        {
            cgiHeaderContentType("text/html;charset=utf-8");
            fprintf(cgiOut, "<html><head><title>注册成功</title></head><body>");
            fprintf(cgiOut, "注册成功 快去登录看看吧");
            fprintf(cgiOut, "</body></html>");
            return 0;
        }
    }
    return 0;
}
