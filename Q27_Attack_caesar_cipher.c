#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 2
#define ALPHABET_SIZE 26

// Function prototypes
void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]);
void multiplyMatrices(int a[MATRIX_SIZE][MATRIX_SIZE], int b[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]);
void modMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int mod);
void transposeMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]);
int modInverse(int a, int mod);
void invertMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]);
void encryptHillCipher(const char *plaintext, int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], char *ciphertext);
void decryptHillCipher(const char *ciphertext, int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], char *plaintext);
void findKeyMatrix(const char *plain1, const char *cipher1, const char *plain2, const char *cipher2, int keyMatrix[MATRIX_SIZE][MATRIX_SIZE]);

int main() {
    // Example plaintext and ciphertext pairs for the chosen plaintext attack
    char plain1[] = "hi";
    char cipher1[] = "om";

    char plain2[] = "he";
    char cipher2[] = "ql";

    int keyMatrix[MATRIX_SIZE][MATRIX_SIZE] = {0}; // Initialize keyMatrix to 0

    // Find the key matrix
    findKeyMatrix(plain1, cipher1, plain2, cipher2, keyMatrix);

    // Test encryption and decryption with the found key matrix
    char plaintext[] = "hi";
    char ciphertext[MATRIX_SIZE + 1]; // Extra space for null terminator
    char decryptedText[MATRIX_SIZE + 1]; // Extra space for null terminator

    encryptHillCipher(plaintext, keyMatrix, ciphertext);
    decryptHillCipher(ciphertext, keyMatrix, decryptedText);

    printf("Original Plaintext: %s\n", plaintext);
    printf("Encrypted Text: %s\n", ciphertext);
    printf("Decrypted Text: %s\n", decryptedText);

    return 0;
}

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void multiplyMatrices(int a[MATRIX_SIZE][MATRIX_SIZE], int b[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void modMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int mod) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] %= mod;
            if (matrix[i][j] < 0) {
                matrix[i][j] += mod;
            }
        }
    }
}

int modInverse(int a, int mod) {
    a = a % mod;
    for (int x = 1; x < mod; x++) {
        if ((a * x) % mod == 1) {
            return x;
        }
    }
    return -1; // Inverse does not exist
}

void invertMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    int invDet = modInverse(det, ALPHABET_SIZE);

    if (invDet == -1) {
        printf("Matrix is not invertible\n");
        exit(1);
    }

    int adjugate[MATRIX_SIZE][MATRIX_SIZE];
    adjugate[0][0] = matrix[1][1];
    adjugate[0][1] = -matrix[0][1];
    adjugate[1][0] = -matrix[1][0];
    adjugate[1][1] = matrix[0][0];

    modMatrix(adjugate, ALPHABET_SIZE);

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = (invDet * adjugate[i][j]) % ALPHABET_SIZE;
        }
    }
}

void encryptHillCipher(const char *plaintext, int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], char *ciphertext) {
    int plainVector[MATRIX_SIZE];
    int cipherVector[MATRIX_SIZE];
    int i;

    for (i = 0; i < MATRIX_SIZE; i++) {
        plainVector[i] = plaintext[i] - 'a';
    }

    int plainMatrix[MATRIX_SIZE][MATRIX_SIZE] = { {plainVector[0], plainVector[1]}, {0, 0} };
    int cipherMatrix[MATRIX_SIZE][MATRIX_SIZE];
    multiplyMatrices(keyMatrix, plainMatrix, cipherMatrix);
    modMatrix(cipherMatrix, ALPHABET_SIZE);

    for (i = 0; i < MATRIX_SIZE; i++) {
        cipherVector[i] = cipherMatrix[0][i]; // Adjust this based on actual matrix size
        ciphertext[i] = cipherVector[i] + 'a';
    }
    ciphertext[MATRIX_SIZE] = '\0';
}

void decryptHillCipher(const char *ciphertext, int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], char *plaintext) {
    int cipherVector[MATRIX_SIZE];
    int plainVector[MATRIX_SIZE];
    int i;

    for (i = 0; i < MATRIX_SIZE; i++) {
        cipherVector[i] = ciphertext[i] - 'a';
    }

    int invKeyMatrix[MATRIX_SIZE][MATRIX_SIZE];
    invertMatrix(keyMatrix, invKeyMatrix);

    int cipherMatrix[MATRIX_SIZE][MATRIX_SIZE] = { {cipherVector[0], cipherVector[1]}, {0, 0} };
    int plainMatrix[MATRIX_SIZE][MATRIX_SIZE];
    multiplyMatrices(invKeyMatrix, cipherMatrix, plainMatrix);
    modMatrix(plainMatrix, ALPHABET_SIZE);

    for (i = 0; i < MATRIX_SIZE; i++) {
        plainVector[i] = plainMatrix[0][i]; // Adjust this based on actual matrix size
        plaintext[i] = plainVector[i] + 'a';
    }
    plaintext[MATRIX_SIZE] = '\0';
}

void findKeyMatrix(const char *plain1, const char *cipher1, const char *plain2, const char *cipher2, int keyMatrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int plainMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {plain1[0] - 'a', plain1[1] - 'a'},
        {plain2[0] - 'a', plain2[1] - 'a'}
    };

    int cipherMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {cipher1[0] - 'a', cipher1[1] - 'a'},
        {cipher2[0] - 'a', cipher2[1] - 'a'}
    };

    int plainMatrixInv[MATRIX_SIZE][MATRIX_SIZE];
    invertMatrix(plainMatrix, plainMatrixInv);

    int keyMatrixTemp[MATRIX_SIZE][MATRIX_SIZE];
    multiplyMatrices(cipherMatrix, plainMatrixInv, keyMatrixTemp);
    modMatrix(keyMatrixTemp, ALPHABET_SIZE);

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            keyMatrix[i][j] = keyMatrixTemp[i][j];
        }
    }

    printf("Recovered Key Matrix:\n");
    printMatrix(keyMatrix);
}
