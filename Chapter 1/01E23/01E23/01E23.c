/* 01E23.c (p. 34) */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 1000

typedef enum { False, True } bool;
typedef enum { FAILURE = -1, SUCCESS };

bool isCFile(char fname[]);
int readFile(FILE *fptr, char ***ptrStrArr);
int delAllComments(char ***ptrArrIn, char ***ptrArrOut, int sCount);

main(int argc, char *argv[]) {
    int reval;
    FILE *fptr;
    system("pause");

    if (argc == 1) {
        printf("ERROR: Please specify a C code file to run the program on.");
        reval = FAILURE;
    }
    else if (!isCFile(argv[1])) {
        printf("ERROR: %s is not a C code file (doesn't have extension .c)", argv[1]);
        reval = FAILURE;
    }
    else if ((fptr = fopen(argv[1], "r")) == NULL) {
        printf("\nCannot open %s file.\n", argv[1]);
        reval = FAILURE;
    }
    else {
        char **arrIn = malloc(0), **arrOut = malloc(0);
        if (arrIn == NULL || arrOut == NULL) {
            printf("ERROR: Could not allocate initial memory space to either arrIn or arrOut.");
            reval = FAILURE;
        }
        else {
            int lines;
            if ((lines = readFile(fptr, &arrIn)) <= 0) {
                printf("ERROR: %s is either empty, or its contents could not be read.\n", argv[1]);
                reval = FAILURE;
            }
            else {
                fclose(fptr);  // No longer need the file pointer if reading was successful
                arrOut = realloc(arrOut, sizeof(char*) * lines);
                if (arrOut == NULL) {
                    printf("ERROR: Could not reallocate memory space to arrOut before deleting comments.");
                    reval = FAILURE;
                }
                else {
                    for (int i = 0; i < lines; i++) {
                        arrOut[i] = malloc(sizeof(char) * MAXSTRLEN);
                        if (arrOut[i] == NULL) {
                            printf("ERROR: Could not allocate memory space to the pointer at index\n");
                            printf("       %d of arrOut.\n", i);
                            reval = FAILURE;
                        }
                    }

                    int newLines = 0;
                    newLines = delAllComments(&arrIn, &arrOut, lines);

                    if (newLines > 0)
                        for (int i = 0; i < newLines; i++)
                            printf("%s", arrOut[i]);
                    else
                        printf("\nERROR: Unable to remove comments from %s file.\n", argv[1]);

                    for (int pIn = 0; pIn < lines; pIn++)
                        free(arrIn[pIn]);
                    for (int pOut = 0; pOut < newLines; pOut++)
                        free(arrOut[pOut]);
                    free(arrIn);  // No longer need the pointer after printing
                    free(arrOut);

                    reval = SUCCESS;
                }
            }
        }
    }

    printf("\n");
    return reval;
}

/*
 * isCFile: Checks to see if fname is a C code file by looking at the extension for .c
 * Returns: True if it is, False if it isn't.
 */
bool isCFile(char fname[]) {
    int sLen = strlen(fname);
    bool reval;
    if (fname[sLen - 2] == '.' && fname[sLen - 1] == 'c')
        reval = True;
    else
        reval = False;
    return reval;
}

/*
 * readFile: Reads the contents of the file pointed to by fptr, and saves it to the char string array pointed to
 *           by the string array pointer ptrStrArr.
 * Returns:  The number of lines read if successful (or 0 if the file is empty or nothing could be
 *           read), and -1 if it isn't.
 */
int readFile(FILE *fptr, char ***ptrStrArr) {
    int lines = -1;
    do {
        lines++;
        // reallocate memory for the string array stored at ptrStrArr
        *ptrStrArr = realloc(*ptrStrArr, sizeof(char*) * (lines + 1));
        if (ptrStrArr == NULL) {
            printf("ERROR: Could not reallocate memory to *ptrStrArr while reading file at line %d.", lines + 1);
            break;
        }
        else {
            // allocate initial memory for each char arr pointer (string) at (*ptrStrArr)[lines]
            // *ptrStrArr is the address of first element in the string array
            (*ptrStrArr)[lines] = malloc(sizeof(char) * MAXSTRLEN);
            if ((*ptrStrArr)[lines] == NULL) {
                printf("ERROR: Could not allocate memory to line %d of *ptrStrArr while reading file.", lines + 1);
                break;
            }
        }
    } while (fgets((*ptrStrArr)[lines], MAXSTRLEN, fptr) != NULL);
    return lines++;
}

/*
 * delAllComments: deletes all comments in a C code file in arrIn and saves it to arrOut
 * Returns:        the number of lines added to arrOut
 */
int delAllComments(char ***ptrArrIn, char ***ptrArrOut, int sCount) {
    bool inQuote = False;  // whether or not we're currently inside a pair of quotes
    bool inComment = False;  // whether or not we're currently inside a comment block
    char q;  // Store the opening quote or comment char
    int linesIn, linesOut, i = 0, j = 0;

    // i -> arrIn, j -> arrOut
    for (linesIn = 0, linesOut = 0; linesIn < sCount; linesIn++, linesOut++) {
        /* If we're in a comment at the beginning of a new line in arrIn, j should continue from where
         * it left off in the current/previous line of arrOut */
        if (!inComment)
            j = 0;
        for (i = 0, j; i < MAXSTRLEN - 1 && (*ptrArrIn)[linesIn][i] != '\0'; i++, j++) {
            char nextIn = (*ptrArrIn)[linesIn][i];
            char nextnextIn = (*ptrArrIn)[linesIn][i + 1];
            /* If we're not in a comment, see if we fall into one, or add the next char. */
            if (!inComment) {
                /* If we aren't in a quote and a comment is starting... */
                if (!inQuote && nextIn == '/' && (nextnextIn == '*' || nextnextIn == '/')) {
                    q = nextnextIn;
                    i++;  // Jump i to the q character
                    j--;  // Need to keep j in the same place for the next for iteration
                    inComment = True;
                }
                /* Otherwise... */
                else {
                    (*ptrArrOut)[linesOut][j] = nextIn;
                    /* If we're in a quote and nextIn matches the opening quote char... */
                    if (inQuote && nextIn == q) {
                        // Check to make sure the quote character wasn't escaped
                        if (!(i > 0 && (*ptrArrIn)[linesIn][i - 1] == '\\'))
                            inQuote = False;
                    }
                    /* Otherwise, if a quote is starting... */
                    else if (!inQuote && (nextIn == '\'' || nextIn == '"')) {
                        q = nextIn;
                        inQuote = True;
                    }
                }
            }
            /* If we are in a comment, see if we get out of it */
            else {
                // Single line comments end at the end of the line
                if (q == '/' && nextIn == '\n') {
                    (*ptrArrOut)[linesOut][j] = nextIn;
                    inComment = False;
                }
                /* Multi-line comments end when the closing characters are found, and can be in the
                 * middle of a line (with non-commented code after it */
                else if (q == '*' && nextIn == q && nextnextIn == '/') {
                    i++;  // Jump i to the /
                    j--;  // Also need to keep j in the same place on the next for iteration
                    inComment = False;
                }
                /* If neither of the two above, we stay in the comment. Decrement j so that arrOut
                 * stays in the same place on the next for iteration. */
                else {
                    j--;
                }
            }
        }

        /* If we're still in a comment at the end of a line of arrIn, decrement linesOut so we stay on
         * the current line of arrOut for the next iteration of the for loop. */
        if (inComment)
            linesOut--;
        /* If characters were added to the current linesOut of arrOut, add a null character to the
         * end. If we're checking this conditional, we're definitely not in a comment. */
        else if (j > 0)
            (*ptrArrOut)[linesOut][j] = '\0';
    }

    *ptrArrOut = realloc(*ptrArrOut, sizeof(char*) * linesOut);  // fitting array at arrOut to size
    if (*ptrArrOut == NULL) {
        printf("ERROR: Could not reallocate memory to arrOut after deleting comments.");
        return -1;
    }
    else {
        return linesOut;
    }
}