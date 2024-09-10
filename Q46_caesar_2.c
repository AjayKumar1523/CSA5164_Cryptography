#include <stdio.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Function to decrypt a message using Caesar cipher with a specific shift
void decrypt(const char *ciphertext, int shift, char *plaintext) {
    while (*ciphertext) {
        if (*ciphertext >= 'a' && *ciphertext <= 'z') {
            *plaintext = (char)(((*ciphertext - 'a' - shift + ALPHABET_SIZE) % ALPHABET_SIZE) + 'a');
        } else {
            *plaintext = *ciphertext;
        }
        ciphertext++;
        plaintext++;
    }
    *plaintext = '\0'; // Null-terminate the string
}

// Function to perform frequency analysis and print possible plaintexts
void analyze_ciphertext(const char *ciphertext) {
    char plaintext[256];
    int shift;

    printf("Possible plaintexts for different shifts:\n");

    for (shift = 0; shift < ALPHABET_SIZE; shift++) {
        decrypt(ciphertext, shift, plaintext);
        printf("Shift %d: %s\n", shift, plaintext);
    }
}

int main() {
    const char ciphertext[] = "cxknxawxccxkncqjcrbcqnzdnbcrxwfruurjvbqjtnbynjan";
    
    analyze_ciphertext(ciphertext);

    return 0;
}
