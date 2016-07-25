/* 04E01.c (p. 71) */
#include <stdio.h>
#include <string.h>
#define MAXSTRLEN 1000

int strrindex(char s[], char t[]);

main() {
    char str[] = "The cat in the hat loves green eggs and ham.";
    char find[][MAXSTRLEN] = { "cat", "hat", "eggs", "ham", "balls", "at"};

    printf("%s\n\n", str);
    for (int i = 0; i < 6; i++) {
        int p = strrindex(str, find[i]);
        if (p >= 0)
            printf("Found '%s' at index %d\n", find[i], p);
        else
            printf("Couldn't find '%s' in the string.\n", find[i]);
    }

    printf("\n");
    system("pause");
    return 0;
}

int strrindex(char s[], char t[]) {
    int i, j, sLen = strlen(s), tLen = strlen(t);
    for (i = sLen - tLen; i >= 0; i--) {
        for (j = 0; t[j] != '\0' && t[j] == s[i + j]; j++);
        if (j > 0 && t[j] == '\0')
            return i;
    }
    return -1;
}