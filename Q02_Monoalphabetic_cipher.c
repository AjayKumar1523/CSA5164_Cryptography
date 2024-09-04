#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
void createSubstitutionMap(char *key, char *map) {
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        map[i] = key[i];
    }
}

void encrypt(char *plaintext, char *map, char *ciphertext) {
    int i = 0;  // Initialize index for ciphertext
    while (*plaintext) {
        if (isalpha(*plaintext)) {
            char base = islower(*plaintext) ? 'a' : 'A';
            ciphertext[i++] = map[tolower(*plaintext) - 'a'];
        } else {
            ciphertext[i++] = *plaintext;
        }
        plaintext++;
    }
    ciphertext[i] = '\0';
}

void decrypt(char *ciphertext, char *map, char *plaintext) {
    char reverseMap[ALPHABET_SIZE];
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        reverseMap[i] = '?';  
    }
    for (i = 0; i < ALPHABET_SIZE; i++) {
        char index = tolower(map[i]) - 'a';
        reverseMap[index] = 'a' + i;
    }
    i = 0;  
    while (*ciphertext) {
        if (isalpha(*ciphertext)) {
            char base = islower(*ciphertext) ? 'a' : 'A';
            char decryptedChar = reverseMap[tolower(*ciphertext) - 'a'];
            plaintext[i++] = isupper(*ciphertext) ? toupper(decryptedChar) : decryptedChar;
        } else {
            plaintext[i++] = *ciphertext;
        }
        ciphertext++;
    }
    plaintext[i] = '\0';
}

int main() {
    char key[ALPHABET_SIZE + 1];
    char plaintext[100];
    char ciphertext[100];
    char decryptedtext[100];
    printf("Enter substitution key (26 unique letters): ");
    scanf("%26s", key);
    if (strlen(key) != ALPHABET_SIZE) {
        printf("Key must be exactly 26 characters long.\n");
        return 1;
    }
    char map[ALPHABET_SIZE];
    createSubstitutionMap(key, map);
    printf("Enter the plaintext: ");
    scanf(" %[^\n]", plaintext);
    encrypt(plaintext, map, ciphertext);
    printf("Encrypted text: %s\n", ciphertext);
    decrypt(ciphertext, map, decryptedtext);
    printf("Decrypted text: %s\n", decryptedtext);

    return 0;
}
