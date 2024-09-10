#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to encrypt plain text using a given shift
void encrypt(char plainText[], int shift) {
    for (int i = 0; i < strlen(plainText); i++) {
        char ch = plainText[i];

        // Encrypt uppercase letters
        if (isupper(ch)) {
            plainText[i] = ((ch - 'A' + shift) % 26) + 'A';
        }
        // Encrypt lowercase letters
        else if (islower(ch)) {
            plainText[i] = ((ch - 'a' + shift) % 26) + 'a';
        }
        // Leave non-alphabetic characters unchanged
    }
}

// Function to decrypt ciphertext using a given shift
void decrypt(char cipherText[], int shift) {
    for (int i = 0; i < strlen(cipherText); i++) {
        char ch = cipherText[i];

        // Decrypt uppercase letters
        if (isupper(ch)) {
            cipherText[i] = ((ch - 'A' - shift + 26) % 26) + 'A';
        }
        // Decrypt lowercase letters
        else if (islower(ch)) {
            cipherText[i] = ((ch - 'a' - shift + 26) % 26) + 'a';
        }
        // Leave non-alphabetic characters unchanged
    }
}

int main() {
    char message[100];
    int shift;

    // Input plain text message
    printf("Enter a message: ");
    fgets(message, sizeof(message), stdin);

    // Input shift value (key)
    printf("Enter shift value: ");
    scanf("%d", &shift);

    // Encrypt the message
    encrypt(message, shift);
    printf("Encrypted message: %s\n", message);

    // Decrypt the message
    decrypt(message, shift);
    printf("Decrypted message: %s\n", message);

    return 0;
}
