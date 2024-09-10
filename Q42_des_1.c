#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Define permutation tables and S-boxes

static const uint8_t IP[64] = {
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8, 0,
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6
};

static const uint8_t IP_INV[64] = {
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25,
    32, 0, 40, 8, 48, 16, 56, 24
};

static const uint8_t E[48] = {
    31, 0, 1, 2, 3, 4,
    3, 4, 5, 6, 7, 8,
    7, 8, 9, 10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31, 0
};

static const uint8_t P[32] = {
    15, 6, 19, 20, 28, 11, 27, 16,
    0, 25, 4, 17, 30, 9, 1, 7,
    23, 13, 29, 31, 26, 8, 16, 24,
    2, 27, 14, 5, 10, 3, 12, 18
};

static const uint8_t S[8][4][16] = {
    { {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
      {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
      {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
      {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13} },
    { {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
      {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
      {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
      {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 9, 1, 14, 10} },
    { {10, 0, 9, 5, 8, 6, 12, 7, 13, 3, 4, 11, 1, 2, 14, 15},
      {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 14, 12, 15, 1, 11, 5},
      {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
      {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15} },
    { {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
      {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
      {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
      {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3} },
    { {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
      {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
      {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
      {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 8, 0, 13} },
    { {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
      {13, 0, 11, 7, 4, 9, 1, 12, 14, 3, 5, 6, 8, 15, 10, 2},
      {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
      {6, 11, 13, 1, 10, 9, 8, 12, 5, 3, 7, 15, 2, 0, 14, 4} },
    { {11, 12, 3, 7, 15, 1, 9, 2, 8, 6, 13, 14, 0, 10, 4, 5},
      {10, 6, 15, 2, 8, 11, 12, 13, 1, 9, 4, 3, 5, 7, 0, 14},
      {5, 9, 6, 15, 0, 14, 7, 10, 8, 4, 12, 13, 11, 2, 3, 1},
      {7, 4, 13, 14, 2, 9, 8, 10, 1, 6, 11, 12, 5, 15, 0, 3} },
    { {9, 3, 5, 11, 6, 10, 14, 1, 12, 7, 0, 4, 13, 15, 8, 2},
      {12, 7, 6, 13, 2, 4, 10, 14, 1, 11, 9, 15, 3, 5, 8, 0},
      {14, 6, 11, 8, 7, 3, 1, 13, 0, 4, 9, 5, 12, 15, 2, 10},
      {13, 4, 8, 6, 11, 15, 2, 1, 12, 9, 10, 5, 14, 7, 0, 3} }
};

static const uint8_t PC1[56] = {
    56, 48, 40, 32, 24, 16, 8,
    0, 57, 49, 41, 33, 25, 17,
    9, 1, 58, 50, 42, 34, 26,
    18, 10, 2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
    6, 61, 53, 45, 37, 29, 21,
    13, 5, 60, 52, 44, 36, 28,
    20, 12, 4, 63, 55, 47, 39,
    31, 23, 15, 7
};

static const uint8_t PC2[48] = {
    13, 16, 10, 23, 0, 4, 2, 27, 
    14, 5, 20, 9, 22, 18, 11, 3, 
    25, 7, 15, 6, 26, 19, 12, 1, 
    40, 51, 30, 36, 46, 54, 29, 39, 
    50, 44, 32, 43, 48, 38, 55, 33, 
    52, 45, 41, 49, 35, 28, 31, 16
};

// Utility functions for permutations and transformations

void permute(uint8_t* input, const uint8_t* table, size_t table_size, uint8_t* output) {
    for (size_t i = 0; i < table_size; ++i) {
        output[i] = input[table[i]];
    }
}

void xor_with_key(uint8_t* data, const uint8_t* key, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        data[i] ^= key[i];
    }
}

// Key generation functions

void generate_keys(const uint8_t* key, uint8_t keys[16][48]) {
    uint8_t perm_key[56];
    permute(key, PC1, 56, perm_key);

    uint8_t c[28], d[28];
    memcpy(c, perm_key, 28);
    memcpy(d, perm_key + 28, 28);

    for (int round = 0; round < 16; ++round) {
        // Shift
        uint8_t shift = (round == 0 || round == 1 || round == 8 || round == 15) ? 1 : 2;
        memmove(c, c + shift, 28 - shift);
        memmove(d, d + shift, 28 - shift);
        memset(c + (28 - shift), 0, shift);
        memset(d + (28 - shift), 0, shift);

        // Permute
        uint8_t combined[56];
        memcpy(combined, c, 28);
        memcpy(combined + 28, d, 28);
        permute(combined, PC2, 48, keys[round]);
    }
}

// DES functions

void des_round(uint8_t* left, uint8_t* right, const uint8_t* subkey) {
    uint8_t expanded[48];
    permute(right, E, 48, expanded);

    xor_with_key(expanded, subkey, 48);

    uint8_t substituted[32];
    for (int i = 0; i < 8; ++i) {
        uint8_t block[6];
        memcpy(block, expanded + i * 6, 6);

        // S-box substitution
        uint8_t row = (block[0] << 1) | block[5];
        uint8_t col = (block[1] << 3) | (block[2] << 2) | (block[3] << 1) | block[4];
        uint8_t sbox_value = S[i][row][col];
        substituted[i * 4]     = (sbox_value >> 3) & 1;
        substituted[i * 4 + 1] = (sbox_value >> 2) & 1;
        substituted[i * 4 + 2] = (sbox_value >> 1) & 1;
        substituted[i * 4 + 3] = sbox_value & 1;
    }

    uint8_t permuted[32];
    permute(substituted, P, 32, permuted);

    xor_with_key(permuted, left, 32);
    memcpy(left, right, 32);
    memcpy(right, permuted, 32);
}

// DES encryption function

void des_encrypt(const uint8_t* input, const uint8_t* key, uint8_t* output) {
    uint8_t permuted_input[64];
    permute(input, IP, 64, permuted_input);

    uint8_t left[32], right[32];
    memcpy(left, permuted_input, 32);
    memcpy(right, permuted_input + 32, 32);

    uint8_t subkeys[16][48];
    generate_keys(key, subkeys);

    for (int round = 0; round < 16; ++round) {
        des_round(left, right, subkeys[round]);
    }

    uint8_t temp[32];
    memcpy(temp, left, 32);
    memcpy(left, right, 32);
    memcpy(right, temp, 32);

    uint8_t final_output[64];
    permute(left, IP_INV, 64, final_output);

    memcpy(output, final_output, 64);
}

// Main function for testing

int main() {
    uint8_t key[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint8_t plaintext[8] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
    uint8_t ciphertext[8];

    des_encrypt(plaintext, key, ciphertext);

    printf("Ciphertext: ");
    for (int i = 0; i < 8; ++i) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    return 0;
}
