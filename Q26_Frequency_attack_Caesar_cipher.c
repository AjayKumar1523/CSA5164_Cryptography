#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_CIPHERTEXT_LENGTH 1024
#define TOP_N_RESULTS 10

// Function prototypes
void analyzeFrequency(const char *ciphertext, int *frequency);
void generatePossiblePlaintexts(const int *frequency, char possiblePlaintexts[TOP_N_RESULTS][MAX_CIPHERTEXT_LENGTH], int *numResults);
void printTopResults(const char possiblePlaintexts[TOP_N_RESULTS][MAX_CIPHERTEXT_LENGTH], int numResults);

int main() {
    char ciphertext[MAX_CIPHERTEXT_LENGTH];
    int frequency[ALPHABET_SIZE] = {0};
    char possiblePlaintexts[TOP_N_RESULTS][MAX_CIPHERTEXT_LENGTH];
    int numResults = 0;

    // Input ciphertext
    printf("Enter ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove newline character

    // Analyze letter frequency
    analyzeFrequency(ciphertext, frequency);

    // Generate possible plaintexts
    generatePossiblePlaintexts(frequency, possiblePlaintexts, &numResults);

    // Print top results
    printTopResults(possiblePlaintexts, numResults);

    return 0;
}

// Analyze letter frequency in ciphertext
void analyzeFrequency(const char *ciphertext, int *frequency) {
    size_t length = strlen(ciphertext);
    for (size_t i = 0; i < length; i++) {
        char c = tolower(ciphertext[i]);
        if (isalpha(c)) {
            frequency[c - 'a']++;
        }
    }
}

// Generate possible plaintexts based on frequency analysis
void generatePossiblePlaintexts(const int *frequency, char possiblePlaintexts[TOP_N_RESULTS][MAX_CIPHERTEXT_LENGTH], int *numResults) {
    // Typical English letter frequencies (descending order)
    const char *commonLetters = "etaoinshrdlcumwfgypbvkjxqz";

    // Generate possible plaintexts by replacing the most frequent letters
    // with the most common letters
    int sortedIndices[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sortedIndices[i] = i;
    }

    // Sort indices based on frequency
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (frequency[sortedIndices[i]] < frequency[sortedIndices[j]]) {
                int temp = sortedIndices[i];
                sortedIndices[i] = sortedIndices[j];
                sortedIndices[j] = temp;
            }
        }
    }

    // Generate possible plaintexts
    for (int i = 0; i < TOP_N_RESULTS; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            char c = sortedIndices[j] + 'a';
            possiblePlaintexts[i][j] = c;
        }
        possiblePlaintexts[i][ALPHABET_SIZE] = '\0'; // Null-terminate the string
    }

    *numResults = TOP_N_RESULTS;
}

// Print the top possible plaintexts
void printTopResults(const char possiblePlaintexts[TOP_N_RESULTS][MAX_CIPHERTEXT_LENGTH], int numResults) {
    printf("\nTop possible plaintexts:\n");
    for (int i = 0; i < numResults; i++) {
        printf("%d: %s\n", i + 1, possiblePlaintexts[i]);
    }
}
