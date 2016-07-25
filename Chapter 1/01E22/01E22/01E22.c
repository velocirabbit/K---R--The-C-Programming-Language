/* 01E22.c */
#include <stdio.h>
#include <stdlib.h>
#define STRINGLENGTH 10000
#define MAXWIDTH (80 + 2)  // The '+2' accounts for the newline and null characters

int getLine(char s[], int sLen);
int replaceTabs(char s[], int tabSize, int sLen);
int lengthNextWord(char s[], int pos);
int wrapLines(char *arr, char s[], int sLen);
void copy(char sIn[], char sOut[]);
void addMultiStr2Arr(char *fromArr, char *toArr, int numLines, int sCount, int sLen);
void printStrInArr(char *arr, int pos, int sLen);


main() {
    int len, lineCount = 0;
    char line[STRINGLENGTH], *strArr = malloc(0);
    char *splitLines;

    while ((len = getLine(line, STRINGLENGTH)) > 0) {
        len = replaceTabs(line, 8, STRINGLENGTH);
        // split the string up into multiple strings of length MAXWIDTH
        int numLines = (len / MAXWIDTH) + 1;  // integer division will give us the floor when we really need the ceiling
        int g;
        splitLines = malloc(sizeof(char) * numLines * MAXWIDTH);
        if ((g = wrapLines(splitLines, line, MAXWIDTH)) != numLines) {
            printf("\n\nError spliting up the input line into multiple lines!\n\n");
            printf("g = %d\tnumLines = %d\n", g, numLines);
        }
        else {
            lineCount = lineCount + numLines;
            strArr = realloc(strArr, sizeof(char) * lineCount * MAXWIDTH);
            addMultiStr2Arr(splitLines, strArr, numLines, lineCount, MAXWIDTH);
        }
    }

    if (lineCount > 0) {
        printf("\n\nHere is the input broken up into shorter lines:\n\n");
        for (int i = 0; i < lineCount; i++)
            printStrInArr(strArr, i, MAXWIDTH);
    }

    free(splitLines);
    free(strArr);
    printf("\n\n");
    system("pause");
    return 0;
}

/* getLine: gets the next line of input from stdin. Returns the number of characters in the line */
int getLine(char s[], int sLen) {
    int i, c;

    for (i = 0; i < sLen - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c == '\n') {
        s[i] = '\n';
        i++;
    }
    s[i] = '\0';
    return i;
}

/* replaceTabs: replaces every tab character ('\t') in the string with tabSize spaces. Returns the
 * total number of characters with the tabs replaced. */
int replaceTabs(char s[], int tabSize, int sLen) {
    char copyS[STRINGLENGTH];
    copy(s, copyS);
    int i, j;  // i for s, j for copyS
    for (i = 0, j = 0; copyS[j] != '\0'; i++, j++) {
        if (copyS[j] == '\t') {
            for (int t = 0; t < tabSize; t++, i++)
                s[i] = ' ';
            i--;  // i should be the index of the last space after replacing the tab character
        }
        else
            s[i] = copyS[j];
    }
    s[i] = '\0';
    return i;
}

/* lengthNextWord: returns the length of the next word starting at position pos. Basically counts each
 * character until it reaches a whitespace (space, tab, newline), null character, or EOF */
int lengthNextWord(char s[], int pos) {
    int i;
    for (i = 0; s[pos + i] != ' ' && s[pos + i] != '\n' && s[pos + i] != '\t'
        && s[pos + i] != '\0' && s[pos + i] != EOF; i++);
    return i;
}

/* wrapLines: takes a long string s and splits it into multiple lines, then adds all of those lines
 * into string array arr. Note that the input string s should have its tabs removed for best results 
 * Returns the number of lines s has been split into. */
int wrapLines(char *arr, char s[], int sLen) {
    int i = 0, j = 0, lines = 0, chars = 0;  // i for s, j for arr
    while (s[i] != '\0') {
        //if (chars == 0)
        //    for (s[i]; s[i] == ' ' || s[i] == '\t'; i++);
        int lenNextWord = lengthNextWord(s, i);
        if (lenNextWord == 0) {
            arr[j] = s[i];
            i++;
            j++;
            chars++;
        }
        else if (lenNextWord > 0 && chars + lenNextWord < sLen - 1) {
            chars = chars + lenNextWord;
            for (int w = 0; w < lenNextWord; i++, j++, w++)
                arr[j] = s[i];
        }
        else {
            lines++;
            arr[j] = '\n';
            j++;
            arr[j] = '\0';
            j = lines * sLen;
            chars = 0;
        }
    }
    arr[j] = '\0';
    lines++;
    return lines;
}

/* copy: deep copies sIn into sOut */
void copy(char sIn[], char sOut[]) {
    int i;
    for (i = 0; sIn[i] != '\0'; i++)
        sOut[i] = sIn[i];
    sOut[i] = '\0';
}

/* addMultiStr2Arr: adds an array of strings fromArr to the end of another array of strings toArr */
void addMultiStr2Arr(char *fromArr, char *toArr, int numLines, int sCount, int sLen) {
    int i, c, startPos = sLen * (sCount - numLines);
    for (i = 0, c = 0; c < numLines; i++) {
        toArr[startPos + i] = fromArr[i];
        if (fromArr[i] == '\0')
            c++;
    }
}

/* printStrInArr: prints out the string at index pos in string array arr */
void printStrInArr(char *arr, int pos, int sLen) {
    int startPos = sLen * pos;
    for (int i = 0; arr[startPos + i] != '\0'; i++)
        printf("%c", arr[startPos + i]);
}