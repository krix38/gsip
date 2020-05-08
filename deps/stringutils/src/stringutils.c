#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <utils.h>
#include <stdlib.h>
#include <stringutils.h>

#define INITIAL_STRING_BUFFER_SIZE 256

String *create_string()
{
	String *string = (String *)allocate(sizeof(String));
	string->string = allocate(sizeof(char *) * INITIAL_STRING_BUFFER_SIZE);
	string->allocatedSize = INITIAL_STRING_BUFFER_SIZE;
	return string;
}

String *set_string(String *string, char *newStringValue)
{
	char *reallocatedPointer;
	int newSize = strlen(newStringValue);
	if (newSize > string->allocatedSize)
	{
		reallocatedPointer = realloc(string->string, sizeof(char *) * newSize);
		if (reallocatedPointer == NULL)
		{
			fprintf(stderr, "string realloc failed: %s\n", strerror(errno));
			exit(1);
		}
		string->string = reallocatedPointer;
		string->allocatedSize = newSize;
	}
	strcpy(string->string, newStringValue);
	return string;
}

void free_string(String *string)
{
	free(string->string);
	free(string);
}
