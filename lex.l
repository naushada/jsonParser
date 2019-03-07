%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"								
#include "MyString.h"
#include "json.h"

extern int yyerror(yyscan_t yyscanner, const char *s);

static MyString *share_string;
static unichar ucs2_hi;
int my_yyinput(char *buf, int max_size);

//extern "C"{
    int yywrap(yyscan_t yyscanner) {
      return 1;
    }
#if 0
    int my_yyinput(char *buf, int max_size) {
      int len;
      char c;
      extern FILE *yyin;

      if(feof(yyin)) {
        return 0;
      }

      for(len = 0; len < max_size; len++) {
        if((c = fgetc(yyin)) == EOF) {
          break;
        }
        buf[len] = c;
      }
						fprintf(stderr, "\nValue of len is %d\n", len);
      return len;
    }
#endif
//}

%}

%option reentrant
%option bison-bridge
%option debug
%option warn
%option header-file="lex.yy.h"

%x STRING
%x STRING_UCS_2

space    [ \t\r\n]
digit    [0-9]
integer  -?(0|[1-9]{digit}*)
number   {integer}\.{digit}+
alpha    [a-zA-Z]
hex      [0-9a-fA-F]

%%

<INITIAL>"}"        return '}';
<INITIAL>"{"        return '{';
<INITIAL>"]"        return ']';
<INITIAL>"["        return '[';
<INITIAL>","        return ',';
<INITIAL>":"        return ':';

<INITIAL>{space}*   ;

<INITIAL>"true" {
    yylval->jvalue = json_new_true();
    return LITERAL;
}

<INITIAL>"false" {
    yylval->jvalue = json_new_false();
    return LITERAL;
}

<INITIAL>"null" {
    yylval->jvalue = json_new_null();
    return LITERAL;
}

<INITIAL>{integer} {
    yylval->jvalue = json_new_integer(atoi(yytext));
    return LITERAL;
}

<INITIAL>{number} {
    yylval->jvalue = json_new_double(atof(yytext));
    return LITERAL;
}

<INITIAL>({integer}|{number})[eE][+-]?{integer}+ {
    yylval->jvalue = json_new_double(strtod(yytext, NULL));
    return LITERAL;
}

<INITIAL>\" {
    share_string = new_string();
    BEGIN STRING;
}

<INITIAL>. {
	yyerror(yyscanner, "Syntax Error!!");
}

<STRING>\" {
    BEGIN INITIAL;
    yylval->jvalue = json_new_string(share_string->str);
    free_string(share_string);
    return lSTRING;
}

<STRING>\\\"       my_chrcat(share_string, '"');
<STRING>\\\\       my_chrcat(share_string, '\\');
<STRING>\\\/       my_chrcat(share_string, '/');
<STRING>\\b        my_chrcat(share_string, '\b');
<STRING>\\n|\n     my_chrcat(share_string, '\n');
<STRING>\\r|\r     my_chrcat(share_string, '\r');
<STRING>\\t        my_chrcat(share_string, '\t');
<STRING>\\f        my_chrcat(share_string, '\f');
<STRING>\\v        my_chrcat(share_string, '\v');
<STRING>\\u{hex}{4} {
    unichar hi;
    sscanf(yytext + 2, "%4x", &hi);
    if (hi >= 0xd800) {
        ucs2_hi = hi;
        BEGIN STRING_UCS_2;
    } else if (hi & 0xff00) {
        hi = unicode2utf8(hi);
        my_chrcat(share_string, (hi >> 16) & 0xff);
        my_chrcat(share_string, (hi >>  8) & 0xff);
        my_chrcat(share_string, hi & 0xff);
    } else {
        my_chrcat(share_string, hi);
    }
}
<STRING>\\.        my_chrcat(share_string, yytext[1]);
<STRING>.          my_chrcat(share_string, yytext[0]);
<STRING_UCS_2>\\u{hex}{4} {
    unichar lo, hi = ucs2_hi;
    if (hi < 0xdc00) {
        sscanf(yytext + 2, "%4x", &lo);
        if (lo < 0xdc00 || lo >= 0xdfff) {
            yyerror(yyscanner, "UCS-2 String Error");
        }
        hi = ((hi - 0xd800) << 16) + (lo - 0xdc00) + 0x10000;
        hi = unicode2utf8(hi);
        my_chrcat(share_string, (hi >> 16) & 0xff);
        my_chrcat(share_string, (hi >>  8) & 0xff);
        my_chrcat(share_string, hi & 0xff);
    } else {
        yyerror(yyscanner, "UCS-2 String Error");
    }
    BEGIN STRING;
}
<STRING,STRING_UCS_2><<EOF>> {
	yyerror(yyscanner, "Unterminated string!");
	yyterminate();
}

%%
