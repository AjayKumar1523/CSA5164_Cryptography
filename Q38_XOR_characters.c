#include <stdio.h>

int main() {
    // Initialize the string
    char str[] = "Hello world";
    
    // Display the original string
    printf("Original string: %s\n", str);

    // XOR each character with 0
    // XORing with 0 leaves the character unchanged
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] ^= 0; // XOR with 0
    }

    // Display the resulting string
    printf("Resulting string after XOR with 0: %s\n", str);

    return 0;
}
