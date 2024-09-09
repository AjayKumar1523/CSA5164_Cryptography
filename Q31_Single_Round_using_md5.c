#include <stdio.h>
#include <stdint.h>

// Macros for left circular rotation
#define ROTL(x, n) ((x << n) | (x >> (32 - n)))

// MD5 auxiliary functions
#define F(B, C, D) ((B & C) | (~B & D))
#define G(B, C, D) ((B & D) | (C & ~D))
#define H(B, C, D) (B ^ C ^ D)
#define I(B, C, D) (C ^ (B | ~D))

// MD5 constants (first four rounds)
const uint32_t K[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821
};

// Rotation amounts for each step
const uint32_t s[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22
};

// Perform a single MD5 round function
void md5_single_round(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D, uint32_t M[16], uint32_t t) {
    uint32_t temp;
    temp = *A + F(*B, *C, *D) + M[t] + K[t];
    *A = *D;
    *D = *C;
    *C = *B;
    *B = *B + ROTL(temp, s[t]);
}

int main() {
    // Initial state variables of MD5
    uint32_t A = 0x67452301;  // Initial value of A
    uint32_t B = 0xefcdab89;  // Initial value of B
    uint32_t C = 0x98badcfe;  // Initial value of C
    uint32_t D = 0x10325476;  // Initial value of D

    // Example message block (512 bits divided into 16 words of 32 bits each)
    uint32_t M[16] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                      0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                      0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                      0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821};

    // Perform a single round of MD5
    printf("Before round:\nA = 0x%x\nB = 0x%x\nC = 0x%x\nD = 0x%x\n", A, B, C, D);

    for (int t = 0; t < 16; ++t) {
        md5_single_round(&A, &B, &C, &D, M, t);
    }

    printf("After round:\nA = 0x%x\nB = 0x%x\nC = 0x%x\nD = 0x%x\n", A, B, C, D);

    return 0;
}
