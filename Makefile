CC = gcc
YACC = bison
LEX = flex
BIN = bin/libjson.so
FLAGS =  -O2 -Wall
INC = -I./ -I./inc -I./parser
LINK_LIB = 
OBJS        = obj/main.o obj/json.o obj/myString.o obj/json_parser.tab.o obj/json_lex.yy.o
GENERAT_SRC = parser/json_parser.tab.c parser/json_parser.tab.h parser/json_lex.yy.c parser/json_lex.yy.h


all: build parser/json_lex.yy.c parser/json_parser.tab.c $(BIN)
.PHONY: all

# /* Creates the directory if don't exists */
build:
	@mkdir -p parser
	@mkdir -p obj
	@mkdir -p bin

parser/json_lex.yy.c: grammar/json_lex.l
	$(LEX) $^

parser/json_parser.tab.c: grammar/json_parser.y
	$(YACC) -d -b json_parser $^ -o $@

# /*Creating Shared library i.e. libjson.so */
$(BIN): $(OBJS)
	$(CC) $(FLAGS) -shared -o $@  $^  $(LINK_LIB)

# /* Creating Object files in obj directory from source files */

obj/%.o: src/%.c
	$(CC) $(FLAGS) -c $< -o $@ $(INC)

# /* Creating Object files from source files kept in parser directory. */

obj/%.o: parser/%.c
	$(CC) $(FLAGS) -c $< -o $@ $(INC)

obj/main.o        : inc/json.h
obj/json.o        : inc/json.h inc/myString.h parser/json_parser.tab.h parser/json_lex.yy.h
obj/myString.o    : inc/myString.h	
obj/json_parser.tab.o  : parser/json_parser.tab.c parser/json_parser.tab.h
obj/json_lex.yy.o      : parser/json_lex.yy.c parser/json_lex.yy.h

.PHONY: clean
clean:
	 rm -f $(BIN) $(OBJS) $(GENERAT_SRC)
