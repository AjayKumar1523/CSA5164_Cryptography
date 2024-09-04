#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hillCipherEncrypt(char message[], int **keyMatrix, int size, char cipherMessage[]) {
    int messageLength = strlen(message);
    int paddedLength = messageLength;

    // Pad message with 'X' if its length is not a multiple of the matrix size
    if (messageLength % size != 0) {
        paddedLength = messageLength + (size - (messageLength % size));
        for (int i = messageLength; i < paddedLength; i++) {
            message[i] = 'X';
        }
        message[paddedLength] = '\0';
    }

    for (int k = 0; k < paddedLength; k += size) {
        int messageVector[size];
        for (int i = 0; i < size; i++) {
            messageVector[i] = message[k + i] - 'A';
        }

        int cipherVector[size];
        for (int i = 0; i < size; i++) {
            cipherVector[i] = 0;
            for (int j = 0; j < size; j++) {
                cipherVector[i] += keyMatrix[i][j] * messageVector[j];
            }
            cipherVector[i] %= 26;
        }

        for (int i = 0; i < size; i++) {
            cipherMessage[k + i] = cipherVector[i] + 'A';
        }
    }

    cipherMessage[paddedLength] = '\0';
}

int main() {
    int size;
    printf("Enter the size of the key matrix (e.g., 2 for 2x2): ");
    scanf("%d", &size);

    char message[100];
    char cipherMessage[100];

    printf("Enter a message: ");
    scanf("%s", message);

    // Dynamically allocate memory for the key matrix
    int *keyMatrix = (int *)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        keyMatrix[i] = (int *)malloc(size * sizeof(int));
    }

    printf("Enter key matrix (%dx%d):\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &keyMatrix[i][j]);
        }
    }

    hillCipherEncrypt(message, keyMatrix, size, cipherMessage);

    printf("Encrypted message: %s\n", cipherMessage);

    // Free dynamically allocated memory
    for (int i = 0; i < size; i++) {
        free(keyMatrix[i]);
    }
    free(keyMatrix);

    return 0;
}
