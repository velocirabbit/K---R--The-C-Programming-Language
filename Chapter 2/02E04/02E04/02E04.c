/* 02E04 (p. 48) */
#include <stdio.h>
#define MAXSTRLEN 1000

void squeeze(char mainStr[], char subStr[]);

main() {
    char mainStr[] = "This is the main string we'll be deleting from!";
    char subStr[] = "Delete these chars.";

    squeeze(mainStr, subStr);
    printf("%s\n", mainStr);

    system("pause");
    return 0;
}

void squeeze(char mainStr[], char subStr[]) {
    int i, j, k;
    // First delete any duplicate letters in subStr
    for (i = 0; subStr[i] != '\0'; i++) {
        for (j = k = i + 1; subStr[j] != '\0'; j++)
            if (subStr[j] != subStr[i])
                subStr[k++] = subStr[j];
        subStr[k] = '\0';
    }

    // Now delete characters from mainStr
    for (i = 0; subStr[i] != '\0'; i++) {
        for (j = k = 0; mainStr[j] != '\0'; j++)
            if (mainStr[j] != subStr[i])
                mainStr[k++] = mainStr[j];
        mainStr[k] = '\0';
    }
}