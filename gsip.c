#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

#define IGNORE_FILE "gsip.ignore"

#define INITIAL_TOKENS_ARRAY_SIZE 10
#define INITIAL_STRING_BUFFER_SIZE 256
#define DEFAULT_TOKEN_DELIMITER " \n"

typedef struct
{
	char *string;
	int allocatedSize;
} String;

typedef struct
{
	String **tokens;
	String *tempBuffer;
	int tempBufferSize;
	int tokensCount;
	int tokensArraySize;
} TokenizationContext;

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

String *create_string()
{
	String *string = (String *)malloc(sizeof(String));
	string->string = malloc(sizeof(char *) * INITIAL_STRING_BUFFER_SIZE);
	string->allocatedSize = INITIAL_STRING_BUFFER_SIZE;
	return string;
}

String *set_string(String *string, char *newStringValue)
{
	char *reallocatedPointer;
	int newSize = strlen(newStringValue);
	if(newSize > string->allocatedSize)
	{
		reallocatedPointer = realloc(string->string, sizeof(char *) * newSize);
		if(reallocatedPointer == NULL)
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

void initialize_tokens_array(TokenizationContext *ctx)
{
	int i;
	for (i = 0; i < ctx->tokensArraySize; i++)
	{
		ctx->tokens[i] = create_string();
	}
}

TokenizationContext *allocate_tokenization_context()
{
	TokenizationContext *ctx = (TokenizationContext *)malloc(sizeof(TokenizationContext));
	ctx->tempBuffer = create_string();
	ctx->tokens = (String **)malloc(sizeof(String) * INITIAL_TOKENS_ARRAY_SIZE);
	ctx->tokensArraySize = INITIAL_TOKENS_ARRAY_SIZE;
	ctx->tokensCount = 0;
	initialize_tokens_array(ctx);
	return ctx;
}

void reinitialize_tokenization_context(TokenizationContext *ctx)
{
	ctx->tokensCount = 0;
}

void tokenize_string(char *string, const char *specifiedDelimiter, TokenizationContext *ctx)
{
	int i, l;
	String **reallocatedTokens;
	const char defaultDelimiter[] = DEFAULT_TOKEN_DELIMITER;
  	const char *delimiter = specifiedDelimiter == NULL ? defaultDelimiter : specifiedDelimiter;
	ctx->tempBuffer = set_string(ctx->tempBuffer, string);
	char *token = strtok(ctx->tempBuffer->string, delimiter);
	for (i = 0; token != NULL; i++)
	{
		if (i == ctx->tokensArraySize)
		{
			ctx->tokensArraySize = ctx->tokensArraySize * 2;
			reallocatedTokens = (String **)realloc(ctx->tokens, sizeof(String) * ctx->tokensArraySize);
			if(reallocatedTokens == NULL)
			{
                        	fprintf(stderr, "tokens realloc failed: %s\n", strerror(errno));
                        	exit(1);
			}
			ctx->tokens = reallocatedTokens;
			for (l = i; l < ctx->tokensArraySize; l++)
			{
				ctx->tokens[l] = create_string();
			}
		}
		ctx->tokens[i] = set_string(ctx->tokens[i], token);
		token = strtok(NULL, delimiter);
	}
	ctx->tokensCount = i;
}

void free_tokenization_context(TokenizationContext *ctx)
{
	int i;
	for (i = 0; i < ctx->tokensArraySize; i++)
	{
		free_string(ctx->tokens[i]);
	}
	free(ctx->tokens);
	free_string(ctx->tempBuffer);
	free(ctx);
}

double count_reappearing_words(PrintLoopContext *printLoopCtx, int lineToMatchIndex)
{
	int i, k, reappearingCount, tokensInFirstStringCount;

	TokenizationContext *tokenizationCtx1 = printLoopCtx->tokenizationCtx1;
	TokenizationContext *tokenizationCtx2 = printLoopCtx->tokenizationCtx2;

	tokenize_string(printLoopCtx->lineToPrint, printLoopCtx->delimiter, tokenizationCtx1);
	tokenize_string(printLoopCtx->linesToMatch[lineToMatchIndex], printLoopCtx->delimiter, tokenizationCtx2);

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

int get_file_lines_count(char *fileName)
{
	int linesCount = 0;
	char c;
	FILE *fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "failed to open %s\n", fileName);
		exit(1);
	}

	for (c = getc(fp); c != EOF; c = getc(fp))
	{
		if (c == '\n')
		{
			linesCount++;
		}
	}
	fclose(fp);
	return linesCount;
}

char **get_ignore_file_lines_array(int allocateLinesCount, char *fileName)
{
	int i;
	char **lines = (char **)malloc(sizeof(char *) * allocateLinesCount);
	if (lines == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	FILE *fp = fopen(fileName, "r");
	for (i = 0; i < allocateLinesCount; i++)
	{
		lines[i] = malloc(LINE_MAX);
		fgets(lines[i], LINE_MAX, fp);
	}
	fclose(fp);
	return lines;
}

void free_match_file_memory(int allocateLinesCount, char **lines)
{
	int i;
	for (i = 0; i < allocateLinesCount; i++)
	{
		free(lines[i]);
	}
	free(lines);
}

void free_memory(PrintLoopContext *printLoopCtx)
{
	free_match_file_memory(printLoopCtx->linesToMatchCount, printLoopCtx->linesToMatch);
	free_tokenization_context(printLoopCtx->tokenizationCtx1);
	free_tokenization_context(printLoopCtx->tokenizationCtx2);
}

void print_line_with_acceptable_count_reappearing_words(PrintLoopContext *printLoopCtx)
{
	int i;
	double ratio;
	double highestRatio = -1.0;
	for (i = 0; i < printLoopCtx->linesToMatchCount; i++)
	{
		if (highestRatio == -1.0)
		{
			highestRatio = count_reappearing_words(printLoopCtx, i);
		}
		else
		{
			ratio = count_reappearing_words(printLoopCtx, i);
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

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 3)
	{
		fprintf(
				stderr, 
				"please provide similarity ratio as first argument (0.00 - 1.00)\n"
				"and optionally tokenization delimiter as second argument\n"
			);
		return 1;
	}
	char lineToPrint[LINE_MAX];
	char *eptr;
	PrintLoopContext printLoopCtx = 
	{
		.delimiter = NULL,
		.lineToPrint = lineToPrint,
		.acceptableRatio = strtod(argv[1], &eptr),
		.tokenizationCtx1 = allocate_tokenization_context(),
		.tokenizationCtx2 = allocate_tokenization_context(),
		.linesToMatchCount = get_file_lines_count(IGNORE_FILE),
	};
	printLoopCtx.linesToMatch = get_ignore_file_lines_array(printLoopCtx.linesToMatchCount, IGNORE_FILE);
	if(argc == 3)
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
