#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <utils.h>

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
	char **lines = (char **)allocate(sizeof(char *) * allocateLinesCount);
	if (lines == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	FILE *fp = fopen(fileName, "r");
	for (i = 0; i < allocateLinesCount; i++)
	{
		lines[i] = allocate(LINE_MAX);
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
