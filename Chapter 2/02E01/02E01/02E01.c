/* 02E01.c (p. 36) */
#include <stdio.h>
#include <limits.h>
#include <float.h>

main() {
    printf("Unsigned types\tMin\t\tMax\n");
    printf("-----------------------------------------\n");
    printf("char          \t%c (%d)\t%c (%d)\n", CHAR_MIN, CHAR_MIN, CHAR_MAX, CHAR_MAX);
    printf("short         \t%d\t\t%d\n", 0, USHRT_MAX);
    printf("int           \t%d\t\t%d\n", 0, UINT_MAX);
    printf("long          \t%d\t\t%d\n", 0, ULONG_MAX);

    printf("\n\n");

    printf("Signed types\tMin\t\tMax\n");
    printf("-----------------------------------------\n");
    printf("char          \t%c (%d)\t%c (%d)\n", SCHAR_MIN, SCHAR_MIN, SCHAR_MAX, SCHAR_MAX);
    printf("short         \t%d\t\t%d\n", SHRT_MIN, SHRT_MAX);
    printf("int           \t%d\t%d\n", INT_MIN, INT_MAX);
    printf("long          \t%d\t%d\n", LONG_MIN, LONG_MAX);
    printf("float         \t%e\t%e\n", FLT_MIN, FLT_MAX);
    printf("double        \t%e\t%e\n", DBL_MIN, DBL_MAX);
    printf("long double   \t%e\t%e\n", LDBL_MIN, LDBL_MAX);

    printf("\n");
    system("pause");
    return 0;
}