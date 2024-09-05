#include <stdio.h>
#include <stdint.h>

#define TOTAL_SUBKEYS 16  // DES generates 16 subkeys
#define KEY_SIZE 56       // DES uses a 56-bit key
#define SUBKEY_SIZE 48    // Each subkey is 48 bits

// Helper function to print binary
void printBinary(uint64_t value, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

// Function to extract the first 24 bits and second 24 bits from the key
void generateSubkeys(uint64_t key56) {
    // 28-bit subsets
    uint32_t leftSubset = (key56 >> 28) & 0x0FFFFFFF;  // First 28 bits
    uint32_t rightSubset = key56 & 0x0FFFFFFF;         // Last 28 bits

    printf("Initial 56-bit Key: ");
    printBinary(key56, 56);

    printf("\nInitial 28-bit Left Subset: ");
    printBinary(leftSubset, 28);

    printf("Initial 28-bit Right Subset: ");
    printBinary(rightSubset, 28);

    // Generate 16 subkeys
    for (int round = 0; round < TOTAL_SUBKEYS; round++) {
        // In a full DES implementation, we would rotate leftSubset and rightSubset
        // according to a predefined rotation schedule, but here we'll simplify by not rotating.

        // Extract the first 24 bits from the left subset and the first 24 bits from the right subset
        uint32_t left24Bits = leftSubset >> 4;  // Extracting first 24 bits from the left 28-bit subset
        uint32_t right24Bits = rightSubset >> 4;  // Extracting first 24 bits from the right 28-bit subset

        // Combine to form a 48-bit subkey
        uint64_t subkey = ((uint64_t)left24Bits << 24) | right24Bits;

        // Print the subkey
        printf("\nSubkey %d (48 bits): ", round + 1);
        printBinary(subkey, SUBKEY_SIZE);
    }
}

int main() {
    // Example 56-bit key (in practice, this would be 64 bits, with 8 parity bits discarded)
    uint64_t key56 = 0x123456789ABC;  // Example key (56 bits)

    // Generate subkeys
    generateSubkeys(key56);

    return 0;
}
