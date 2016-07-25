/* 01E20.c */
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000  // longest line length

int getLine(char s[], int sLen);
int replaceTabs(char s[], char sym, int num, int sLen);
void copy(char sIn[], char sOut[]);
void addStr2Arr(char *arr, char s[], int sCount, int sLen);
void printStrInArr(char *arr, int pos, int sLen);

main() {
    int lineCount = 0;
    char line[MAXLINE], sym = '.', *strArr = malloc(0);  // initially allocate 0 memory

    while (getLine(line, MAXLINE) > 0) {
        lineCount++;
        replaceTabs(line, '.', 8, MAXLINE);  // replace all tabs with 8 .'s
        strArr = realloc(strArr, sizeof(char) * lineCount * MAXLINE);
        if (strArr != NULL)
            addStr2Arr(strArr, line, lineCount, MAXLINE);
        else
            printf("ERROR: Unable to reallocate memory for string at index %d.\n\n", lineCount - 1);
    }
    if (lineCount > 0) {  // if there were any lines in the input
        printf("\n\nHere are the input strings with the tabs replaced by %c's:\n\n", sym);
        for (int i = 0; i < lineCount; i++)
            printStrInArr(strArr, i, MAXLINE);
    }

    free(strArr);
    printf("\n\n");
    system("pause");
    return 0;
}

/* getLine: saves a line of input from stdin into string s and returns the number
 * of characters in the line (including the null character) */
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

/* replaceTabs: replace any tab characters ('\t') in the string with the specified number of 
 * the specified symbol. Returns the new numbers of characters in the string */
int replaceTabs(char s[], char sym, int num, int sLen) {
    int i, numTabs = 0;
    // count the number of tab characters ('\t') in string s
    for (i = 0; i < sLen && s[i] != '\0'; i++)
        if (s[i] == '\t')
            numTabs++;
    int numChars = i + (numTabs * (num - 1));
    if (numChars < sLen) {  // if there's room in the string to replace the tabs
        char copiedStr[MAXLINE];
        copy(s, copiedStr);  // use copiedStr as a reference to the original
        for (int i = 0, j = 0; i < numChars; i++, j++) {  // i -> s, j -> copiedStr
            if (copiedStr[j] == '\t') {
                int tabSpaces = num - (i % num);
                for (int t = 0; t < tabSpaces; t++) {
                    s[i] = sym;
                    i++;
                }
                i--;  // Should be on the last inserted char after replacing the tabs
            }
            else
                s[i] = copiedStr[j];
        }
    }
    else {
        // if there isn't room to replace the tabs, just do nothing for now
        numChars = i;
    }

    return numChars;
}

/* deep copies sIn into sOut */
void copy(char sIn[], char sOut[]) {
    int i;
    for (i = 0; sIn[i] != '\0'; i++)
        sOut[i] = sIn[i];
    sOut[i] = '\0';
}

/* adds string s into the string array arr */
void addStr2Arr(char *arr, char s[], int sCount, int sLen) {
    int i, startPos = sLen * (sCount - 1);
    for (i = 0; i < sLen - 1 && s[i] != '\0'; i++)
        arr[startPos + i] = s[i];
    arr[startPos + i] = '\0';
}

/* prints the string at index pos in string array arr to stdout */
void printStrInArr(char *arr, int pos, int sLen) {
    int startPos = sLen * pos;
    for (int i = 0; i < sLen && arr[startPos + i] != '\0'; i++)
        printf("%c", arr[startPos + i]);
}