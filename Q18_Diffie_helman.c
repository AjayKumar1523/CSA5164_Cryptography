#include <stdio.h>
#include <math.h>

// Function to perform modular exponentiation
// It returns (base^exp) % mod
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main() {
    // Publicly known prime number q and base a
    long long q, a;

    // Private keys chosen by both participants
    long long xA, xB;

    // Computed values sent between participants
    long long yA, yB;

    // Shared secret keys for both participants
    long long sharedA, sharedB;

    printf("Enter a prime number (q): ");
    scanf("%lld", &q);

    printf("Enter a base (a): ");
    scanf("%lld", &a);

    // Participant A chooses a private key xA
    printf("Participant A, choose your private key (xA): ");
    scanf("%lld", &xA);

    // Participant B chooses a private key xB
    printf("Participant B, choose your private key (xB): ");
    scanf("%lld", &xB);

    // A computes yA = (a^xA) % q and sends it to B
    yA = mod_exp(a, xA, q);
    printf("Participant A computes yA = %lld and sends it to B\n", yA);

    // B computes yB = (a^xB) % q and sends it to A
    yB = mod_exp(a, xB, q);
    printf("Participant B computes yB = %lld and sends it to A\n", yB);

    // A computes the shared secret key: sharedA = (yB^xA) % q
    sharedA = mod_exp(yB, xA, q);
    printf("Participant A computes the shared secret key = %lld\n", sharedA);

    // B computes the shared secret key: sharedB = (yA^xB) % q
    sharedB = mod_exp(yA, xB, q);
    printf("Participant B computes the shared secret key = %lld\n", sharedB);

    // Both sharedA and sharedB should be equal
    if (sharedA == sharedB) {
        printf("Key exchange successful! Shared secret key = %lld\n", sharedA);
    } else {
        printf("Key exchange failed! The keys do not match.\n");
    }

    return 0;
}
