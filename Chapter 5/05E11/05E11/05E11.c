/* 05E11.c (p. 118) */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 1000

void entab(char s[], int n, char use[], int sLen);
void detab(char s[], int n, char use[], int sLen);
int lookAhead(char s[], char c, int pos, int sLen);

int main(int argc, char *argv[]) {
    // Long usage message below
    if (argc < 2) {
        printf("\nUsage: 05E11 -(t/d) [-n] [...]\n");
        printf("\nRequired arguments:\n");
        printf("       -t:    Convert sets of n space characters into tab\n");
        printf("              characters.\n");
        printf("       -d:    Convert tab characters into n space characters.\n");
        printf("              Only uses the first in the list of space characters\n");
        printf("              to use, if specified.\n");
        printf("\nOptional arguments:\n");
        printf("       [-n]:  Specifies the number of space characters. Should be\n");
        printf("              prefaced with a '-'. Defaults to 4.\n");
        printf("       [...]: Specifies a list of characters to interpret as\n");
        printf("              spaces. Don't add any spaces between characters.\n");
        printf("              Space characters (' ') will always be considered as\n");
        printf("              spaces.\n");
        return 1;
    }

    int num = 4;
    char *use = malloc(sizeof(char[2]));
    strcpy(use, " ");

    // Get number of characters to use
    if (argc > 4)
        printf("\nERROR: Too many arguments!\n");
    else if (argc >= 3 && argv[2][0] == '-') {
        num = atoi(++(argv[2]));
        strcpy(++use, argv[3]);
    }
    else {
        if (argc == 4) {
            printf("\nERROR: Char num n should be prefaced with a '-' to work.\n");
            printf("         Using n = 4 instead.\n\n");
        }
        if (argv[1][1] == 't')
            use++;
        strcpy(use, argv[argc - 1]);
        if (argv[1][1] == 't')
            use--;
    }

    char s[MAXSTRLEN];
    strcpy(s, ".........\tlol jk\tnot.....X");
    printf("\nOriginal:\n%s\n", s);
    switch (argv[1][1]) {
        case 't':
            entab(s, num, use, MAXSTRLEN);
            break;
        case 'd':
            detab(s, num, use, MAXSTRLEN);
            break;
        default:
            break;
    }

    printf("\nAfter:\n%s\n", s);
    return 0;
}

/**
* entab: Replaces strings of n blanks by the minimum number of tabs and blanks to
*   achieve the same spacing. Looks for n-length strings of each character in string
*   use.
*/
void entab(char s[], int num, char use[], int sLen) {
    int i, j;
    char copied[MAXSTRLEN];
    strcpy(copied, s);  // Use copied as a reference string
    for (i = 0, j = 0; i < sLen && copied[j]; i++, j++) {
        int found = 0;
        for (unsigned int k = 0; k < strlen(use); k++) {
            char c = use[k];
            if (!found && copied[j] == c) {
                int blanks = lookAhead(copied, c, j, sLen);
                j = j + blanks - 1;
                int tabs = 0;
                if (i % num > 0 && blanks > i % num) {
                    tabs++;
                    blanks = blanks - (num - (i % num));
                    tabs = tabs + (blanks / num);  // integer division will truncate
                    blanks = blanks - ((tabs - 1) * num);
                }
                else {
                    tabs = tabs + (blanks / num);  // integer division will truncate
                    blanks = blanks - (tabs * num);
                }
                if (tabs > 0) {
                    for (int t = 0; t < tabs; t++)
                        s[i++] = '\t';
                }
                if (blanks > 0) {
                    for (int b = 0; b < blanks; b++)
                        s[i++] = c;
                }
                i--;  // Should end up on the last tab or space after adding them to s
                found = 1;
            }
        }
        if (!found)
            s[i] = copied[j];
    }
    s[i] = 0;
}

/**
* detab: Replaces tabs with n blanks. Uses the first character in the string use.
*/
void detab(char s[], int num, char use[], int sLen) {
    int i, numtabs = 0;
    char c = use[0];
    for (i = 0; i < sLen && s[i]; i++)
        if (s[i] == '\t')
            numtabs++;
    int numchars = i + (numtabs * (num - 1));
    if (numchars < sLen) {
        char copied[MAXSTRLEN];
        strcpy(copied, s);  // Use copied as a reference string
        for (int i = 0, j = 0; i < numchars; i++, j++) {
            if (copied[j] == '\t') {
                int tabSpaces = num - (i % num);
                for (int t = 0; t < tabSpaces; t++)
                    s[i++] = c;
                i--;  // Should be on the last inserted character
            }
            else
                s[i] = copied[j];
        }
    }
    else
        numchars = i;  // if there isn't room, do nothing for now
    s[i] = 0;
}

/**
* lookAhead: starting at index pos in string s, looks at the remainder of the string
* and counts the number of occurances of char c up until either a different char is
* encountered, or it reaches the end of the string.
*
* Returns: The number of consecutive occurances of char c
*/
int lookAhead(char s[], char c, int pos, int sLen) {
    int count = 0;
    for (int i = pos; i < sLen && s[i] == c; i++, count++);
    return count;
}