#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// Function to find modular multiplicative inverse of a mod m
// a*x ≡ 1 (mod m)
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Should not reach here if a and m are coprime
}

// Function to encrypt plaintext using the affine Caesar cipher
void encrypt(const char *plaintext, int a, int b, char *ciphertext) {
    size_t text_length = strlen(plaintext);
    for (size_t i = 0; i < text_length; i++) {
        if (isalpha(plaintext[i])) {
            char base = islower(plaintext[i]) ? 'a' : 'A';
            size_t p = tolower(plaintext[i]) - 'a';
            ciphertext[i] = (a * p + b) % ALPHABET_SIZE + base;
        } else {
            ciphertext[i] = plaintext[i]; // Non-alphabetic characters remain unchanged
        }
    }
    ciphertext[text_length] = '\0'; // Null-terminate the ciphertext string
}

// Function to decrypt ciphertext using the affine Caesar cipher
void decrypt(const char *ciphertext, int a, int b, char *plaintext) {
    size_t text_length = strlen(ciphertext);
    int a_inv = modInverse(a, ALPHABET_SIZE); // Find modular inverse of a
    if (a_inv == -1) {
        printf("Error: The key 'a' is not coprime with %d.\n", ALPHABET_SIZE);
        exit(1);
    }

    for (size_t i = 0; i < text_length; i++) {
        if (isalpha(ciphertext[i])) {
            char base = islower(ciphertext[i]) ? 'a' : 'A';
            size_t c = tolower(ciphertext[i]) - 'a';
            plaintext[i] = (a_inv * (c - b + ALPHABET_SIZE)) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters remain unchanged
        }
    }
    plaintext[text_length] = '\0'; // Null-terminate the plaintext string
}

int main() {
    char plaintext[1024];
    char ciphertext[1024];
    char decryptedtext[1024];
    int a, b;

    // Input plaintext
    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove newline character

    // Input values for a and b
    printf("Enter key a (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter key b: ");
    scanf("%d", &b);

    // Check if 'a' is coprime with 26
    if (modInverse(a, ALPHABET_SIZE) == -1) {
        printf("Error: Key 'a' must be coprime with %d.\n", ALPHABET_SIZE);
        return 1;
    }

    // Encrypt plaintext
    encrypt(plaintext, a, b, ciphertext);
    printf("Encrypted ciphertext: %s\n", ciphertext);

    // Decrypt ciphertext
    decrypt(ciphertext, a, b, decryptedtext);
    printf("Decrypted plaintext: %s\n", decryptedtext);

    return 0;
}
