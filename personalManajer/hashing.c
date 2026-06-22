#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct HashTable {
    struct Node** buckets;  // Array of linked list heads
    int size;               // Number of buckets
};

// Create a new hash table
struct HashTable* createHashTable(int size) {
    struct HashTable* ht = (struct HashTable*)malloc(sizeof(struct HashTable));
    ht->size = size;
    
    // Allocate array of linked list heads
    ht->buckets = (struct Node**)malloc(size * sizeof(struct Node*));
    
    // Initialize all buckets to NULL
    for (int i = 0; i < size; i++) {
        ht->buckets[i] = NULL;
    }
    
    return ht;
}

// Hash function (simple modulo)
int hash(int key, int size) {
    return key % size;
}

// Insert into appropriate bucket
void insert(struct HashTable* ht, int value) {
    int index = hash(value, ht->size);
    
    // Create new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    
    // Insert at beginning of bucket's list
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
    
    printf("Inserted %d into bucket %d\n", value, index);
}

// Print entire hash table
void printHashTable(struct HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        printf("Bucket %d: ", i);
        struct Node* current = ht->buckets[i];
        while (current != NULL) {
            printf("%d -> ", current->data);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main() {
    // Create hash table with 5 buckets
    struct HashTable* ht = createHashTable(5);
    
    // Insert numbers (they'll be distributed by modulo 5)
    int numbers[] = {10, 15, 22, 37, 42, 57, 63, 78, 81, 99};
    int numCount = sizeof(numbers) / sizeof(numbers[0]);
    
    for (int i = 0; i < numCount; i++) {
        insert(ht, numbers[i]);
    }
    
    printf("\nFinal Hash Table:\n");
    printHashTable(ht);
    
    // Clean up (in real code, you'd free all memory)
    
    return 0;
}