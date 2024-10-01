#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 27 // should find something better than 27^word_length because it will get too big

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct HashTable {
    Node *table[TABLE_SIZE];
} HashTable;

Node *create_node(char *key, int value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

unsigned int hash(char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

HashTable *create_table() {
    HashTable *hash_table = (HashTable *)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table->table[i] = NULL;
    }
    return hash_table;
}

void insert(HashTable *hash_table, char *key, int value) {
    unsigned int index = hash(key);
    Node *new_node = create_node(key, value);    
    if (hash_table->table[index] == NULL) {
        hash_table->table[index] = new_node;
    } else {
        new_node->next = hash_table->table[index];
        hash_table->table[index] = new_node;
    }
}

int search(HashTable *hash_table, char *key) {
    unsigned int index = hash(key);
    Node *temp = hash_table->table[index];    
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            return temp->value;
        }
        temp = temp->next;
    }
    return -1;
}

void delete(HashTable *hash_table, char *key) {
    unsigned int index = hash(key);
    Node *temp = hash_table->table[index];
    Node *prev = NULL;
    
    while (temp != NULL && strcmp(temp->key, key) != 0) {
        prev = temp;
        temp = temp->next;
    }    
    if (temp == NULL) {
        printf("Key not found\n");
        return;
    }
    if (prev == NULL) {
        hash_table->table[index] = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp->key);
    free(temp);
    printf("Key deleted\n");
}

void print_table(HashTable *hash_table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: ", i);
        Node *temp = hash_table->table[i];
        while (temp != NULL) {
            printf("(%s, %d) -> ", temp->key, temp->value);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}
