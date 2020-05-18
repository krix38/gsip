#include <configuration.h>
#include <unistd.h>
#include <stdlib.h>

#define DEFAULT_TOKEN_DELIMITER " \n"

static void set_defaults(Configuration *cfg)
{
    int i;
    cfg->acceptableRatio = 0.0;
    cfg->delimiter = DEFAULT_TOKEN_DELIMITER;
    cfg->matchFileName = NULL;
    cfg->showUsage = false;
    cfg->printRatio = false;
    cfg->reverseMatch = false;
    cfg->selectedOptionsCount = 0;
    for (i = 0; i < MAX_SELECTED_OPTIONS; i++)
    {
        cfg->selectedOptions[i] = -1;
    }
}

static bool is_option_selected(Option *options, Option option)
{
    int i;
    for (i = 0; i < MAX_SELECTED_OPTIONS; i++)
    {
        if (options[i] == option)
        {
            return true;
        }
    }
    return false;
}

static bool all_mandatory_options_selected(Configuration *cfg)
{
    return is_option_selected(cfg->selectedOptions, MatchFileName) && is_option_selected(cfg->selectedOptions, AcceptableRatio);
}

static void validate_config(Configuration *cfg)
{
    if (cfg->selectedOptionsCount == 0 || !all_mandatory_options_selected(cfg))
    {
        cfg->showUsage = true;
    }
}

static void add_selected_option(Configuration *cfg, Option option)
{
    cfg->selectedOptions[cfg->selectedOptionsCount] = option;
    if (cfg->selectedOptionsCount < MAX_SELECTED_OPTIONS)
    {
        cfg->selectedOptionsCount++;
    }
}

Configuration *get_configuration(Configuration *cfg, int argc, char **argv)
{
    char c;
    set_defaults(cfg);
    while ((c = getopt(argc, argv, "hvrf:a:d:")) != -1)
    {
        switch (c)
        {
        case 'h':
            cfg->showUsage = true;
            add_selected_option(cfg, ShowUsage);
            break;
        case 'v':
            cfg->printRatio = true;
            add_selected_option(cfg, PrintRatio);
            break;
        case 'r':
            cfg->reverseMatch = true;
            add_selected_option(cfg, ReverseMatch);
            break;
        case 'f':
            cfg->matchFileName = optarg;
            add_selected_option(cfg, MatchFileName);
            break;
        case 'a':
            cfg->acceptableRatio = strtod(optarg, NULL);
            add_selected_option(cfg, AcceptableRatio);
            break;
        case 'd':
            cfg->delimiter = optarg;
            add_selected_option(cfg, Delimiter);
            break;
        default:
            break;
        }
    }
    validate_config(cfg);
    return cfg;
}