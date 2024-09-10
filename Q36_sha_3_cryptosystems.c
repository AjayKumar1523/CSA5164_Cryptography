#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define LANES 25           // Total lanes in SHA-3 (5x5 matrix)
#define CAPACITY_LANES 2    // Number of lanes dedicated to capacity (1024 bits)
#define BIT_MASK 0xFFFFFFFFFFFFFFFF // Mask for 64 bits (each lane)

uint64_t state[LANES]; // State matrix (5x5 lanes)

// Function to check if all capacity lanes have non-zero values
int all_capacity_nonzero() {
    for (int i = 0; i < CAPACITY_LANES; i++) {
        if (state[LANES - CAPACITY_LANES + i] == 0) {
            return 0; // Capacity lane is still zero
        }
    }
    return 1; // All capacity lanes are non-zero
}

// Function to apply the message block (P0) and update the state
void apply_message_block() {
    for (int i = 0; i < LANES; i++) {
        // XOR random bits into the lanes
        state[i] ^= (uint64_t)rand() & BIT_MASK;
    }
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    // Initialize state matrix, with all capacity lanes set to zero
    for (int i = 0; i < LANES; i++) {
        state[i] = 0;
    }

    int iterations = 0;
    
    // Keep applying message blocks until all capacity lanes have non-zero bits
    while (!all_capacity_nonzero()) {
        apply_message_block();
        iterations++;
    }

    printf("All capacity lanes became non-zero after %d iterations.\n", iterations);
    
    return 0;
}
