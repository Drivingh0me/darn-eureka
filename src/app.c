#include "app.h"

#include "export.h"
#include "error.h"
#include <target.h>

#include <stdio.h>
#include <stdlib.h>

#define ARG_MAX 16
#define VERSION "Darn Eureka v0.1.1 (pre-alpha)\n"

typedef enum {
    TEST = 1,
    DEBUG = 2,
    SILENT = 9,
} Flag;

typedef struct {
    int flag[ARG_MAX];
    int arg[2 * ARG_MAX];
} ArgParse;

int same_str(char* sample, char *key)
{
    int i = 0;
    while (key[i] != '\0') {
        if (sample[i] != key[i]) {
            return 0;
        }
        i++;
    }
    if (sample[i] != '\0') {
        return 0;
    }
    return 1;
}

int get_short_flag(char c)
{
    switch (c) {
        case 't':
            /* Test app for speed and stability */
            return 1;
        case 'd':
            /* Run game in debug mode */
            return 2;
        default:
            return INVALID_ARG;
    }
}

int get_long_flag(char *c)
{
    if (same_str(c, "--silent")) {
        return 9;
    }
}

static void parse_args(int argc, char **argv, ArgParse *args)
{
    int i;
    int x = 0;

    if (argc <= 1) {
        /* If no args, run interpreter */
        args->flag[0] = 4;
        args->flag[1] = 0;
        return;
    }

    /* Check for --version and --help explicitely */
    if (argc == 2) {
        if (same_str(argv[1], "--version")) {
            printf(VERSION);
            args->flag[0] = 0;
            return;
        }

        if (same_str(argv[1], "--help")) {
            printf("Help not implemented\n");
            args->flag[0] = 0;
            return;
        }
    }

    for (i = 1; i < argc && x < ARG_MAX; i++) {
        if (argv[i][0] == '-') {
            /* Check for --long */
            if (argv[i][1] == '-') {
                args->flag[x] = get_long_flag(argv[i]);
            }
            args->flag[x] = get_short_flag(argv[i][1]);
            args->arg[2 * x] = i + 1;
            if (x > 0) {
                args->arg[2 * x - 1] = i - 1;
            }

            if (args->flag[x] == INVALID_ARG) {
                /* Not a flag, error */
                args->flag[0] = INVALID_ARG;
                args->flag[1] = 0;
            }

            x++;
        }
    }

    /* terminates flag */
    args->flag[x] = 0;

    /* Set previous arg to end */
    if (x > 0) {
        args->arg[2 * x - 1] = i - 1;
    }
}

int set_game_info(ArgParse args, char **argv)
{
    int i = 0;
    int status = 0;

    while (args.flag[i] > 0) {
        /* must match args */
        switch (args.flag[i]) {
            case 0:
                run_game();
            case 1:
                /* Test */
                break;
            case 2:
                /* Debug */
                break;
            case 9:
                /* silent */
                break;
            default:
                return INVALID_VAL;
        }
        i++;
    }
    return status;
}

/* Not implemented */
int app_startup(GameInfo *info, char* message)
{
    int mem;
    int status;

    status = TARG_mem_limit(&mem);

    return 0;
}

int run_app(int argc, char **argv, GameInfo* info, char* message)
{
    int status = 0;
    ArgParse args;

    /* Use this to set debug mode, and test state for info */
    if (argc > 1) {
        status = parse_args(argc, argv, &args);
        if (status) {
            return status;
        }
        return set_game_info(args, argv, info);
    }

    return run_game(info, message);
}

void app_shutdown(GameInfo *info)
{
    /* Save temp files and free memory here. */
}
