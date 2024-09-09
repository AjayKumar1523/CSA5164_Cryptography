#include <stdio.h>
#include <stdint.h>

#define N 16  // Number of rounds

// P-array and S-boxes initialized with hexadecimal digits of pi
uint32_t P[18] = {
    0x243F6A88, 0x85A308D3, 0x13198A2E, 0x03707344,
    0xA4093822, 0x299F31D0, 0x082EFA98, 0xEC4E6C89,
    0x452821E6, 0x38D01377, 0xBE5466CF, 0x34E90C6C,
    0xC0AC29B7, 0xC97C50DD, 0x3F84D5B5, 0xB5470917,
    0x9216D5D9, 0x8979FB1B
};

uint32_t S[4][256] = { /* Large S-boxes omitted for brevity, these need to be initialized correctly for full security */ };

uint32_t F(uint32_t x) {
    // Blowfish F function, which operates on S-boxes
    uint16_t a = (x >> 24) & 0xFF;
    uint16_t b = (x >> 16) & 0xFF;
    uint16_t c = (x >>  8) & 0xFF;
    uint16_t d =  x        & 0xFF;

    return ((S[0][a] + S[1][b]) ^ S[2][c]) + S[3][d];
}

void blowfish_encrypt(uint32_t *left, uint32_t *right) {
    uint32_t L = *left;
    uint32_t R = *right;

    for (int i = 0; i < N; i++) {
        L ^= P[i];
        R ^= F(L);

        // Swap L and R
        uint32_t temp = L;
        L = R;
        R = temp;
    }

    // Undo last swap
    uint32_t temp = L;
    L = R;
    R = temp;

    R ^= P[N];
    L ^= P[N + 1];

    *left = L;
    *right = R;
}

void blowfish_decrypt(uint32_t *left, uint32_t *right) {
    uint32_t L = *left;
    uint32_t R = *right;

    for (int i = N + 1; i > 1; i--) {
        L ^= P[i];
        R ^= F(L);

        // Swap L and R
        uint32_t temp = L;
        L = R;
        R = temp;
    }

    // Undo last swap
    uint32_t temp = L;
    L = R;
    R = temp;

    R ^= P[1];
    L ^= P[0];

    *left = L;
    *right = R;
}

int main() {
    uint32_t left = 0x12345678;
    uint32_t right = 0x9ABCDEF0;

    printf("Plaintext: %08X %08X\n", left, right);

    blowfish_encrypt(&left, &right);
    printf("Encrypted: %08X %08X\n", left, right);

    blowfish_decrypt(&left, &right);
    printf("Decrypted: %08X %08X\n", left, right);

    return 0;
}
