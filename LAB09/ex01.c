/* iaed-23 - ist1107245 - lab09/ex01 */

#include <stdio.h>

#define MAX_LENGTH 1001

int main() {
    char *pindex,  word[MAX_LENGTH];
    scanf("%s", word);
    for(pindex = word; *pindex != '\0'; pindex++)
        printf("%s\n", pindex);
    return 0;
}
