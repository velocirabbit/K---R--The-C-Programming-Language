/* 05E07.c (p. 110) */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAXSTRLEN 10000
#define MAXLINES 1000

int strcmp(char *s, char *t);
int getline(char *s, int maxlen);
int readlines(char *lineptr[], int maxlines);
int medStr(char *v[], int one, int two, int three);
int median(char *v[], int left, int right);
int ninther(char *v[], int left, int right);
int *partition(char *v[], int pivot, int left, int right);
void writelines(char *lineptr[], int nlines);
void strcpy(char *s, char *t);
void swap(char *v[], int i, int j);
void quicksort(char *v[], int left, int right);

int main(void) {
    int result = 0;  // final result returned by main()
    int nlines;      // number of input lines read
    char *lineptr[MAXLINES];
    for (int i = 0; i < MAXLINES; i++)
        if ((lineptr[i] = malloc(sizeof(char[MAXSTRLEN]))) == NULL) {
            printf("ERROR: Couldn't allocate char pointer at index %d\n", i);
            result = 1;
        }
    //if (result == 0) {
    //    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
    //        quicksort(lineptr, 0, nlines - 1);
    //        printf("\n\n");
    //        writelines(lineptr, nlines);
    //        result = 0;
    //    }
    //    else {
    //        printf("ERROR: Input too big to sort\n");
    //        result = 1;
    //    }
    //}
    nlines = 37;
    lineptr[0] = "Sometimes life it takes you by the hair";
    lineptr[1] = "Pulls you down before you know it";
    lineptr[2] = "It's gone and you're dead again";
    lineptr[3] = "I've been in places and I won't pretend";
    lineptr[4] = "Yeah I'd make it out just to fall on my head";
    lineptr[5] = "Wake up strange and take a walk downstairs";
    lineptr[6] = "Hit the pawn up on the corner and pay for my rent";
    lineptr[7] = "You know that I could not believe my own truth";
    lineptr[8] = "Just show them what I choose, got nothin' to lose";
    lineptr[9] = "Yeah yeah and it's okay";
    lineptr[10] = "I tie my hands up to a chair so I don't fall that way";
    lineptr[11] = "Yeah yeah and I'm alright";
    lineptr[12] = "I took a sip of something poison but I'll hold on tight";
    lineptr[13] = "You know those days when you want to choose";
    lineptr[14] = "To not get out of bed, you're lost in your head again";
    lineptr[15] = "You play the game but you're kind of cut";
    lineptr[16] = "Cause you're coming down hard, your joints are all stuck";
    lineptr[17] = "I tried to say that it's not the only way";
    lineptr[18] = "I never knew if I could face myself to change";
    lineptr[19] = "You were pacing, I was insecure";
    lineptr[20] = "Slip and fall, I'm dodging calls, hug the prison I've been living in";
    lineptr[21] = "Yeah yeah and it's okay";
    lineptr[22] = "I tie my hands up to a chair so I don't fall that way";
    lineptr[23] = "Yeah yeah and I'm alright";
    lineptr[24] = "I took a sip of something poison but I'll hold on tight";
    lineptr[25] = "I'm a reasonable man";
    lineptr[26] = "Get off my case";
    lineptr[27] = "Get off my case";
    lineptr[28] = "Get off my case";
    lineptr[29] = "I'm a reasonable man";
    lineptr[30] = "Get off my case";
    lineptr[31] = "Get off my case";
    lineptr[32] = "Get off my case";
    lineptr[33] = "After years of waiting";
    lineptr[34] = "Nothing changed";
    lineptr[35] = "I realized I'm looking in";
    lineptr[36] = "Looking in the wrong place";

    quicksort(lineptr, 0, nlines - 1);
    writelines(lineptr, nlines);

    printf("\n\n");
    system("pause");
    return 0;
}

/** 
 * Compares string s against string t
 * Returns: <0 if s<t, 0 if s==t, >0 if s>t)
 */
int strcmp(char *s, char *t) {
    for (; *s == *t; s++, t++)  // check corresponding char in each string for sameness
        if (*s == 0)  // if both same and equal to '\0', both strings equal
            return 0;
    return *s - *t;
}

/**
 * Copies string t into string s
 */
void strcpy(char *s, char *t) {
    while (*s++ = *t++);
}

/**
 * Swaps the two strings at indices i and j in char pointer array v
 */
void swap(char *v[], int i, int j) {
    char *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/**
 * Compares three strings in an array against each other using strcmp()
 * Returns: the index of the median string
 */
int medStr(char *v[], int one, int two, int three) {
    if (strcmp(v[one], v[two]) <= 0) {           // v[one] <= v[two]
        if (strcmp(v[one], v[three]) >= 0)       // v[three] <= v[one]
            return one;
        else if (strcmp(v[two], v[three]) <= 0)  // v[two] <= v[three]
            return two;
        else                                     // v[one] < v[three] < v[two]
            return three;
    }
    else {                                       // v[two] < v[one]
        if (strcmp(v[two], v[three]) >= 0)       // v[three] <= v[two]
            return two;
        if (strcmp(v[one], v[three]) <= 0)       // v[one] <= v[three]
            return one;
        else                                     // v[two] < v[three] < v[one]
            return three;
    }
}

/**
 * Compares the values of the array at index left, right, and (left + right) / 2
 * Returns: the index of the median of those three
 */
int median(char *v[], int left, int right) {
    int mid = (left + right) / 2;
    return medStr(v, left, mid, right);
}

/**
 * Finds the median value of the three thirds of the array. Left should be smaller than
 *   right.
 * Returns: the index of the ninther
 */
int ninther(char *v[], int left, int right) {
    int divOne = (left + right) / 3;
    divOne = (divOne < left) ? left : divOne;
    int divTwo = (divOne + right) / 2;
    divTwo = (divTwo < divOne) ? divOne : divTwo;

    // get medians
    int medOne = median(v, left, divOne);
    int medTwo = median(v, divOne + 1, divTwo);
    int medThree = median(v, divTwo + 1, right);

    return medStr(v, medOne, medTwo, medThree);
}

/**
 * Returns an int[2] array of the indices of the left and right thirds of array v
 * Returns: int[2], where:
 *                    int[0] = upper index of first 1/3
 *                    int[1] = lower index of last 1/3
 */
int *partition(char *v[], int pivot, int left, int right) {
    int i = left - 1;
    int j = right + 1;
    char *p = malloc(sizeof(char[MAXSTRLEN]));
    strcpy(p, v[pivot]);  // Save pivot value in case it moves
    int *result = malloc(sizeof(int[2]));  // store results here
    while (1) {
        /**
         * Infinitely loop through the array, starting from either end. Compare each
         * end with the pivot, swapping items to the other side of the pivot if they
         * don't belong 
         */
        do {
            i++;
        } while (strcmp(v[i], p) < 0);  // v[i] < v[pivot]

        do {
            j--;
        } while (strcmp(v[j], p) > 0);  // v[j] > v[pivot]

        if (i >= j) {
            // Check to see if the strings on either side of j are the same value
            for (j; j >= left && strcmp(v[j], p) == 0; j--);
            for (i; i <= right && strcmp(v[i], p) == 0; i++);
            result[0] = j;
            result[1] = i;

            free(p);
            return result;
        }

        swap(v, i, j);
    }
}

/**
 * Recursively sorts the strings in the char pointer array v using quicksort.
 * Uses ninther pivoting and triple partitions.
 */
void quicksort(char *v[], int left, int right) {
    if (left >= right)                           // base case. Do nothing if array
        return;                                  // contains fewer than two elements

    int p = ninther(v, left, right);             // get ninther pivot
    int *parts = partition(v, p, left, right); // for storing left and right indices

    // recursion on the two partitions
    quicksort(v, left, parts[0]);
    quicksort(v, parts[1], right);
}

/**
 * Gets a line of input
 * Returns: number of characters in the line, including any trailing whitespace ('\n',
 *   ' ', etc.) and the terminating null character ('\0')
 */
int getline(char *s, int maxlen) {
    int i, c;
    for (i = 0; i < maxlen && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = 0;
    return i;
}

/**
 * Reads input lines
 * Returns: number of lines read
 */
int readlines(char *lineptr[], int maxlines) {
    int len, nlines;
    char line[MAXSTRLEN];

    nlines = 0;
    while ((len = getline(line, MAXSTRLEN)) - 2 > 0) {
        if (nlines >= maxlines)
            return -1;
        else {
            line[len - 1] = 0;  // delete '\n' at the end of the string
            strcpy(lineptr[nlines++], line);  // copy line into lineptr[nlines] string
        }
    }
    return nlines;
}

/**
 * Prints the char strings in the string array
 */
void writelines(char *lineptr[], int nlines) {
    for (int i = 0; i < nlines; i++)
        printf("%s\n", lineptr[i]);
}