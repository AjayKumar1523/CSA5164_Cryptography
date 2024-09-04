#include <stdio.h>
#include <ctype.h>
void encrypt(char *text, int shift) {
    while (*text) {
        if (isalpha(*text)) {
            char base = islower(*text) ? 'a' : 'A';
            *text = (char)(((*text - base) + shift) % 26 + base);
        }
        text++;
    }
}
void decrypt(char *text, int shift) {
    while (*text) {
        if (isalpha(*text)) {
            char base = islower(*text) ? 'a' : 'A';
            *text = (char)(((*text - base) - shift + 26) % 26 + base);
        }
        text++;
    }
}
int main() {
    char text[100];
    int shift; 
    printf("Enter the plaintext (max 99 characters): ");
    scanf("%99[^\n]", text);   
    printf("Enter the shift value: ");
    scanf("%d", &shift);   
    encrypt(text, shift); 
    printf("Encrypted text: %s\n", text);    
    decrypt(text, shift);
    printf("Decrypted text: %s\n", text);
    return 0;
}
