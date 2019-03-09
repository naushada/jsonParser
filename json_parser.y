%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "json_parser.tab.h"
#include "json_lex.yy.h"

int yyerror(yyscan_t yyscanner, const char *s);

JSONValue *json;
%}

/*! This piece of code is copied into header file verbatim */
%code requires {
#include "json.h"
typedef void *yyscan_t;
}

%union {
  JSONObject  *jobject;
  JSONMember  *jmember;
  JSONArray   *jarray;
  JSONElement *jelement;
  JSONValue   *jvalue;
}

/*! For Re-entrant parser*/
%define api.pure full
%param   {yyscan_t yyscanner}

/* %token <fieldname> token OR
 * %type  <fieldname> non-terminal
 */
%token <jvalue>   lSTRING LITERAL
%type  <jobject>  object
%type  <jmember>  member
%type  <jarray>   array
%type  <jelement> element
%type  <jvalue>   value

/*! Grammer Parsing kicks in at input BNF Grammer */
%start input

/* Rules section */
%%

input
 : /* empty */
 | value { json = $1 ; }
 ;

value
 : LITERAL
 | lSTRING
 | object      { $$ = json_new_object_value( $1 ); }
 | array       { $$ = json_new_array_value( $1 ); }
 ;

object
 : '{' '}'         { $$ = json_new_object( NULL ); }
 | '{' member '}'  { $$ = json_new_object( $2 );}
 ;

member
 : lSTRING ':' value             { $$ = json_new_member( $1 , $3 ); }
 | member ',' lSTRING ':' value  { $$ = json_value_add_member( $1 , $3 , $5 );}
 ;

array
 : '[' ']'            { $$ = json_new_array( NULL ); }
 | '[' element ']'    { $$ = json_new_array( $2 );}

element
 : value              { $$ = json_new_element( $1 ); }
 | element ',' value  { $$ = json_value_add_element( $1 , $3 ); }
 ;

%%

/*! Routines Section */
int yyerror(yyscan_t scanner, const char *s) {
     printf( "Error: %s \n " , s);
     return 0 ;
}
