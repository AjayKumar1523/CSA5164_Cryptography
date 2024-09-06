#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>  // For uint64_t

#define LANES 25          // 5x5 matrix of lanes
#define LANE_SIZE 64      // 64 bits per lane (total 1024-bit block size)

// Function to fill a message block with random data
void fill_random_message(uint64_t message_block[LANES]) {
    for (int i = 0; i < LANES; i++) {
        // Fill the message block with random data
        message_block[i] = rand() | ((uint64_t)rand() << 32);
    }
}

// Function to check if all lanes have non-zero values
bool check_all_nonzero(uint64_t state[LANES]) {
    for (int i = 0; i < LANES; i++) {
        if (state[i] == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    uint64_t state[LANES];                // Internal state (5x5 lanes)
    uint64_t message_block[LANES];        // A message block (1024 bits total)
    int rounds = 0;

    // Initialize the internal state (capacity portion is zero)
    memset(state, 0, sizeof(state));

    srand(time(NULL));  // Seed the random number generator

    printf("Simulation of SHA-3 lane filling.\n");

    // User input: how many message blocks to simulate
    int max_blocks;
    printf("Enter the maximum number of message blocks to absorb: ");
    scanf("%d", &max_blocks);

    clock_t start = clock();  // Start measuring time

    // Absorb message blocks until all lanes are non-zero or max_blocks is reached
    while (rounds < max_blocks && !check_all_nonzero(state)) {
        fill_random_message(message_block);

        // XOR the message block into the state
        for (int i = 0; i < LANES; i++) {
            state[i] ^= message_block[i];
        }

        rounds++;
    }

    clock_t end = clock();  // End measuring time
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (check_all_nonzero(state)) {
        printf("All lanes became nonzero after %d rounds.\n", rounds);
    } else {
        printf("Not all lanes became nonzero after %d rounds.\n", rounds);
    }

    printf("Time taken: %f seconds\n", time_taken);

    return 0;
}
