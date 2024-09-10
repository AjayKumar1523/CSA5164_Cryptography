#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Permutation tables, S-boxes, and other required tables for DES
int initial_permutation[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

int final_permutation[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

// Expansion table for the right half of the block in DES Feistel function
uint8_t expansion_table[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// Permutation function used inside Feistel function after applying S-boxes
uint8_t permutation_function[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

// Simplified S-box (real DES uses 8 S-boxes, each 6x4 bits)
uint8_t SBox[8][4] = {
    {14, 4, 13, 1},
    {0, 15, 7, 4},
    {4, 1, 14, 8},
    {15, 12, 8, 2},
    {7, 11, 2, 1},
    {13, 2, 8, 10},
    {5, 3, 15, 2},
    {9, 7, 13, 8}
};

// Key schedule generation
void generate_subkeys(uint64_t key, uint64_t *subkeys) {
    // Perform key scheduling to generate 16 subkeys from the main key
    for (int i = 0; i < 16; i++) {
        subkeys[i] = (key << i) ^ (key >> (64 - i)); // Simplified logic
    }
}

// Feistel function
uint32_t feistel(uint32_t half_block, uint64_t subkey) {
    // Expansion, XOR with subkey, S-boxes, and permutation.
    // Simplified Feistel for explanation purpose.
    half_block = (half_block ^ subkey) & 0xFFFFFFFF; // Simplified XOR
    uint32_t result = 0;
    // Apply S-boxes (simplified)
    for (int i = 0; i < 8; i++) {
        result |= SBox[i][(half_block >> (4 * i)) & 0xF];
    }
    return result;
}

// DES encryption function (simplified)
uint64_t des_encrypt(uint64_t block, uint64_t key) {
    uint64_t subkeys[16];
    generate_subkeys(key, subkeys);

    // Initial Permutation
    uint64_t permuted_block = 0;
    for (int i = 0; i < 64; i++) {
        permuted_block |= ((block >> (64 - initial_permutation[i])) & 1) << (63 - i);
    }

    uint32_t left = (permuted_block >> 32) & 0xFFFFFFFF;
    uint32_t right = permuted_block & 0xFFFFFFFF;

    // 16 rounds of Feistel
    for (int i = 0; i < 16; i++) {
        uint32_t temp = right;
        right = left ^ feistel(right, subkeys[i]);
        left = temp;
    }

    // Combine halves and final permutation
    uint64_t combined_block = ((uint64_t)right << 32) | left;
    uint64_t final_block = 0;
    for (int i = 0; i < 64; i++) {
        final_block |= ((combined_block >> (64 - final_permutation[i])) & 1) << (63 - i);
    }
    return final_block;
}

// DES decryption function (same as encryption but with reversed subkeys)
uint64_t des_decrypt(uint64_t block, uint64_t key) {
    uint64_t subkeys[16];
    generate_subkeys(key, subkeys);

    // Reverse the subkeys for decryption
    for (int i = 0; i < 8; i++) {
        uint64_t temp = subkeys[i];
        subkeys[i] = subkeys[15 - i];
        subkeys[15 - i] = temp;
    }

    return des_encrypt(block, key); // Perform the same encryption but with reversed subkeys
}

// Triple DES (3DES) encryption
uint64_t triple_des_encrypt(uint64_t block, uint64_t key1, uint64_t key2, uint64_t key3) {
    // Encrypt with key1, decrypt with key2, encrypt with key3
    block = des_encrypt(block, key1);
    block = des_decrypt(block, key2);
    block = des_encrypt(block, key3);
    return block;
}

// Triple DES (3DES) decryption
uint64_t triple_des_decrypt(uint64_t block, uint64_t key1, uint64_t key2, uint64_t key3) {
    // Decrypt with key3, encrypt with key2, decrypt with key1
    block = des_decrypt(block, key3);
    block = des_encrypt(block, key2);
    block = des_decrypt(block, key1);
    return block;
}

int main() {
    // 64-bit data block and 3 keys
    uint64_t data = 0x0123456789ABCDEF;
    uint64_t key1 = 0x133457799BBCDFF1;
    uint64_t key2 = 0x1A1A1A1A1A1A1A1A;
    uint64_t key3 = 0x2B2B2B2B2B2B2B2B;

    // Print original data
    printf("Original Data: %016llX\n", data);

    // Encrypt with Triple DES
    uint64_t encrypted = triple_des_encrypt(data, key1, key2, key3);
    printf("Encrypted Data: %016llX\n", encrypted);

    // Decrypt with Triple DES
    uint64_t decrypted = triple_des_decrypt(encrypted, key1, key2, key3);
    printf("Decrypted Data: %016llX\n", decrypted);

    return 0;
}
