#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to encrypt the plain text using columnar transposition cipher
void encrypt(char plainText[], char key[]) {
    int textLength = strlen(plainText);
    int keyLength = strlen(key);

    // Calculate number of rows required
    int numRows = (textLength + keyLength - 1) / keyLength;
    char matrix[numRows][keyLength];

    // Fill the matrix row-wise with the plain text
    int k = 0;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < keyLength; j++) {
            if (k < textLength)
                matrix[i][j] = plainText[k++];
            else
                matrix[i][j] = '_'; // Padding character if needed
        }
    }

    // Sort the key to get column read order
    char sortedKey[keyLength];
    strcpy(sortedKey, key);
    for (int i = 0; i < keyLength - 1; i++) {
        for (int j = i + 1; j < keyLength; j++) {
            if (sortedKey[i] > sortedKey[j]) {
                char temp = sortedKey[i];
                sortedKey[i] = sortedKey[j];
                sortedKey[j] = temp;
            }
        }
    }

    // Print cipher text by reading column-wise based on sorted key order
    printf("Encrypted text: ");
    for (int i = 0; i < keyLength; i++) {
        int column = strchr(key, sortedKey[i]) - key;
        for (int j = 0; j < numRows; j++) {
            printf("%c", matrix[j][column]);
        }
    }
    printf("\n");
}

// Function to decrypt the cipher text using columnar transposition cipher
void decrypt(char cipherText[], char key[]) {
    int cipherLength = strlen(cipherText);
    int keyLength = strlen(key);

    // Calculate number of rows required
    int numRows = (cipherLength + keyLength - 1) / keyLength;
    char matrix[numRows][keyLength];

    // Sort the key to get column order for decryption
    char sortedKey[keyLength];
    strcpy(sortedKey, key);
    for (int i = 0; i < keyLength - 1; i++) {
        for (int j = i + 1; j < keyLength; j++) {
            if (sortedKey[i] > sortedKey[j]) {
                char temp = sortedKey[i];
                sortedKey[i] = sortedKey[j];
                sortedKey[j] = temp;
            }
        }
    }

    // Fill the matrix column-wise using the cipher text based on sorted key order
    int k = 0;
    for (int i = 0; i < keyLength; i++) {
        int column = strchr(key, sortedKey[i]) - key;
        for (int j = 0; j < numRows; j++) {
            if (k < cipherLength) {
                matrix[j][column] = cipherText[k++];
            }
        }
    }

    // Print plain text by reading row-wise
    printf("Decrypted text: ");
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < keyLength; j++) {
            if (matrix[i][j] != '_') // Ignore padding
                printf("%c", matrix[i][j]);
        }
    }
    printf("\n");
}

int main() {
    char plainText[100], key[100];
    
    // Input plain text
    printf("Enter plain text: ");
    fgets(plainText, sizeof(plainText), stdin);
    plainText[strcspn(plainText, "\n")] = '\0'; // Remove newline character from input

    // Input key
    printf("Enter key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // Remove newline character from input

    // Encryption
    encrypt(plainText, key);

    // Input cipher text for decryption
    char cipherText[100];
    printf("Enter cipher text for decryption: ");
    fgets(cipherText, sizeof(cipherText), stdin);
    cipherText[strcspn(cipherText, "\n")] = '\0'; // Remove newline character from input

    // Decryption
    decrypt(cipherText, key);

    return 0;
}
