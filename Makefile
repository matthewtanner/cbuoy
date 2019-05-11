LIBDIR		:= lib
TESTDIR		:= test
SRCDIR		:= src
INCDIR		:= include
LIBTGT		:= $(LIBDIR)/libndbcc.a
TESTTGT		:= $(TESTDIR)/test_ndbcc
INCLUDES    := -I$(SRCDIR) -I$(INCDIR)
TESTLIBS    := -L$(LIBDIR) -lndbcc -lcurl
CC			:= gcc
CFLAGS		:= -Wall

all: $(TESTTGT) $(LIBTGT)
test: $(TESTTGT)
lib: $(LIBTGT)

$(TESTTGT): $(LIBTGT) $(TESTTGT).c
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTTGT).c $(TESTLIBS) -o $(TESTTGT)

$(LIBTGT): $(LIBDIR)/ndbcc.o $(INCDIR)/ndbcc.h
	ar rcs $(LIBTGT) $(LIBDIR)/ndbcc.o

$(LIBDIR)/ndbcc.o: $(SRCDIR)/ndbcc.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCDIR)/ndbcc.c -o $(LIBDIR)/ndbcc.o

format:
	/usr/bin/uncrustify -c ./uncrustify.cfg --no-backup \
		$(shell find $(SRCDIR) $(TESTDIR) $(INCDIR) -name "*.[ch]")

clean:
	rm -f $(LIBDIR)/*.a
	rm -f $(LIBDIR)/*.o
	rm -f $(TESTDIR)/*.o
	rm -f $(TESTTGT)
