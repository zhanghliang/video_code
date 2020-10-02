all: httpd client
LIBS = -lpthread #-lsocket
httpd: httpd.c
	gcc -g -w -Wall $(LIBS) -o $@ $<

client: simpleclient.c
	gcc -w -Wall -o $@ $<
clean:
	rm httpd
