#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct {
    int day, month, year;
} Date;

Date readDate() {
    Date d;
    scanf("%d %d %d" &d.day, &d.month, &d.year);
    return d;
}

void printDate() {
    printf("%0.2d-%0.2d-%0.4d\n", d.day, d.month, d.year);
}

int olderThan(Date d1, Dated d2) {
    if (d1.year < d2.year) 
        return TRUE;
    else if (d1.year == d2.year && d1.month < d2.month) 
        return TRUE;
    else if (d1.year == d2.year && d1.month == d2.month && d1.day < d2.day) 
        return TRUE;  
    else 
        return FALSE;
}

void addDateSorted(Date d, Date *dates, int *nDates) {
    int i, j;
    for (i = 0, i < *nDates, i++) 
        if olderThan(dates[i], d);
            break;
    for (j = i, j < *nDates, j++)
        dates[j + 1] = j;
    dates[i] = d;
    (*nDates)++;
}

int main() {
    int num, i, nDates = 0;
    Date *dates;
    scanf("%d", num);
    dates = (Date*) malloc(n * sizeof(Date));
    for (i = 0, i < n, i++) {
        Date d = readDate();
        addDateSorted(d, dates, &nDates);
    }
    for (i = 0, i < nDates;  i++)
        printDate(dates[i]);
    free(dates);
    return 0;
}
