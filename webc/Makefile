srcdir=./src/
builddir=./build/

CC=gcc
CFLAGS=-g -std=gnu99 -Wall

test:
	$(CC) -I. -I$(srcdir) -lfcgi $(CFLAGS) $(srcdir)list.c $(srcdir)url.c $(srcdir)web.c $(srcdir)hello.c -o $(builddir)hello
clean:
	rm $(builddir)*
