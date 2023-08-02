#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stru_node {
    struct stru_node *next;
    int v;
} node;

node* push(node *head, int v) {
    node *new = (node*) malloc (sizeof(node));
    new -> v = v;
    new -> next = head;
    return new;
}

node* pop(node *head) {
    node *a = head -> next;
    free(head);
    return a;
} 

node* destroy(node *head) {
    while (head)
        head = pop (head);
    return NULL;
}

void print(node *head) {
    while (head) { 
        printf("%d\n", head -> v);
        head = head -> next;
    }
}

int main() {
    int n;
    node *head = NULL;
    while (scanf ("%d", &n) == 1 && n != 0)
        head = push(head, n);
    print(head);
    destroy(head);
    return 0;
}
