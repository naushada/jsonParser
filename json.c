#include "json.h"
#include "parser.tab.h"
#include "lex.yy.h"

JSONValue *json_parser_ex(char *pIn) 
{
  yyscan_t yyscanner;
	extern JSONValue *json;

  if(yylex_init(&yyscanner))
  {
    fprintf(stderr, "%s:%d initialization to scanner failed\n", __FILE__, __LINE__);
    return((JSONValue *)0);				
  }

  YY_BUFFER_STATE buff = pIn ? yy_scan_string(pIn, yyscanner) : 0;

		if(yyparse(yyscanner))
  {
    fprintf(stderr, "%s:%d yyparse failed", __FILE__, __LINE__);
    return((JSONValue *)0);				
  }

  yy_delete_buffer(buff, yyscanner);
  yylex_destroy(yyscanner);

	 return(json);
}

JSONValue *json_new_value() {
	 JSONValue *value = (JSONValue *) malloc(sizeof(JSONValue));
	 return value;
}

JSONValue *json_new_integer(int i) {
	 JSONValue *value = json_new_value ();

	 value->type = JSON_VALUE_TYPE_INTEGER;
	 value->i_value = i;

	 return value;
}

JSONValue *json_new_double(double d) {
	 JSONValue *value = json_new_value();

	 value->type = JSON_VALUE_TYPE_DOUBLE;
	 value->d_value = d;

	 return value;
}

JSONValue *json_new_string(char * str) {
	 return json_new_nstring (str, strlen (str));
}

JSONValue *json_new_nstring(char *str, int size) {
	 JSONValue *value = json_new_value();

	 value->type = JSON_VALUE_TYPE_STRING;
	 value->s_value = (char *) malloc(size + 1);
	 strncpy(value-> s_value, str, size);
	 value->s_value[size] = '\0' ;

	 return value;
}

JSONValue *json_new_object_value(JSONObject *object) {
	 JSONValue *value;

	 if(!object) {
		 return NULL ;
	 }

	 value = json_new_value();
	 value->type = JSON_VALUE_TYPE_OBJECT;
	 value->o_value = object;

	 return value;
}

JSONValue *json_new_array_value(JSONArray * array) {
	 JSONValue *value;

	 if(!array) {
		 return NULL ;
	 }
	 value = json_new_value();
	 value->type = JSON_VALUE_TYPE_ARRAY;
	 value->a_value = array;

	 return value;
}

JSONValue *json_new_true() {
	 JSONValue *value = json_new_value();

	 value->type = JSON_VALUE_TYPE_TRUE;

	 return value;
}

JSONValue *json_new_false() {
	 JSONValue *value = json_new_value();

	 value->type = JSON_VALUE_TYPE_FALSE;

	 return value;
}

JSONValue *json_new_null() {
	 JSONValue *value = json_new_value();

	 value->type = JSON_VALUE_TYPE_NULL;

	 return value;
}

void json_free_value(JSONValue * value) {
	 if(!value) {
		  return ;
	 }

	 if(value-> type == JSON_VALUE_TYPE_STRING) {
		  free(value->s_value);
	 } 
		else if(value->type == JSON_VALUE_TYPE_OBJECT) {
		  json_free_object(value->o_value);
	 } 
		else if(value->type == JSON_VALUE_TYPE_ARRAY) {
		  json_free_array(value->a_value);
	 }

	 free(value);
}

void json_print_value(FILE *fp, JSONValue *value, int depth) {
	 MyString * str;

	 switch (value->type) {
	   case JSON_VALUE_TYPE_INTEGER:
		    fprintf(fp, " %d " , value->i_value);
		    break ;

	   case JSON_VALUE_TYPE_DOUBLE:
		    fprintf(fp, " %f " , value->d_value);
		    break ;

	   case JSON_VALUE_TYPE_STRING:
		    str = get_ustring(value->s_value);
		    fprintf(fp, " \"%s \" " , str->str);
		    free_string(str);
		    break ;

	   case JSON_VALUE_TYPE_OBJECT:
		    json_print_object(fp, value->o_value, depth);
		    break ;

	   case JSON_VALUE_TYPE_ARRAY:
		    json_print_array(fp, value->a_value, depth);
		    break ;

	   case JSON_VALUE_TYPE_TRUE:
		    fprintf(fp, "true" );
		    break ;

	   case JSON_VALUE_TYPE_FALSE:
		    fprintf(fp, "false" );
		    break ;

	   case JSON_VALUE_TYPE_NULL:
		    fprintf(fp, "null" );
		    break ;
  }
}

JSONElement *json_new_element(JSONValue *value) {
	 JSONElement *element = (JSONElement *) malloc(sizeof(JSONElement));

	 element->value = value;
	 element->next = NULL ;

	 return element;
}

void json_free_element(JSONElement *element) {
	 if(! element) {
		 return ;
	 }

	 json_free_element(element->next);
	 free (element);
}

JSONElement *json_value_add_element(JSONElement *element, JSONValue *value) {
	 JSONElement *e;

	 if(!element) {
		 return json_new_element (value);
	 }

	 for(e = element; e->next; e = e->next) {}
	 e->next = json_new_element(value);

	 return element;
}

void json_print_element(FILE *fp, JSONElement *element, int depth) {
	 for(; element; element = element->next) {
		  json_print_indent(fp, depth); 
				json_print_value(fp, element->value, depth);
		  if(element->next) {
			   fprintf(fp, "," );
		  }
		  fprintf(fp, " \n " );
	 }
}

JSONArray *json_new_array(JSONElement *element) {
	 JSONArray *array = (JSONArray *) malloc(sizeof(JSONArray));

	 array->elements = element;

	 return array;
}

void json_free_array(JSONArray * array) {
	 if(!array) {
		 return ;
	 }

	 json_free_element(array-> elements);
	 free(array);
}

void json_print_array(FILE *fp, JSONArray *array, int depth) {
	 json_print_indent(fp, depth); 
		fprintf(fp, "[ \n " );
	 json_print_element(fp, array->elements, depth + 1 );
	 json_print_indent(fp, depth); 
		fprintf(fp, "]" );
}

JSONMember *json_new_member(JSONValue *key, JSONValue *value) {
	 JSONMember *member = (JSONMember *) malloc( sizeof(JSONMember));

	 member->key = key;
	 member->value = value;
	 member->next = NULL ;

	 return member;
}

void json_free_member(JSONMember *member) {
	 if(!member) {
		 return ;
	 }

	 json_free_member(member->next);
	 json_free_value(member->key);
	 json_free_value(member->value);
	 free (member);
}

JSONMember *json_value_add_member(JSONMember *member, JSONValue *key, JSONValue *value) {
	 return json_member_add_member(member, json_new_member(key, value));
}

JSONMember *json_member_add_member(JSONMember *member, JSONMember *value) {
	 JSONMember *m;

	 if(!member) {
		 return value;
	 }

	 for(m = member; m -> next; m = m -> next) {}
	 m->next = value;

	 return member;
}

void json_print_member(FILE *fp, JSONMember *member, int depth) {
	 for(; member; member = member-> next) {
		 json_print_indent(fp, depth);
		 json_print_value(fp, member->key, depth);
		 fprintf (fp, ":" );
		 json_print_value(fp, member->value, depth);
		 if(member->next) {
			 fprintf (fp, "," );
		 }
		 fprintf(fp, " \n " );
	 }
}

JSONObject *json_new_object(JSONMember *member) {
	 JSONObject *object = (JSONObject *) malloc(sizeof(JSONObject));

	 object->members = member;

	 return object;
}

void json_free_object(JSONObject *object) {
	 if(!object) {
		 return ;
	 }

	 json_free_member(object-> members);
	 free(object);
}

void json_print_object(FILE *fp, JSONObject *object, int depth) {
	 json_print_indent(fp, depth); 
		fprintf(fp, "{ \n " );
	 json_print_member(fp, object->members, depth + 1 );
	 json_print_indent(fp, depth); 
		fprintf(fp, "}" );
}

JSONValue *json_value_at_index(JSONValue *value, int index) {
	 JSONElement *e;

	 if (value->type != JSON_VALUE_TYPE_ARRAY) {
		 return NULL ;
	 }

	 for(e = value->a_value->elements; index && e; index--, e = e->next) {}

	 if(!e) {
		 return NULL ;
	 }

	 return e->value;
}

JSONValue *json_value_at_key(JSONValue *value, char *key) {
	 JSONMember *m;

	 if(!value || value->type != JSON_VALUE_TYPE_OBJECT) {
		 return NULL ;
	 }

	 for(m = value->o_value->members; m; m = m->next) {
		 assert (m->key->type == JSON_VALUE_TYPE_STRING);
		 if (strcmp(m->key->s_value, key) == 0 ) {
			 return m->value;
		 }
	 }

	 return NULL ;
}

MyString *get_ustring(char *p) {
	 MyString *str = new_string();
	 char c;

	 for(; (c = * p); p ++) {

		  switch(c) {

		    case '\\'  : my_chrcat (str, '\\' ); c = '\\' ; break ;
		    case '/'   : my_chrcat (str, '\\' ); c = '/' ;  break ;
		    case '"'   : my_chrcat (str, '\\' ); c = '"' ;  break ;
		    case '\b'  : my_chrcat (str, '\\' ); c = 'b' ;  break ;
		    case '\n'  : my_chrcat (str, '\\' ); c = 'n' ;  break ;
		    case '\r'  : my_chrcat (str, '\\' ); c = 'r' ;  break ;
		    case '\t'  : my_chrcat (str, '\\' ); c = 't' ;  break ;
		    case '\f'  : my_chrcat (str, '\\' ); c = 'f' ;  break ;
		  }
		  my_chrcat (str, c);
	 }

	 return str;
}

unichar unicode2utf8(unichar uc) {
	 return 
	 ( 0xe0 | ((uc >> 12 ) & 0x0f )) << 16 |
	 ( 0x80 | ((uc >> 6 ) & 0x3f )) << 8 |
	 ( 0x80 | (uc & 0x3f ));
}

void json_print(FILE *fp, JSONValue *value) {
	 json_print_value(fp, value, 0 );
	 fprintf(fp, " \n " );
}

void json_print_indent(FILE *fp, int depth) {
	 int i;

	 for(i = 0 ; i < depth; i ++) {
		  fprintf(fp, JSON_INDENT_STR);
	 }
}
