#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct str_node  {
    struct str_node *next, *previous;
    char c;
} node;

typedef struct  {
    struct str_node *head, *last;
} list;

list* mk_list() {
    list *ls = (list*) malloc (sizeof(list));
    ls -> head = ls -> last = NULL;
    return ls;
}

void  free_list(list *ls) {
    while (ls -> head) {
        node *a = ls -> head -> next;
        free(ls -> head);
        ls -> head = a;
    }
    free(ls);
}

void  add_last(list *ls, char c) {
    node *new = (node*) malloc (sizeof(node));
    new -> c = c;
    new -> next = NULL;
    new -> previous = ls -> last;
    if (ls -> last != NULL)
        ls -> last -> next = new;
    ls -> last = new;
    if (ls -> head == NULL)
        ls -> head = new;
}

int   is_paly(const list *ls) {
    node *p= ls -> head, *q = ls -> last;
    while (p && q) {
        if (p -> c != q -> c)
            return 0;
        p = p -> next;
        q = q -> previous;
    }
    return 1;
}

int main() {
    int c;
    list *ls = mk_list();
    while ( (c = getchar()) != EOF && c != '\n')
        add_last(ls, c);
    printf("%s\n", is_paly(ls) ? "yes" : "no");
    free_list(ls);
    return 0;
}
