#include <stdio.h>
#include <stdint.h>

#define ROTL(x, n) ((x << n) | (x >> (32 - n)))  // Left circular rotation

// SHA-1 constants for each round
const uint32_t K[4] = {
    0x5A827999,  // (0 <= t <= 19)
    0x6ED9EBA1,  // (20 <= t <= 39)
    0x8F1BBCDC,  // (40 <= t <= 59)
    0xCA62C1D6   // (60 <= t <= 79)
};

// SHA-1 logical functions
uint32_t f(uint32_t t, uint32_t B, uint32_t C, uint32_t D) {
    if (t <= 19) {
        return (B & C) | ((~B) & D);  // Ch (choice)
    } else if (t <= 39 || t >= 60) {
        return B ^ C ^ D;  // Parity
    } else {
        return (B & C) | (B & D) | (C & D);  // Maj (majority)
    }
}

// Single SHA-1 round function
void sha1_single_round(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D, uint32_t *E, uint32_t W, uint32_t Kt, uint32_t t) {
    uint32_t temp = ROTL(*A, 5) + f(t, *B, *C, *D) + *E + W + Kt;
    *E = *D;
    *D = *C;
    *C = ROTL(*B, 30);
    *B = *A;
    *A = temp;
}

int main() {
    // Example internal state variables of SHA-1
    uint32_t A = 0x67452301;  // Initial value of A
    uint32_t B = 0xEFCDAB89;  // Initial value of B
    uint32_t C = 0x98BADCFE;  // Initial value of C
    uint32_t D = 0x10325476;  // Initial value of D
    uint32_t E = 0xC3D2E1F0;  // Initial value of E

    // Example expanded message schedule W for round 0
    uint32_t W = 0x5A827999;  // Example message word

    // Choose the round number (e.g., t = 0 for the first round)
    uint32_t t = 0;

    // Select the constant based on the round number
    uint32_t Kt = K[t / 20];

    // Perform a single SHA-1 round function
    printf("Before round:\nA = 0x%x\nB = 0x%x\nC = 0x%x\nD = 0x%x\nE = 0x%x\n", A, B, C, D, E);

    sha1_single_round(&A, &B, &C, &D, &E, W, Kt, t);

    printf("After round:\nA = 0x%x\nB = 0x%x\nC = 0x%x\nD = 0x%x\nE = 0x%x\n", A, B, C, D, E);

    return 0;
}
