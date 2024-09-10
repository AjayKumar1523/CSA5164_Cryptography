#include <stdio.h>
#include <string.h>

#define MAX 100

void rail_fence_encrypt(char *message, int rails, char *ciphertext) {
    int len = strlen(message);
    char rail[rails][MAX];
    int dir = 1; // 1 for down, -1 for up
    int row = 0, col = 0;

    // Initialize rail matrix
    for (int i = 0; i < rails; i++) {
        for (int j = 0; j < len; j++) {
            rail[i][j] = '\0'; // Empty character
        }
    }

    // Fill the rail matrix
    for (int i = 0; i < len; i++) {
        rail[row][col++] = message[i];
        if (row == 0 || row == rails - 1) {
            dir = -dir;
        }
        row += dir;
    }

    // Read the rail matrix to form ciphertext
    int index = 0;
    for (int i = 0; i < rails; i++) {
        for (int j = 0; j < len; j++) {
            if (rail[i][j] != '\0') {
                ciphertext[index++] = rail[i][j];
            }
        }
    }
    ciphertext[index] = '\0'; // Null-terminate the string
}

int main() {
    char message[] = "RAILFENCE";
    int rails = 3;
    char ciphertext[MAX];

    rail_fence_encrypt(message, rails, ciphertext);

    printf("Encrypted message: %s\n", ciphertext);

    return 0;
}

