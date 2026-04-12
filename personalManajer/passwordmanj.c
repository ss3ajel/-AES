#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_SIZE 10

typedef struct {
    char pw[8];
    char email[20];
} sec;

sec *hash_table[TABLE_SIZE];

unsigned int hash(char *pw) {
    int length = strnlen(pw, 8);
    unsigned int hash_value = 0;
    for (int i = 0; i < length; i++) {
        hash_value += pw[i];
        hash_value = (hash_value * pw[i]) % TABLE_SIZE;
    }
    return hash_value;
}

void init_table_hash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

void print_table() {
    printf("\nHash Table Contents:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] == NULL) {
            printf("\t%d\t---\n", i);
        } else {
            printf("\t%d\t%s\t%s\n", i, hash_table[i]->pw, hash_table[i]->email);
        }
    }
}

bool hash_table_insert(sec *s) {
    if (s == NULL) return false;
    int index = hash(s->pw);
    if (hash_table[index] != NULL) {
        return false;
    }
    hash_table[index] = s;
    return true;
}
bool hash_table_delete(sec *s) {
    if (s == NULL) return false;
    int index = hash(s->pw);
    if (hash_table[index] != NULL &&strcmp(hash_table[index]->pw, s->pw) == 0){
        
    hash_table[index] = NULL;
    return true;
}
}


sec *hash_table_look(char *pw) {
    int index = hash(pw);
    if (hash_table[index] != NULL && strcmp(hash_table[index]->pw, pw) == 0) {
        return hash_table[index];
    } else {
        return NULL;
    }
}

int main() {
    FILE *fP = fopen("/home/borhen20/Documents/pass.txt", "r");
    char buffer[225];
    
    if (fP == NULL) {
        printf("Unable to reach file\n");
        return 1;
    }
    
    init_table_hash();
    
    int success_count = 0;
    int fail_count = 0;
    
    while (fgets(buffer, sizeof(buffer), fP) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  
        if (strlen(buffer) == 0) continue;
        
        sec *new = malloc(sizeof(sec));
        if (new == NULL) {
            printf("Memory allocation failed\n");
            continue;
        }
        
        int parsed = sscanf(buffer, "%7s %19s", new->pw, new->email);
        if (parsed == 2) {
            unsigned int index = hash(new->pw);
            if (hash_table_insert(new)) {
                printf("✓ Inserted: %s - %s at index %u\n", new->pw, new->email, index);
                success_count++;
            } else {
                printf("✗ Failed insertion for %s - slot %u already occupied\n", new->pw, index);
                free(new);
                fail_count++;
            }
        } else {
            printf("✗ Failed parsing line: %s\n", buffer);
            free(new);
            fail_count++;
        }
         hash_table_delete(new);

    }
    
    fclose(fP);  
    
    printf("\nSuccessfully inserted: %d", success_count);
    printf("\nFailed: %d", fail_count);
    
    print_table();
    
    printf("\nlooking for password 'oresai17'...\n");
    sec *tmp = hash_table_look("oresai1");
    if (tmp == NULL) {
        printf(" not found\n");
    } else {
        printf("found: Email: %s, Password: %s\n", tmp->email, tmp->pw);
    }
   
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] != NULL) {
            free(hash_table[i]);
        }
    }
    
    printf("\nMemory freed. Program completed.\n");
    
    return 0;
}