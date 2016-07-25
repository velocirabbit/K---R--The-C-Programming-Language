/* 02E02.c (p. 42) */
#include <stdio.h>

#define LIMIT 17

main() {
    int i;
    char c, s[LIMIT], w[LIMIT];

    for (i = 0; i < LIMIT - 1 && (c = getchar()) != '\n' && c != EOF; i++)
        s[i] = c;
    s[i] = '\0';

    // This should work the same as the above for loop
    for (i = 0; i < LIMIT - 1; i++) {
        if ((c = getchar()) == '\n')
            break;
        else if (c == EOF)
            break;
        else
            w[i] = c;
    }
    w[i] = '\0';

    printf("String 1: %s\n", s);
    printf("String 2: %s\n\n", w);
    system("pause");
    return 0;
}