/* 02E05.c (p. 48) */
#include <stdio.h>

int any(char s1[], char s2[]);

main() {
    char s1[] = "Search through this string to find an instance of a character in s2.";
    char s2[] = "Reference string.";

    int p = any(s1, s2);
    if (p == -1)
        printf("Couldn't find any characters from s2 in s1.\n");
    else
        printf("Found char %c at index %d.\n", s1[p], p);

    system("pause");
    return 0;
}

int any(char s1[], char s2[]) {
    int i, j, k, pos = -1;
    // Delete duplicate characters in s2
    for (i = 0; s2[i] != '\0'; i++) {
        for (j = k = 0; s2[j] != '\0'; j++)
            if (s2[j] != s2[i])
                s2[k++] = s2[j];
        s2[k] = '\0';
    }

    for (i = 0; s2[i] != '\0'; i++) {
        for (j = 0; s1[j] != '\0'; j++) {
            if (s1[j] == s2[i] && (j < pos || pos == -1)) {
                pos = j;
                break;
            }
        }
    }

    return pos;
}