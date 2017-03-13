CC = gcc
CFLAGS = -g -std=c99
CPPFLAGS = -I./includes
VPATH =
LDFLAGS = -lfl -ly

SOURCES_Y := $(wildcard *.y)
SOURCES_L := $(wildcard *.l)

OBJECTS = y.tab.o lex.yy.o interpreterIMP.o #environment.o

PROGS = iimp 

#.PHONY: all
#all: $(PROGS)


prog : $(OBJECTS) 
	$(CC) $^ $(LDFLAGS) -o $@
	

lex.yy.o: lex.yy.c includes/mini-projet.h
		$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
		
y.tab.o: y.tab.c y.tab.h includes/mini-projet.h includes/environment.h
		$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

interpreterIMP.o: interpreterIMP.c y.tab.h includes/environment.h includes/mini-projet.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
	
lex.yy.c: $(SOURCES_L) y.tab.h
	lex $<

y.tab.c: $(SOURCES_Y) 
	yacc $^

y.tab.h: $(SOURCES_Y)
	yacc -d $^

.PHONY: clean
clean :
	-rm -f $(OBJECTS) $(PROGS) lex.yy.c y.tab.c *.h *.o



#yacc --file-prefix=anasynt -d anasynt.y
#cc -std=c89 -c -o anasynt.o anasynt.c
#lex -o analex.c analex.l
#cc -std=c89 -c -o analex.o analex.c
#cc -o anasynt anasynt.o analex.o
#./anasynt < test.in > test.out
