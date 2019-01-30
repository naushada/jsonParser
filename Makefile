CC = gcc
YACC = bison
LEX = flex
BIN = JSON
FLAGS =  -O2 -Wall
LINK_LIB = 
OBJS        = main.o json.o MyString.o parser.tab.o lex.yy.o
GENERAT_SRC = parser.tab.c parser.tab.h lex.yy.c

$(BIN): $(OBJS)
	$(CC)    $(FLAGS) -o $@   $^    $(LINK_LIB)



.c.o:
	$(CC)    $(FLAGS) -c $<

lex.yy.c: lex.l
	$(LEX)  $^

parser.tab.c: parser.y
	$(YACC) -d -b parser $^

main.o        : json.h
json.o        : json.h MyString.h
MyString.o    : MyString.h	
parser.tab.o  : parser.tab.c parser.tab.h

clean:
	 rm -f $(BIN) $(OBJS) $(GENERAT_SRC)
