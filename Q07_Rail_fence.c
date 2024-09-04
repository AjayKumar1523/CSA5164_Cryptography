#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Function to encrypt plaintext using Rail Fence Cipher
void railFenceEncrypt(char plaintext[], int rails, char ciphertext[]) {
    int len = strlen(plaintext);
    char grid[rails][MAX];
    
    // Initialize the grid with spaces
    for (int i = 0; i < rails; i++) {
        for (int j = 0; j < len; j++) {
            grid[i][j] = ' ';
        }
    }
    
    int row = 0;
    int step = 1;
    
    // Fill the grid in a zigzag pattern
    for (int i = 0; i < len; i++) {
        grid[row][i] = plaintext[i];
        if (row == 0 || row == rails - 1) {
            step = -step;
        }
        row += step;
    }
    
    // Read the grid row by row
    int index = 0;
    for (int i = 0; i < rails; i++) {
        for (int j = 0; j < len; j++) {
            if (grid[i][j] != ' ') {
                ciphertext[index++] = grid[i][j];
            }
        }
    }
    ciphertext[index] = '\0';
}

// Function to decrypt ciphertext using Rail Fence Cipher
void railFenceDecrypt(char ciphertext[], int rails, char plaintext[]) {
    int len = strlen(ciphertext);
    char grid[rails][MAX];
    
    // Initialize the grid with spaces
    for (int i = 0; i < rails; i++) {
        for (int j = 0; j < len; j++) {
            grid[i][j] = ' ';
        }
    }
    
    int row = 0;
    int step = 1;
    
    // Mark the positions in the grid where ciphertext characters will go
    for (int i = 0; i < len; i++) {
        grid[row][i] = '*';  // Temporary mark
        if (row == 0 || row == rails - 1) {
            step = -step;
        }
        row += step;
    }
    
    // Fill the grid with ciphertext
    int index = 0;
    for (int i = 0; i < rails; i++) {
        for (int j = 0; j < len; j++) {
            if (grid[i][j] == '*') {
                grid[i][j] = ciphertext[index++];
            }
        }
    }
    
    // Read the grid row by row to get plaintext
    row = 0;
    step = 1;
    index = 0;
    for (int i = 0; i < len; i++) {
        plaintext[index++] = grid[row][i];
        if (row == 0 || row == rails - 1) {
            step = -step;
        }
        row += step;
    }
    plaintext[index] = '\0';
}

int main() {
    char plaintext[MAX], ciphertext[MAX], decryptedtext[MAX];
    int rails;
    
    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    
    // Remove newline character from plaintext if present
    size_t len = strlen(plaintext);
    if (len > 0 && plaintext[len-1] == '\n') {
        plaintext[len-1] = '\0';
    }
    
    printf("Enter number of rails: ");
    scanf("%d", &rails);
    
    // Encrypt the plaintext
    railFenceEncrypt(plaintext, rails, ciphertext);
    printf("Encrypted ciphertext: %s\n", ciphertext);
    
    // Decrypt the ciphertext
    railFenceDecrypt(ciphertext, rails, decryptedtext);
    printf("Decrypted plaintext: %s\n", decryptedtext);
    
    return 0;
}
