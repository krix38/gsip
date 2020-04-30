#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define IGNORE_FILE "gsip.ignore"

#define INITIAL_TOKENS_ARRAY_SIZE 5
#define INITIAL_STRING_BUFFER_SIZE 1014

typedef struct
{
	char **tokens;
	char *tempBuffer;
	int tempBufferSize;
	int tokensCount;
	int tokensArraySize;
} TokenizationContext;

void initialize_tokens_array(TokenizationContext *ctx)
{
	int i;
	for (i = 0; i < ctx->tokensArraySize; i++)
	{
		ctx->tokens[i] = malloc(sizeof(char *) * INITIAL_STRING_BUFFER_SIZE);
	}
}

TokenizationContext *allocate_tokenization_context()
{
	TokenizationContext *ctx = (TokenizationContext *)malloc(sizeof(TokenizationContext));
	ctx->tempBuffer = malloc(sizeof(char *) * INITIAL_STRING_BUFFER_SIZE);
	ctx->tokens = (char **)malloc(sizeof(char *) * INITIAL_TOKENS_ARRAY_SIZE);
	ctx->tempBufferSize = INITIAL_STRING_BUFFER_SIZE;
	ctx->tokensArraySize = INITIAL_TOKENS_ARRAY_SIZE;
	ctx->tokensCount = 0;
	initialize_tokens_array(ctx);
	return ctx;
}

void reinitialize_tokenization_context(TokenizationContext *ctx)
{
	ctx->tokensCount = 0;
}

void prepare_temp_buffer(TokenizationContext *ctx, char *string)
{
	int stringSize = strlen(string);
	if (ctx->tempBufferSize < stringSize)
	{
		ctx->tempBuffer = realloc(ctx->tempBuffer, sizeof(char *) * stringSize);
		ctx->tempBufferSize = stringSize;
	}
	strcpy(ctx->tempBuffer, string);
}

void copy_and_possibly_realoc(char *firstString, char *secondString)
{
	int secondStringSize = strlen(secondString);
	if (INITIAL_STRING_BUFFER_SIZE < secondStringSize && strlen(firstString) < secondStringSize)
	{
		firstString = realloc(firstString, sizeof(char *) * secondStringSize);
	}
	strcpy(firstString, secondString);
}

void tokenize_string(char *string, const char *delimiter, TokenizationContext *ctx)
{
	int i, l;
	prepare_temp_buffer(ctx, string);
	char *token = strtok(ctx->tempBuffer, delimiter);
	for (i = 0; token != NULL; i++)
	{
		if (i == ctx->tokensArraySize)
		{
			ctx->tokensArraySize = ctx->tokensArraySize * 2;
			ctx->tokens = (char **)realloc(ctx->tokens, sizeof(char *) * ctx->tokensArraySize);
			for (l = i; l < ctx->tokensArraySize; l++)
			{
				ctx->tokens[l] = malloc(sizeof(char *) * INITIAL_STRING_BUFFER_SIZE);
			}
		}
		copy_and_possibly_realoc(ctx->tokens[i], token);
		token = strtok(NULL, delimiter);
	}
}

void free_tokenization_context(TokenizationContext *ctx)
{
	int i;
	for (i = 0; i < ctx->tokensArraySize; i++)
	{
		free(ctx->tokens[i]);
	}
	free(ctx->tokens);
	free(ctx->tempBuffer);
	free(ctx);
}

double count_reappearing_words(char *firstString, char *secondString, TokenizationContext *ctx1, TokenizationContext *ctx2)
{
	const char delimiter[] = " ";
	int i, k, reappearingCount, tokensInFirstStringCount;

	tokenize_string(firstString, delimiter, ctx1);
	tokenize_string(secondString, delimiter, ctx2);

	reappearingCount = 0;
	for (i = 0; i < ctx1->tokensCount; i++)
	{
		for (k = 0; k < ctx2->tokensCount; k++)
		{
			if (strcmp(ctx1->tokens[i], ctx2->tokens[k]) == 0)
			{
				reappearingCount++;
			}
		}
	}

	tokensInFirstStringCount = ctx1->tokensCount;

	reinitialize_tokenization_context(ctx1);
	reinitialize_tokenization_context(ctx2);

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

void free_memory(int allocateLinesCount, char **lines)
{
	int i;
	for (i = 0; i < allocateLinesCount; i++)
	{
		free(lines[i]);
	}
	free(lines);
}

void print_line_with_acceptable_count_reappearing_words(double acceptableRatio, int linesToMatchCount, char **linesToMatch, char *lineToPrint, TokenizationContext *ctx1, TokenizationContext *ctx2)
{
	int i;
	double ratio;
	double highestRatio = -1.0;
	for (i = 0; i < linesToMatchCount; i++)
	{
		if (highestRatio == -1.0)
		{
			highestRatio = count_reappearing_words(lineToPrint, linesToMatch[i], ctx1, ctx2);
		}
		else
		{
			ratio = count_reappearing_words(lineToPrint, linesToMatch[i], ctx1, ctx2);
			if (ratio > highestRatio)
			{
				highestRatio = ratio;
			}
		}
	}
	if (highestRatio >= 0.0 && highestRatio <= acceptableRatio)
	{
		printf("%s", lineToPrint);
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "please provide similarity ratio as argument (0.00 - 1.00)\n");
		return 1;
	}
	char buffer[LINE_MAX];
	char *eptr;
	double acceptableRatio = strtod(argv[1], &eptr);
	int allocateLinesCount = get_file_lines_count(IGNORE_FILE);
	char **linesToMatch = get_ignore_file_lines_array(allocateLinesCount, IGNORE_FILE);
	TokenizationContext *ctx1 = allocate_tokenization_context();
	TokenizationContext *ctx2 = allocate_tokenization_context();
	while (fgets(buffer, LINE_MAX, stdin))
	{
		print_line_with_acceptable_count_reappearing_words(acceptableRatio, allocateLinesCount, linesToMatch, buffer, ctx1, ctx2);
	}
	free_memory(allocateLinesCount, linesToMatch);
	free_tokenization_context(ctx1);
	free_tokenization_context(ctx2);
	return 0;
}
