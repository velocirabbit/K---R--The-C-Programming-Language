/* 01E21.c */
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000  // max line length

int getLine(char s[], int sLen);
int replaceBlanks(char s[], int tabSize, int sLen);
int lookAhead(char s[], char c, int pos, int sLen);
void copy(char sIn[], char sOut[]);
void addStr2Arr(char *arr, char s[], int sCount, int sLen);
void printStrInArr(char *arr, int pos, int sLen);

main() {
    int lineCount = 0;
    char line[MAXLINE], *strArr = malloc(0);

    while (getLine(line, MAXLINE) > 0) {
        lineCount++;
        replaceBlanks(line, 8, MAXLINE);
        strArr = realloc(strArr, sizeof(char) * lineCount * MAXLINE);
        if (strArr != NULL)
            addStr2Arr(strArr, line, lineCount, MAXLINE);
        else
            printf("ERROR: Could not reallocate memory size for string at index %d.\n\n", lineCount - 1);
    }

    if (lineCount > 0) {
        printf("\n\nHere is each line of input with sufficiently large strings of blanks\n");
        printf("replaced with tabs:\n\n");
        for (int i = 0; i < lineCount; i++)
            printStrInArr(strArr, i, MAXLINE);
    }

    free(strArr);
    printf("\n\n");
    system("pause");
    return 0;
}

/* getLine: gets a line of text from stdin. Returns the char length of the line. */
int getLine(char s[], int sLen) {
    int c, i;
    for (i = 0; i < sLen - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c == '\n') {
        s[i] = '\n';
        i++;
    }
    s[i] = '\0';
    return i;
}

/* replaceBlanks: replaces strings of tabSize blanks with the tab character ('\t').
 * Returns the new char length of the line. */
int replaceBlanks(char s[], int tabSize, int sLen) {
    int i, j;
    char copiedStr[MAXLINE];
    copy(s, copiedStr);  // create a reference copy of string s
    for (i = 0, j = 0; i < sLen && copiedStr[j] != '\0'; i++, j++) {
        if (copiedStr[j] == ' ') {
            int numBlanks = lookAhead(copiedStr, ' ', j, sLen);
            j = j + numBlanks - 1;
            int numTabs = 0;
            if (i % tabSize > 0 && numBlanks > i % tabSize) {
                numTabs++;
                numBlanks = numBlanks - (tabSize - (i % tabSize));
                numTabs = numTabs + (numBlanks / tabSize);  // integer division will truncate down to whole number
                numBlanks = numBlanks - ((numTabs - 1) * tabSize);
            }
            else {
                numTabs = numTabs + (numBlanks / tabSize);  // integer division will truncate down to whole number
                numBlanks = numBlanks - (numTabs * tabSize);
            }
            if (numTabs > 0)
                for (int t = 0; t < numTabs; t++) {
                    s[i] = '\t';
                    i++;
                }
            if (numBlanks > 0)
                for (int b = 0; b < numBlanks; b++) {
                    s[i] = ' ';
                    i++;
                }
            i--;  // should end up on the last tab or space after adding them to s
        }
        else
            s[i] = copiedStr[j];
    }
    s[i] = '\0';
    return i;
}

/* lookAhead: starting at index pos in string s, looks at the remainder of the string and
 * counts the number of occurances of char c up until a different char is encounter or it
 * reaches the end of the string */
int lookAhead(char s[], char c, int pos, int sLen) {
    int count = 0;
    for (int i = pos; i < sLen && s[i] == c; i++, count++);
    return count;
}

/* copy: deep copies string sIn into sOut */
void copy(char sIn[], char sOut[]) {
    int i;
    for (i = 0; sIn[i] != '\0'; i++)
        sOut[i] = sIn[i];
    sOut[i] = '\0';
}

/* addStr2Arr: adds str s to the end of string array arr */
void addStr2Arr(char *arr, char s[], int sCount, int sLen) {
    int i, startPos = sLen * (sCount - 1);
    for (i = 0; i < sLen - 1 && s[i] != '\0'; i++)
        arr[startPos + i] = s[i];
    arr[startPos + i] = '\0';
}

/* printStrInArr: prints out the string at index pos of string array arr to stdout */
void printStrInArr(char *arr, int pos, int sLen) {
    int startPos = sLen * pos;
    for (int i = 0; i < sLen && arr[startPos + i] != '\0'; i++)
        printf("%c", arr[startPos + i]);
}