#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 1024
#define ALPHABET_SIZE 26

// Function to encrypt plaintext using the one-time pad Vigenère cipher
void encrypt(const char *plaintext, const int *key, size_t key_length, char *ciphertext) {
    size_t text_length = strlen(plaintext);
    for (size_t i = 0; i < text_length; i++) {
        if (isalpha(plaintext[i])) {
            char base = islower(plaintext[i]) ? 'a' : 'A';
            size_t shift = key[i % key_length];
            ciphertext[i] = (plaintext[i] - base + shift) % ALPHABET_SIZE + base;
        } else {
            ciphertext[i] = plaintext[i]; // Non-alphabetic characters remain unchanged
        }
    }
    ciphertext[text_length] = '\0'; // Null-terminate the ciphertext string
}

// Function to decrypt ciphertext using the one-time pad Vigenère cipher
void decrypt(const char *ciphertext, const int *key, size_t key_length, char *plaintext) {
    size_t text_length = strlen(ciphertext);
    for (size_t i = 0; i < text_length; i++) {
        if (isalpha(ciphertext[i])) {
            char base = islower(ciphertext[i]) ? 'a' : 'A';
            size_t shift = key[i % key_length];
            plaintext[i] = (ciphertext[i] - base - shift + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters remain unchanged
        }
    }
    plaintext[text_length] = '\0'; // Null-terminate the plaintext string
}

int main() {
    char plaintext[MAX_LENGTH];
    char ciphertext[MAX_LENGTH];
    char decryptedtext[MAX_LENGTH];
    int key[MAX_LENGTH];
    size_t key_length;
    
    // Input plaintext
    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove newline character
    
    // Input key stream
    printf("Enter the key stream (space-separated integers between 0 and 25, end with -1): ");
    size_t i = 0;
    while (scanf("%d", &key[i]) && key[i] != -1) {
        if (key[i] < 0 || key[i] >= ALPHABET_SIZE) {
            printf("Invalid key value. Key values must be between 0 and 25.\n");
            return 1;
        }
        i++;
    }
    key_length = i;
    
    // Encrypt plaintext
    encrypt(plaintext, key, key_length, ciphertext);
    printf("Encrypted ciphertext: %s\n", ciphertext);
    
    // Decrypt ciphertext
    decrypt(ciphertext, key, key_length, decryptedtext);
    printf("Decrypted plaintext: %s\n", decryptedtext);
    
    return 0;
}
