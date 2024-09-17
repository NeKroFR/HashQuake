#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hashing(const char* word) {
    return strdup(word);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: No word specified.\n");
        return 1;
    }

    size_t len = 0;
    for (int i = 1; i < argc; i++) {
        len += strlen(argv[i]) + 1;
    }

    // if there is multiple arguments, concatenate them.
    char* word = malloc(len+1);
    if (!word) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    word[0] = '\0';
    for (int i = 1; i < argc; i++) {
        strcat(word, argv[i]);
        if (i < argc - 1) {
            strcat(word, " ");
        }
    }

    char* res = hashing(word);
    printf("%s\n", res);

    free(word);
    free(res);
    return 0;
}
