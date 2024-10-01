#include <stdio.h>
#include <stdlib.h>
#include "help.h"

void help_message(int exit_status) {
    printf("Usage: ./hash_collider [OPTIONS] <command to run the algorithm>\n");
    printf("\nDisclaimer:\n");
    printf("   The program will run the command and add the word as argument at the end.\n");
    printf("   The program expects to get the hash in stdout. If you want to specify other\n");
    printf("   arguments to your function, do it before because the word will be the last argument.\n");
    printf("\nOptions:\n");
    printf("   -h, --help            Display this help message\n");
    printf("   -v, --version         Display the version of the program\n");
    printf("   -w, --wordlist <filepath>   Specify a wordlist file\n");
    printf("   -s, --size <num>      Specify the length of words to generate (default 3)\n");
    printf("   -d --display          Display the collisions in real time (default set to False)\n");
    printf("   -o, --output <filepath>      Specify an output file to store the report\n");
    printf("\nExample:\n");
    printf("    ./hash_collider -w dictionary.txt 'python3 md5.py'\n");
    printf("    ./hash_collider -s 8 -o report.txt -d './sha256'\n");
    exit(exit_status);
}

void version() {
    printf("HashQuake: 1.0\n");
    exit(0);
}
