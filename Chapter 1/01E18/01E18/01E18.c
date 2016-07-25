/* 01E18.c (p. 31) */
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000

int getLine(char s[], int sLen);
int trimRight(char s[], int sLen);
void addStr2Arr(char *arr, char s[], int sCount, int sLen);
void printStrInArr(char *arr, int pos, int sLen);

main() {
    int len, lineCount;
    char line[MAXLINE], *strArr;
    strArr = malloc(0);  // create a pointer allocated with no memory space
    lineCount = 0;

    while ((len = getLine(line, MAXLINE)) > 0) {
        if (trimRight(line, MAXLINE) > 0) {
            lineCount++;
            strArr = realloc(strArr, sizeof(char) * lineCount * MAXLINE);
            if (strArr != NULL)
                addStr2Arr(strArr, line, lineCount, MAXLINE);
            else
                printf("Unable to allocate memory in strArr at index %d....\n", lineCount - 1);
        }
    }

    if (lineCount > 0) {  // if any nonblank lines were found
        printf("\nRemoving whitespace characters at the end of each line and deleting blank lines:\n\n");
        for (int i = 0; i < lineCount; i++)
            printStrInArr(strArr, i, MAXLINE);
    }

    free(strArr);
    printf("\n\n");
    system("pause");
    return 0;
}

/* gets the next line from the input source and returns the number of characters */
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

/* removes any trailing blanks and tabs from each line of input. Returns the number of remaining
 * characters, or -1 if a line is entirely blank */
int trimRight(char s[], int sLen) {
    int i, lastNonWS = -1;
    for (i = 0; i < sLen - 2 && s[i] != '\n' && s[i] != '\0'; i++)
        if (s[i] != ' ' && s[i] != '\t')
            lastNonWS = i;
    if (lastNonWS != -1) {
        lastNonWS++;
        if (s[lastNonWS] != '\n') {
            s[lastNonWS] = '\n';
            lastNonWS++;
        }
        s[lastNonWS] = '\0';
    }
    return lastNonWS;
}

/* adds a string to the array at the end */
void addStr2Arr(char *arr, char s[], int sCount, int sLen) {
    int i, startPos;
    startPos = sLen * (sCount - 1);
    for (i = 0; i < sLen - 1 && s[i] != EOF && s[i] != '\n'; i++)
        arr[startPos + i] = s[i];
    arr[startPos + i] = '\0';
}

/* prints the string in the array at the specified index */
void printStrInArr(char *arr, int pos, int sLen) {
    int i, p = pos * sLen;
    for (i = 0; i < sLen && arr[p + i] != '\0' && arr[p + i] != '\n'; i++)
        printf("%c", arr[p + i]);
    printf("\n");
}