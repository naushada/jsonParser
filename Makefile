CC = gcc
YACC = bison
LEX = flex
BIN = JSON
FLAGS =  -O2 -Wall
LINK_LIB = 
OBJS        = main.o json.o MyString.o parser.tab.o lex.yy.o
GENERAT_SRC = parser.tab.c parser.tab.h lex.yy.c


all: lex.yy.c parser.tab.c $(BIN)
.PHONY: all

lex.yy.c: lex.l
	$(LEX) $^

parser.tab.c: parser.y
	$(YACC) -d -b parser $^

$(BIN): $(OBJS)
	$(CC) $(FLAGS) -o $@  $^  $(LINK_LIB)

.c.o:
	$(CC) $(FLAGS) -c $<

main.o        : json.h
json.o        : json.h MyString.h
MyString.o    : MyString.h	
parser.tab.o  : parser.tab.c parser.tab.h

.PHONY: clean
clean:
	 rm -f $(BIN) $(OBJS) $(GENERAT_SRC)
