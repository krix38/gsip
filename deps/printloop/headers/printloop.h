#ifndef CONFIGURATION_HEADER
#define CONFIGURATION_HEADER

#include <configuration.h>

#endif

#ifndef TOKENIZE_HEADER
#define TOKENIZE_HEADER

#include <tokenize.h>

#endif

#ifndef RATIO_HEADER
#define RATIO_HEADER

#include <ratio.h>

#endif

#ifndef PRINTLOOP_STRUCT
#define PRINTLOOP_STRUCT
typedef struct
{
	Configuration *config;
	int linesToMatchCount;
	char **linesToMatch;
	char *lineToPrint;
	TokenizationContext *tokenizationCtx1;
	TokenizationContext *tokenizationCtx2;
} PrintLoopContext;

#endif

void print_line_with_acceptable_count_reappearing_words(PrintLoopContext *printLoopCtx);