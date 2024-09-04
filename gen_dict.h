#ifndef GEN_DICT_H
#define GEN_DICT_H

void free_wordlist(char **wordlist, int total_words);
char **generate_wordlist(int word_length, const char *filename, int *total_words);
char **read_dict_file(const char *filename, int *total_words);

#endif // GEN_DICT_H
