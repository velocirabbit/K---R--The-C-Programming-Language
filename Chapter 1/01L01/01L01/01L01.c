/* 01L01.c: The First C Program (p. 7) */
#include <stdio.h>  // include information about standard library

// define a function named main() that receives no argument values
main() {  // statements of main() are enclosed in braces
    /* main() calls library function printf() to printf this
     * sequence of characters;
     * '\n' represents the newline character
     */
    printf("hello, world\n");

    // Next two lines are used to make it easier to run the program from the folder
    printf("\n");
    system("pause");

    return 0;  // return statement
}