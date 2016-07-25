/* 01E24.c (p. 34) */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 10000
#define ENDCHAR ';'

typedef enum { False, True } bool;
typedef enum { FAILURE = -1, SUCCESS } result;

bool isCFile(char fname[]);
int saveFile(FILE *fptr, char ***ptrStrArr);
bool checkSyntax(char ***ptrStrArr, int arrLen);

main(int argc, char *argv[]) {
    result reval;
    FILE *fptr;
    system("pause");  // allows for an easier time attaching debugger to process if needed
    printf("\n");  // Give a couple lines of breather room

    if (argc == 1) {
        printf("ERROR: Please specify a C code file (with .c extension) to run this program on.\n");
        reval = FAILURE;
    }
    else if (!isCFile(argv[1])) {
        printf("ERROR: %s not a C code file (doesn't have extension .c)\n", argv[1]);
        reval = FAILURE;
    }
    else if ((fptr = fopen(argv[1], "r")) == NULL) {
        printf("ERROR: Cannot open file %s.\n", argv[1]);
        reval = FAILURE;
    }
    else {
        // If the file opens successfully, save it to a string array
        int numLines;
        char **strArr = malloc(0);

        if (strArr == NULL) {
            printf("ERROR: Could not allocate inital memory space to strArr before reading file.\n");
            reval = FAILURE;
        }
        else {
            if ((numLines = saveFile(fptr, &strArr)) <= 0) {
                printf("ERROR: Could not read contents of %s.\n", argv[1]);
                reval = FAILURE;
            }
            else if (numLines == 0) {
                printf("ERROR: %s is either empty, or the contents are unable to be read or invisible.\n", argv[1]);
                reval = FAILURE;
            }
            else if (numLines > 0) {  // If it's neither of the above, it should always be this unless there's a huge fuckup somewhere
                if (checkSyntax(&strArr, numLines))
                    printf("Syntax looks good! All of your blocks are closed properly.\n");
                else
                    printf("\nLooks like you have some unclosed code blocks. See the above output for details.\n");
                reval = SUCCESS;
            }
        }

        for (int p = 0; p < numLines; p++)
            free(strArr[p]);
        free(strArr);
    }

    return reval;
}

/* isCFile: Checks to see if the passed in file name is a C code file by looking at the
 *          file extension.
 * Returns: True (1) if it is a C code file, False (0) if it isn't
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

/* saveFile: Takes the file pointed to by fptr and saves it to the string array pointed to by
 *           ptrStrArr.
 * Returns:  the number of lines written to the string array if successful (0 if nothing was
 *           written), or -1 if there was an error.
 */
int saveFile(FILE *fptr, char ***ptrStrArr) {
    int lines = -1;
    do {
        lines++;
        *ptrStrArr = realloc(*ptrStrArr, sizeof(char*) * (lines + 1));
        if (*ptrStrArr == NULL) {
            printf("ERROR: Couldn't reallocate memory to *ptrStrArr while saving file contents at line %d.\n", lines + 1);
            return -1;
        }
        else {
            (*ptrStrArr)[lines] = malloc(sizeof(char) * MAXSTRLEN);
            if ((*ptrStrArr)[lines] == NULL) {
                printf("ERROR: Couldn't allocate memory to string at line %d while saving file contents.\n", lines + 1);
                return -1;
            }
        }
    } while (fgets((*ptrStrArr)[lines], MAXSTRLEN, fptr) != NULL);

    return lines;
}

/* checkSyntax: Checks the syntax of the C code in string array pointed to by ptrStrArr for
 *              rudimentary syntax errors like unbalanced parenthesis, brackets, braces,
 *              quotes (both single and double), escape sequences, and comments.
 * Returns:     True if the C code is syntactically correct (at least with closing all blocks
 *              correctly, as per the list of things it checks above), False if it's not.
 */
bool checkSyntax(char ***ptrStrArr, int arrLen) {
    char *brackets = malloc(0), ch;  // The brackets char array will hold the order of bracket appearance
    bool inQuote = False, inComment = False, syntaxOK = True;
    int totalBrackets = 0;

    for (int ln = 0; ln < arrLen; ln++) {
        for (int col = 0; col < MAXSTRLEN - 1 && (*ptrStrArr)[ln][col] != '\0'; col++) {
            char nextCh = (*ptrStrArr)[ln][col];
            char nextnextCh = (*ptrStrArr)[ln][col + 1];
            /* If we're currently inside a comment */
            if (inComment) {
                /* If we exit a single-line comment */
                if (ch == '/' && (nextCh == '\n' || nextCh == '\0')) {
                    inComment = False;
                }
                /* If we exit a multi-line comment */
                else if (ch == '*' && nextCh == ch && nextnextCh == '/') {
                    inComment = False;
                    col++;
                }
            }
            /* If we're currently not inside a comment */
            else {
                /* If we're currently inside a quote */
                if (inQuote) {
                    /* If we encounter an escape character, ignore whatever's next by jumping to next char */
                    if (nextCh == '\\') {
                        col++;
                    }
                    /* If we reach the end of a line before the quote is closed */
                    else if (nextCh == '\n') {
                        inQuote = False;
                        printf("ERROR: missing closing quote ('%c') at Line %d, Col %d\n", ch, ln + 1, col + 1);
                        syntaxOK = False;
                    }
                    /* If the quote ends */
                    else if (nextCh == ch) {
                        inQuote = False;
                    }
                }
                /* If we're currently not inside a quote. Since we're also not inside a comment, this is where
                 * typical code will fall under, including checking for completion of braces, brackets, etc. */
                else {
                    /* If we enter a comment */
                    if (nextCh == '/' && (nextnextCh == '/' || nextnextCh == '*')) {
                        inComment = True;
                        ch = nextnextCh;
                        col++;
                    }
                    /* If we enter a quote */
                    else if (nextCh == '\'' || nextCh == '"') {
                        inQuote = True;
                        ch = nextCh;
                    }
                    /* Everything else */
                    else {
                        if (nextCh == '(' || nextCh == '{' || nextCh == '[') {
                            totalBrackets++;
                            brackets = realloc(brackets, sizeof(char) * totalBrackets);
                            // ASCII value for right closing bracket is left opening bracket + 2
                            // Except for parenthesis, which is +1
                            brackets[totalBrackets - 1] = (nextCh == '(') ? nextCh + 1 : nextCh + 2;
                        }
                        else if (nextCh == ')' || nextCh == '}' || nextCh == ']') {
                            if (totalBrackets <= 0) {
                                printf("ERROR: Unexpected '%c' at Line %d, Col %d\n", nextCh, ln + 1, col + 1);
                                syntaxOK = False;
                            }
                            else {
                                char lastBrack = brackets[totalBrackets - 1];
                                if (nextCh == lastBrack) {
                                    totalBrackets--;
                                }
                                else {
                                    printf("ERROR: Expected a '%c' at Line %d, Col %d (found a %c)\n", lastBrack, ln + 1, col + 1, nextCh);
                                    syntaxOK = False;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (inComment) {
        printf("ERROR: Unclosed multi-line comment at EOF.\n");
        syntaxOK = False;
    }

    if (totalBrackets > 0) {
        printf("ERROR: Missing closing brackets at EOF. In order from inside to out:\n       ");
        for (int i = totalBrackets - 1; i >= 0; i--) {
            printf("%c", brackets[totalBrackets]);
            if (i > 0)
                printf(", ");
        }
        printf("\n");
    }

    free(brackets);
    return syntaxOK;
}