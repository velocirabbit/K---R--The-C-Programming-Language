/* 05E05.c (p. 107) */
#include <stdio.h>
#include <stdlib.h>

#define MAXSTRLEN 1000

char *strncpy(char *s, char *t, int n);
char *strncat(char *s, char *t, int n);
int strncmp(char *s, char *t, int n);
int strlen(char *s);
void strcpy(char *s, char *t);

int main(void) {
    char *s = malloc(sizeof(char[MAXSTRLEN]));
    char *t = malloc(sizeof(char[MAXSTRLEN]));

    // strncpy
    strcpy(t, "Hi! I'm testing out the strncpy() function! How quaint!");
    strncpy(s, t, 17);
    printf("TEST 1\nt: %s\ns: %s\n", t, s);
    strcpy(t, "This is the second test.");
    strncpy(s, t, 90);
    printf("\nTEST 2\nt: %s\ns: %s\n\n", t, s);

    // strncat
    strcpy(t, "Just kidding! We're testing strncat now!");
    strncat(s, t, 17);
    printf("TEST 3\nt: %s\ns: %s\n", t, s);
    strcpy(t, "This is fun!");
    strncat(s, t, 90);
    printf("\nTEST 4\nt: %s\ns: %s\n\n", t, s);

    // strncmp
    printf("Which is bigger?\n\tt: %s\n\ts: %s\n", t, s);
    strncmp(s, t, 12);
    printf("TEST 5\nn = 8: ");
    int cmp = strncmp(s, t, 8);
    switch (cmp) {
        case -1:
            printf(" -1\n");
            break;
        case 0:
            printf(" 0\n");
            break;
        case 1:
            printf(" 1\n");
            break;
    }
    strcpy(s, "Hi, I am a dog!");
    strcpy(t, "Hi, I am a cat!");
    printf("\n\tt: %s\n\ts: %s\n", t, s);
    printf("\nTEST 6\nn = 14: ");
    cmp = strncmp(s, t, 14);
    switch (cmp) {
        case -1:
            printf(" -1\n");
            break;
        case 0:
            printf(" 0\n");
            break;
        case 1:
            printf(" 1\n");
            break;
    }

    printf("\n\n");
    system("pause");
    return 0;
}

/**
 * strncpy: Copy at most n characters of string t to s. Pad with '\0's if t has fewer
 *   than n characters.
 * Return: s
 */
char *strncpy(char *s, char *t, int n) {
    int i, tLen = strlen(t);
    for (i = 0; i < n; i++) {
        if (i > tLen)
            s[i] = 0;
        else
            s[i] = t[i];
    }
    s[i] = 0;
    return s;
}

/**
 * Concatenate at most n characters of string t to string s, terminate s with '\0'.
 * Return: s
 */
char *strncat(char *s, char *t, int n) {
    int i, sLen = strlen(s) + 1;
    for (i = 0; i < n && (s[sLen + i] = t[i]); i++);
    return s;
}

/**
 * Compare at most n characters of string s to string t.
 * Return: <0 if s<t, 0 if s==t, or >0 if s>t
 */
int strncmp(char *s, char *t, int n) {
    int i;
    for (i = 0; i < n && s[i] == t[i]; i++);
    if (i == n)
        return 0;
    else
        return (s[i] > t[i]) ? 1 : -1;
}

/**
 * Get the number of characters in string s, excluding the terminating '\0'.
 * Return: Number of characters, -1 if string is empty
 */
int strlen(char *s) {
    int i;
    for (i = 0; *(s + i); i++);
    return i - 1;
}

/**
 * strcpy: Copy string t into string s.
 */
void strcpy(char *s, char *t) {
    while (*s++ = *t++);
}