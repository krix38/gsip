#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define IGNORE_FILE "gsip.ignore"

#define INITIAL_TOKENS_ARRAY_SIZE 5

typedef struct {
        char **tokens;
        int tokensCount;
        int tokensArraySize;
} TokenizationContext;

TokenizationContext *tokenize_string(char *string, const char *delimiter)
{
        int i, k;
        char **tokens = NULL;
	char *bufferedString = NULL;
	bufferedString = malloc(sizeof(char*) * strlen(string));
	strcpy(bufferedString, string);
        char *token = strtok(bufferedString, delimiter);
        int tokensArraySize = INITIAL_TOKENS_ARRAY_SIZE;
        tokens = (char **)malloc(sizeof(char*) * tokensArraySize);
        for(i = 0; token != NULL; i++)
        {
                if(i == tokensArraySize)
                {
                        tokensArraySize = tokensArraySize * 2;
                        tokens = (char **)realloc(tokens, sizeof(char*) * tokensArraySize);
                }
                tokens[i] = malloc(sizeof(char*) * strlen(token));
                strcpy(tokens[i], token);
                token = strtok(NULL, delimiter);
	}
        for(k = i; k < tokensArraySize; k++)
        {
                tokens[k] = NULL;
        }
	free(bufferedString);
        TokenizationContext *ctx = (TokenizationContext *) malloc(sizeof(TokenizationContext));
        ctx->tokens = tokens;
        ctx->tokensCount = i;
        ctx->tokensArraySize = tokensArraySize;
        return ctx;
}

void free_tokenization_context(TokenizationContext *ctx)
{
        int i;
        for(i = 0; i < ctx->tokensArraySize; i++)
        {
                free(ctx->tokens[i]);
        }
        free(ctx->tokens);
        free(ctx);
}


double count_reappearing_words(char *firstString, char *secondString)
{
	const char delimiter[] = " ";
	int i, k, reappearingCount, tokensInFirstStringCount;
	
	TokenizationContext *ctx1 = tokenize_string(firstString, delimiter);
	TokenizationContext *ctx2 = tokenize_string(secondString, delimiter);

	reappearingCount = 0;
	for(i = 0; i < ctx1->tokensCount; i++)
	{
		for(k = 0; k < ctx2->tokensCount; k++)
		{
			if(strcmp(ctx1->tokens[i], ctx2->tokens[k]) == 0)
			{
				reappearingCount++;
			}
		}
	}
	
	tokensInFirstStringCount = ctx1->tokensCount;

	free_tokenization_context(ctx1);
	free_tokenization_context(ctx2);


	if(tokensInFirstStringCount > 0)
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
	if(fp == NULL)
	{
		fprintf(stderr, "failed to open %s\n", fileName);
		exit(1);
	}

	for(c = getc(fp); c != EOF; c = getc(fp))
	{
		if(c == '\n')
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
	char **lines = (char **)malloc(sizeof(char*) * allocateLinesCount);
	if(lines == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	FILE *fp = fopen(fileName, "r");
	for(i = 0; i < allocateLinesCount; i++)
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
	for(i = 0; i < allocateLinesCount; i++)
	{
		free(lines[i]);
	}
	free(lines);
}

void print_line_with_acceptable_count_reappearing_words(double acceptableRatio, int linesToMatchCount, char **linesToMatch, char *lineToPrint)
{
	int i;
	double ratio;
	double highestRatio = -1.0;
	for(i = 0; i < linesToMatchCount; i++)
	{	
		if(highestRatio == -1.0)
		{
			highestRatio = count_reappearing_words(lineToPrint, linesToMatch[i]);
		}
		else
		{
			ratio = count_reappearing_words(lineToPrint, linesToMatch[i]);
			if(ratio > highestRatio)
			{
				highestRatio = ratio;
			}
		}
	}
	if(highestRatio >= 0.0 && highestRatio <= acceptableRatio)
	{
		printf("%s",lineToPrint);
	}
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "please provide similarity ratio as argument (0.00 - 1.00)\n");
		return 1;
	}
	char buffer[LINE_MAX];
	char *eptr;
	double acceptableRatio = strtod(argv[1], &eptr);
	int allocateLinesCount = get_file_lines_count(IGNORE_FILE);
	char **linesToMatch = get_ignore_file_lines_array(allocateLinesCount, IGNORE_FILE);
	while(fgets(buffer, LINE_MAX, stdin))
	{
		print_line_with_acceptable_count_reappearing_words(acceptableRatio, allocateLinesCount, linesToMatch, buffer);
	}
	free_memory(allocateLinesCount, linesToMatch);
	return 0;
}

