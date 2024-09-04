#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_wordlist(char **wordlist, int total_words) {
    if (wordlist) {
        for (int i = 0; i < total_words; ++i) {
            free(wordlist[i]);
        }
        free(wordlist);
    }
}

char **generate_wordlist(int word_length, const char *filename, int *total_words) {
    if (word_length < 1) {
        fprintf(stderr, "Invalid word length specified. It must be greater than 0.\n");
        *total_words = 0;
        return NULL;
    }

    int num_words = 1;
    for (int i = 0; i < word_length; ++i) {
        num_words *= 26;
    }

    char **wordlist = (char **)malloc(num_words * sizeof(char *));
    if (!wordlist) {
        perror("Failed to allocate memory for wordlist");
        *total_words = 0;
        return NULL;
    }

    char *word = (char *)malloc(word_length + 1);
    if (!word) {
        perror("Failed to allocate memory for word");
        free(wordlist);
        *total_words = 0;
        return NULL;
    }
    memset(word, 'a', word_length);
    word[word_length] = '\0';

    int index = 0;
    while (index < num_words) {
        wordlist[index] = strdup(word);
        if (!wordlist[index]) {
            perror("Failed to allocate memory for word copy");
            break;
        }
        ++index;

        for (int i = word_length - 1; i >= 0; --i) {
            if (word[i] < 'z') {
                ++word[i];
                break;
            } else {
                word[i] = 'a';
            }
        }

        if (index >= num_words) {
            break;
        }
    }

    free(word);

    if (filename) {
        FILE *file = fopen(filename, "w");
        if (!file) {
            perror("Failed to open file");
            free_wordlist(wordlist, index);
            *total_words = 0;
            return NULL;
        }

        for (int i = 0; i < index; ++i) {
            fprintf(file, "%s\n", wordlist[i]);
        }

        fclose(file);
    } else {
        for (int i = 0; i < index; ++i) {
            printf("%s\n", wordlist[i]);
        }
        free_wordlist(wordlist, index);
    }

    *total_words = index;
    return wordlist;
}

char **read_dict_file(const char *filename, int *total_words) {
    printf("Reading dictionary file %s...\n", filename);
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        *total_words = 0;
        return NULL;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        ++count;
    }
    rewind(file);

    char **wordlist = (char **)malloc(count * sizeof(char *));
    if (!wordlist) {
        perror("Failed to allocate memory for wordlist");
        *total_words = 0;
        fclose(file);
        return NULL;
    }

    int index = 0;
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        wordlist[index] = strdup(line);
        if (!wordlist[index]) {
            perror("Failed to allocate memory for word copy");
            break;
        }
        ++index;
    }

    if (ferror(file)) {
        perror("Error reading file");
    }

    fclose(file);
    *total_words = index;
    return wordlist;
}
