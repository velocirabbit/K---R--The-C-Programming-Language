/* 05E09.c (p. 114) */
#include <stdio.h>
#include <stdlib.h>

char daytab[2][13] = {
    { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);

int main(void) {

}

/* day_of_year: set day of year from month & day */
int day_of_year(int year, int month, int day) {
    int leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    char *p = leap ? daytab[1] : daytab[0];
    if (month > 12 || day > p[month]) {
        printf("ERROR: %d/%d is not a valid day of the year\n", month, day);
        return 0;
    }
    
    for (int i = 1; i < month; i++)
        day += p[i];
    return day;
}

/* month_day: set month, day from day of year */
void month_day(int year, int yearday, int *pmonth, int *pday) {
    int leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    char *p = leap ? daytab[1] : daytab[0];
    if ((leap && yearday > 366) || (!leap && yearday > 365)) {
        printf("ERROR: yearday cannot be greater than number of days in year\n");
        return;
    }

    int i;
    for (i = 1; yearday > p[i]; i++)
        yearday -= p[i];
    *pmonth = i;
    *pday = yearday;
}