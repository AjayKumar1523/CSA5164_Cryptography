#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define DES_BLOCK_SIZE 8
#define DES_KEY_SIZE 8

// Function prototypes
void des_encrypt(const uint8_t *block, const uint8_t *key, uint8_t *out);
void des_decrypt(const uint8_t *block, const uint8_t *key, uint8_t *out);
void pad_block(uint8_t *data, size_t *length, size_t block_size);
void unpad_block(uint8_t *data, size_t *length);
void xor_blocks(const uint8_t *block1, const uint8_t *block2, uint8_t *out);
void print_hex(const uint8_t *data, size_t length);

// Dummy DES encryption and decryption (replace with actual DES implementation)
void des_encrypt(const uint8_t *block, const uint8_t *key, uint8_t *out) {
    // Simple XOR-based encryption for illustration
    for (size_t i = 0; i < DES_BLOCK_SIZE; i++) {
        out[i] = block[i] ^ key[i];
    }
}

void des_decrypt(const uint8_t *block, const uint8_t *key, uint8_t *out) {
    // Simple XOR-based decryption for illustration
    for (size_t i = 0; i < DES_BLOCK_SIZE; i++) {
        out[i] = block[i] ^ key[i];
    }
}

// Padding function
void pad_block(uint8_t *data, size_t *length, size_t block_size) {
    size_t padding_length = block_size - (*length % block_size);
    for (size_t i = *length; i < *length + padding_length; i++) {
        data[i] = (uint8_t)padding_length;
    }
    *length += padding_length;
}

// Unpadding function
void unpad_block(uint8_t *data, size_t *length) {
    uint8_t padding_length = data[*length - 1];
    *length -= padding_length;
}

// XOR two blocks
void xor_blocks(const uint8_t *block1, const uint8_t *block2, uint8_t *out) {
    for (size_t i = 0; i < DES_BLOCK_SIZE; i++) {
        out[i] = block1[i] ^ block2[i];
    }
}

// Print data in hex format
void print_hex(const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

// ECB Encryption and Decryption
void ecb_encrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key) {
    size_t blocks = length / DES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        des_encrypt(&input[i * DES_BLOCK_SIZE], key, &output[i * DES_BLOCK_SIZE]);
    }
}

void ecb_decrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key) {
    size_t blocks = length / DES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        des_decrypt(&input[i * DES_BLOCK_SIZE], key, &output[i * DES_BLOCK_SIZE]);
    }
}

// CBC Encryption and Decryption
void cbc_encrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key, uint8_t *iv) {
    uint8_t previous_block[DES_BLOCK_SIZE];
    memcpy(previous_block, iv, DES_BLOCK_SIZE);

    size_t blocks = length / DES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        uint8_t block[DES_BLOCK_SIZE];
        xor_blocks(&input[i * DES_BLOCK_SIZE], previous_block, block);
        des_encrypt(block, key, &output[i * DES_BLOCK_SIZE]);
        memcpy(previous_block, &output[i * DES_BLOCK_SIZE], DES_BLOCK_SIZE);
    }
}

void cbc_decrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key, uint8_t *iv) {
    uint8_t previous_block[DES_BLOCK_SIZE];
    memcpy(previous_block, iv, DES_BLOCK_SIZE);

    size_t blocks = length / DES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        uint8_t block[DES_BLOCK_SIZE];
        des_decrypt(&input[i * DES_BLOCK_SIZE], block, &output[i * DES_BLOCK_SIZE]);
        xor_blocks(block, previous_block, &output[i * DES_BLOCK_SIZE]);
        memcpy(previous_block, &input[i * DES_BLOCK_SIZE], DES_BLOCK_SIZE);
    }
}

// CFB Encryption and Decryption
void cfb_encrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key, uint8_t *iv) {
    uint8_t feedback[DES_BLOCK_SIZE];
    memcpy(feedback, iv, DES_BLOCK_SIZE);

    size_t blocks = length / DES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        uint8_t encrypted_feedback[DES_BLOCK_SIZE];
        des_encrypt(feedback, key, encrypted_feedback);
        xor_blocks(&input[i * DES_BLOCK_SIZE], encrypted_feedback, &output[i * DES_BLOCK_SIZE]);
        memcpy(feedback, &output[i * DES_BLOCK_SIZE], DES_BLOCK_SIZE);
    }
}

void cfb_decrypt(const uint8_t *input, uint8_t *output, size_t length, const uint8_t *key, uint8_t *iv) {
    uint8_t feedback[DES_BLOCK_SIZE];
    memcpy(feedback, iv, DES_BLOCK_SIZE);

    size_t blocks = length / DES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        uint8_t encrypted_feedback[DES_BLOCK_SIZE];
        des_encrypt(feedback, key, encrypted_feedback);
        xor_blocks(&input[i * DES_BLOCK_SIZE], encrypted_feedback, &output[i * DES_BLOCK_SIZE]);
        memcpy(feedback, &input[i * DES_BLOCK_SIZE], DES_BLOCK_SIZE);
    }
}

int main() {
    // Key and initialization vector
    uint8_t key[DES_KEY_SIZE] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    uint8_t iv[DES_BLOCK_SIZE] = { '1', '2', '3', '4', '5', '6', '7', '8' };

    // Message to encrypt and decrypt
    size_t max_length = 64;
    uint8_t *plaintext = (uint8_t *)malloc(max_length);
    uint8_t *ciphertext = (uint8_t *)malloc(max_length);
    uint8_t *decryptedtext = (uint8_t *)malloc(max_length);

    // Example plaintext
    const char *message = "This is a test message for DES encryption.";
    size_t length = strlen(message);
    memcpy(plaintext, message, length);

    // Padding
    pad_block(plaintext, &length, DES_BLOCK_SIZE);

    // ECB Encryption and Decryption
    ecb_encrypt(plaintext, ciphertext, length, key);
    ecb_decrypt(ciphertext, decryptedtext, length, key);
    unpad_block(decryptedtext, &length);
    printf("ECB Decrypted Text: %s\n", decryptedtext);

    // CBC Encryption and Decryption
    cbc_encrypt(plaintext, ciphertext, length, key, iv);
    cbc_decrypt(ciphertext, decryptedtext, length, key, iv);
    unpad_block(decryptedtext, &length);
    printf("CBC Decrypted Text: %s\n", decryptedtext);

    // CFB Encryption and Decryption
    cfb_encrypt(plaintext, ciphertext, length, key, iv);
    cfb_decrypt(ciphertext, decryptedtext, length, key, iv);
    unpad_block(decryptedtext, &length);
    printf("CFB Decrypted Text: %s\n", decryptedtext);

    // Free allocated memory
    free(plaintext);
    free(ciphertext);
    free(decryptedtext);

    return 0;
}
