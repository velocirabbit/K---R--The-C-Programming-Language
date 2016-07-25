/* 02E03.c (p. 46) */
#include <stdio.h>
#define MAXHEXSTR 5

int htoi(char *s);

main() {
    int i, hexint;
    char hexstr[MAXHEXSTR], c;
    printf("Enter a 4-digit (or smaller) hexidecimal number:\n");

    for (i = 0; i < MAXHEXSTR - 1 && (c = getchar()) != '\n' && c != '\0'; i++) {
        if (i == 1 && (c == 'x' || c == 'X') && hexstr[0] == 0)
            i = 0;
        else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
            hexstr[i] = c;
        else
            i--;
    }
    hexstr[i] = '\0';
    hexint = htoi(hexstr);
    printf("\nYour hex number in base-10: %d\n", hexint);
    system("pause");
    return 0;
}

int htoi(char *s) {
    int n = 0;
    for (int i = 0; i < MAXHEXSTR && s[i] != '\0'; i++) {
        n = n << 4;
        if (s[i] >= '0' && s[i] <= '9')
            n += s[i] - '0';
        else if (s[i] >= 'A' && s[i] <= 'F')
            n += s[i] - 'A' + 10;
        else if (s[i] >= 'a' && s[i] <= 'f')
            n += s[i] - 'a' + 10;
    }
    return n;
}