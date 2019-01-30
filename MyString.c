#include "MyString.h"

MyString *new_string() {
	 MyString *mstr = (MyString *) malloc( sizeof(MyString));
	
	 mstr->str = ( char *) malloc (STRING_ALLOC_SIZE);
	 mstr ->str[0] = '\0' ;
	 mstr->length = 0 ;
	 mstr->buff_length = STRING_ALLOC_SIZE;

	 return mstr;
}

void free_string (MyString * mstr) {
	 if(mstr) {
		 free(mstr->str);
		 free(mstr);
	 }
}

size_t my_strlen(MyString * mstr) {
	 return mstr->length;
}

void resize_string(MyString *mstr, size_t size) {
	 if(size > mstr->buff_length) {
		 mstr->buff_length = (size / STRING_ALLOC_SIZE + 1 ) * STRING_ALLOC_SIZE * 2 ;
		 mstr->str = (char *) realloc(mstr->str, mstr->buff_length);
	 }
}

char *my_strcat(MyString *mstr, const char *str) {
	 size_t len = mstr->length + strlen (str);
	 resize_string(mstr, len + 1);
	 strcpy(mstr->str + mstr->length, str);
	 mstr->length = len;

	 return mstr->str;
}

char *my_strncat(MyString *mstr, const char *str, size_t n) {
	 size_t len = mstr->length + n;
	 resize_string(mstr, len + 1 );
	 strncpy(mstr->str + mstr->length, str, n);
	 mstr->length = len;

	 return mstr->str;
}

char *my_chrcat(MyString *mstr, const char c) {
	 resize_string(mstr, mstr->length + 2 );
	 mstr->str[mstr->length ++] = c;
	 mstr->str[mstr->length] = '\0' ;

	 return mstr->str;
}

char *my_strcat2str(MyString *mstr, const char *str, const char *end_str) {
	 char *end = strstr(str, end_str);

	 if(end != NULL ) {
 // end + = strlen (end_str);
		 return my_strcat2ptr(mstr, str, end);
	 }

	 return(char *) mstr->str;
}

char *my_strcat2chr(MyString *mstr, const char *str, const char endc) {
	 char *end = strchr(str, endc);

	 if(end != NULL) {
 // end ++;
		 return my_strcat2ptr(mstr, str, end);
	 }

	 return ( char *) mstr->str;
}

char *my_strcat2ptr(MyString *mstr, const char *str, const char *endp) {
	 size_t cat_len = endp - str - 1 ;

	 if(cat_len > 0) {
		 my_strncat(mstr, str, cat_len);
	 }

	 return(char *) mstr->str;
}
