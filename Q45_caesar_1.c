#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define SHIFT 1 // Define the shift key

// Function to encrypt a message using Caesar cipher
void encrypt(char *message, int shift) {
    while (*message) {
        if (isalpha(*message)) {
            char base = islower(*message) ? 'a' : 'A';
            *message = (char)(((int)(*message - base + shift) % 26) + base);
        }
        message++;
    }
}

// Function to decrypt a message using Caesar cipher
void decrypt(char *message, int shift) {
    while (*message) {
        if (isalpha(*message)) {
            char base = islower(*message) ? 'a' : 'A';
            *message = (char)(((int)(*message - base - shift + 26) % 26) + base);
        }
        message++;
    }
}

int main() {
    // Define the message to be encrypted and decrypted
    char message[] = "defend the east wall of the castle";

    // Print the original message
    printf("Original message: %s\n", message);

    // Encrypt the message
    encrypt(message, SHIFT);
    printf("Encrypted message: %s\n", message);

    // Decrypt the message
    decrypt(message, SHIFT);
    printf("Decrypted message: %s\n", message);

    return 0;
}
