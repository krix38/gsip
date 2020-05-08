#include <stdio.h>
#include <ratio.h>
#include <printloop.h>

void map_print_loop_context_to_calculate_ratio_context(RatioCalculationContext *ratioCalculationCtx, PrintLoopContext *printLoopCtx)
{
	ratioCalculationCtx->acceptableRatio = printLoopCtx->acceptableRatio;
	ratioCalculationCtx->lineToCompare = printLoopCtx->lineToPrint;
	ratioCalculationCtx->delimiter = printLoopCtx->delimiter;
	ratioCalculationCtx->tokenizationCtx1 = printLoopCtx->tokenizationCtx1;
	ratioCalculationCtx->tokenizationCtx2 = printLoopCtx->tokenizationCtx2;
}

void print_line_with_acceptable_count_reappearing_words(PrintLoopContext *printLoopCtx)
{
	int i;
	double ratio;
	double highestRatio = -1.0;
	RatioCalculationContext ratioCalculationCtx;
	map_print_loop_context_to_calculate_ratio_context(&ratioCalculationCtx, printLoopCtx);
	for (i = 0; i < printLoopCtx->linesToMatchCount; i++)
	{
		if (highestRatio == -1.0)
		{
			ratioCalculationCtx.lineToMatch = printLoopCtx->linesToMatch[i];
			highestRatio = calculate_ratio(&ratioCalculationCtx);
		}
		else
		{
			ratioCalculationCtx.lineToMatch = printLoopCtx->linesToMatch[i];
			ratio = calculate_ratio(&ratioCalculationCtx);
			if (ratio > highestRatio)
			{
				highestRatio = ratio;
			}
		}
	}
	if (highestRatio >= 0.0 && highestRatio <= printLoopCtx->acceptableRatio)
	{
		printf("%s", printLoopCtx->lineToPrint);
	}
}