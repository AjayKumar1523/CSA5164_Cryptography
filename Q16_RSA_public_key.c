#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to compute the greatest common divisor using Euclidean algorithm
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to compute modular multiplicative inverse using Extended Euclidean Algorithm
int mod_inverse(int a, int m) {
    int m0 = m;
    int y = 0, x = 1;

    if (m == 1) return 0;

    while (a > 1) {
        int q = a / m;
        int t = m;

        m = a % m;
        a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0) x += m0;

    return x;
}

// Function to generate a random prime number (not a secure prime number generator)
int generate_prime() {
    return rand() % 100 + 2; // Generate a small random prime number
}

// Function to find primes p and q
void find_primes(int *p, int *q) {
    while (1) {
        *p = generate_prime();
        *q = generate_prime();
        if (*p != *q && gcd(*p, *q) == 1) break;
    }
}

// Function to encrypt a message
int encrypt(int message, int e, int n) {
    int result = 1;
    for (int i = 0; i < e; i++) {
        result = (result * message) % n;
    }
    return result;
}

// Function to decrypt a message
int decrypt(int ciphertext, int d, int n) {
    int result = 1;
    for (int i = 0; i < d; i++) {
        result = (result * ciphertext) % n;
    }
    return result;
}

int main() {
    int p, q, n, phi_n, e, d;
    int message = 65; // Example plaintext message

    srand(time(NULL));

    // Generate new prime numbers p and q
    find_primes(&p, &q);
    n = p * q;
    phi_n = (p - 1) * (q - 1);

    // Choose a public exponent e (should be coprime with phi_n and 1 < e < phi_n)
    e = 3; // Small prime number for demonstration
    if (gcd(e, phi_n) != 1) {
        printf("Error: e is not coprime with phi(n)\n");
        return 1;
    }

    // Compute the private key d
    d = mod_inverse(e, phi_n);

    printf("Generated primes: p = %d, q = %d\n", p, q);
    printf("Modulus n = %d\n", n);
    printf("Public key (e) = %d\n", e);
    printf("Private key (d) = %d\n", d);

    // Encrypt the message
    int ciphertext = encrypt(message, e, n);
    printf("Encrypted message: %d\n", ciphertext);

    // Decrypt the message
    int decrypted_message = decrypt(ciphertext, d, n);
    printf("Decrypted message: %d\n", decrypted_message);

    return 0;
}
