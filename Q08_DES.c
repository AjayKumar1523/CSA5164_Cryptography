#include <stdio.h>
#include <stdint.h>

// Initial Permutation (IP) table
static const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// Final Permutation (FP) table
static const int FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// Expansion table (E) used in Feistel function
static const int E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

// P permutation table
static const int P[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

// S-boxes (S1 to S8)
static const int S_BOX[8][4][16] = {
    // S1
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    // S4
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    // S5
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    // S6
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    // S7
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    // S8
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
};

// Rotate left function
uint32_t rotate_left(uint32_t value, int shift, int size) {
    return ((value << shift) | (value >> (size - shift))) & ((1 << size) - 1);
}

// Key generation with appropriate shifts
void generate_keys(uint64_t key, uint64_t subkeys[16]) {
    static const int shifts[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    uint32_t C = (key >> 28) & 0x0FFFFFFF, D = key & 0x0FFFFFFF;
    
    for (int i = 0; i < 16; i++) {
        C = rotate_left(C, shifts[i], 28);
        D = rotate_left(D, shifts[i], 28);
        subkeys[i] = ((uint64_t)C << 28) | D; // Store keys for encryption
    }
}

// Feistel function (F)
uint32_t feistel(uint32_t R, uint64_t subkey) {
    uint64_t expanded_R = 0;
    for (int i = 0; i < 48; i++) {
        expanded_R |= ((R >> (32 - E[i])) & 1) << (47 - i);
    }
    
    expanded_R ^= subkey;
    
    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        int row = ((expanded_R >> (42 - 6 * i)) & 0x20) | ((expanded_R >> (42 - 6 * i - 5)) & 1);
        int col = (expanded_R >> (42 - 6 * i - 1)) & 0xF;
        output |= S_BOX[i][row][col] << (28 - 4 * i);
    }
    
    uint32_t permuted_output = 0;
    for (int i = 0; i < 32; i++) {
        permuted_output |= ((output >> (32 - P[i])) & 1) << (31 - i);
    }
    
    return permuted_output;
}

// DES encryption function
uint64_t des_encrypt(uint64_t plaintext, uint64_t subkeys[16]) {
    uint64_t permuted_input = 0;
    for (int i = 0; i < 64; i++) {
        permuted_input |= ((plaintext >> (64 - IP[i])) & 1) << (63 - i);
    }
    
    uint32_t L = (permuted_input >> 32) & 0xFFFFFFFF, R = permuted_input & 0xFFFFFFFF;
    
    for (int i = 0; i < 16; i++) {
        uint32_t temp = R;
        R = L ^ feistel(R, subkeys[i]);
        L = temp;
    }
    
    uint64_t pre_output = ((uint64_t)R << 32) | L;
    
    uint64_t ciphertext = 0;
    for (int i = 0; i < 64; i++) {
        ciphertext |= ((pre_output >> (64 - FP[i])) & 1) << (63 - i);
    }
    
    return ciphertext;
}

// DES decryption function
uint64_t des_decrypt(uint64_t ciphertext, uint64_t subkeys[16]) {
    uint64_t permuted_input = 0;
    for (int i = 0; i < 64; i++) {
        permuted_input |= ((ciphertext >> (64 - IP[i])) & 1) << (63 - i);
    }
    
    uint32_t L = (permuted_input >> 32) & 0xFFFFFFFF, R = permuted_input & 0xFFFFFFFF;
    
    for (int i = 15; i >= 0; i--) {  // Reverse order for decryption
        uint32_t temp = R;
        R = L ^ feistel(R, subkeys[i]);
        L = temp;
    }
    
    uint64_t pre_output = ((uint64_t)R << 32) | L;
    
    uint64_t plaintext = 0;
    for (int i = 0; i < 64; i++) {
        plaintext |= ((pre_output >> (64 - FP[i])) & 1) << (63 - i);
    }
    
    return plaintext;
}

int main() {
    uint64_t key = 0x133457799BBCDFF1;  // Example key
    uint64_t subkeys[16];
    generate_keys(key, subkeys);

    // Input plaintext from the user
    uint64_t plaintext;
    printf("Enter 16 hex digits for plaintext: ");
    scanf("%16llx", &plaintext);

    // Encrypt the plaintext
    uint64_t ciphertext = des_encrypt(plaintext, subkeys);
    printf("Encrypted ciphertext: %016llX\n", ciphertext);

    // Decrypt the ciphertext
    uint64_t decrypted_text = des_decrypt(ciphertext, subkeys);
    printf("Decrypted plaintext: %016llX\n", decrypted_text);

    return 0;
}
