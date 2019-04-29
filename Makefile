LIBDIR		:= lib
TESTDIR		:= test
SRCDIR		:= src
INCDIR		:= include
LIBTGT		:= $(LIBDIR)/libcbuoy.a
TESTTGT		:= $(TESTDIR)/test_cbuoy
INCLUDES    := -I$(SRCDIR) -I$(INCDIR)
TESTLIBS    := -L$(LIBDIR) -lcbuoy -lcurl
CC			:= gcc
CFLAGS		:= -Wall

all: $(TESTTGT) $(LIBTGT)
test: $(TESTTGT)
lib: $(LIBTGT)

$(TESTTGT): $(LIBTGT)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTTGT).c $(TESTLIBS) -o $(TESTTGT)

$(LIBTGT): $(LIBDIR)/cbuoy.o $(INCDIR)/cbuoy.h
	ar rcs $(LIBTGT) $(LIBDIR)/cbuoy.o

$(LIBDIR)/cbuoy.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCDIR)/cbuoy.c -o $(LIBDIR)/cbuoy.o

clean:
	rm -f $(LIBDIR)/*.o
	rm -f $(LIBTGT)
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTTGT)
