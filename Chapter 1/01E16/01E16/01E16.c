/* 01E16.c (p. 30) */
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000
#define THRESHOLD 15  // arbitrary threshold; any line longer than this length will be printed

int getline(char line[], int maxLine);
void addStr2Arr(char *arr, char s[], int maxLine, int strCount);
void printStrInArr(char *arr, int pos, int maxLine);

main() {
    int len, lineCount;
    int *lenArr = malloc(0);
    char *strArr = malloc(0);  // create a pointer with no memory space allocated
    char line[MAXLINE];

    lineCount = 0;
    while ((len = getline(line, MAXLINE)) > 0)
        if (len > THRESHOLD) {
            lineCount++;
            strArr = realloc(strArr, sizeof(char) * MAXLINE * lineCount);
            lenArr = realloc(lenArr, sizeof(int) * lineCount);
            if (strArr != NULL && lenArr != NULL) {
                addStr2Arr(strArr, line, MAXLINE, lineCount);
                *(lenArr + lineCount - 1) = len;
            }
            else {
                if (strArr == NULL)
                    printf("Could not add line to strArr at index %d....\n", lineCount - 1);
                if (lenArr == NULL)
                    printf("Could not add length of line to lenArr at index %d....\n", lineCount - 1);
            }
        }

    if (lineCount > 0) {  // there was at least one line longer than the threshold length
        printf("\nThese lines were longer than the threshold length of %d:\n\n", THRESHOLD);
        for (int i = 0; i < lineCount; i++)
            printStrInArr(strArr, i, MAXLINE);
    }
    free(strArr);
    printf("\n\n");
    system("pause");
    return 0;
}

int getline(char s[], int maxline) {
    int c, i;

    for (i = 0; i < maxline - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        i++;
    }
    s[i] = '\0';
    return i;
}

void addStr2Arr(char *arr, char s[], int maxline, int strCount) {
    int startPos, i;
    startPos = maxline * (strCount - 1);
    for (i = 0; i < maxline - 1 && s[i] != EOF; i++)  // all strings added to arr will end with \0
        arr[startPos + i] = s[i];
    arr[startPos + i] = '\0';
}

void printStrInArr(char *arr, int pos, int maxline) {
    int p = pos * maxline;
    int i;
    for (i = 0; i < maxline && arr[p + i] != EOF && arr[p + i] != '\n'; i++)
        printf("%c", arr[p + i]);
    printf(":       \t%d chars\n", i - 1);
}