#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define TOP_PLAINTEXTS 10

// English letter frequencies based on relative usage in the English language
double englishFrequencies[ALPHABET_SIZE] = {
    12.02, 9.10, 8.12, 7.68, 7.31, 6.95, 6.28, 6.02, 5.92, 5.85,
    4.10, 2.71, 2.53, 2.36, 2.18, 2.09, 2.03, 1.82, 1.70, 1.54,
    1.33, 1.02, 0.89, 0.56, 0.14, 0.12
};

// Function prototypes
void performFrequencyAttack(const char *ciphertext, int topN);
double scorePlaintext(const char *plaintext);
void generateFrequencyTable(const char *text, double *freqTable);
void printTopNResults(const char possiblePlaintexts[ALPHABET_SIZE][100], double scores[ALPHABET_SIZE], int topN);
void substituteCiphertext(const char *ciphertext, const char *substitution, char *result);

int main() {
    char ciphertext[100];
    int topN;

    // Get the ciphertext from the user
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove newline character

    // Get the number of top plaintexts to show
    printf("Enter the number of top possible plaintexts (e.g., 10): ");
    scanf("%d", &topN);

    // Perform frequency analysis and show topN results
    performFrequencyAttack(ciphertext, topN);

    return 0;
}

// Function to perform the frequency attack and display top N results
void performFrequencyAttack(const char *ciphertext, int topN) {
    char substitution[ALPHABET_SIZE] = {0}; // Substitution alphabet
    char possiblePlaintexts[ALPHABET_SIZE][100]; // Store possible plaintexts
    double scores[ALPHABET_SIZE]; // Store scores for each substitution

    // Generate frequency table for the ciphertext
    double cipherFreqTable[ALPHABET_SIZE] = {0};
    generateFrequencyTable(ciphertext, cipherFreqTable);

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            substitution[j] = 'a' + ((i + j) % ALPHABET_SIZE); // Generate substitutions
        }

        // Substitute ciphertext to generate a possible plaintext
        substituteCiphertext(ciphertext, substitution, possiblePlaintexts[i]);

        // Score the plaintext based on letter frequency
        scores[i] = scorePlaintext(possiblePlaintexts[i]);
    }

    // Print the top N results
    printTopNResults(possiblePlaintexts, scores, topN);
}

// Function to generate the frequency table for a given text
void generateFrequencyTable(const char *text, double *freqTable) {
    int letterCounts[ALPHABET_SIZE] = {0};
    int totalLetters = 0;

    // Count letter occurrences
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            int letter = tolower(text[i]) - 'a';
            letterCounts[letter]++;
            totalLetters++;
        }
    }

    // Calculate the frequency of each letter
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freqTable[i] = (totalLetters > 0) ? (double)letterCounts[i] / totalLetters * 100 : 0;
    }
}

// Function to score a plaintext based on English letter frequencies
double scorePlaintext(const char *plaintext) {
    double plaintextFreqTable[ALPHABET_SIZE] = {0};
    double score = 0.0;

    // Generate frequency table for the plaintext
    generateFrequencyTable(plaintext, plaintextFreqTable);

    // Calculate the score by comparing frequencies
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double observedFrequency = plaintextFreqTable[i];
        score += (englishFrequencies[i] - observedFrequency) * (englishFrequencies[i] - observedFrequency);
    }

    return score; // Lower score means a better match
}

// Function to substitute ciphertext using a given substitution alphabet
void substituteCiphertext(const char *ciphertext, const char *substitution, char *result) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            result[i] = substitution[tolower(ciphertext[i]) - 'a'];
            if (isupper(ciphertext[i])) {
                result[i] = toupper(result[i]);
            }
        } else {
            result[i] = ciphertext[i]; // Non-alphabetic characters remain unchanged
        }
    }
    result[strlen(ciphertext)] = '\0'; // Null-terminate the result string
}

// Function to print the top N plaintexts with the best scores
void printTopNResults(const char possiblePlaintexts[ALPHABET_SIZE][100], double scores[ALPHABET_SIZE], int topN) {
    int indices[ALPHABET_SIZE];

    // Initialize indices
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        indices[i] = i;
    }

    // Sort the scores using a simple selection sort
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (scores[i] > scores[j]) {
                // Swap scores
                double tempScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempScore;

                // Swap corresponding indices
                int tempIndex = indices[i];
                indices[i] = indices[j];
                indices[j] = tempIndex;
            }
        }
    }

    // Print the top N results
    printf("\nTop %d possible plaintexts:\n", topN);
    for (int i = 0; i < topN; i++) {
        int index = indices[i];
        printf("%d. Substitution %d: %s (Score: %.2f)\n", i + 1, index, possiblePlaintexts[index], scores[i]);
    }
}
