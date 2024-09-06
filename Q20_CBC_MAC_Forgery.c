#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Simulate a block size of 16 bytes (128 bits) for simplicity
#define BLOCK_SIZE 16

// XOR function for two blocks of data
void xor_blocks(uint8_t *output, uint8_t *block1, uint8_t *block2) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = block1[i] ^ block2[i];
    }
}

// Simple encryption function (for demonstration, using XOR with key)
void encrypt_block(uint8_t *output, uint8_t *block, uint8_t *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = block[i] ^ key[i];  // XOR encryption
    }
}

// Function to compute CBC-MAC for a one-block message
void compute_mac(uint8_t *T, uint8_t *X, uint8_t *key) {
    encrypt_block(T, X, key);  // In CBC-MAC, we just encrypt the block with the key
}

// Print block as hex
void print_block(uint8_t *block) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x", block[i]);
    }
    printf("\n");
}

int main() {
    // Secret key (for simplicity, 128-bit key)
    uint8_t key[BLOCK_SIZE] = {0x1f, 0x2e, 0x3d, 0x4c, 0x5b, 0x6a, 0x79, 0x88, 
                               0x97, 0xa6, 0xb5, 0xc4, 0xd3, 0xe2, 0xf1, 0x00};

    // Message X (one-block message)
    uint8_t X[BLOCK_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                             0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00};

    // T = MAC(K, X)
    uint8_t T[BLOCK_SIZE];  // MAC of one-block message X
    compute_mac(T, X, key);

    // Print MAC T
    printf("MAC for one-block message X (T): ");
    print_block(T);

    // Compute the second block as (X ⊕ T)
    uint8_t X_xor_T[BLOCK_SIZE];
    xor_blocks(X_xor_T, X, T);

    // Now create the two-block message X || (X ⊕ T)
    uint8_t two_block_mac[BLOCK_SIZE];

    // Step 1: Encrypt the first block X to get the first intermediate MAC
    uint8_t intermediate[BLOCK_SIZE];
    encrypt_block(intermediate, X, key);

    // Step 2: XOR the intermediate MAC with the second block (X ⊕ T) and encrypt
    encrypt_block(two_block_mac, X_xor_T, key);

    // Print the CBC-MAC for the two-block message X || (X ⊕ T)
    printf("MAC for two-block message X || (X ⊕ T): ");
    print_block(two_block_mac);

    return 0;
}
