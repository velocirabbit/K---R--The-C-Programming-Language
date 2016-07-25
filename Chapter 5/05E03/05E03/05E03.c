/* 05E03.c (p. 107) */
#include <stdio.h>
#include <stdlib.h>

#define MAXSTRLEN 1000

void strcat(char *s, char *t);
int strlen(char *s);
void strcpy(char *s, char *t);

int main(void) {
    char *s = (char*) malloc(sizeof(char[MAXSTRLEN]));
    strcpy("We must fight for our right ", s);
    char *t = "to party all fucking night!";
    strcat(s, t);
    printf("%s\n", s);
    system("pause");
    return 0;
}

/**
 * strcat: Concatenate t to the end of s. s must be big enough.
 */
void strcat(char *s, char *t) {
    int i = strlen(s) - 1;  // -1 so we overwrite the '/0' at the end of s
    while (*(s + i++) = *t++);
}

/**
 * strlen: Returns the number of characters in a string (without the trailing null char)
 */
int strlen(char *s) {
    int i = 0;
    while (*(s + i++));
    return i;
}

/**
 * strcpy: Copies str s into str t
 */
void strcpy(char *s, char *t) {
    while (*t++ = *s++);
}