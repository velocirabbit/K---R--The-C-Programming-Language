/* 03E01.c (p. 58) */
#include <stdio.h>
#include <time.h>

int binsearch(int x, int v[], int n);
int binsearchBook(int x, int v[], int n);

main() {
    int x = 3;
    int v[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 51, 53, 59, 61, 67 };  // size = 14
    int n = sizeof(v) / sizeof(int);
    
    // For measuring runtime
    clock_t begin, end, beginB, endB;
    double timespent, timespentBook;

    system("pause");

    // First test
    begin = clock();
    int p = binsearch(x, v, n);
    end = clock();
    timespent = (double)(end - begin) / CLOCKS_PER_SEC;

    // Second test
    beginB = clock();
    int pBook = binsearchBook(x, v, n);
    endB = clock();
    timespentBook = (double)(endB - beginB) / CLOCKS_PER_SEC;

    printf("binsearch found x = %d at position %d. Took %e seconds.\n", x, p, timespent);
    printf("binsearchBook found x = %d at position %d. Took %e seconds.\n", x, pBook, timespentBook);

    printf("\n");
    system("pause");
    return 0;
}

/*
 * binsearch: Searches for int x in ascendingly sorted int array v. Returns the position if found, -1 if not.
 *   n is the number of elements in array v
 */
int binsearch(int x, int v[], int n) {
    int low, mid, high;

    low = 0;
    high = n - 1;

    while (low < high) {
        mid = (low + high) / 2;
        if (x <= v[mid])
            high = mid;
        else
            low = mid + 1;
    }
    
    if (x == v[mid])
        return mid;
    else if (high == low)
        return high;
    else
        return -1;
}

/* binsearchBook: version of binsearch in the textbook (for runtime comparisons) */
int binsearchBook(int x, int v[], int n) {
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x < v[mid])
            high = mid - 1;
        else if (x > v[mid])
            low = mid + 1;
        else  /* found match */
            return mid;
    }
    return -1;  /* no match */
}