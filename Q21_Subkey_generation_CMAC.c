#include <stdio.h>
#include <stdint.h>
#include <string.h>

// AES block size
#define BLOCK_SIZE 16
#define R128 0x87  // Constant used in CMAC subkey generation

// AES S-box (substitution box)
static const uint8_t SBOX[256] = {
    // Initialize with the standard AES S-box values
    // This is a partial example; the actual S-box is more extensive
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab,
    // More values here
};

// AES round constants
static const uint8_t RCON[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// AES key expansion (simplified)
void aes_key_schedule(const uint8_t *key, uint8_t *round_keys) {
    // Initialize round keys with the given key
    memcpy(round_keys, key, BLOCK_SIZE);

    // Simplified key expansion (only for AES-128)
    for (int i = 1; i < 11; i++) {
        // Key schedule logic (omitted for brevity)
        // In a full implementation, you would add round constants and apply S-box transformations
    }
}

// AES encryption function
void aes_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *round_keys) {
    // Simple substitution (not a real AES implementation)
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = SBOX[input[i]];
    }

    // AES encryption rounds (simplified, only for demonstration)
    for (int round = 0; round < 10; round++) {
        // AddRoundKey, SubBytes, ShiftRows, and MixColumns steps would be here
    }
}

// Left shift a block of data by 1 bit
void left_shift_block(uint8_t *output, const uint8_t *input) {
    uint8_t carry = 0;
    for (int i = BLOCK_SIZE - 1; i >= 0; i--) {
        uint8_t next_carry = (input[i] >> 7) & 1;
        output[i] = (input[i] << 1) | carry;
        carry = next_carry;
    }
}

// XOR a block of data with a constant
void xor_block(uint8_t *block, uint8_t constant) {
    block[BLOCK_SIZE - 1] ^= constant;
}

// Generate the CMAC subkeys K1 and K2
void generate_subkeys(uint8_t *K1, uint8_t *K2, const uint8_t *key) {
    uint8_t L[BLOCK_SIZE] = {0};  // L is the result of AES encryption of a zero block
    uint8_t zero_block[BLOCK_SIZE] = {0};  // Block of all zeros
    uint8_t round_keys[BLOCK_SIZE];  // Round keys for AES encryption

    // Generate AES round keys
    aes_key_schedule(key, round_keys);

    // Encrypt the zero block with AES using the provided key
    aes_encrypt(zero_block, L, round_keys);

    // Derive K1 from L by left-shifting and conditional XOR
    left_shift_block(K1, L);
    if (L[0] & 0x80) {  // If the MSB of L is 1, XOR with the constant R128
        xor_block(K1, R128);
    }

    // Derive K2 from K1 by left-shifting and conditional XOR
    left_shift_block(K2, K1);
    if (K1[0] & 0x80) {  // If the MSB of K1 is 1, XOR with the constant R128
        xor_block(K2, R128);
    }
}

// Print a block as hex
void print_block(const uint8_t *block) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x", block[i]);
    }
    printf("\n");
}

int main() {
    uint8_t key[BLOCK_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                               0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};  // Example key
    uint8_t K1[BLOCK_SIZE] = {0};  // First subkey
    uint8_t K2[BLOCK_SIZE] = {0};  // Second subkey

    // Generate subkeys K1 and K2
    generate_subkeys(K1, K2, key);

    // Print the generated subkeys
    printf("Subkey K1: ");
    print_block(K1);
    printf("Subkey K2: ");
    print_block(K2);

    return 0;
}

