#ifndef STRINGUTILS_HEADER
#define STRINGUTILS_HEADER

#include <stringutils.h>

#endif

#ifndef TOKENIZATION_STRUCT
#define TOKENIZATION_STRUCT

typedef struct
{
	String **tokens;
	String *tempBuffer;
	int tempBufferSize;
	int tokensCount;
	int tokensArraySize;
} TokenizationContext;

#endif

TokenizationContext *allocate_tokenization_context();
void reinitialize_tokenization_context(TokenizationContext *ctx);
void tokenize_string(char *string, const char *specifiedDelimiter, TokenizationContext *ctx);
void free_tokenization_context(TokenizationContext *ctx);
