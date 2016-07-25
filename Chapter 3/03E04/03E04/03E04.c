/* 03E04.c */
#include <stdio.h>
#include <string.h>
#define MAXSTRLEN 1000
#define MAXITEM 3
#define CHARCOUNT 38  // 38 = 32 bits + 7 spaces - 1 (0-index)

void reverse(char *s);
void itoa(int n, char s[]);
void itoaBinary(int n, char s[]);

main() {
    int i, intArray[MAXITEM];
    char outStr[MAXSTRLEN];

    intArray[0] = 2016;
    intArray[1] = -1776;
    intArray[2] = ~(~0U >> 1);

    system("pause");
    for (i = 0; i < MAXITEM; i++) {
        itoa(intArray[i], outStr);
        printf("itoa(%d) = %s\n", intArray[i], outStr);
        itoaBinary(intArray[i], outStr);
        printf("itoaBinary(%d) = %s\n\n", intArray[i], outStr);
    }

    printf("\n");
    system("pause");
    return 0;
}

/* reverse: Reverses character string s in place */
void reverse(char *s) {
    int len = strlen(s);

    for (int i = 0; i < len / 2; i++) {
        char temp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = temp;
    }
}

/* itoa: Converts int n to character string in s. Includes max negative value
 *       fix. 
 *
 *  FIX: For demical numbers, multiply each individual digit's value by -1 (i.e. take
 *       the 2's complement of each) before adding it to the '0' char.
 */
void itoa(int n, char s[]) {
    int i = 0, sign = n < 0 ? -1 : 1;

    do {                                 /* generate digits in reverse order */
        s[i++] = sign * (n % 10) + '0';  /* get next digit. Multiplying by sign
                                            ensures char added will be a digit */
    } while ((n /= 10) < 0);             /* delete it*/
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

/* itoaBinary: Converts in n to character string in s using binary notation. Fixes
 *             the issue with signed binary numbers at max negative value. Since
 *             negative numbers are represented by the 2's complement of it's, the
 *             correlating positive, the max negative value is (say, for 8-bits)
 *             1000 0000. The positive of a negative number is found by taking the
 *             1's complement (0111 1111), then adding 1 (1000 0000), giving you the
 *             same number again (negative max value) when we would actually want
 *             (0 1000 0000). NOTE that because of this, the absolute value of the
 *             largest negative number is 1 more than the largest positive number.
 *
 *        FIX: For binary representation, simply don't multiply by -1 at the end,
 *             multiply each while check by the sign, and don't include the '-' sign.
 */
void itoaBinary(int n, char s[]) {
    int i = 0, sign = n < 0 ? -1 : 1, div = 0;
    
    do {
        if (n & 1)
            s[i++] = '1';
        else
            s[i++] = '0';
        n >>= 1;
        if (++div % 4 == 0) {
            s[i++] = ' ';
            div = 0;
        }
    } while (i < CHARCOUNT);

    /* Could also do:
     *     while (sign * (n >>= 1) > (-sign + 1));
     * So negative numbers would break when -(n >>= 1) > 2 and positive numbers would
     * break when (n >>= 1) > 0. This also bit shifts n one to the right each loop
     * iteration, so you wouldn't need to do that within the loop itself.
     */

    if (n & 1)
        s[i++] = '1';
    else
        s[i++] = '0';
    s[i] = '\0';
    reverse(s);
}