/* 05E16 (p. 121) */
/**
 *
 * The Quicksort algorithm used in this program works as follows:
 *   1. (base case) In a given range (initially the whole array to be sorted), if the size of
 *      that range is 0 or 1, do nothing.
 *   2. If the size of the range is smaller than a given threshold amount (default 20), that range
 *      is sorted using a shell sort algorithm instead (faster for smaller arrays) with gap sizes of
 *      { 10, 4, 1 }.
 *   3. If the size of the range is larger than the given threshold amount, first find the
 *      pivot point to use by taking index of the range's ninther, which is the median value
 *      OF THE median value of each third of the range.
 *   4. Starting from both ends of the range and moving in, swap items around so that items
 *      smaller than the pivot are at the front, items larger than the pivot are at the end,
 *      and items equal to the pivot are between the two. The range is now subdivided into
 *      three partitions.
 *   5. Recursively go through steps 1-4 on the first and last partitions (the second
 *      partition, with elements equal to the ninther pivot, is already sorted), starting
 *      the smaller of the two partitions. Tail call recursion is performed on the larger
 *      partition.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define THRESHOLD   20      //  Use insertion sort with arrays smaller than this
#define ARRSIZE     100000  //  Size of array of numbers
#define MAXSTRLEN   1000    //  Max length of a string
#define UPPER       1000    //  Upper limit of the random numbers generated
#define SCREENWIDTH 80      //  Width of the screen (used for printing)
#define DECIMAL     10      //  Code for decimal option (used with itoa)
#define OPTIONSAT   1       //  Index of argv where the options should start at
#define INTTYPE     0       //  Sorting an int array
#define STRTYPE     1       //  Sorting a string array
#define DIVCHAR     '\\'    //  divider character for file paths in Windows

 // Possible option codes
#define USEFILE     'f'     // specify a file with lines of text to sort
#define NUMERIC     'n'     // Sort numerically, as opposed to as strings
#define REVERSE     'r'     // Sort in reverse order
#define PRINT       'p'     // Print the sorted array
#define MULTIPLE    'x'     // Do the search multiple times
#define IGNORE      'i'     // Fold upper and lower case together
#define DIRECTORY   'd'     // Compares only letters, numbers, and blanks

int getfile(char ***lines, char fn[], int len);
int randint(int n);
int widthof(int n);
int pivot(void *a, int type, int lo, int hi,
    int(*comp)(int, int, int), int r, int f, int d);
int median(void *a, int type, int x, int y, int z,
    int(*comp)(int, int, int), int r, int f, int d);
int *partition(void *a, int type, int lo, int p, int hi,
    int(*comp)(int, int, int), int r, int f, int d);
int numcmp(int i1, int i2, int r);
int tostrcmp(int i1, int i2, int r);
int strcmp2(char s1[], char s2[], int r, int f, int d);
int dstrcmp(char s1[], char s2[], char div);
int quicksort(void *a, int type, int lo, int hi, int t,
    int(*comp)(int, int, int), int r, int f, int d);
int shellsort(void *a, int type, int lo, int hi,
    int(*comp)(int, int, int), int r, int f, int d);
void strlower(char s[]);
void swap(void *a, int type, int p, int q);
double average(double *d, int n);

const char codes[] = { USEFILE, NUMERIC, REVERSE, PRINT, IGNORE, DIRECTORY };
const int gaps[] = { 10, 4, 1, 0 };  // gap sizes to use for shell sorting

int main(int argc, char *argv[]) {
    // Parse arguments
    int file = 0, numeric = 0, print = 0, reverse = 0, times = 1, fold = 0, dir = 0;
    int fnPos;
    if (argc > OPTIONSAT - 1) {
        for (int i = OPTIONSAT; i < argc; i++) {
            if (argv[i][0] == '-' && strlen(argv[i]) == 2) {
                switch (argv[i][1]) {
                    case USEFILE:          // Gets lines of text from the specified file
                        file = 1;
                        numeric = 0;
                        fnPos = ++i;
                        break;
                    case NUMERIC:       // Sets option to sort numerically
                        numeric = file ? 0 : 1;
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
                            printf("\nERROR: Usage of -x option should be followed by\n");
                            printf("         a number\n");
                            return 1;
                        }
                        break;
                    }
                    case IGNORE:
                        fold = 1;
                        break;
                    case DIRECTORY:
                        dir = 1;
                        break;
                    default:
                        printf("\nERROR: Unknown parameter '-%c'\n", argv[i][1]);
                        return 1;
                }
            }
            // Usage statement
            else {
                printf("\nUsage: 05E16 [-%c filename] [-%c] [-%c] [-%c] [-%c times] [-%c]\n",
                    USEFILE, NUMERIC, REVERSE, PRINT, MULTIPLE, IGNORE);
                printf("\nOptions:\n");
                printf("\t-%c filename\tSpecify a file with text to sort, otherwise\n",
                    USEFILE);
                printf("\t\t\tSorts arrays of randomly generated integers. If set, -n is\n");
                printf("\t\t\tforced to False (sort array as strings)\n");
                printf("\t-%c\t\tSort numerically (otherwise sorts as strings)\n", NUMERIC);
                printf("\t-%c\t\tSort in reverse order\n", REVERSE);
                printf("\t-%c\t\tPrint out the contents of the sorted array\n", PRINT);
                printf("\t-%c times\tPerform the quicksort multiple times, initializing\n",
                    MULTIPLE);
                printf("\t\t\ta new unsorted array each time. 'times' must be an int\n");
                printf("\t-%c\t\tIgnore letter cases when sorting by strings\n", IGNORE);
                printf("\n");
                return 1;
            }
        }
    }

    // Perform quicksort trials, saving the time taken for each trial to timeSpent
    double *timeSpent = malloc(sizeof(double) * times);
    int nlines;
    if (timeSpent == NULL) {
        printf("ERROR: Couldn't allocate memory space for timeSpent array\n");
        return 1;
    }
    for (int t = 0; t < times; t++) {
        clock_t begin, end;
        // Sorting lines of text from a file rather than an array of random ints
        if (file) {
            // Pull lines of text from specified file
            char **lines = malloc(sizeof(char**));
            if (lines == NULL) {
                printf("ERROR: Couldn't create pointer to lines of text from file.\n");
                return 1;
            }
            int n;
            if ((n = getfile(&lines, argv[fnPos], MAXSTRLEN)) <= 0) {
                printf("ERROR: File '%s' is either empty or couldn't be read \
                    (n = %d)\n", argv[fnPos], n);
                return 1;
            }
            nlines = n;

            // Quicksort + timing
            begin = clock();
            quicksort(lines, STRTYPE, 0, n - 1, THRESHOLD, (int(*)(int, int, int))(tostrcmp),
                reverse, fold, dir);
            end = clock();

            // Print results
            if (print) {
                for (int i = 0; i < n; i++)
                    printf("%s", lines[i]);
                printf("\n");
            }

            // Clean up pointers
            for (--n; n >= 0; n--)
                free(lines[n]);
            free(lines);
        }
        // Sorting array of random integers (default behavior)
        else {
            // Generate an array of ARRSIZE integers between 0 and UPPER
            int demo[ARRSIZE];
            for (int i = 0; i < ARRSIZE; i++)
                demo[i] = randint(UPPER);

            // Quicksort + timing
            begin = clock();
            quicksort(demo, INTTYPE, 0, ARRSIZE - 1, THRESHOLD,
                (int(*)(int, int, int))(numeric ? numcmp : tostrcmp), reverse, fold, dir);
            end = clock();

            // Print results
            if (print) {
                int width = 0;
                for (int i = 0; i < ARRSIZE; i++) {
                    width += widthof(demo[i]) + 2;  // '+2' accounts for ', '
                    if (width > SCREENWIDTH - 1) {
                        printf("\n");
                        width = widthof(demo[i]) + 2;  // '+2' accounts for ', ';
                    }
                    printf("%d%s", demo[i], (i == ARRSIZE - 1) ? "\n\n" : ", ");
                }
            }
        }
        timeSpent[t] = (double)(end - begin) / CLOCKS_PER_SEC;
    }

    printf("\nTime spent sorting %d %s %d times %s:\n",
        file ? nlines : ARRSIZE, file ? "lines of text" : "numbers", times,
        numeric ? "numerically" : "as strings");
    for (int t = 0; t < times; t++)
        printf("\tTrial %d:\t%.3f seconds\n", t + 1, timeSpent[t]);
    printf("\nAverage: %.3f seconds\n", average(timeSpent, times));

    free(timeSpent);
    return 0;
}

/* OVERHEAD STUFF */
/**
* getfile: Reads file fn and puts the lines of text into string array lines.
*   Uses getline(), so the newline character '\n' at the end of each line is
*   included. Assumes lines is empty and needs to be resized.
*
* Returns: The number of lines read, or -1 if an error occured.
*/
int getfile(char ***lines, char fn[], int len) {
    FILE *fp = fopen(fn, "r");
    if (fp == NULL) {
        printf("ERROR: Couldn't open and read file '%s'\n", fn);
        return -1;
    }

    int i = 0;
    do {
        i++;
        *lines = realloc(*lines, sizeof(char**) * i);
        // These memory spaces will be freed in main()
        if (*lines == NULL) {
            printf("ERROR: Couldn't reallocate memory space for text lines array\n");
            return -1;
        }
        (*lines)[i - 1] = malloc(sizeof(char) * len);
        if ((*lines)[i - 1] == NULL) {
            printf("ERROR: Couldn't reallocate memory space for line %d of text\n",
                len);
            return -1;
        }
    } while (fgets((*lines)[i - 1], len, fp) != NULL);

    return i - 1;
}

/**
* randint: Uses rand() to generate a random integer in the range [0, n). Does
*   some correcting for uniformity.
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
* widthof: Finds the character width of int n
*
* Returns: width
*/
int widthof(int n) {
    int i;
    for (i = 1; n /= 10; i++);
    return i;
}
/* END OVERHEAD STUFF */

/* SORTING HELPER FUNCTIONS */
/**
* pivot: Obtain a pivot value for the subarray of v between lo and hi
*
* Returns: index position of value to use as pivot
*/
int pivot(void *a, int type, int lo, int hi,
    int(*comp)(int, int, int), int r, int f, int d) {
    int ptOne = (lo + hi) / 3;
    ptOne = (ptOne < lo) ? lo : ptOne;
    int ptTwo = (ptOne + hi) / 2;
    ptTwo = (ptTwo < ptOne) ? ptOne : ptTwo;

    // Find the median of each third of array v between lo and hi
    int first = median(a, type, lo, (lo + ptOne) / 2, ptOne, comp, r, f, d);
    int second = median(a, type, ptOne + 1, (ptOne + 1 + ptTwo) / 2, ptTwo, comp, r, f, d);
    int third = median(a, type, ptTwo + 1, (ptTwo + 1 + hi) / 2, hi, comp, r, f, d);

    return median(a, type, first, second, third, comp, r, f, d);
}

/**
* median: Obtains the median value for the three items at positions x, y, and z
*   in array v
*
* Returns: x, y, z (whichever is holds the median value), or -1 if something
*   goes wrong
*/
int median(void *a, int type, int x, int y, int z,
    int(*comp)(int, int, int), int r, int f, int d) {
    if (type == INTTYPE) {
        int vX = ((int*)a)[x];
        int vY = ((int*)a)[y];
        int vZ = ((int*)a)[z];

        if ((*comp)(vX, vY, r) < 0) {
            if ((*comp)(vX, vZ, r) < 0)
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
    else if (type == STRTYPE) {
        char *vX = ((char**)a)[x];
        char *vY = ((char**)a)[y];
        char *vZ = ((char**)a)[z];

        if (strcmp2(vX, vY, r, f, d) < 0) {
            if (strcmp2(vX, vZ, r, f, d) < 0)
                return (strcmp2(vY, vZ, r, f, d) < 0) ? y : z;
            else
                return x;
        }
        else {
            if (strcmp2(vY, vZ, r, f, d))
                return (strcmp2(vX, vZ, r, f, d) < 0) ? x : z;
            else
                return y;
        }
    }
    else
        return -1;
}

/**
* partition: Returns an int[2] containing the two indices to use for
*   partitioning the subarray of array v between lo and hi. int[0] and int[1]
*   shouldn't ever be the same number, although they may be only one apart.
*
* Returns: int[2] of partition indices
*/
int *partition(void *a, int type, int lo, int p, int hi,
    int(*comp)(int, int, int), int r, int f, int d) {
    static int fail[] = { -1, -1 };  // just in case
    int i = lo - 1;
    int j = hi + 1;

    void *pivot = malloc(sizeof(char));
    int e = 0;
    if (pivot != NULL) {
        if (type == INTTYPE) {
            pivot = (int*)malloc(sizeof(int));
            if (pivot != NULL)
                *((int*)pivot) = ((int*)a)[p];
            else
                e = 1;
        }
        else if (type == STRTYPE) {
            pivot = (char*)malloc(sizeof(char[MAXSTRLEN]));
            // Copy string at a[p] into pivot so pivot doesn't change
            if (pivot != NULL)
                strcpy((char*)pivot, ((char**)a)[p]);
            else
                e = 1;
        }
    }
    else
        e = 1;
    if (e) {
        printf("ERROR: Couldn't create temp %s pointer to pivot value while partitioning\n",
            (type == INTTYPE) ? "int*" : "char*");
        printf("       subarray a[%d:%d] (pivot index = %d)\n", lo, hi, p);
        return fail;
    }

    while (1) {
        if (type == INTTYPE) {
            // Starting from i, ignore any values <pivot
            do {
                i++;
            } while ((*comp)(((int*)a)[i], *((int*)pivot), r) < 0);

            // Starting from j, ignore any values >pivot
            do {
                j--;
            } while ((*comp)(((int*)a)[j], *((int*)pivot), r) > 0);
        }
        else if (type == STRTYPE) {
            do {
                i++;
            } while (strcmp2(((char**)a)[i], (char*)pivot, r, f, d) < 0);

            do {
                j--;
            } while (strcmp2(((char**)a)[j], (char*)pivot, r, f, d) > 0);
        }

        // If no other pairs to swap found, return partition values
        if (i >= j) {
            int *result = malloc(sizeof(int[2]));
            if (result != NULL) {
                // Leave values equal to pivot value alone
                if (type == INTTYPE) {
                    for (j; j >= lo && (*comp)(((int*)a)[j], *((int*)pivot), r) == 0; j--);
                    for (i; i <= hi && (*comp)(((int*)a)[i], *((int*)pivot), r) == 0; i++);
                }
                else if (type == STRTYPE) {
                    for (j; j >= lo && strcmp2(((char**)a)[j], (char*)pivot, r, f, d) == 0; j--);
                    for (i; i <= hi && strcmp2(((char**)a)[i], (char*)pivot, r, f, d) == 0; i++);
                }
            }
            else {
                printf("ERROR: Couldn't create pointer to partition values while partitioning\n");
                printf("       subarray a[%d:%d] (indices i = %d, j = %d, p = %d)\n",
                    lo, hi, i, j, p);
                return fail;
            }
            result[0] = j;
            result[1] = (i == j) ? i + 1 : i;  // So i and j aren't equal
            free(pivot);
            return result;
        }

        // If a pair to swap found, swap them
        swap(a, type, i, j);
    }
}

/**
* Swaps the values at indices p and q in array a
*/
void swap(void *a, int type, int p, int q) {
    if (type == INTTYPE) {
        int temp = ((int*)a)[p];
        ((int*)a)[p] = ((int*)a)[q];
        ((int*)a)[q] = temp;
    }
    else if (type == STRTYPE) {
        char temp[MAXSTRLEN];
        strcpy(temp, ((char**)a)[p]);
        strcpy(((char**)a)[p], ((char**)a)[q]);
        strcpy(((char**)a)[q], temp);
    }
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
* strcmp2: Basically just strcmp(), but can set options to ignore case in each sentence
*   or even compare in reverse order.
*
* Returns: 0 if the two strings are equal, <0 if the s1<s2, >0 if s2<s1 (unless r = 1)
*/
int strcmp2(char s1[], char s2[], int r, int f, int d) {
    char c1[MAXSTRLEN], c2[MAXSTRLEN];
    strcpy(c1, s1);
    strcpy(c2, s2);
    if (f) {
        strlower(c1);
        strlower(c2);
    }
    if (d)
        return r ? dstrcmp(c2, c1, DIVCHAR) : dstrcmp(c1, c2, DIVCHAR);
    else
        return r ? strcmp(c2, c1) : strcmp(c1, c2);
}

/**
* dstrcmp: Like strcmp, but suited for directory path comparison by only making comparisons
*   on letters, numbers, and blanks. Note that the file path div character technically
*   doesn't need to be passed in, but I'm going to leave it for now in case I want to use
*   it to implement something in the future.
*
* Returns: 0 if the two file paths are equal, <0 if the s1<s2, >0 if s2<s1 (unless r = 1)
*/
int dstrcmp(char s1[], char s2[], char div) {
    int i;
    for (i = 0; (isalnum(s1[i]) || isblank(s1[i])) && (isalnum(s2[i]) || isblank(s2[i])) &&
        (s1[i] == s2[i]); i++);
        if (s1[i] == 0 && s2[i] == 0)
            return 0;
        else
            return s2[i] - s1[i];
}

/**
* strlower: Converts string s to all lowercase in place.
*/
void strlower(char s[]) {
    for (int i = 0; s[i] = tolower(s[i]); i++);
}

/**
* Shell sort algorithm
*/
int shellsort(void *a, int type, int lo, int hi,
    int(*comp)(int, int, int), int r, int f, int d) {
    // Temp pointer to be used with shifting elements
    void *temp = malloc(sizeof(char));
    int e = 0;
    if (temp != NULL) {
        if (type == INTTYPE) {
            temp = (int*)malloc(sizeof(int));
            if (temp == NULL)
                e = 1;
        }
        else {  // STRTYPE
            temp = (char*)malloc(sizeof(char[MAXSTRLEN]));
            if (temp == NULL)
                e = 1;
        }
    }
    else
        e = 1;
    if (e) {
        printf("ERROR: Couldn't cast temp pointer to %s while shellsorting subarray\n",
            (type == INTTYPE) ? "int*" : "char*");
        printf("       a[%d:%d]\n", lo, hi);
        return 1;
    }

    // Actual Shellsorting starts here. For each gap size...
    for (int g = 0; gaps[g] > 0; g++) {
        /* ... do a gapped insertion sort. The first item in each gapped subarray is
        trivially already sorted, so we start from lo+gap and work up from there */
        for (int i = lo + gaps[g]; i <= hi; i++) {
            // Store a[i] in temp in case it needs to be shifted
            if (type == INTTYPE)
                *((int*)temp) = ((int*)a)[i];
            else  // STRTYPE
                strcpy((char*)temp, ((char**)a)[i]);

            /* Check each item sequentially, comparing them to all of the other elements in
            its gapped subarray that came before (if any) */
            int j;
            for (j = i; j >= lo + gaps[g]; j -= gaps[g]) {
                /*  Shift any larger elements up to where element i was, then put element i
                in the new space so it's now ordered in its gapped sub */
                int res;
                if (type == INTTYPE)
                    res = (*comp)(*((int*)temp), ((int*)a)[j - gaps[g]], r);
                else // STRTYPE
                    res = strcmp2((char*)temp, ((char**)a)[j - gaps[g]], r, f, d);

                if (res < 0)
                    swap(a, type, j, j - gaps[g]);
                else
                    break;
            }

            // Put temp where there is now an empty slot in j
            if (type == INTTYPE)
                ((int*)a)[j] = *((int*)temp);
            else  // STRTYPE
                strcpy(((char**)a)[j], temp);
        }
    }

    // Clean up
    free(temp);
    return 0;
}
/* END SORTING HELPER FUNCTIONS */

/**
* Recursive quicksort algorithm
*/
int quicksort(void *a, int type, int lo, int hi, int t,
    int(*comp)(int, int, int), int r, int f, int d) {
    // Base case. Do nothing if there are fewer than two elements in subarray
    if (lo >= hi)
        return 0;
    // Arrays smaller than threshold size t will be sorted with shell sort
    if (hi - lo + 1 <= t)  // '+1' to make it an inclusive range
        return shellsort(a, type, lo, hi, comp, r, f, d);

    int p = pivot(a, type, lo, hi, comp, r, f, d);              // pivot index
    int *parts = partition(a, type, lo, p, hi, comp, r, f, d);  // partition indices

    // Recursion over the two partitions, starting with the smaller of the two
    if (parts[0] - lo <= hi - parts[1]) {
        quicksort(a, type, lo, parts[0], t, comp, r, f, d);
        return quicksort(a, type, parts[1], hi, t, comp, r, f, d);
    }
    else {
        quicksort(a, type, parts[1], hi, t, comp, r, f, d);
        return quicksort(a, type, lo, parts[0], t, comp, r, f, d);
    }
}

/**
* average: Finds the average of an array of doubles. n is the number of doubles
*   in the array.
*
* Returns: The average as a double
*/
double average(double *d, int n) {
    double avg = 0.0;
    for (int i = 0; i < n; i++)
        avg += d[i];
    return avg / (double)n;
}