CC = gcc
YACC = bison
LEX = flex
BIN = JSON
FLAGS =  -O2 -Wall
LINK_LIB = 
OBJS        = main.o json.o myString.o json_parser.tab.o json_lex.yy.o
GENERAT_SRC = json_parser.tab.c json_parser.tab.h json_lex.yy.c json_lex.yy.h


all: json_lex.yy.c json_parser.tab.c $(BIN)
.PHONY: all

json_lex.yy.c: json_lex.l
	$(LEX) $^

json_parser.tab.c: json_parser.y
	$(YACC) -d -b parser $^

$(BIN): $(OBJS)
	$(CC) $(FLAGS) -o $@  $^  $(LINK_LIB)

.c.o:
	$(CC) $(FLAGS) -c $<

main.o        : json.h
json.o        : json.h myString.h json_parser.tab.h json_lex.yy.h
myString.o    : myString.h	
json_parser.tab.o  : json_parser.tab.c json_parser.tab.h
json_lex.yy.o      : json_lex.yy.c json_lex.yy.h

.PHONY: clean
clean:
	 rm -f $(BIN) $(OBJS) $(GENERAT_SRC)
