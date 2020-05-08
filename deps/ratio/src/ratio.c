#include <string.h>
#include <stringutils.h>
#include <ratio.h>

double calculate_ratio(RatioCalculationContext *calculateRatioCtx)
{
	int i, k, reappearingCount, tokensInFirstStringCount;

	TokenizationContext *tokenizationCtx1 = calculateRatioCtx->tokenizationCtx1;
	TokenizationContext *tokenizationCtx2 = calculateRatioCtx->tokenizationCtx2;

	tokenize_string(calculateRatioCtx->lineToCompare, calculateRatioCtx->delimiter, tokenizationCtx1);
	tokenize_string(calculateRatioCtx->lineToMatch, calculateRatioCtx->delimiter, tokenizationCtx2);

	reappearingCount = 0;
	for (i = 0; i < tokenizationCtx1->tokensCount; i++)
	{
		for (k = 0; k < tokenizationCtx2->tokensCount; k++)
		{
			if (strcmp(tokenizationCtx1->tokens[i]->string, tokenizationCtx2->tokens[k]->string) == 0)
			{
				reappearingCount++;
			}
		}
	}

	tokensInFirstStringCount = tokenizationCtx1->tokensCount;

	reinitialize_tokenization_context(tokenizationCtx1);
	reinitialize_tokenization_context(tokenizationCtx2);

	if (tokensInFirstStringCount > 0)
	{
		return (float)reappearingCount / tokensInFirstStringCount;
	}
	return 0;
}