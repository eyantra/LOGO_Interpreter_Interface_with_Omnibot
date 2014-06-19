CC	= gcc
CFLAGS	= -g -O   -O0 -DUSE_OLD_TTY
CXX     = g++
CXXFLAGS = -g 
LDFLAGS	= 
LIBS  =   -lm -ltermcap  
prefix = /usr/local
BINDIR        = $(prefix)/bin
LIBLOC        = $(prefix)/lib/logo
LINKER = $(CC)

# LIBLOC      = `pwd`

OBJS 	= coms.o error.o eval.o files.o graphics.o init.o intern.o \
	  libloc.o lists.o logodata.o main.o math.o mem.o paren.o parse.o \
	  print.o wrksp.o  term.o  my_methods.o 

SRCS	= coms.c error.c eval.c files.c graphics.c init.c intern.c \
	  libloc.c lists.c logodata.c main.c math.c mem.c paren.c parse.c \
	  print.c wrksp.c  term.c  my_methods.c 

HDRS	= config.h globals.h logo.h

all:	$(OBJS) $(SRCS) $(HDRS)
	$(LINKER) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o logo


mem.o:	mem.c
	$(CC) $(CFLAGS) -O0 -c mem.c



libloc.c:
	echo 'char *libloc="'$(LIBLOC)'/logolib";' > libloc.c
	echo 'char *helploc="'$(LIBLOC)'/helpfiles";' >> libloc.c
	echo 'char *cslsloc="'$(LIBLOC)'/csls";' >> libloc.c
	echo 'char *temploc="/tmp";' >> libloc.c
	echo 'char *separator="/";' >> libloc.c

clean:
	rm -f *.o libloc.c logo

*.o : $(HDRS)

graphics.o : $(HDRS) my_methods.h

my_methods.o : $(HDRS) my_methods.h
