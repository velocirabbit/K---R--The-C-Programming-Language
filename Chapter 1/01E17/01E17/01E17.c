/* 01E17.c (p. 31) */
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000  // Max character length for a string (including null character)
#define THRESHOLD 40  // Threshold character length for strings to print

int getLine(char s[], int sLen);
void addStr2Arr(char *arr, char s[], int sLen, int strCount);
void printStrInArr(char *arr, int pos, int sLen);

main() {
    int len, lineCount;
    char line[MAXLINE];
    char *strArr = malloc(0);  // create a pointer with no space allocated

    lineCount = 0;
    while ((len = getLine(line, MAXLINE)) > 0) {
        if (len >= THRESHOLD) {
            lineCount++;
            strArr = realloc(strArr, MAXLINE * sizeof(char) * lineCount);  // reallocate memory to allow more room for a new string
            if (strArr != NULL)
                addStr2Arr(strArr, line, MAXLINE, lineCount);  // MAXLINE + 1 accounts for the null char
            else
                printf("\n\nERROR: Could not add another string to the string array\nat index %d.\n\n", lineCount - 1);
        }
    }
    if (lineCount > 0) {  // there was at least one line longer than the threshold length
        printf("\nThese lines were longer than %d characters:\n\n", THRESHOLD);
        for (int i = 0; i < lineCount; i++)
            printStrInArr(strArr, i, MAXLINE);
    }
    free(strArr);
    printf("\n\n");
    system("pause");
    return 0;
}

/* reads a line into s and returns the length */
int getLine(char s[], int sLen) {
    int c, i;

    for (i = 0; i < sLen - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        i++;
    }
    s[i] = '\0';
    return i;
}

/* adds the string to an array of strings*/
void addStr2Arr(char *arr, char s[], int sLen, int strCount) {
    int startPos, i;
    startPos = sLen * (strCount - 1);
    for (i = 0; i < sLen - 1 && s[i] != EOF; i++)
        arr[startPos + i] = s[i];
    arr[startPos + i] = '\0';
}

/* prints the string in the array at the specified position */
void printStrInArr(char *arr, int pos, int sLen) {
    int p = pos * sLen;
    int i;
    for (i = 0; i < sLen && arr[p + i] != EOF && arr[p + i] != '\n'; i++)
        printf("%c", arr[p + i]);
    printf("\n");
}