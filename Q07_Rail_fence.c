#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to encrypt a message using Rail Fence Cipher
void encryptRailFence(char *text, int key, char *result) {
    int len = strlen(text);
    char rail[key][len];

    // Fill the rail matrix with '\n' to mark unused spaces
    for (int i = 0; i < key; i++) {
        for (int j = 0; j < len; j++) {
            rail[i][j] = '\n';
        }
    }

    // Mark the places to put text characters
    int row = 0, col = 0;
    int dir_down = 0;  // Flag to change the direction

    for (int i = 0; i < len; i++) {
        // Reverse direction if we are at the top or bottom row
        if (row == 0 || row == key - 1) {
            dir_down = !dir_down;
        }
        // Fill the corresponding alphabet
        rail[row][col++] = text[i];

        // Move to the next row in the zig-zag pattern
        if (dir_down) {
            row++;
        } else {
            row--;
        }
    }

    // Construct the cipher by reading the rail matrix
    int index = 0;
    for (int i = 0; i < key; i++) {
        for (int j = 0; j < len; j++) {
            if (rail[i][j] != '\n') {
                result[index++] = rail[i][j];
            }
        }
    }
    result[index] = '\0';  // Null-terminate the result
}

// Function to decrypt a Rail Fence Cipher
void decryptRailFence(char *cipher, int key, char *result) {
    int len = strlen(cipher);
    char rail[key][len];

    // Fill the rail matrix with '\n' to mark unused spaces
    for (int i = 0; i < key; i++) {
        for (int j = 0; j < len; j++) {
            rail[i][j] = '\n';
        }
    }

    // Mark the positions in zig-zag pattern where the text will be placed
    int row = 0, col = 0;
    int dir_down = 0;  // Flag to change the direction

    for (int i = 0; i < len; i++) {
        if (row == 0 || row == key - 1) {
            dir_down = !dir_down;
        }
        rail[row][col++] = '*';  // Mark positions with '*'

        if (dir_down) {
            row++;
        } else {
            row--;
        }
    }

    // Now fill the rail matrix with the cipher text
    int index = 0;
    for (int i = 0; i < key; i++) {
        for (int j = 0; j < len; j++) {
            if (rail[i][j] == '*' && index < len) {
                rail[i][j] = cipher[index++];
            }
        }
    }

    // Now read the matrix in zig-zag manner to get the original text
    row = 0;
    col = 0;
    dir_down = 0;  // Reset the direction flag

    for (int i = 0; i < len; i++) {
        if (row == 0 || row == key - 1) {
            dir_down = !dir_down;
        }
        result[i] = rail[row][col++];

        if (dir_down) {
            row++;
        } else {
            row--;
        }
    }
    result[len] = '\0';  // Null-terminate the result
}

// Driver code
int main() {
    int key;
    char text[100];
    char encrypted[100];
    char decrypted[100];

    // Get input from the user
    printf("Enter the message to encrypt: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';  // Remove the newline character at the end

    printf("Enter the key (number of rails): ");
    scanf("%d", &key);

    // Encryption
    encryptRailFence(text, key, encrypted);
    printf("\nEncrypted Message: %s\n", encrypted);

    // Decryption
    decryptRailFence(encrypted, key, decrypted);
    printf("Decrypted Message: %s\n", decrypted);

    return 0;
}
