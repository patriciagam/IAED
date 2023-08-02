#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    int num, i;
    Date *dates;
    scanf("%d", num);
    dates = (Date*) malloc(n * sizeof(Date));
    for (i = 0, i < n, i++)
        dates[i] = readDate();
    while (i--)
        printDate(dates[i]);
    free(dates);
    return 0;
}
