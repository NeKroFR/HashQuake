#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "help.h"
#include "collider.h"
#include "posix.h"



char* read_wordlist(const char *filename, int *word_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    int count = 0;
    int capacity = 1024;
    char *content = malloc(capacity);
    if (!content) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return NULL;
    }

    char c;
    int pos = 0;
    while ((c = fgetc(file)) != EOF) {
        if (pos >= capacity-1) {
            capacity *= 2;
            char *new_content = realloc(content, capacity);
            if (!new_content) {
                fprintf(stderr, "Memory reallocation error\n");
                free(content);
                fclose(file);
                return NULL;
            }
            content = new_content;
        }
        content[pos++] = c;
        
        if (c == '\n') {
            count++;
        }
    }
    content[pos] = '\0';
    *word_count = count;
    fclose(file);
    return content;
}


int main(int argc, char *argv[]) {
    int word_length = 0;
    int display_mode = 0;
    char *wordlist_file = NULL;
    char *algorithm_command = NULL;
    char *output_file = NULL;
    char *filename = NULL;
    char *wordlist = NULL;
    int word_count = 27;

    parse_arguments(argc, argv, &word_length, &wordlist_file, &algorithm_command, &output_file, &display_mode);
    // Display values for debugging
    /*
    printf("Word Length: %d\n", word_length);
    printf("Wordlist File: %s\n", wordlist_file);
    printf("Algorithm Command: %s\n", algorithm_command);
    printf("Output File: %s\n", output_file);
    printf("Display Mode: %d\n", display_mode);
    */

    if (wordlist_file) {
        printf("Wordlist File: %s\n", wordlist_file);
        wordlist = read_wordlist(wordlist_file, &word_count);
    } else {
        word_count = (int)pow(27, word_length);
        printf("No wordlist file specified.\n");
        printf("Word Length: %d\n", word_length);
        char choice[2];
        printf("Do you want to save the generated wordlist to a file? (y/n): ");
        fgets(choice, sizeof(choice), stdin);
        if (choice[0] == 'y' || choice[0] == 'Y') {
            printf("Enter filename (default is 'wordlist.txt'): ");
            char tmp[100];
            if (scanf("%99s", tmp) == 1) { // Cannot set empty strings so never set the filename to 'wordlist.txt'
                filename = (strlen(tmp) > 0) ? strdup(tmp) : "wordlist.txt";
            } else {
                filename = "wordlist.txt";
            }
            printf("\nExporting wordlist to: %s\n", filename);
        }
    }

    collide(algorithm_command, wordlist, word_count, word_length, output_file, filename, display_mode);

    // Free allocated memory
    if (wordlist) {
        for (int i = 0; i < word_count; i++) {
            free(wordlist[i]);
        }
        free(wordlist);
    }
    if (filename && strcmp(filename, "wordlist.txt") != 0) {
        free(filename);
    }

    return 0;
}
