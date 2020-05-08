#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <printloop.h>
#include <matchfile.h>

#define IGNORE_FILE "gsip.ignore"

void free_memory(PrintLoopContext *printLoopCtx)
{
	free_match_file_memory(printLoopCtx->linesToMatchCount, printLoopCtx->linesToMatch);
	free_tokenization_context(printLoopCtx->tokenizationCtx1);
	free_tokenization_context(printLoopCtx->tokenizationCtx2);
}

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 3)
	{
		fprintf(
			stderr,
			"please provide similarity ratio as first argument (0.00 - 1.00)\n"
			"and optionally tokenization delimiter as second argument\n");
		return 1;
	}
	char lineToPrint[LINE_MAX];
	PrintLoopContext printLoopCtx =
		{
			.delimiter = NULL,
			.lineToPrint = lineToPrint,
			.acceptableRatio = strtod(argv[1], NULL),
			.tokenizationCtx1 = allocate_tokenization_context(),
			.tokenizationCtx2 = allocate_tokenization_context(),
			.linesToMatchCount = get_file_lines_count(IGNORE_FILE),
		};
	printLoopCtx.linesToMatch = get_ignore_file_lines_array(printLoopCtx.linesToMatchCount, IGNORE_FILE);
	if (argc == 3)
	{
		printf("setting delimtier");
		printLoopCtx.delimiter = argv[2];
	}
	while (fgets(printLoopCtx.lineToPrint, LINE_MAX, stdin))
	{
		print_line_with_acceptable_count_reappearing_words(&printLoopCtx);
	}
	free_memory(&printLoopCtx);
	return 0;
}
