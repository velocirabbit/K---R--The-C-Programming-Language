/* 05_stringsort: stripped-down version of the UNIX program sort (p. 108) */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000  // max #lines to be sorted
#define MAXLEN 1000  // max length of any input line

char *lineptr[MAXLINES];  // pointers to text lines

int getline(char *s, int lim);
int readlines(char *lineptr[], int nlines);
int strcmp(char *s, char *t);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);
void swap(char *lineptr[], int i, int j);

/* sort input lines */
int main(void) {
    int nlines, result;  // number of input lines read
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        qsort(lineptr, 0, nlines - 1);
        printf("\n");
        writelines(lineptr, nlines);
        result = 0;
    }
    else {
        printf("ERROR: input too big to sort\n");
        result = 1;
    }

    printf("\n\n");
    system("pause");
    return result;
}

/* strcmp: return <0 if s<t, 0 if s==t, >0 if s>t */
int strcmp(char *s, char *t) {
    for (; *s == *t; s++, t++)
        if (*s == 0)
            return 0;
    return *s - *t;
}

/* qsort */
void qsort(char *lineptr[], int left, int right) {
    int i, last;
    if (left > right)  // do nothing if array contains fewer than two elements
        return;
    swap(lineptr, left, (left + right) / 2);  // move partition element
    last = left;  // to lineptr[0];
    for (i = left + 1; i <= right; i++)
        if (strcmp(lineptr[i], lineptr[left]) < 0)  // if lineptr[i] < lineptr[left]
            swap(lineptr, ++last, i);
    swap(lineptr, left, last);  // restore partition elem
    qsort(lineptr, left, last - 1);
    qsort(lineptr, last + 1, right);
}

/* swap */
void swap(char *lineptr[], int i, int j) {
    char *temp = lineptr[i];
    lineptr[i] = lineptr[j];
    lineptr[j] = temp;
}

/* getline */
int getline(char *s, int lim) {
    int i, c;
    i = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        s[i++] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = 0;
    return i;
}

/* readlines: read input lines */
int readlines(char *lineptr[], int maxlines) {
    int len, nlines;
    char *p, line[MAXLEN];

    nlines = 0;
    while ((len = getline(line, MAXLEN)) - 2 > 0) {
        if (nlines >= maxlines || (p = (char*)malloc(sizeof(char) * len)) == NULL)
            return -1;
        else {
            line[len - 1] = '\0';  // delete newline
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

/* writelines: write output lines */
void writelines(char *lineptr[], int nlines) {
    int i;

    for (i = 0; i < nlines; i++)
        printf("%s\n", lineptr[i]);
}