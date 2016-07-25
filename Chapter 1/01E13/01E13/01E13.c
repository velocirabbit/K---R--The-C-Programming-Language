/* 01E13.c (p. 24) */
#include <stdio.h>

#define IN 1
#define OUT 0

main() {
    int c, state, count;
    int wlength[16];

    state = OUT;
    count = 0;
    for (int i = 0; i < 16; i++)
        wlength[i] = 0;

    while ((c = getchar()) != '\n') {
        if ((c == ' ' || c == '\n' || c == '\t') && state == IN) {
            wlength[count - 1]++;
            count = 0;
        }
        else if (state == OUT)
            state = IN;
        else if (state == IN)
            count++;
    }
    printf("\n");
    for (int i = 0; i < 16; i++) {
        printf("%d\t|", i + 1);
        for (int j = 0; j < wlength[i]; j++)
            printf("x");
        printf("\n");
    }

    printf("\n");
    system("pause");
    return 0;
}