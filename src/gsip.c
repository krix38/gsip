#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <printloop.h>
#include <matchfile.h>
#include <configuration.h>
#include <utils.h>

void free_memory(PrintLoopContext *printLoopCtx)
{
	free_match_file_memory(printLoopCtx->linesToMatchCount, printLoopCtx->linesToMatch);
	free_tokenization_context(printLoopCtx->tokenizationCtx1);
	free_tokenization_context(printLoopCtx->tokenizationCtx2);
}

void run(Configuration *configuration)
{
	char lineToPrint[LINE_MAX];
	TokenizationContext ctx1, ctx2 = {};
	PrintLoopContext printLoopCtx =
		{
			.config = configuration,
			.lineToPrint = lineToPrint,
			.tokenizationCtx1 = initialize_tokenization_context(&ctx1),
			.tokenizationCtx2 = initialize_tokenization_context(&ctx2),
			.linesToMatchCount = get_file_lines_count(configuration->matchFileName),
		};
	printLoopCtx.linesToMatch = get_ignore_file_lines_array(printLoopCtx.linesToMatchCount, configuration->matchFileName);
	while (fgets(printLoopCtx.lineToPrint, LINE_MAX, stdin))
	{
		print_line_with_acceptable_count_reappearing_words(&printLoopCtx);
	}
	free_memory(&printLoopCtx);
}

void show_usage(char *executableName)
{
	printf("\nusage: %s -[fahvrd]\n\n"
		"  Options:\n"
		"    -f [MATCH FILE NAME]            [MANDATORY] name of file with lines to match\n"
		"    -a [ACCEPTABLE RATIO]           [MANDATORY] acceptable ratio (value between 0.00-1.00)\n"
		"    -h                              print this help\n"
		"    -v                              print highest ratio per line\n"
		"    -r                              reverse match (print lines least matching the match file)\n"
		"    -d                              set delimiter for tokenization (\" \\n\" is default)\n", executableName);
}

int main(int argc, char **argv)
{
	Configuration config = {};
	get_configuration(&config, argc, argv);
	if (config.showUsage)
	{
		show_usage(argv[0]);
		exit(INVALID_USAGE);
	}
	run(&config);
	return 0;
}
