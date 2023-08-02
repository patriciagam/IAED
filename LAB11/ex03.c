/* iaed-23 - ist1107245 - lab11/ex03 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 1000

typedef struct stru_node {
    struct stru_node *next;
    char *v;
} node;

node* push(node *head, const char *v) {
    node *new = (node*) malloc (sizeof(node));
    new -> v = (char*) malloc (sizeof(char) * (strlen(v) + 1));
    strcpy (new -> v,  v);
    new -> next = head;
    return new;
}

node* pop(node *head) {
    node *a = head -> next;
    free(head -> v);
    free(head);
    return a;
} 

node* destroy(node *head) {
    while (head)
        head = pop (head);
    return NULL;
}

void print(node *head) {
    for(; head; head = head -> next)
        printf("%s\n", head -> v);
}

int main() {
    char buffer[MAXSTRING];
    node *head= NULL;
    while (scanf("%s", buffer) == 1 && strcmp(buffer, "x") != 0)
        head = push(head, buffer);
    print(head);
    destroy(head);
    return 0;
}