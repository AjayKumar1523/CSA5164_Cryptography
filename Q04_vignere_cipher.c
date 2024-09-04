#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

void generateKey(char *text, char *key, char *keyGen) {
    int x = strlen(text);
    for (int i = 0; i < x; ++i) {
        keyGen[i] = key[i % strlen(key)];
    }
    keyGen[x] = '\0';
}

void encrypt(char *text, char *key, char *cipherText) {
    char keyGen[MAX_LEN];
    generateKey(text, key, keyGen);
    
    for (int i = 0; text[i]; ++i) {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            cipherText[i] = (text[i] - base + (tolower(keyGen[i]) - 'a')) % 26 + base;
        } else {
            cipherText[i] = text[i];
        }
    }
    cipherText[strlen(text)] = '\0';
}

void decrypt(char *cipherText, char *key, char *plainText) {
    char keyGen[MAX_LEN];
    generateKey(cipherText, key, keyGen);
    
    for (int i = 0; cipherText[i]; ++i) {
        if (isalpha(cipherText[i])) {
            char base = islower(cipherText[i]) ? 'a' : 'A';
            plainText[i] = (cipherText[i] - base - (tolower(keyGen[i]) - 'a') + 26) % 26 + base;
        } else {
            plainText[i] = cipherText[i];
        }
    }
    plainText[strlen(cipherText)] = '\0';
}

int main() {
    char text[MAX_LEN], key[MAX_LEN], cipherText[MAX_LEN], decryptedText[MAX_LEN];
    int choice;

    printf("Enter the text: ");
    fgets(text, MAX_LEN, stdin);
    text[strcspn(text, "\n")] = '\0';  // Remove trailing newline character

    printf("Enter the key: ");
    fgets(key, MAX_LEN, stdin);
    key[strcspn(key, "\n")] = '\0';  // Remove trailing newline character

    printf("Choose an option:\n1. Encrypt\n2. Decrypt\n");
    scanf("%d", &choice);
    getchar();  // Consume newline left by scanf

    if (choice == 1) {
        encrypt(text, key, cipherText);
        printf("Encrypted text: %s\n", cipherText);
    } else if (choice == 2) {
        decrypt(text, key, decryptedText);
        printf("Decrypted text: %s\n", decryptedText);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}
