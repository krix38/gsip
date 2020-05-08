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
	double acceptableRatio;
	int linesToMatchCount;
	char **linesToMatch;
	char *lineToPrint;
	char *delimiter;
	TokenizationContext *tokenizationCtx1;
	TokenizationContext *tokenizationCtx2;
} PrintLoopContext;

#endif

void map_print_loop_context_to_calculate_ratio_context(RatioCalculationContext *ratioCalculationCtx, PrintLoopContext *printLoopCtx);
void print_line_with_acceptable_count_reappearing_words(PrintLoopContext *printLoopCtx);