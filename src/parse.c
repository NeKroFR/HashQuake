#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"
#include "help.h"

int is_positive_integer(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

void parse_arguments(int argc, char *argv[], int *word_length, char **wordlist_file, char **algorithm_command, char **output_file, int *display_mode) {
    // Function to parse command-line arguments
    if (argc < 2) {
        help_message(1);
    }

    *word_length = 3;
    *wordlist_file = NULL;
    *algorithm_command = NULL;
    *output_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help_message(0);
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--display") == 0) {
            *display_mode = 1;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            version();
        } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--wordlist") == 0) {
            if (i + 1 < argc) {
                *wordlist_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing argument for -w/--wordlist\n");
                help_message(1);
            }
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0) {
            if (i + 1 < argc) {
                if (is_positive_integer(argv[i + 1])) {
                    *word_length = atoi(argv[++i]);
                } else {
                    fprintf(stderr, "Invalid size specified. Using default value: 3\n");
                    *word_length = 3;
                }
            } else {
                fprintf(stderr, "Error: Missing argument for -s/--size\n");
                help_message(1);
            }
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) {
                *output_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing argument for -o/--output\n");
                help_message(1);
            }
        } else {
            *algorithm_command = argv[i];
        }
    }

    if (*algorithm_command == NULL || *algorithm_command[0] == '-') {
        fprintf(stderr, "Error: Command to run the algorithm must be specified.\n");
        help_message(1);
    }
}
