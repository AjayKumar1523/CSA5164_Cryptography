#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// S-DES parameters
#define BLOCK_SIZE 8 // 8 bits for S-DES
#define KEY_SIZE 10 // 10 bits for S-DES key

// S-DES S-boxes and Permutations
static const uint8_t S1[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

static const uint8_t S2[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {1, 2, 3, 0},
    {1, 1, 2, 2}
};

static const uint8_t P4[4] = {2, 4, 3, 1}; // P4 permutation
static const uint8_t P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6}; // P10 permutation
static const uint8_t P8[8] = {6, 3, 7, 4, 8, 5, 10, 9}; // P8 permutation

// Key schedule functions
void permute(const uint8_t *in, uint8_t *out, const uint8_t *perm, size_t size) {
    for (size_t i = 0; i < size; i++) {
        out[i] = in[perm[i] - 1];
    }
}

void generate_subkeys(const uint8_t *key, uint8_t *k1, uint8_t *k2) {
    uint8_t permuted_key[KEY_SIZE / 8];
    uint8_t left[5], right[5];
    
    permute(key, permuted_key, P10, KEY_SIZE / 8);
    
    // Split into left and right halves
    memcpy(left, permuted_key, 5);
    memcpy(right, permuted_key + 5, 5);
    
    // Perform shifts and generate subkeys
    // Shift left
    left[0] = left[1];
    left[1] = left[2];
    left[2] = left[3];
    left[3] = left[4];
    left[4] = left[0];
    
    right[0] = right[1];
    right[1] = right[2];
    right[2] = right[3];
    right[3] = right[4];
    right[4] = right[0];
    
    uint8_t combined[10] = {0};
    memcpy(combined, left, 5);
    memcpy(combined + 5, right, 5);
    permute(combined, k1, P8, 8);
    
    // Shift left twice
    left[0] = left[2];
    left[1] = left[3];
    left[2] = left[4];
    left[3] = left[0];
    left[4] = left[1];
    
    right[0] = right[2];
    right[1] = right[3];
    right[2] = right[4];
    right[3] = right[0];
    right[4] = right[1];
    
    memcpy(combined, left, 5);
    memcpy(combined + 5, right, 5);
    permute(combined, k2, P8, 8);
}

// Function to apply the S-DES F function
uint8_t f_function(uint8_t r, uint8_t key) {
    uint8_t expanded_r = r; // Apply expansion permutation (not used here for simplicity)
    
    uint8_t temp = expanded_r ^ key;
    uint8_t s1_out = S1[(temp >> 2) & 0x03][temp & 0x03];
    uint8_t s2_out = S2[(temp >> 2) & 0x03][temp & 0x03];
    
    uint8_t combined = (s1_out << 2) | s2_out;
    return combined;
}

// S-DES Encryption function
void sdes_encrypt(uint8_t *block, const uint8_t *key1, const uint8_t *key2) {
    uint8_t left = block[0];
    uint8_t right = block[1];
    
    // Initial permutation
    uint8_t temp = left;
    left = right;
    right = temp;
    
    // Round 1
    uint8_t temp_right = right;
    right = left ^ f_function(right, *key1);
    left = temp_right;
    
    // Round 2
    temp_right = right;
    right = left ^ f_function(right, *key2);
    left = temp_right;
    
    // Final permutation
    block[0] = right;
    block[1] = left;
}

// CBC Mode Encryption
void sdes_encrypt_cbc(const uint8_t *data, uint8_t *encrypted_data, size_t data_len, const uint8_t *key, const uint8_t *iv) {
    uint8_t k1[8], k2[8];
    uint8_t block[2];
    size_t i;
    
    generate_subkeys(key, k1, k2);
    
    uint8_t previous_block[2];
    memcpy(previous_block, iv, 2);
    
    for (i = 0; i < data_len; i += 2) {
        memcpy(block, data + i, 2);
        
        // XOR with previous ciphertext block (or IV for the first block)
        block[0] ^= previous_block[0];
        block[1] ^= previous_block[1];
        
        // Encrypt block
        sdes_encrypt(block, k1, k2);
        
        memcpy(encrypted_data + i, block, 2);
        memcpy(previous_block, block, 2);
    }
}

// CBC Mode Decryption
void sdes_decrypt_cbc(const uint8_t *data, uint8_t *decrypted_data, size_t data_len, const uint8_t *key, const uint8_t *iv) {
    uint8_t k1[8], k2[8];
    uint8_t block[2];
    size_t i;
    
    generate_subkeys(key, k1, k2);
    
    uint8_t previous_block[2];
    memcpy(previous_block, iv, 2);
    
    for (i = 0; i < data_len; i += 2) {
        memcpy(block, data + i, 2);
        
        // Decrypt block
        sdes_encrypt(block, k2, k1);
        
        // XOR with previous ciphertext block (or IV for the first block)
        block[0] ^= previous_block[0];
        block[1] ^= previous_block[1];
        
        memcpy(decrypted_data + i, block, 2);
        memcpy(previous_block, data + i, 2);
    }
}

// Function to convert binary string to byte array
void binary_str_to_byte_array(const char *str, uint8_t *arr, size_t arr_size) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 8; i++) {
        arr[i] = str[i * 8] - '0';
        for (size_t j = 1; j < 8; j++) {
            arr[i] = (arr[i] << 1) | (str[i * 8 + j] - '0');
        }
    }
}

// Function to print binary array
void print_binary_array(const uint8_t *arr, size_t arr_size) {
    for (size_t i = 0; i < arr_size; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (arr[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

int main() {
    const char *key_str = "0111111101"; // Binary key for S-DES (10 bits)
    const char *iv_str = "10101010"; // Binary IV (8 bits)
    const char *plaintext_str = "00000001 00100011"; // Binary plaintext (16 bits)
    const char *expected_ciphertext_str = "11110100 00001011"; // Expected ciphertext (16 bits)

    uint8_t key[KEY_SIZE / 8];
    uint8_t iv[BLOCK_SIZE / 8];
    uint8_t plaintext[BLOCK_SIZE / 8];
    uint8_t ciphertext[BLOCK_SIZE / 8];
    uint8_t decrypted[BLOCK_SIZE / 8];

    binary_str_to_byte_array(key_str, key, sizeof(key));
    binary_str_to_byte_array(iv_str, iv, sizeof(iv));
    binary_str_to_byte_array(plaintext_str, plaintext, sizeof(plaintext));

    // Encrypt
    sdes_encrypt_cbc(plaintext, ciphertext, sizeof(plaintext), key, iv);

    printf("Encrypted: ");
    print_binary_array(ciphertext, sizeof(ciphertext));

    // Decrypt
    sdes_decrypt_cbc(ciphertext, decrypted, sizeof(ciphertext), key, iv);

    printf("Decrypted: ");
    print_binary_array(decrypted, sizeof(decrypted));

    return 0;
}
