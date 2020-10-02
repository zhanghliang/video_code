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
    mysql_close(mysql);
    return result;
error:
    printf("mysql执行出错 %s", mysql_error(mysql));
exit:
    mysql_close(mysql);
    return NULL;
}

void PrintErrorLogin(char *msg, char *username, char *password)
{
    cgiHeaderContentType("text/html;charset=utf-8");
    fprintf(cgiOut, "<html><head><title>登录失败</title></head><body>");
    fprintf(cgiOut, "<form action='http://127.0.0.1:4000/cgiLogin.cgi' method='GET'>");
    fprintf(cgiOut, "<table>");
    fprintf(cgiOut, "<tr>");
    fprintf(cgiOut, "<td>用户名:</td><td><input type='text' name='username' placeholder='用户名' value='%s'></td></tr>", username);
    fprintf(cgiOut, "<tr><td>密码:</td><td><input type='password' name='password' placeholder='密码' value='%s'></td></tr>", password);
    fprintf(cgiOut, "<tr><td><input type='submit' name='login' value='登录'></td><td><input type='submit' name='get' value='注册'></td></tr>");
    fprintf(cgiOut, "<font color='red'>%s</font>", msg);
    fprintf(cgiOut, "</table></form></body></html>");
}

int cgiMain(void)
{
    char login[256] = {0};
    char get[256] = {0};
    if (cgiFormString("login", login, sizeof(login)) != cgiFormSuccess && cgiFormString("get", get, sizeof(get)) != cgiFormSuccess)
    {
        cgiHeaderContentType("text/html;charset=utf-8");
        fprintf(cgiOut, "<html><head><title>登录</title></head><body>");
        fprintf(cgiOut, "<form action='http://127.0.0.1:4000/cgiLogin.cgi' method='GET'>");
        fprintf(cgiOut, "<table>");
        fprintf(cgiOut, "<tr>");
        fprintf(cgiOut, "<td>用户名:</td><td><input type='text' name='username' placeholder='用户名'></td>");
        fprintf(cgiOut, "</tr>");
        fprintf(cgiOut, "<tr><td>密码:</td><td><input type='password' name='password' placeholder='密码'></td></tr>");
        fprintf(cgiOut, "<tr><td><input type='submit' name='login' value='登录'></td><td><input type='submit' name='get' value='注册'></td></tr>");
        fprintf(cgiOut, "</table></form></body></html>");
        return 0;
    }
    else if (cgiFormString("login", login, sizeof(login)) == cgiFormSuccess)
    {
        char Username[256] = {0};
        char Password[256] = {0};
        if (cgiFormString("username", Username, sizeof(Username)) != cgiFormSuccess)
        {
            PrintErrorLogin("用户名没有填写", Username, Password);
            return 0;
        }
        if (cgiFormString("password", Password, sizeof(Password)) != cgiFormSuccess)
        {
            PrintErrorLogin("密码没有填写", Username, Password);
            return 0;
        }
        char sql[256] = {0};
        sprintf(sql, "select Password from T_Users where Username='%s'", Username);
        MYSQL_RES *res = executeQuery(sql);
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        if (row == NULL)
            PrintErrorLogin("请检查用户名", Username, Password);
        else
        {
            if (strcmp(Password, row[0]) == 0)
            {
                cgiHeaderContentType("text/html;charset=utf-8");
                fprintf(cgiOut, "<html><head><title>登录成功</title></head><body>");
                fprintf(cgiOut, "<font color='red'>恭喜你成功了</font>");
                fprintf(cgiOut, "</body></html>");
            }
            else
            {
                PrintErrorLogin("请检查密码是否正确", Username, Password);
            }
            mysql_free_result(res);
        }
        return 0;
    }
    else if (cgiFormString("get", get, sizeof(get)) == cgiFormSuccess)
    {
        cgiHeaderContentType("text/html;charset=utf-8");
        fprintf(cgiOut, "<html><head><title>注册</title></head><body>");
        fprintf(cgiOut, "<form action='http://127.0.0.1:4000/cgiGet.cgi' method='GET'>");
        fprintf(cgiOut, "<table>");
        fprintf(cgiOut, "<tr>");
        fprintf(cgiOut, "<td>用户名:</td><td><input type='text' name='username' placeholder='用户名'></td>");
        fprintf(cgiOut, "</tr>");
        fprintf(cgiOut, "<tr><td>密码:</td><td><input type='password' name='password' placeholder='密码'></td></tr>");
        fprintf(cgiOut, "<tr><td><input type='submit' name='get' value='注册'></td></tr>");
        fprintf(cgiOut, "</table></form></body></html>");
    }
    return 0;
}