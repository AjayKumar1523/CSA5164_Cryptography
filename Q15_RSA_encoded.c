#include <stdio.h>
#include <stdlib.h>

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

// Function to factorize n using a given common factor
void factorize_n(int n, int common_factor, int *p, int *q) {
    if (n % common_factor == 0) {
        *p = common_factor;
        *q = n / common_factor;
    } else {
        *p = *q = -1; // Not a valid factorization
    }
}

int main() {
    int n = 3599;         // RSA modulus
    int e = 31;           // Public exponent
    int common_factor = 59; // Given common factor of the plaintext block with n
    int p, q;
    int phi_n;
    int d;

    // Step 1: Factorize n using the common factor
    factorize_n(n, common_factor, &p, &q);
    if (p == -1 || q == -1) {
        printf("Error: Common factor does not divide n\n");
        return 1;
    }

    printf("p = %d\n", p);
    printf("q = %d\n", q);

    // Step 2: Compute phi(n)
    phi_n = (p - 1) * (q - 1);
    printf("phi(n) = %d\n", phi_n);

    // Step 3: Compute the modular multiplicative inverse of e modulo phi(n)
    d = mod_inverse(e, phi_n);
    printf("Private key (d) = %d\n", d);

    return 0;
}
