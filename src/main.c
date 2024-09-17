#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "help.h"

int main(int argc, char *argv[]) {
    int word_length;
    char *wordlist_file;
    char *algorithm_command;
    char *output_file;

    // Display values for debugging
    parse_arguments(argc, argv, &word_length, &wordlist_file, &algorithm_command, &output_file);
    printf("Parsed values:\n");
    printf("  Word length: %d\n", word_length);
    printf("  Wordlist file: %s\n", wordlist_file ? wordlist_file : "None");
    printf("  Algorithm command: %s\n", algorithm_command ? algorithm_command : "None");
    printf("  Output file: %s\n", output_file ? output_file : "None");
    return 0;
}
