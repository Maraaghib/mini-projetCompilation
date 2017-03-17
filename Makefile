CC = gcc
CFLAGS = -g -std=c99
CPPFLAGS = -I./includes
VPATH =
LDFLAGS = -lfl -ly

SOURCES_Y := $(wildcard *.y)
#SOURCES_L := $(wildcard *.l)
SOURCES_L = iimp.l

OBJECTS = y.tab.o lex.yy.o interpreterIMP.o compilerIMP.o bilquad.o #environment.o

PROGS = iimp interpreterC3A compilerC3A

.PHONY: all
all: $(PROGS)

# Interpreter of Code à 3 Adresses (C3A)
iimp : $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@


interpreterC3A: interpreterC3A.yy.o environment.o bilquad.o
	$(CC) $^ $(LDFLAGS) -o $@

interpreterC3A.yy.o: interpreterC3A.yy.c includes/environment.h includes/bilquad.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

interpreterC3A.yy.c: interpreterC3A.l
	lex -o $@ $<


compilerC3A: compilerC3A.yy.o environment.o bilquad.o
	$(CC) $^ $(LDFLAGS) -o $@

compilerC3A.yy.o: compilerC3A.yy.c includes/environment.h includes/bilquad.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

compilerC3A.yy.c: compilerC3A.l
	lex -o $@ $<


# Compiler of Code à 3 Adresses (C3A)


compilerIMP.o: compilerIMP.c includes/iimp.h includes/environment.h includes/bilquad.h y.tab.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<


bilquad.o: utils/bilquad.c includes/bilquad.h includes/environment.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

environment.o: utils/environment.c includes/environment.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<


lex.yy.o: lex.yy.c includes/iimp.h
		$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

y.tab.o: y.tab.c y.tab.h includes/iimp.h
		$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

interpreterIMP.o: interpreterIMP.c y.tab.h  includes/iimp.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

lex.yy.c: $(SOURCES_L) y.tab.h
	lex $<

y.tab.c: $(SOURCES_Y)
	yacc $^

y.tab.h: $(SOURCES_Y)
	yacc -d $^

.PHONY: clean
clean :
	-rm -f $(OBJECTS) $(PROGS) *.yy.c y.tab.c *.h *.o
