#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hashing(const char* word) {
    return (char*)word;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: No word specified.\n");
        return 1;
    }
    size_t len = 0;
    for (int i = 1; i < argc; i++) {
        len += strlen(argv[i]) + 1;
    }
    char* result = malloc(len);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }
    result[0] = '\0';
    for (int i = 1; i < argc; i++) {
        strcat(result, argv[i]);
        if (i < argc - 1)
            strcat(result, " ");
    }
    printf("%s\n", hashing(result));
    free(result);
    return 0;
}

