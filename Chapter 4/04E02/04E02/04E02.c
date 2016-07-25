/* 04E02.c (p. 73) */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXSTRLEN 1000
#define EXPSIZE 8

double atof(char s[]);
double pow(double n, int e);
void trim(char s[]);

main() {
    char strs[][MAXSTRLEN] = {"    4205.203e9   \n\n  ", "\n209.23293E8    ",
                              "292.e-2    ", "  -384E+4\t\t\n",
                              "  290E2", "\n\n28.209",
                              "999.99E9 \n\n", "  \n\t-22.22e-2\t\n "};

    for (int i = 0; i < EXPSIZE; i++)
        printf("%f\n", atof(strs[i]));

    printf("\n");
    system("pause");
    return 0;
}

/*
 * atof: Converts string s to and returns it as a double. Can handle floating-point
 *   numbers in the form of:
 *
 *       123.45e-6
 *
 *   where a floating-point number may be followed by e or E and an optionally signed
 *   exponent.
 */
double atof(char s[]) {
    trim(s);
    int exp;
    int sign = s[0] == '-' ? -1 : 1;
    int i = s[0] == '-' || s[0] == '+' ? 1 : 0;
    double v, p;

    for (v = 0.0; isdigit(s[i]); i++)
        v = 10.0 * v + (s[i] - '0');
    if (s[i] == '.') {
        i++;
        for (p = 1.0; isdigit(s[i]); i++) {
            v = 10.0 * v + (s[i] - '0');
            p *= 10.0;
        }
        v /= p;
    }
    if (tolower(s[i]) == 'e') {
        i++;
        int eSign = s[i] == '-' ? -1 : 1;
        i = s[i] == '-' || s[i] == '+' ? i + 1 : i;
        for (exp = 0; isdigit(s[i]); i++)
            exp = 10 * exp + (s[i] - '0');
        v *= pow(10, eSign * exp);
    }
    return sign * v;
}

/*
 * pow: Returns n raised to the e power as a double.
 */
double pow(double n, int e) {
    double p = 1.0;
    int sign = e < 0 ? -1 : 1;
    n = e < 0 ? 1 / n : n;
    e *= sign;

    for (e; e > 0; e--)
        p *= n;
    return p;
}

/*
 * trim: Removes all trailing and leading white spaces from s in place
 */
void trim(char s[]) {
    int i, j;
    for (i = strlen(s) - 1; i > 0 && isspace(s[i]); i--);
    s[++i] = '\0';
    for (i = 0; s[i] != '\0' && isspace(s[i]); i++);
    for (j = 0; s[i + j] != '\0'; j++)
        s[j] = s[i + j];
    s[j] = '\0';
}