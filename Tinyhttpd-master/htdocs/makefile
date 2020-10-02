app:cgiLogin.cgi cgiGet.cgi
cgiLogin.cgi:cgiLogin.c 
	gcc -w cgiLogin.c -o cgiLogin.cgi -I../cgic207/ -I/usr/include/mysql/ -L../cgic207/ -Lusr/lib/x86_64-linux-gnu/ -lcgic -lmysqlclient
cgiGet.cgi:cgiGet.c 
	gcc -w cgiGet.c -o cgiGet.cgi -I../cgic207/ -I/usr/include/mysql/ -L../cgic207/ -Lusr/lib/x86_64-linux-gnu/ -lcgic -lmysqlclient
clean:
	rm -r cgiLogin.cgi