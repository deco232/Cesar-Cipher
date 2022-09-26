#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 40
#define ENCODE 0
#define SUCCESS 0
#define DECODE 1
#define OFFSET 26

#define COMMAND 1
#define K 2
#define INPUT_FILE 3
#define OUTPUT_FILE 4

#define USAGE_ERROR "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n"
#define INVALID_COMMAND "The given command is invalid\n"
#define USAGE_ERROR_CHECK "Usage: cipher <check> <source path file> <output path file>\n"
#define INVALID_FILE "The given file is invalid\n"
#define INVALID_ENCRYPTING "Invalid encrypting\n"
#define VALID_ENCRYPTING "Valid encrypting with k = %d\n"

bool both_capital_or_small(const char *str1, const char *str2, int i){
    return ((str1[i] >= 'A' && str1[i] <= 'Z') && (str2[i] >= 'A' && str2[i] <= 'Z')) ||
           ((str1[i] >= 'a' && str1[i] <= 'z') && (str2[i] >= 'a' && str2[i] <= 'z'));
}

/**
 *  This function get String, integer and index, updates string[i] into String[i] shifted k times forward if it
 *  belongs to {a,...,z,A,...,Z}
 */
void encode_or_decode_letter(char *str1, int k, int i, int enc_or_dec) {
    char letter = str1[i];
    if (enc_or_dec == DECODE)
        k = k * (-1);

    char new_letter = letter;

    // if letter is Capital and out of range after shifted.
    if (letter >= 'A' && letter <= 'Z'){
        new_letter = (char)(letter + k);
        if (new_letter > 'Z')
            new_letter -= OFFSET;
        if (new_letter < 'A')
            new_letter += OFFSET;
    }

    // if letter is small and out of range after shifted.
    if (letter >= 'a' && letter <= 'z'){
        new_letter = (char)(letter + k);
        if (new_letter > 'z')
            new_letter -= OFFSET;
        if (new_letter < 'a')
            new_letter += OFFSET;
    }

    str1[i] = new_letter;
}

/**
 * This function gets String and integer and Shifts every letter belongs to {a,...,z,A,...,Z} in str1 k
 * times forward in a cyclic way.
 */
void encode(char* str1, int k) {
    int len = (int) strlen(str1);
    for (int i = 0; i < len; i++)
        encode_or_decode_letter(str1, k, i, ENCODE);
}

/**
 * gets string (str1) and integer (k).
 * the function shifts every letter in str1 k times backwards
 */
void decode(char* str1, int k)
{
  int len = (int) strlen(str1);
  for (int i = 0; i < len ; i++)
        encode_or_decode_letter(str1, k, i, DECODE);
}


/**
 * Checks if str2 is valid encryption of str1.
 * @param str1 - source string.
 * @param str2 - encoded string
 * @param k - the parameter to return.
 * @return - the encryption value (k) if str2 is valid encryption of str2, -1 else.
 */
int check(char* str1, char* str2, int k) {
    int len1 = (int) strlen(str1);
    int len2 = (int) strlen(str2);

    if (len1 != len2)
        return -1;

    int diff;

    for (int i = 0; i < len1; i++) {
        diff = str2[i] - str1[i];

        if (both_capital_or_small(str1, str2, i)) {
            if (i == 0)
                k = diff;
            else if (k != diff)
                return -1;
        }
        else if (diff != 0)
            return -1;
    }
    return k;
}


int main(int argc, char *argv[]) {
    // If we didn't get any arguments.
    if (argc <= 1) {
        fprintf(stderr, INVALID_COMMAND);
        return EXIT_FAILURE;
    }

    // Input check for encode and decode commands.
    if (strcmp(argv[COMMAND], "encode") == SUCCESS || strcmp(argv[COMMAND], "decode") == SUCCESS) {
        // If command is encode or decode but there is no 3 additional arguments.
        if (argc != 5) {
            fprintf(stderr, USAGE_ERROR);
            return EXIT_FAILURE;
        }

        // read files
        char str1[BUFFER_SIZE];
        FILE *f1 = fopen(argv[INPUT_FILE], "r");
        FILE *f2 = fopen(argv[OUTPUT_FILE], "w");

        //  Check for reading errors.
        if (f1 == NULL || f2 == NULL) {
            fprintf(stderr, INVALID_FILE);
            return EXIT_FAILURE;
        }

        // Set k between -25 to 25
        int k = (int) strtol(argv[K], NULL, 10);
        k = k % 25;

        // Start encoding or decoding.
        if (strcmp(argv[COMMAND], "encode") == SUCCESS) {
            while (fgets(str1, sizeof(str1), f1) != NULL) {
                encode(str1, k);
                fprintf(f2, "%s", str1);
            }
        }
        else if (strcmp(argv[COMMAND], "decode") == SUCCESS) {
            while (fgets(str1, sizeof(str1), f1) != NULL) {
                decode(str1, k);
                fprintf(f2, "%s", str1);
            }
        }

        // close files.
        fclose(f1);
        fclose(f2);
    }

    // Input check for check command.
    else if (strcmp(argv[COMMAND], "check") == SUCCESS) {
        /* Check amount of variables */
        if (argc != 4) {
            fprintf(stderr, USAGE_ERROR_CHECK);
            return EXIT_FAILURE;
        }

        // read files
        char str1[BUFFER_SIZE];
        char str2[BUFFER_SIZE];
        FILE *f1 = fopen(argv[2], "r");
        FILE *f2 = fopen(argv[INPUT_FILE], "r");

        // Check reading errors.
        if (f1 == NULL || f2 == NULL) {
            fprintf(stderr, INVALID_FILE);
            return EXIT_FAILURE;
        }

        int k;
        while (fgets(str1, sizeof(str1), f1) != NULL) {
            if (fgets(str2, sizeof(str2), f2) == NULL) {
                fprintf(stdout, INVALID_ENCRYPTING);
                return EXIT_SUCCESS;
            }
            k = check(str1, str2, -1);
            if (k == -1) {
                fprintf(stdout, INVALID_ENCRYPTING);
                return EXIT_SUCCESS;
            }
        }
        fprintf(stdout, VALID_ENCRYPTING, k);
        fclose(f1);
        fclose(f2);
    }

    // Invalid command.
    else {
        fprintf(stderr, INVALID_COMMAND);
        return EXIT_FAILURE;
    }
}