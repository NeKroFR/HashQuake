#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hashing(const char* word) {
    // This function creates a 16-bit hash by processing the input string in 2-byte blocks
    // and XORing them together. Each block is created by shifting the first byte left by 8
    // and combining it with the second byte using OR.

    // It will cause collisions when:
    // 1. Two strings have the same pairs of bytes in different orders (e.g., "abcd" vs "cdab")
    // 2. Strings have repeating 2-byte patterns that cancel out through XOR
    // 3. Different strings whose 2-byte blocks XOR to the same value
    // 4. Strings longer than 2 bytes where subsequent blocks cancel out previous ones

    // Example:
    // "ABCDEF" -> Process as blocks "AB" "CD" "EF"
    // Block 1: 'A'(65) << 8 | 'B'(66)   = 0x4142
    // Block 2: 'C'(67) << 8 | 'D'(68)   = 0x4344
    // Block 3: 'E'(69) << 8 | 'F'(70)   = 0x4546
    // Result: 0x4142 ^ 0x4344 ^ 0x4546  = 0x4740
    // hash value encoded in base32: I5AA
    // final hash value: I5AA

    static char hash[5];
    size_t len = strlen(word);
    unsigned short result = 0;

    for (size_t i = 0; i < len; i += 2) {
        unsigned short block = (unsigned char)word[i];

        if (i + 1 < len)
            block = (block << 8) | (unsigned char)word[i + 1];

        result ^= block;
    }

    // Convert to base32
    static const char base32_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    
    hash[0] = base32_chars[(result >> 11) & 0x1F];
    hash[1] = base32_chars[(result >> 6) & 0x1F];
    hash[2] = base32_chars[(result >> 1) & 0x1F];
    hash[3] = base32_chars[(result & 0x01) << 4];
    hash[4] = '\0';

    return hash;

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

