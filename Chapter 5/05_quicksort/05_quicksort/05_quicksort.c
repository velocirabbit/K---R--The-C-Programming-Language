/**
 * 05_quicksort.c
 *
 * Implementation of quicksort using triple partitions (to sort duplicate values
 * faster) and a ninther pivot.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRSIZE 100000
#define UPPER 1000
#define SCREENWIDTH 80

int randint(int n);
int widthof(int n);
int pivot(int *v, int lo, int hi);
int median(int *v, int x, int y, int z);
int *partition(int *v, int lo, int p, int hi);
void quicksort(int *v, int lo, int hi);
void swap(int *v, int p, int q);

int main(void) {
    // Generate an array of ARRSIZE integers between 0 and UPPER
    int demo[ARRSIZE];
    for (int i = 0; i < ARRSIZE; i++)
        demo[i] = randint(UPPER);

    // Quicksort + timing
    clock_t begin = clock();
    quicksort(demo, 0, ARRSIZE - 1);
    clock_t end = clock();

    int width = 0;
    for (int i = 0; i < ARRSIZE; i++) {
        width += widthof(demo[i]) + 2;  // '+2' accounts for ', '
        if (width > SCREENWIDTH - 1) {
            printf("\n");
            width = widthof(demo[i]) + 2;  // '+2' accounts for ', ';
        }
        printf("%d, ", demo[i]);
    }
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nTime spent: %.3f seconds\n", time_spent);
    system("pause");
    return 0;
}

/* OVERHEAD STUFF */
/**
 * Uses rand() to generate a random integer in the range [0, n). Does some correcting
 * for uniformity.
 *
 * Returns: randomly generated integer in the range [0, n)
 */
int randint(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    }
    else {
        // Remove all of the values that would cause a skew
        long end = RAND_MAX / n;  // truncate skew=
        end *= n;

        // Ignore results from rand() that would fall above the limit
        int r;
        while ((r = rand()) >= end);

        return r % n;
    }
}

/** 
 * Finds the character width of int n
 *
 * Returns: width
 */
int widthof(int n) {
    int i;
    for (i = 1; n /= 10; i++);
    return i;
}
/* END OVERHEAD STUFF */

/**
 * Obtain a pivot value for the subarray of v between lo and hi
 *
 * Returns: index position of value to use as pivot
 */
int pivot(int *v, int lo, int hi) {
    int ptOne = (lo + hi) / 3;
    ptOne = (ptOne < lo) ? lo : ptOne;
    int ptTwo = (ptOne + hi) / 2;
    ptTwo = (ptTwo < ptOne) ? ptOne : ptTwo;

    // Find the median of each third of array v between lo and hi
    int first = median(v, lo, (lo + ptOne) / 2, ptOne);
    int second = median(v, ptOne + 1, (ptOne + 1 + ptTwo) / 2, ptTwo);
    int third = median(v, ptTwo + 1, (ptTwo + 1 + hi) / 2, hi);

    return median(v, first, second, third);
}

/**
 * Obtains the median value for the three items at positions x, y, and z in array v
 *
 * Returns: x, y, or z (whichever is holds the median value)
 */
int median(int *v, int x, int y, int z) {
    int vX = v[x];
    int vY = v[y];
    int vZ = v[z];

    if (vX < vY) {
        if (vX < vZ)
            return (vY < vZ) ? y : z;
        else
            return x;
    }
    else {
        if (vY < vZ)
            return (vX < vZ) ? x : z;
        else
            return y;
    }
}

/**
 * Returns an int[2] containing the two indices to use for partitioning the subarray
 * of array v between lo and hi. int[0] and int[1] shouldn't ever be the same number,
 * although they may be only one apart.
 *
 * Returns: int[2] of partition indices
 */
int *partition(int *v, int lo, int p, int hi) {
    int pivot = v[p];
    int i = lo - 1;
    int j = hi + 1;

    while (1) {
        // Starting from i, ignore any values <pivot
        do {
            i++;
        } while (v[i] < pivot);

        // Starting from j, ignore any values >pivot
        do {
            j--;
        } while (v[j] > pivot);

        // If no other pairs to swap found, return partition values
        if (i >= j) {
            int *result = malloc(sizeof(int[2]));
            for (j; j >= lo && v[j] == p; j--);
            for (i; i <= hi && v[i] == p; i++);
            result[0] = j;
            result[1] = (i == j) ? i + 1 : i;  // So i and j aren't equal
            return result;
        }

        // If a pair to swap found, swap them
        swap(v, i, j);
    }
}

/**
 * Recursive quicksort algorithm
 */
void quicksort(int *v, int lo, int hi) {
    // Base case. Do nothing if there are fewer than two elements in subarray
    if (lo >= hi)
        return;

    int p = pivot(v, lo, hi);              // Get the pivot index
    int *parts = partition(v, lo, p, hi);  // Get partition indices

    // Recursion over the two partitions
    quicksort(v, lo, parts[0]);
    quicksort(v, parts[1], hi);
}

/**
 * Swaps the values at indices p and q in array v
 */
void swap(int *v, int p, int q) {
    int temp = v[p];
    v[p] = v[q];
    v[q] = temp;
}