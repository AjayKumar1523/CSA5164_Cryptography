#include <stdio.h>

int main() {
    // Initialize the string
    char str[] = "Hello world";
    int length = sizeof(str) / sizeof(str[0]) - 1; // Calculate the length excluding null terminator

    // Display the original string
    printf("Original string: %s\n", str);

    // Arrays to hold results after AND and XOR operations
    char and_result[length + 1];
    char xor_result[length + 1];

    // Perform AND and XOR operations
    for (int i = 0; i < length; i++) {
        and_result[i] = str[i] & 127; // AND operation with 127
        xor_result[i] = str[i] ^ 127; // XOR operation with 127
    }

    // Null terminate the result strings
    and_result[length] = '\0';
    xor_result[length] = '\0';

    // Display the results
    printf("Resulting string after AND with 127: %s\n", and_result);
    printf("Resulting string after XOR with 127: %s\n", xor_result);

    return 0;
}
