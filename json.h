#ifndef __JSON_H__
 #define __JSON_H__

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <assert.h>
 #include "parser.tab.h"
 #include "MyString.h"

 enum {
	  JSON_VALUE_TYPE_INTEGER = 0 ,
	  JSON_VALUE_TYPE_DOUBLE,
	  JSON_VALUE_TYPE_STRING,
	  JSON_VALUE_TYPE_OBJECT,
	  JSON_VALUE_TYPE_ARRAY,
	  JSON_VALUE_TYPE_TRUE,
	  JSON_VALUE_TYPE_FALSE,
	  JSON_VALUE_TYPE_NULL,
	  ALL_JSON_VALUE_TYPE_NUM,
 };

 #define JSON_VALUE_NAME_INTEGER "Integer"
 #define JSON_VALUE_NAME_DOUBLE  "Double"
 #define JSON_VALUE_NAME_String  "String"
 #define JSON_VALUE_NAME_Object  "Object"
 #define JSON_VALUE_NAME_Array   "Array"
 #define JSON_VALUE_NAME_TRUE    "true"
 #define JSON_VALUE_NAME_FALSE   "false"
 #define JSON_VALUE_NAME_NULL    "null"

 #define JSON_TOKEN_EOF    (- 1 )
 #define JSON_TOKEN_ERROR  ( 0 )
 #define JSON_TOKEN_LC     '{'
 #define JSON_TOKEN_RC     '}'
 #define JSON_TOKEN_COMMA  ','
 #define JSON_TOKEN_COLON  ':'
 #define JSON_TOKEN_LB     '['
 #define JSON_TOKEN_RB     ']'
 #define JSON_TOKEN_VALUE  'v'

 #define IS_HEX_CHAR (c) (isdigit (c) || (('a' <= c && 'f' > = c) || ('A' <= c && 'F' > = c)))
 #define HEX_CHAR2INT (c) (isdigit (c) ? c - '0' ? tolower (c) - ('a' + 10) : - 1 )

 /* Debug */
 #define JSON_DEBUG_MODE 0
 #define JSON_DEBUG_OUT  stderr
 #if JSON_DEBUG_MODE
  #define JSON_DEBUG (str) fprintf (JSON_DEBUG_OUT, " % s ( % d ):" str " \ n " , __FILE__ , __LINE__ )
 # else
  #define JSON_DEBUG (str)
 # endif
 #define JSON_INDENT_STR ""

 /* static char json_value_name [ALL_JSON_VALUE_TYPE_NUM] [8] = {
 JSON_VALUE_NAME_INTEGER,
 JSON_VALUE_NAME_DOUBLE,
 JSON_VALUE_NAME_String,
 JSON_VALUE_NAME_Object,
 JSON_VALUE_NAME_Array,
 JSON_VALUE_NAME_TRUE,
 JSON_VALUE_NAME_FALSE,
 JSON_VALUE_NAME_NULL,
 }; */

 typedef struct JSONObject  JSONObject;
 typedef struct JSONMember  JSONMember;
 typedef struct JSONArray   JSONArray;
 typedef struct JSONElement JSONElement;
 typedef struct JSONValue   JSONValue;
 typedef struct JSONToken   JSONToken;

 struct JSONObject {
	  JSONMember *members;
 };

 struct JSONMember {
	  JSONValue  *key;
	  JSONValue  *value;
	  JSONMember *next;
 };

 struct JSONArray {
	  JSONElement *elements;
 };

 struct JSONElement {
	  JSONValue   *value;
	  JSONElement *next;
 };

 struct JSONValue {
	  int type;
	  union {
		   int        i_value;
		   double     d_value;
		   char       *s_value;
		   JSONObject *o_value;
		   JSONArray  *a_value;
	  };
 };

 struct JSONToken {
	  char      *str;
	  char      *next;
	  int       token;
	  JSONValue *value;
 };

 JSONValue *json_parser(FILE *fp);
 JSONValue *json_parser_ex(char *pIn);
 //int yylex();
 int yyparse(yyscan_t yyscanner);

 JSONValue *json_new_value();
 JSONValue *json_new_integer(int i);
 JSONValue *json_new_double(double d);
 JSONValue *json_new_string(char *str);
 JSONValue *json_new_nstring(char *str, int size);
 JSONValue *json_new_object_value(JSONObject *object);
 JSONValue *json_new_array_value(JSONArray *array);
 JSONValue *json_new_true();
 JSONValue *json_new_false();
 JSONValue *json_new_null();
 void json_free_value(JSONValue *value);
 void json_print_value(FILE *fp, JSONValue *value, int depth);

 JSONElement *json_new_element(JSONValue *value);
 void json_free_element(JSONElement *element);
 JSONElement *json_value_add_element(JSONElement *element, JSONValue *value);
 void json_print_element(FILE *fp, JSONElement *element, int depth);

 JSONArray *json_new_array(JSONElement *element);
 void json_free_array(JSONArray *array);
 void json_print_array(FILE *fp, JSONArray *array, int depth);

 JSONMember *json_new_member(JSONValue *key, JSONValue *value);
 void json_free_member(JSONMember *member);
 JSONMember *json_value_add_member(JSONMember *member, JSONValue *key, JSONValue *value);
 JSONMember *json_member_add_member(JSONMember *member, JSONMember *value);
 void json_print_member(FILE *fp, JSONMember *member, int depth);

 JSONObject *json_new_object(JSONMember *member);
 void json_free_object(JSONObject *object);
 void json_print_object(FILE *fp, JSONObject *object, int depth);

 JSONValue *json_value_at_index(JSONValue *value, int index);
 JSONValue *json_value_at_key(JSONValue *value, char *key);

 MyString *get_ustring(char *p);
 unichar unicode2utf8(unichar uc);

 void json_print(FILE *fp, JSONValue *value);
 void json_print_indent(FILE *fp, int depth);

 #endif
