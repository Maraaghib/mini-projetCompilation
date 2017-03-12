CC = gcc
CFLAGS = -g -std=c99
CPPFLAGS = -I./includes
VPATH =
LDFLAGS = -lfl -ly

SOURCES_Y := $(wildcard *.y)
SOURCES_L := $(wildcard *.l)

OBJECTS = y.tab.o lex.yy.o

PROGS = iimp interpreterIMP

#.PHONY: all
#all: $(PROGS)


interpreterIMP: interpreterIMP.o
	$(CC) $^ -o $@

prog : $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@
	
interpreterIMP.o: utils/environment.c y.tab.h

lex.yy.o: lex.yy.c

y.tab.o: y.tab.c y.tab.h

lex.yy.c: $(SOURCES_L) y.tab.h
	lex $<

y.tab.c: $(SOURCES_Y)
	yacc $^

y.tab.h: $(SOURCES_Y)
	yacc -d $^

.PHONY: clean
clean :
	-rm -f $(OBJECTS) $(PROGS) *.c *.h



#yacc --file-prefix=anasynt -d anasynt.y
#cc -std=c89 -c -o anasynt.o anasynt.c
#lex -o analex.c analex.l
#cc -std=c89 -c -o analex.o analex.c
#cc -o anasynt anasynt.o analex.o
#./anasynt < test.in > test.out
