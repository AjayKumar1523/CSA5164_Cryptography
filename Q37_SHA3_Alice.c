#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define LANES 25          // Total lanes in the 5x5 SHA-3 matrix
#define CAPACITY_LANES 16 // 1024 bits -> 16 lanes for capacity
#define BIT_MASK 0xFFFFFFFFFFFFFFFF // Mask for 64 bits (each lane)

// SHA-3 state matrix
uint64_t state[LANES];

// Function to initialize the capacity lanes with random nonzero values
void initialize_capacity() {
    for (int i = LANES - CAPACITY_LANES; i < LANES; i++) {
        state[i] = ((uint64_t)rand() & BIT_MASK) | 1; // Ensure nonzero initialization
    }
}

// Function to apply the message block and update the state matrix
void apply_message_block() {
    for (int i = 0; i < LANES; i++) {
        // XOR random values to simulate message absorption into lanes
        state[i] ^= (uint64_t)rand() & BIT_MASK;
    }
}

// Function to check if all capacity lanes are nonzero
int all_capacity_nonzero() {
    for (int i = LANES - CAPACITY_LANES; i < LANES; i++) {
        if (state[i] == 0) {
            return 0; // If any capacity lane is zero, return false
        }
    }
    return 1; // All capacity lanes are nonzero
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    // Initialize the state matrix, particularly the capacity lanes
    initialize_capacity();

    int iterations = 0;

    // Simulate message block processing until all capacity lanes become nonzero
    while (!all_capacity_nonzero()) {
        apply_message_block();
        iterations++;
    }

    printf("All capacity lanes became nonzero after %d iterations.\n", iterations);

    return 0;
}
