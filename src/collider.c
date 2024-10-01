#include <stdio.h>
#include <string.h>

const char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const int alphabet_size = sizeof(alphabet) - 1;

void next_word(char *str) {
    int len = strlen(str);

    for (int i = len - 1; i >= 0; i--) {
        int index = -1;
        for (int j = 0; j < alphabet_size; j++) {
            if (alphabet[j] == str[i]) {
                index = j;
                break;
            }
        }

        if (index == -1) {
            printf("Error: Invalid character '%c' in the input string\n", str[i]);
            return;
        }
        if (index == alphabet_size - 1) {
            str[i] = alphabet[0];
        } else {
            str[i] = alphabet[index + 1];
            return;
        }
    }
    char temp[len + 2];
    temp[0] = 'a';
    strcpy(temp + 1, str);
    strcpy(str, temp);
}


void collide(const char *algorithm_command, char *wordlist, long word_count, int word_length, const char *output_file,const char * filename, int display_mode) {
    // if  no wordlist file is specified then we set currentword to "a"*word_length else it will be the first word in the wordlist
    currentword = malloc(word_length + 1);
    if (wordlist) {
        // we set the word to the first word in the wordlist all chars until the first \n or \0
        int beg = 0;
        int end = 0;
        while (wordlist[end] != '\n' && wordlist[end] != '\0') {
            end++;
        }
    }
    else {
        char currentword[word_length + 1];
        for (int i = 0; i < word_length; i++) {
            currentword[i] = 'a';
        }
        currentword[word_length] = '\0';
    }
    int i = 0;
    int collisions = 0;
    while (i < word_length) {
        // hash = run command(word)
        // add word in hash table (key is the hash)
        // if filename -> write word + \n
        // if collision in the hashtable
        // collisions++
        // if output_file -> write collision
        // if display_mode: -> print collision
        // next_word(currentword)
        i++;
    }
    // print "collisions collisions on word_count words: {collisions/word_count} %"
}
