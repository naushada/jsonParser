#ifndef __MY_STRING_H__
 #define __MY_STRING_H__

 #include <stdlib.h>
 #include <string.h>

 #define STRING_ALLOC_SIZE ( 1024 )

 typedef unsigned int unichar;

 typedef struct MyString {
	  char *str;
	  size_t length;
	  size_t buff_length;
 }MyString;

 MyString *new_string();
 void free_string(MyString *mstr);
 size_t my_strlen(MyString *mstr);
 void resize_string(MyString *mstr, size_t size);
 char *my_strcat(MyString *mstr, const char *str);
 char *my_strncat(MyString *mstr, const char *str, size_t n);
 char *my_chrcat(MyString *mstr, const char c);
 char *my_strcat2str(MyString *mstr, const char *str, const char *end_str);
 char *my_strcat2chr(MyString *mstr, const char *str, const char endc);
 char *my_strcat2ptr(MyString *mstr, const char *str, const char *endp);

#endif
