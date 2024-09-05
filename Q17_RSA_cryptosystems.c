#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to compute modular exponentiation
unsigned long mod_exp(unsigned long base, unsigned long exp, unsigned long mod) {
    unsigned long result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Function to encrypt a message using RSA
void rsa_encrypt(const char *message, unsigned long e, unsigned long n, unsigned long *encrypted) {
    size_t len = strlen(message);
    for (size_t i = 0; i < len; i++) {
        unsigned long plaintext = message[i] - 'A'; // Convert character to integer
        encrypted[i] = mod_exp(plaintext, e, n);
    }
}

// Function to decrypt a message using RSA
void rsa_decrypt(const unsigned long *encrypted, size_t len, unsigned long d, unsigned long n, char *decrypted) {
    for (size_t i = 0; i < len; i++) {
        unsigned long plaintext = mod_exp(encrypted[i], d, n);
        decrypted[i] = plaintext + 'A'; // Convert integer back to character
    }
    decrypted[len] = '\0'; // Null-terminate the string
}

int main() {
    // RSA public key parameters (example values, not secure)
    unsigned long n = 3233; // Modulus (should be a large prime product in real applications)
    unsigned long e = 17;   // Public exponent (should be chosen carefully in real applications)

    // RSA private key parameters (for demonstration; normally kept secret)
    unsigned long d = 2753; // Private exponent (calculated based on e and n)

    char message[] = "HELLO";
    size_t len = strlen(message);

    // Allocate memory for encrypted and decrypted data
    unsigned long *encrypted = (unsigned long *)malloc(len * sizeof(unsigned long));
    char *decrypted = (char *)malloc((len + 1) * sizeof(char)); // +1 for null-terminator

    if (encrypted == NULL || decrypted == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Encrypt the message
    rsa_encrypt(message, e, n, encrypted);

    printf("Encrypted message:\n");
    for (size_t i = 0; i < len; i++) {
        printf("%lu ", encrypted[i]);
    }
    printf("\n");

    // Decrypt the message
    rsa_decrypt(encrypted, len, d, n, decrypted);

    printf("Decrypted message: %s\n", decrypted);

    // Free allocated memory
    free(encrypted);
    free(decrypted);

    return 0;
}
