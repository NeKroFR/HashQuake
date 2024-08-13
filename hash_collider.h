#ifndef HASH_COLLIDER_H
#define HASH_COLLIDER_H

#include <getopt.h>

// Default values
#define WORD_LENGTH 3
#define HASH_SIZE 3

// Function declarations
void help();
void version();
void parse_arguments(int argc, char *argv[], int *hash_size, int *word_length, char **dict_file, char **algorithm_file);

#endif // HASH_COLLIDER_H
