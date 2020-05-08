int get_file_lines_count(char *fileName);
char **get_ignore_file_lines_array(int allocateLinesCount, char *fileName);
void free_match_file_memory(int allocateLinesCount, char **lines);