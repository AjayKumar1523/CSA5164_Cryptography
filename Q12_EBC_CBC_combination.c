#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Definitions
#define BLOCK_SIZE 8
#define NUM_ROUNDS 16
#define NUM_KEYS 3

// Example S-box (replace with actual S-box values)
static const uint8_t S[8][64] = { /* Actual S-box values should be filled here */ };

// Initial and final permutations (IP and FP) (fill with actual permutation tables)
static const uint8_t IP[64] = { /* Actual IP values here */ };
static const uint8_t FP[64] = { /* Actual FP values here */ };

// Key schedule and Feistel function definitions
// Dummy implementations, replace with actual algorithms
void generate_keys(const uint8_t *key, uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2]) {
    // Generate 3 DES sub-keys from the main key
    // This is a placeholder; replace with actual key schedule logic
}

uint32_t feistel(uint32_t R, uint32_t key) {
    // Feistel function (placeholder)
    return R ^ key; // Replace with actual Feistel function
}

void permute(const uint8_t *in, uint8_t *out, const uint8_t *perm, size_t size) {
    // Perform permutation (placeholder)
    // Replace with actual permutation logic
}

// Function to perform DES encryption
void des_encrypt(const uint8_t *in, uint8_t *out, uint32_t sub_keys[NUM_ROUNDS][2]) {
    uint8_t L[4], R[4];
    memcpy(L, in, 4);
    memcpy(R, in + 4, 4);
    for (int i = 0; i < NUM_ROUNDS; i++) {
        uint32_t L_part = (L[0] << 24) | (L[1] << 16) | (L[2] << 8) | L[3];
        uint32_t R_part = (R[0] << 24) | (R[1] << 16) | (R[2] << 8) | R[3];
        uint32_t feistel_out = feistel(R_part, sub_keys[i][0]);
        uint32_t temp = L_part;
        L_part = R_part;
        R_part = temp ^ feistel_out;
        memcpy(L, &L_part, 4);
        memcpy(R, &R_part, 4);
    }
    uint8_t final[8] = {0};
    memcpy(final, R, 4);
    memcpy(final + 4, L, 4);
    permute(final, out, FP, 64);
}

// Function to perform 3DES encryption in ECB mode
void triple_des_encrypt_ecb(const uint8_t *data, uint8_t *encrypted_data, size_t data_len, const uint8_t *key) {
    uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2];
    uint8_t block[BLOCK_SIZE];
    size_t i;
    
    generate_keys(key, sub_keys);
    
    for (i = 0; i < data_len; i += BLOCK_SIZE) {
        memcpy(block, data + i, BLOCK_SIZE);
        des_encrypt(block, block, sub_keys[0]);
        des_encrypt(block, block, sub_keys[1]);
        des_encrypt(block, block, sub_keys[2]);
        memcpy(encrypted_data + i, block, BLOCK_SIZE);
    }
}

// Function to perform 3DES decryption in ECB mode
void triple_des_decrypt_ecb(const uint8_t *data, uint8_t *decrypted_data, size_t data_len, const uint8_t *key) {
    uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2];
    uint8_t block[BLOCK_SIZE];
    size_t i;
    
    generate_keys(key, sub_keys);
    
    for (i = 0; i < data_len; i += BLOCK_SIZE) {
        memcpy(block, data + i, BLOCK_SIZE);
        des_encrypt(block, block, sub_keys[2]);
        des_encrypt(block, block, sub_keys[1]);
        des_encrypt(block, block, sub_keys[0]);
        memcpy(decrypted_data + i, block, BLOCK_SIZE);
    }
}

// Function to perform 3DES encryption in CBC mode
void triple_des_encrypt_cbc(const uint8_t *data, uint8_t *encrypted_data, size_t data_len, const uint8_t *key, const uint8_t *iv) {
    uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2];
    uint8_t block[BLOCK_SIZE];
    uint8_t previous_block[BLOCK_SIZE];
    size_t i;
    
    generate_keys(key, sub_keys);
    memcpy(previous_block, iv, BLOCK_SIZE);
    
    for (i = 0; i < data_len; i += BLOCK_SIZE) {
        memcpy(block, data + i, BLOCK_SIZE);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= previous_block[j];
        }
        des_encrypt(block, block, sub_keys[0]);
        des_encrypt(block, block, sub_keys[1]);
        des_encrypt(block, block, sub_keys[2]);
        memcpy(encrypted_data + i, block, BLOCK_SIZE);
        memcpy(previous_block, block, BLOCK_SIZE);
    }
}

// Function to perform 3DES decryption in CBC mode
void triple_des_decrypt_cbc(const uint8_t *data, uint8_t *decrypted_data, size_t data_len, const uint8_t *key, const uint8_t *iv) {
    uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2];
    uint8_t block[BLOCK_SIZE];
    uint8_t previous_block[BLOCK_SIZE];
    size_t i;
    
    generate_keys(key, sub_keys);
    memcpy(previous_block, iv, BLOCK_SIZE);
    
    for (i = 0; i < data_len; i += BLOCK_SIZE) {
        memcpy(block, data + i, BLOCK_SIZE);
        des_encrypt(block, block, sub_keys[2]);
        des_encrypt(block, block, sub_keys[1]);
        des_encrypt(block, block, sub_keys[0]);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            decrypted_data[i + j] = block[j] ^ previous_block[j];
        }
        memcpy(previous_block, data + i, BLOCK_SIZE);
    }
}

// Function to perform 3DES encryption in CFB mode
void triple_des_encrypt_cfb(const uint8_t *data, uint8_t *encrypted_data, size_t data_len, const uint8_t *key, const uint8_t *iv) {
    uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2];
    uint8_t block[BLOCK_SIZE];
    uint8_t feedback[BLOCK_SIZE];
    size_t i;
    
    generate_keys(key, sub_keys);
    memcpy(feedback, iv, BLOCK_SIZE);
    
    for (i = 0; i < data_len; i += BLOCK_SIZE) {
        des_encrypt(feedback, block, sub_keys[0]);
        des_encrypt(block, block, sub_keys[1]);
        des_encrypt(block, block, sub_keys[2]);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            encrypted_data[i + j] = data[i + j] ^ block[j];
        }
        memcpy(feedback, encrypted_data + i, BLOCK_SIZE);
    }
}

// Function to perform 3DES decryption in CFB mode
void triple_des_decrypt_cfb(const uint8_t *data, uint8_t *decrypted_data, size_t data_len, const uint8_t *key, const uint8_t *iv) {
    uint32_t sub_keys[NUM_KEYS][NUM_ROUNDS][2];
    uint8_t block[BLOCK_SIZE];
    uint8_t feedback[BLOCK_SIZE];
    size_t i;
    
    generate_keys(key, sub_keys);
    memcpy(feedback, iv, BLOCK_SIZE);
    
    for (i = 0; i < data_len; i += BLOCK_SIZE) {
        des_encrypt(feedback, block, sub_keys[0]);
        des_encrypt(block, block, sub_keys[1]);
        des_encrypt(block, block, sub_keys[2]);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            decrypted_data[i + j] = data[i + j] ^ block[j];
        }
        memcpy(feedback, data + i, BLOCK_SIZE);
    }
}

// Function to pad data to ensure it is a multiple of BLOCK_SIZE
void pad_data(uint8_t **data, size_t *data_len) {
    size_t padding_len = BLOCK_SIZE - (*data_len % BLOCK_SIZE);
    if (padding_len == BLOCK_SIZE) padding_len = 0;
    size_t new_len = *data_len + padding_len;
    
    *data = (uint8_t *)realloc(*data, new_len);
    memset(*data + *data_len, padding_len, padding_len);
    *data_len = new_len;
}

// Function to unpad data
void unpad_data(uint8_t *data, size_t *data_len) {
    size_t padding_len = data[*data_len - 1];
    *data_len -= padding_len;
}

// Main function
int main() {
    uint8_t *data, *encrypted_data, *decrypted_data;
    size_t data_len, i;
    char key_str[24], iv_str[16];
    
    // Get input from user
    printf("Enter plaintext (multiple of 8 bytes): ");
    fgets(key_str, sizeof(key_str), stdin);
    data_len = strlen(key_str) - 1; // Remove newline character
    data = (uint8_t *)malloc(data_len);
    for (i = 0; i < data_len; i++) {
        data[i] = key_str[i];
    }
    
    // Pad data
    pad_data(&data, &data_len);
    
    printf("Enter key (24 hex digits): ");
    fgets(key_str, sizeof(key_str), stdin);
    uint8_t key[24];
    for (i = 0; i < 24; i++) {
        sscanf(&key_str[i * 2], "%2hhx", &key[i]);
    }
    
    printf("Enter IV (16 hex digits): ");
    fgets(iv_str, sizeof(iv_str), stdin);
    uint8_t iv[8];
    for (i = 0; i < 8; i++) {
        sscanf(&iv_str[i * 2], "%2hhx", &iv[i]);
    }
    
    encrypted_data = (uint8_t *)malloc(data_len);
    decrypted_data = (uint8_t *)malloc(data_len);
    
    // Perform encryption and decryption in ECB mode
    triple_des_encrypt_ecb(data, encrypted_data, data_len, key);
    triple_des_decrypt_ecb(encrypted_data, decrypted_data, data_len, key);
    
    // Print ECB results
    printf("ECB Mode:\n");
    printf("Encrypted: ");
    for (i = 0; i < data_len; i++) {
        printf("%02X ", encrypted_data[i]);
    }
    printf("\nDecrypted: %s\n", decrypted_data);
    
    // Perform encryption and decryption in CBC mode
    triple_des_encrypt_cbc(data, encrypted_data, data_len, key, iv);
    triple_des_decrypt_cbc(encrypted_data, decrypted_data, data_len, key, iv);
    
    // Unpad data
    unpad_data(decrypted_data, &data_len);
    
    // Print CBC results
    printf("CBC Mode:\n");
    printf("Encrypted: ");
    for (i = 0; i < data_len; i++) {
        printf("%02X ", encrypted_data[i]);
    }
    printf("\nDecrypted: %s\n", decrypted_data);
    
    // Perform encryption and decryption in CFB mode
    triple_des_encrypt_cfb(data, encrypted_data, data_len, key, iv);
    triple_des_decrypt_cfb(encrypted_data, decrypted_data, data_len, key, iv);
    
    // Print CFB results
    printf("CFB Mode:\n");
    printf("Encrypted: ");
    for (i = 0; i < data_len; i++) {
        printf("%02X ", encrypted_data[i]);
    }
    printf("\nDecrypted: %s\n", decrypted_data);
    
    // Clean up
    free(data);
    free(encrypted_data);
    free(decrypted_data);
    
    return 0;
}
