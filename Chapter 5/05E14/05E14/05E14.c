/* 05E14 (p. 121) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARRSIZE 100000
#define MAXSTRLEN 1000
#define UPPER 1000
#define SCREENWIDTH 80
#define DECIMAL 10
#define OPTIONSAT 1      // index of argv where the options should start at

// Possible option codes
#define NUMERIC 'n'   // Sort numerically, as opposed to as strings
#define REVERSE 'r'   // Sort in reverse order
#define PRINT   'p'   // Print the sorted array
#define MULTIPLE 'x'  // Do the search multiple times

int randint(int n);
int widthof(int n);
int pivot(int *v, int lo, int hi, int (*comp)(int, int, int), int r);
int median(int *v, int x, int y, int z, int (*comp)(int, int, int), int r);
int *partition(int *v, int lo, int p, int hi, int (*comp)(int, int, int), int r);
int numcmp(int i1, int i2, int r);
int tostrcmp(int i1, int i2, int r);
void quicksort(int *v, int lo, int hi, int (*comp)(int, int, int), int r);
void swap(int *v, int p, int q);
double average(double *d, int n);

const char codes[] = { NUMERIC, REVERSE, PRINT };

int main(int argc, char *argv[]) {
    // Parse arguments
    int numeric = 0, print = 0, reverse = 0, times = 1;
    if (argc > OPTIONSAT - 1) {
        for (int i = OPTIONSAT; i < argc; i++) {
            if (argv[i][0] == '-' && strlen(argv[i]) == 2) {
                switch (argv[i][1]) {
                    case NUMERIC:       // Sets option to sort numerically
                        numeric = 1;
                        break;
                    case REVERSE:       // Sets option to sort in reverse order
                        reverse = 1;
                        break;
                    case PRINT:         // Sets option to print sorted array
                        print = 1;
                        break;
                    case MULTIPLE:      // Sets option to perform sort multiple times
                    {
                        int error = 0;
                        if (++i >= argc) {
                            error = 1;
                        }
                        else {
                            int c;
                            for (c = 0; isdigit(argv[i][c]); c++);
                            if (argv[i][c] == 0)
                                times = atoi(argv[i]);
                            else
                                error = 1;
                            if (times == 0) {
                                printf("\nERROR: Usage of -x option must use a number >0\n");
                                return 1;
                            }
                        }
                        if (error) {
                            printf("\nERROR: Usage of -x option should be followed by a number\n");
                            return 1;
                        }
                        break;
                    }
                    default:
                        printf("\nERROR: Unknown parameter '-%c'\n", argv[i][1]);
                        return 1;
                        break;
                }
            }
            // Usage statement
            else {
                printf("\nUsage: 05E14 [-n] [-r] [-p] [-x times]\n\n");
                printf("Options:\n");
                printf("\t-n\t\tSort numerically (otherwise sorts as strings)\n");
                printf("\t-r\t\tSort in reverse order\n");
                printf("\t-p\t\tPrint out the contents of the sorted array\n");
                printf("\t-x times\tPerform the quicksort multiple times, initializing\n");
                printf("\t\t\ta new unsorted array each time. 'times' must be an int\n");
                printf("\n");
                return 1;
            }
        }
    }
    
    // Perform quicksort trials, saving the time taken for each trial to timeSpent
    double *timeSpent = malloc(sizeof(double) * times);
    for (int t = 0; t < times; t++) {
        // Generate an array of ARRSIZE integers between 0 and UPPER
        int demo[ARRSIZE];
        for (int i = 0; i < ARRSIZE; i++)
            demo[i] = randint(UPPER);

        // Quicksort + timing
        clock_t begin = clock();
        quicksort(demo, 0, ARRSIZE - 1,
            (int(*)(int, int, int))(numeric ? numcmp : tostrcmp), reverse);
        clock_t end = clock();

        if (print) {
            int width = 0;
            for (int i = 0; i < ARRSIZE; i++) {
                width += widthof(demo[i]) + 2;  // '+2' accounts for ', '
                if (width > SCREENWIDTH - 1) {
                    printf("\n");
                    width = widthof(demo[i]) + 2;  // '+2' accounts for ', ';
                }
                printf("%d, ", demo[i]);
            }
        }

        timeSpent[t] = (double)(end - begin) / CLOCKS_PER_SEC;
    }
    
    printf("\nTime spent sorting %d numbers %d times %s:\n",
        ARRSIZE, times, numeric ? "numerically" : "as strings");
    for (int t = 0; t < times; t++)
        printf("\tTrial %d:\t%.3f seconds\n", t + 1, timeSpent[t]);
    printf("\nAverage: %.3f seconds\n", average(timeSpent, times));

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
int pivot(int *v, int lo, int hi, int (*comp)(int, int, int), int r) {
    int ptOne = (lo + hi) / 3;
    ptOne = (ptOne < lo) ? lo : ptOne;
    int ptTwo = (ptOne + hi) / 2;
    ptTwo = (ptTwo < ptOne) ? ptOne : ptTwo;

    // Find the median of each third of array v between lo and hi
    int first = median(v, lo, (lo + ptOne) / 2, ptOne, comp, r);
    int second = median(v, ptOne + 1, (ptOne + 1 + ptTwo) / 2, ptTwo, comp, r);
    int third = median(v, ptTwo + 1, (ptTwo + 1 + hi) / 2, hi, comp, r);

    return median(v, first, second, third, comp, r);
}

/**
* Obtains the median value for the three items at positions x, y, and z in array v
*
* Returns: x, y, or z (whichever is holds the median value)
*/
int median(int *v, int x, int y, int z, int (*comp)(int, int, int), int r) {
    int vX = v[x];
    int vY = v[y];
    int vZ = v[z];

    if ((*comp)(vX, vY, r) < 0) {
        if ((*comp)(vX, vY, r) < 0)
            return ((*comp)(vY, vZ, r) < 0) ? y : z;
        else
            return x;
    }
    else {
        if ((*comp)(vY, vZ, r) < 0)
            return ((*comp)(vX, vZ, r) < 0) ? x : z;
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
int *partition(int *v, int lo, int p, int hi, int (*comp)(int, int, int), int r) {
    int pivot = v[p];
    int i = lo - 1;
    int j = hi + 1;

    while (1) {
        // Starting from i, ignore any values <pivot
        do {
            i++;
        } while ((*comp)(v[i], pivot, r) < 0);

        // Starting from j, ignore any values >pivot
        do {
            j--;
        } while ((*comp)(v[j], pivot, r) > 0);

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
void quicksort(int *v, int lo, int hi, int (*comp)(int, int, int), int r) {
    // Base case. Do nothing if there are fewer than two elements in subarray
    if (lo >= hi)
        return;

    int p = pivot(v, lo, hi, comp, r);              // Get the pivot index
    int *parts = partition(v, lo, p, hi, comp, r);  // Get partition indices

    // Recursion over the two partitions
    quicksort(v, lo, parts[0], comp, r);
    quicksort(v, parts[1], hi, comp, r);
}

/**
* Swaps the values at indices p and q in array v
*/
void swap(int *v, int p, int q) {
    int temp = v[p];
    v[p] = v[q];
    v[q] = temp;
}

/**
 * numcmp: Compares the two integers as numbers
 *
 * Returns: 0 if the two ints are equal, <0 if the i1<i2, >0 if i2<i1 (unless r = 1)
 */
int numcmp(int i1, int i2, int r) {
    return r ? i2 - i1 : i1 - i2;
}

/**
 * tostrcmp: Compares the two integers as strings
 *
 * Returns: 0 if the two ints are equal, <0 if the i1<i2, >0 if i2<i1 (unles r = 1)
 */
int tostrcmp(int i1, int i2, int r) {
    char c1[MAXSTRLEN], c2[MAXSTRLEN];
    _itoa(i1, c1, DECIMAL);
    _itoa(i2, c2, DECIMAL);

    return r ? strcmp(c2, c1) : strcmp(c1, c2);
}

/**
 * average: Finds the average of an array of doubles. n is the number of doubles in the
 *   array.
 *
 * Returns: The average as a double
 */
double average(double *d, int n) {
    double avg = 0.0;
    for (int i = 0; i < n; i++)
        avg += d[i];
    return avg / (double)n;
}