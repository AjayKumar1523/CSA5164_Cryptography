#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BLOCK_SIZE 8
#define NUM_ROUNDS 16
#define NUM_KEYS 3

// Function prototypes
void permute(const uint8_t *in, uint8_t *out, const uint8_t *table, int size);
void generate_keys(const uint8_t *key, uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2]);
uint32_t feistel(uint32_t R, uint32_t *key);
void des_encrypt(const uint8_t in[8], uint8_t out[8], uint32_t sub_keys[NUM_ROUNDS][2]);
void triple_des_encrypt_ecb(const uint8_t *data, uint8_t *encrypted_data, size_t data_len, const uint8_t *key);
void triple_des_decrypt_ecb(const uint8_t *data, uint8_t *decrypted_data, size_t data_len, const uint8_t *key);

// Define permutation and S-box tables
static const uint8_t IP[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

static const uint8_t FP[64] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

static const uint8_t E[48] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static const uint8_t P[32] = {
    16,  7, 20, 21,
    29, 12, 28, 17,
    1,  15, 23, 26,
    5,  18, 31, 10,
    2,  8, 24, 14,
    32, 27, 3,  9,
    19, 13, 30,  6,
    22, 11, 4,  25
};

// Example S-boxes (placeholders, you need to fill these with actual values)
static const uint8_t S_BOX[8][4][16] = {
    // Example S-boxes, fill these with actual values
    { /* S1 */ },
    { /* S2 */ },
    { /* S3 */ },
    { /* S4 */ },
    { /* S5 */ },
    { /* S6 */ },
    { /* S7 */ },
    { /* S8 */ }
};

// Function to perform permutation based on a given table
void permute(const uint8_t *in, uint8_t *out, const uint8_t *table, int size) {
    int i;
    memset(out, 0, size / 8);
    for (i = 0; i < size; i++) {
        int bit = (in[(table[i] - 1) / 8] >> (7 - ((table[i] - 1) % 8))) & 1;
        if (bit) {
            out[i / 8] |= (1 << (7 - (i % 8)));
        }
    }
}

// Function to generate subkeys for 3DES
void generate_keys(const uint8_t *key, uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2]) {
    // Key scheduling for DES (simplified for demonstration)
    // Actual implementation needed here

    for (int i = 0; i < NUM_KEYS; i++) {
        // Generate subkeys for each DES key
        // This is a placeholder
        for (int j = 0; j < NUM_ROUNDS; j++) {
            sub_keys[i][j][0] = 0x0; // Placeholder values
            sub_keys[i][j][1] = 0x0; // Placeholder values
        }
    }
}

// Function to apply Feistel function in DES
uint32_t feistel(uint32_t R, uint32_t *key) {
    uint32_t expanded = 0;
    uint32_t xor_result = R ^ *key;
    uint32_t sbox_output = 0;
    int i, j;
    
    // Expansion
    for (i = 0; i < 48; i++) {
        int bit = (xor_result >> (32 - E[i])) & 1;
        expanded |= (bit << (47 - i));
    }
    
    // S-boxes
    for (i = 0; i < 8; i++) {
        int row = ((expanded >> (6 * (7 - i))) & 0x20) | ((expanded >> (6 * (7 - i) + 5)) & 0x1);
        int col = (expanded >> (6 * (7 - i) + 1)) & 0xF;
        sbox_output |= (S_BOX[i][row][col] << (4 * (7 - i)));
    }
    
    // Permutation
    uint32_t permuted = 0;
    for (i = 0; i < 32; i++) {
        int bit = (sbox_output >> (32 - P[i])) & 1;
        permuted |= (bit << (31 - i));
    }
    
    return permuted;
}

// Function to perform DES encryption
void des_encrypt(const uint8_t in[8], uint8_t out[8], uint32_t sub_keys[NUM_ROUNDS][2]) {
    uint8_t IP_out[8] = {0};
    uint8_t L[4], R[4];
    uint32_t L_part, R_part;
    int i;
    
    // Initial Permutation
    permute(in, IP_out, IP, 64);
    
    // Split into L and R
    memcpy(L, IP_out, 4);
    memcpy(R, IP_out + 4, 4);
    
    for (i = 0; i < NUM_ROUNDS; i++) {
        L_part = (L[0] << 24) | (L[1] << 16) | (L[2] << 8) | L[3];
        R_part = (R[0] << 24) | (R[1] << 16) | (R[2] << 8) | R[3];
        
        // Feistel function
        uint32_t feistel_out = feistel(R_part, sub_keys[i]);
        
        // XOR with L
        uint32_t temp = L_part;
        L_part = R_part;
        R_part = temp ^ feistel_out;
        
        // Update L and R
        memcpy(L, &L_part, 4);
        memcpy(R, &R_part, 4);
    }
    
    // Combine L and R
    uint8_t final[8] = {0};
    memcpy(final, R, 4);
    memcpy(final + 4, L, 4);
    
    // Final Permutation
    permute(final, out, FP, 64);
}

// Function to perform 3DES encryption in ECB mode
void triple_des_encrypt_ecb(const uint8_t *data, uint8_t *encrypted_data, size_t data_len, const uint8_t *key) {
    uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2];
    uint8_t block[BLOCK_SIZE];
    size_t i;
    
    generate_keys(key, sub_keys);
    
    // Encrypt each block
    for (i = 0; i < data_len; i += BLOCK_SIZE) {
        memcpy(block, data + i, BLOCK_SIZE);
        
        // Perform 3DES encryption
        des_encrypt(block, block, sub_keys[0]);
        des_encrypt(block, block, sub_keys[1]);
        des_encrypt(block, block, sub_keys[2]);
        
        // Copy encrypted block
        memcpy(encrypted_data + i, block, BLOCK_SIZE);
    }
}

// Function to perform DES decryption
void des_decrypt(const uint8_t in[8], uint8_t out[8], uint32_t sub_keys[NUM_ROUNDS][2]) {
    uint8_t IP_out[8] = {0};
    uint8_t L[4], R[4];
    uint32_t L_part, R_part;
    int i;
    
    // Initial Permutation
    permute(in, IP_out, IP, 64);
    
    // Split into L and R
    memcpy(L, IP_out, 4);
    memcpy(R, IP_out + 4, 4);
    
    for (i = NUM_ROUNDS - 1; i >= 0; i--) {
        L_part = (L[0] << 24) | (L[1] << 16) | (L[2] << 8) | L[3];
        R_part = (R[0] << 24) | (R[1] << 16) | (R[2] << 8) | R[3];
        
        // Feistel function
        uint32_t feistel_out = feistel(R_part, sub_keys[i]);
        
        // XOR with L
        uint32_t temp = L_part;
        L_part = R_part;
        R_part = temp ^ feistel_out;
        
        // Update L and R
        memcpy(L, &L_part, 4);
        memcpy(R, &R_part, 4);
    }
    
    // Combine L and R
    uint8_t final[8] = {0};
    memcpy(final, R, 4);
    memcpy(final + 4, L, 4);
    
    // Final Permutation
    permute(final, out, FP, 64);
}

// Function to perform 3DES decryption in ECB mode
void triple_des_decrypt_ecb(const uint8_t *data, uint8_t *decrypted_data, size_t data_len, const uint8_t *key) {
    uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2];
    uint8_t block[BLOCK_SIZE];
    size_t i;
    
    generate_keys(key, sub_keys);
    
    // Decrypt each block
    for (i = 0; i < data_len; i += BLOCK_SIZE) {
        memcpy(block, data + i, BLOCK_SIZE);
        
        // Perform 3DES decryption
        des_decrypt(block, block, sub_keys[2]);
        des_decrypt(block, block, sub_keys[1]);
        des_decrypt(block, block, sub_keys[0]);
        
        // Copy decrypted block
        memcpy(decrypted_data + i, block, BLOCK_SIZE);
    }
}

int main() {
    // Sample input data
    const char *sample_data = "This is a test data for 3DES ECB encryption.";
    size_t data_len = strlen(sample_data);
    uint8_t data[data_len + BLOCK_SIZE] = {0};
    uint8_t encrypted_data[data_len + BLOCK_SIZE] = {0};
    uint8_t decrypted_data[data_len + BLOCK_SIZE] = {0};
    uint8_t key[24] = {0}; // 24-byte key for 3DES
    
    // Convert sample_data to uint8_t array
    memcpy(data, sample_data, data_len);
    
    // Initialize key with some values (for demo purposes)
    const char *key_str = "0123456789ABCDEF01234567";
    for (int i = 0; i < 24; i++) {
        sscanf(key_str + 2 * i, "%2hhX", &key[i]);
    }
    
    // Encrypt data using 3DES in ECB mode
    triple_des_encrypt_ecb(data, encrypted_data, data_len, key);
    
    // Output encrypted data
    printf("Encrypted data:\n");
    for (size_t i = 0; i < data_len; i++) {
        printf("%02X ", encrypted_data[i]);
    }
    printf("\n");
    
    // Decrypt data using 3DES in ECB mode
    triple_des_decrypt_ecb(encrypted_data, decrypted_data, data_len, key);
    
    // Output decrypted data
    printf("Decrypted data:\n");
    for (size_t i = 0; i < data_len; i++) {
        printf("%c", decrypted_data[i]);
    }
    printf("\n");
    
    return 0;
}
