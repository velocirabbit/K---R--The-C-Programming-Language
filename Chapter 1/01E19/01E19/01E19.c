/* 01E19.c (p. 31) */
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000

int getLine(char s[], int sLen);
int len(char s[]);
void reverseStr(char sIn[], char sOut[]);
void addStr2Arr(char *arr, char s[], int sCount, int sLen);
void printStrInArr(char *arr, int pos, int sLen);

main() {
    int lineCount = 0;
    char *strArr = malloc(0);
    char line[MAXLINE];

    while (getLine(line, MAXLINE) > 0) {
        char revLine[MAXLINE];
        reverseStr(line, revLine);
        lineCount++;
        strArr = realloc(strArr, sizeof(char) * lineCount * MAXLINE);
        if (strArr != NULL)
            addStr2Arr(strArr, revLine, lineCount, MAXLINE);
        else
            printf("ERROR: Couldn't add string to strArr at index %d.\n", lineCount - 1);
    }

    if (lineCount > 0) {
        printf("\n\nHere is each line in the input reversed:\n\n");
        for (int i = 0; i < lineCount; i++)
            printStrInArr(strArr, i, MAXLINE);
    }

    free(strArr);
    printf("\n\n");
    system("pause");
    return 0;
}

/* reads a line of input and stores it into s. Returns the length of the line string */
int getLine(char s[], int sLen) {
    int c, i;
    for (i = 0; i < sLen - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c != '\n') {
        s[i] = '\n';
        i++;
    }
    s[i] = '\0';
    return i;
}

/* takes a string and returns its length (including the null character) regardless of its allocated size */
int len(char s[]) {
    int i;
    for (i = 0; s[i] != '\0' && s[i] != '\n'; i++);
    return i;
}

/* takes a string (sIn) and reverses it, storing the reversed string into sOut */
void reverseStr(char sIn[], char sOut[]) {
    int i, l = len(sIn);
    for (i = 0; i <= l - 1; i++)
        sOut[i] = sIn[l - 1 - i];
    sOut[i] = '\n';
    sOut[i + 1] = '\0';
}

/* adds string s into the array of strings arr */
void addStr2Arr(char *arr, char s[], int sCount, int sLen) {
    int i, startPos = sLen * (sCount - 1);
    for (i = 0; i < sLen - 1 && s[i] != '\0'; i++)
        arr[startPos + i] = s[i];
    arr[startPos + i] = '\0';
}

/* prints the string in string array arr at the specified position */
void printStrInArr(char *arr, int pos, int sLen) {
    int i, p = pos * sLen;
    for (i = 0; i < sLen && arr[p + i] != '\0'; i++)
        printf("%c", arr[p + i]);
}