#include <stdio.h>
#include <ratio.h>
#include <printloop.h>

static void map_print_loop_context_to_calculate_ratio_context(RatioCalculationContext *ratioCalculationCtx, PrintLoopContext *printLoopCtx)
{
	ratioCalculationCtx->acceptableRatio = printLoopCtx->config->acceptableRatio;
	ratioCalculationCtx->lineToCompare = printLoopCtx->lineToPrint;
	ratioCalculationCtx->delimiter = printLoopCtx->config->delimiter;
	ratioCalculationCtx->tokenizationCtx1 = printLoopCtx->tokenizationCtx1;
	ratioCalculationCtx->tokenizationCtx2 = printLoopCtx->tokenizationCtx2;
}

static bool print_line_condition_met(double ratio, Configuration *config)
{
	if (config->reverseMatch)
	{
		return ratio >= 0.0 && ratio <= config->acceptableRatio;
	}
	return ratio > config->acceptableRatio;
}

static void process_line_to_print(double highestRatio, PrintLoopContext *printLoopCtx)
{
	if (print_line_condition_met(highestRatio, printLoopCtx->config))
	{
		if (printLoopCtx->config->printRatio)
		{
			printf("%.2f: %s", highestRatio, printLoopCtx->lineToPrint);
		}
		else
		{
			printf("%s", printLoopCtx->lineToPrint);
		}
	}
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
	process_line_to_print(highestRatio, printLoopCtx);
}