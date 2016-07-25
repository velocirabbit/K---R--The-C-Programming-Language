/* 05E06.c (p. 107) */
#include <stdio.h>
#include <ctype.h>

#define MAXSTRLEN 1000

int strlen(char *s);
int getline(char *s, int lim);
int strindex(char *s, char *t);
int atoi(char *s);
double atof(char *s);
void itoa(char *s, int n);
void ftoa(char *s, double n);
void reverse(char *s);

int main(void) {
    // strlen
    char s[] = "This is a test string.";
    printf("TEST 1\n\t'%s' has %d characters\n\n", s, strlen(s));

    // atof
    char f1[] = "12.234";
    char f2[] = "-23.42";
    printf("TEST 2\n\t'%s' -> %f\n", f1, atof(f1));
    printf("\t'%s' -> %f\n\n", f2, atof(f2));

    // atoi
    char i1[] = "9203";
    char i2[] = "-8492";
    printf("TEST 3\n\t'%s' -> %d\n", i1, atoi(i1));
    printf("\t'%s' -> %d\n\n", i2, atoi(i2));

    // ftoa
    double nf1 = 458.209;
    double nf2 = -8329.4920;
    char sf1[MAXSTRLEN];
    char sf2[MAXSTRLEN];
    ftoa(sf1, nf1);
    ftoa(sf2, nf2);
    printf("TEST 4\n\t%f -> '%s'\n", nf1, sf1);
    printf("\t%f -> '%s'\n\n", nf2, sf2);

    // itoa
    int ni1 = 238204;
    int ni2 = -348924;
    char si1[MAXSTRLEN];
    char si2[MAXSTRLEN];
    itoa(si1, ni1);
    itoa(si2, ni2);
    printf("TEST 5\n\t%d -> '%s'\n", ni1, si1);
    printf("\t%d -> '%s'\n\n", ni2, si2);

    // strindex
    char str[] = "This string is some dumb test string.";
    char sub1[] = "dumb test";
    char sub2[] = "fat cat";
    char sub3[] = "string";
    printf("TEST 6\n'%s'\n", str);
    printf("\t'%s' -> pos %d\n", sub1, strindex(str, sub1));
    printf("\t'%s' -> pos %d\n", sub2, strindex(str, sub2));
    printf("\t'%s' -> pos %d\n\n", sub3, strindex(str, sub3));

    // getline
    printf("TEST 7\n");
    char strbuff[MAXSTRLEN];
    getline(strbuff, MAXSTRLEN);
    printf("%s\n\n", strbuff);

    // done
    printf("\n");
    system("pause");
    return 0;
}

/**
 * Get line into s
 * Return: length
 */
int getline(char *s, int lim) {
    int c, i;
    i = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        s[i++] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = 0;
    return i;
}

/**
 * Looks for string t in string s
 * Return: index of where t starts in s, -1 if none
 */
int strindex(char *s, char *t) {
    int sLen = strlen(s);
    int tLen = strlen(t);
    int i;
    if (t > s)
        return -1;
    for (i = 0; i < sLen - tLen; i++) {
        int j;
        for (j = 0; j < tLen && s[i + j] == t[j]; j++);
        if (j == tLen)
            return i;
    }
    return -1;
}

/**
 * Convert string s to int
 * Return: the converted int
 */
int atoi(char *s) {
    int i, sign;
    int ans = 0;
    for (i = 0; isspace(s[i]); i++);  // skip white spaces
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-')
        i++;
    for (i; isdigit(s[i]); i++) {
        ans *= 10;
        ans += s[i] - '0';
    }
    return sign * ans;
}

/**
 * Convert string s to double
 * Return: the converted double
 */
double atof(char *s) {
    int i, sign;
    double d = 0.0;
    for (i = 0; isspace(s[i]); i++);  // skip white spaces
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-')
        i++;
    for (i; isdigit(s[i]); i++) {
        d *= 10;
        d += s[i] - '0';
    }
    if (s[i] == '.') {
        int mag = 1;
        for (++i; isdigit(s[i]); i++) {
            d *= 10;
            d += s[i] - '0';
            mag *= 10;
        }
        d /= mag;
    }
    return sign * d;
}

/**
 * itoa: Convert int i into a string stored in s
 */
void itoa(char *s, int n) {
    int i, sign;
    sign = (n < 0) ? -1 : 1;
    for (i = 0; n; i++, n /= 10)
        s[i] = (sign * (n % 10)) + '0';
    if (sign == -1)
        s[i++] = '-';
    s[i] = 0;
    reverse(s);
}

/**
 * ftoa: Convert float f into a string stored in s
 */
void ftoa(char *s, double n) {
    int i = 0, sign;
    sign = (n < 0) ? -1 : 1;
    double dec = (sign * n) - (sign * (int)n);
    if (dec > 0.0)
        s[i++] = '.';
    for (i; (int)n; i++, n /= 10)
        s[i] = (sign * ((int)n % 10)) + '0';
    if (sign == -1)
        s[i++] = '-';
    s[i] = 0;
    reverse(s);
    int iStart = i;
    for (i; (dec *= 10) != 0.0 && i - iStart < 8; i++, dec = dec - (int)dec)
        s[i] = (int)dec + '0';
    s[i] = 0;
}

/**
 * Gets the length of string s, excluding the null char '\0'
 * Returns: length of string s
 */
int strlen(char *s) {
    int i;
    for (i = 0; *(s + i); i++);
    return i;
}

/**
 * reverse: Reverse string s in place
 */
void reverse(char *s) {
    int len = strlen(s) - 1;
    for (int i = 0; i < (len + 1) / 2; i++) {
        char t = s[i];
        s[i] = s[len - i];
        s[len - i] = t;
    }
}