#ifndef STDBOOL_HEADER
#define STDBOOL_HEADER

#include <stdbool.h>

#endif

#ifndef OPTIONS_ENUM
#define OPTIONS_ENUM

#define MAX_SELECTED_OPTIONS 6

typedef enum
{
    AcceptableRatio,
    Delimiter,
    MatchFileName,
    ShowUsage,
    PrintRatio,
    ReverseMatch
} Option;

#endif

#ifndef CONFIGURATION_STRUCT
#define CONFIGURATION_STRUCT

typedef struct
{
    double acceptableRatio;
    char *delimiter;
    char *matchFileName;
    bool showUsage;
    bool printRatio;
    bool reverseMatch;
    Option selectedOptions[MAX_SELECTED_OPTIONS];
    int selectedOptionsCount;
} Configuration;

#endif

Configuration *get_configuration(Configuration *cfg, int argc, char **argv);