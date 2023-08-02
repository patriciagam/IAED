#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 10000
#define MAX_LENGTH 1000

int main() {
    char buffer[MAX_LENGTH + 1];
    char *words[MAX_WORDS];
    int counter = 0;
    while (scanf("%s", buffer) == 1) {
        words[counter] = (char*) malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(words[counter++], buffer);
    }
    while (counter--) {
        puts(words[counter]);
        free(words[counter]);
    }
    return 0;
}