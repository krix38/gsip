#ifndef STRING_STRUCT
#define STRING_STRUCT

typedef struct
{
	char *string;
	int allocatedSize;
} String;

#endif

String *create_string();
String *set_string(String *string, char *newStringValue);
void free_string(String *string);
