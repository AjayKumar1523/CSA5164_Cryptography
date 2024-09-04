#include <stdio.h>

void generateKeyMatrix(char key[], char keyMatrix[5][5]) {
    int used[26] = {0}, i = 0, j = 0, k;

    while (key[i] != '\0') {
        if (key[i] == 'J') key[i] = 'I'; 
        if (used[key[i] - 'A'] == 0) {
            keyMatrix[j / 5][j % 5] = key[i];
            used[key[i] - 'A'] = 1;
            j++;
        }
        i++;
    }

    for (k = 0; k < 26; k++) {
        if (k == 9) continue;  
        if (used[k] == 0) {
            keyMatrix[j / 5][j % 5] = 'A' + k;
            j++;
        }
    }
}

void printKeyMatrix(char keyMatrix[5][5]) {
    int i, j;
    printf("Key Matrix:\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            printf("%c ", keyMatrix[i][j]);
        }
        printf("\n");
    }
}

void encryptPair(char *a, char *b, char keyMatrix[5][5]) {
    int i1, j1, i2, j2, i, j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (keyMatrix[i][j] == *a) {
                i1 = i;
                j1 = j;
            }
            if (keyMatrix[i][j] == *b) {
                i2 = i;
                j2 = j;
            }
        }
    }

    if (i1 == i2) {
        *a = keyMatrix[i1][(j1 + 1) % 5];
        *b = keyMatrix[i2][(j2 + 1) % 5];
    } else if (j1 == j2) {
        *a = keyMatrix[(i1 + 1) % 5][j1];
        *b = keyMatrix[(i2 + 1) % 5][j2];
    } else {
        *a = keyMatrix[i1][j2];
        *b = keyMatrix[i2][j1];
    }
}

void decryptPair(char *a, char *b, char keyMatrix[5][5]) {
    int i1, j1, i2, j2, i, j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (keyMatrix[i][j] == *a) {
                i1 = i;
                j1 = j;
            }
            if (keyMatrix[i][j] == *b) {
                i2 = i;
                j2 = j;
            }
        }
    }

    if (i1 == i2) {
        *a = keyMatrix[i1][(j1 + 4) % 5];
        *b = keyMatrix[i2][(j2 + 4) % 5];
    } else if (j1 == j2) {
        *a = keyMatrix[(i1 + 4) % 5][j1];
        *b = keyMatrix[(i2 + 4) % 5][j2];
    } else {
        *a = keyMatrix[i1][j2];
        *b = keyMatrix[i2][j1];
    }
}

void encryptMessage(char message[], char keyMatrix[5][5]) {
    int i = 0;
    while (message[i] != '\0') {
        if (message[i + 1] == '\0') {
            message[i + 1] = 'X';
            message[i + 2] = '\0';
        }
        if (message[i] == message[i + 1]) {
            message[i + 1] = 'X';
        }
        encryptPair(&message[i], &message[i + 1], keyMatrix);
        i += 2;
    }
}

void decryptMessage(char message[], char keyMatrix[5][5]) {
    int i = 0;
    while (message[i] != '\0') {
        decryptPair(&message[i], &message[i + 1], keyMatrix);
        i += 2;
    }
}

int main() {
    char key[100], message[100];
    char keyMatrix[5][5];
    int i = 0, choice;

    printf("Enter key: ");
    while ((key[i] = getchar()) != '\n') i++;
    key[i] = '\0';

    generateKeyMatrix(key, keyMatrix);
    printKeyMatrix(keyMatrix);

    printf("Enter message: ");
    i = 0;
    while ((message[i] = getchar()) != '\n') i++;
    message[i] = '\0';

    printf("Choose an option:\n1. Encrypt\n2. Decrypt\n");
    scanf("%d", &choice);

    if (choice == 1) {
        encryptMessage(message, keyMatrix);
        printf("Encrypted message: %s\n", message);
    } else if (choice == 2) {
        decryptMessage(message, keyMatrix);
        printf("Decrypted message: %s\n", message);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}
