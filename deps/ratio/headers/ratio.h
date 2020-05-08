#ifndef TOKENIZE_HEADER
#define TOKENIZE_HEADER

#include <tokenize.h>

#endif

#ifndef RATIO_STRUCT
#define RATIO_STRUCT

typedef struct
{
	double acceptableRatio;
	char *lineToMatch;
	char *lineToCompare;
	char *delimiter;
	TokenizationContext *tokenizationCtx1;
	TokenizationContext *tokenizationCtx2;
} RatioCalculationContext;

#endif

double calculate_ratio(RatioCalculationContext *calculateRatioCtx);