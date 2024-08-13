#include "hash_collider.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void help() {
    printf("Usage: ./hash_collider [OPTIONS] <algorithm_file>\n");
    printf("Options:\n");
    printf("  -h, --help            Display this help message\n");
    printf("  -v, --version         Display the version of the program\n");
    printf("  -d, --dict <file>     Specify a dictionary file with words for collision testing\n");
    printf("  -w, --word-len <num>  Specify the length of words to generate for collision testing\n");
    printf("  -s, --hash-size <num> Specify the size (in bytes) of the generated hash (default: 3)\n");
    printf("\nExample:\n");
    printf("  ./hash_collider -d dictionary.txt -w 3 -s 4 blake2b\n");
    exit(0);
}

void version() {
    printf("1.0\n");
    exit(0);
}


void parse_arguments(int argc, char *argv[], int *hash_size, int *word_length, char **dict_file, char **algorithm_file) {
    *hash_size = HASH_SIZE;
    *word_length = WORD_LENGTH;
    *dict_file = NULL;
    *algorithm_file = NULL;
    int i = 1;
    while (i < argc){
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help();
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            version();
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dict") == 0) {
            if (i++ < argc) {
                *dict_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing argument for -d/--dict\n");
                help();
            }
        } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--word-len") == 0) {
            if (i++ < argc) {
                *word_length = atoi(argv[++i]);
                if (*word_length <= 0) {
                    fprintf(stderr, "Invalid word length specified. Using default value: %d\n", WORD_LENGTH);
                    *word_length = WORD_LENGTH;
                }
            } else {
                fprintf(stderr, "Error: Missing argument for -w/--word-len\n");
                help();
            }
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--hash-size") == 0) {
            if (i++ < argc) {
                *hash_size = atoi(argv[++i]);
                if (*hash_size <= 0) {
                    fprintf(stderr, "Invalid hash size specified. Using default value: %d\n", HASH_SIZE);
                    *hash_size = HASH_SIZE;
                }
            } else {
                fprintf(stderr, "Error: Missing argument for -s/--hash-size\n");
                help();
            }
        } else {
            *algorithm_file = argv[i];
        }
        i++;
    }

    if (*algorithm_file == NULL || strncmp(*algorithm_file, "-", 1) == 0){
        fprintf(stderr, "Error: Algorithm file must be specified.\n");
        help();
    }
}

int main(int argc, char *argv[]) {
    int hash_size = HASH_SIZE;
    int word_length = WORD_LENGTH;
    char *dict_file = NULL;
    char *algorithm_file = NULL;

    parse_arguments(argc, argv, &hash_size, &word_length, &dict_file, &algorithm_file);


    // Display information:
    printf("Hash Size: %d\n", hash_size);
    printf("Word Length: %d\n", word_length);
    if (dict_file) {
        printf("Dictionary File: %s\n", dict_file);
    } else {
        printf("No dictionary file specified.\n");
    }
    printf("Algorithm File: %s\n", algorithm_file);
    return 0;
}
