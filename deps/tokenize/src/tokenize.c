#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <utils.h>
#include <tokenize.h>

#define INITIAL_TOKENS_ARRAY_SIZE 10

static void initialize_tokens_array(TokenizationContext *ctx)
{
	int i;
	for (i = 0; i < ctx->tokensArraySize; i++)
	{
		ctx->tokens[i] = create_string();
	}
}

TokenizationContext *initialize_tokenization_context(TokenizationContext *ctx)
{
	ctx->tempBuffer = create_string();
	ctx->tokens = (String **)allocate(sizeof(String) * INITIAL_TOKENS_ARRAY_SIZE);
	ctx->tokensArraySize = INITIAL_TOKENS_ARRAY_SIZE;
	ctx->tokensCount = 0;
	initialize_tokens_array(ctx);
	return ctx;
}

void reinitialize_tokenization_context(TokenizationContext *ctx)
{
	ctx->tokensCount = 0;
}

void tokenize_string(char *string, const char *delimiter, TokenizationContext *ctx)
{
	int i, l;
	String **reallocatedTokens;
	ctx->tempBuffer = set_string(ctx->tempBuffer, string);
	char *token = strtok(ctx->tempBuffer->string, delimiter);
	for (i = 0; token != NULL; i++)
	{
		if (i == ctx->tokensArraySize)
		{
			ctx->tokensArraySize = ctx->tokensArraySize * 2;
			reallocatedTokens = (String **)realloc(ctx->tokens, sizeof(String) * ctx->tokensArraySize);
			if (reallocatedTokens == NULL)
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
}
