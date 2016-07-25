/* 04E13.c (p. 88) */
#include <stdio.h>
#include <string.h>

#define MAXSTRLEN 1000

int reverse(char s[], int left, int right);

main() {
    char examples[][MAXSTRLEN] = { "apple", "blue is the warmest colour", "coroner",
                                   "dogs jump over foxes", "elementary", "foxes lie" };
    for (int i = 0; i < 6; i++) {
        reverse(examples[i], 0, strlen(examples[i]) - 1);
        printf("%s\n", examples[i]);
    }

    printf("\n");
    system("pause");
    return 0;
}

/**
 * reverse: Recursive implementation of a string reverse() function that reverses string
 *   s in place.
 */
int reverse(char s[], int left, int right) {
    if (left > right)
        return -1;
    else if (left == right)
        return 0;
    else {
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;
        reverse(s, ++left, --right);
        return 1;
    }
}