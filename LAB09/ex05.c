#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *v;    
    int cap;   
    int sz;    
} stack;

stack* build() {
    stack *s = (stack*) malloc(sizeof(stack));
    s->v = (int*) malloc(sizeof(int) * 4);
    s->cap = 4;
    s->sz = 0;
    return s;
}

void push(stack * s, int e) { 
    if (s->sz == s->cap) {
        s->cap *= 2;
        s->v = (int*) realloc(s->v, sizeof(int) * s->cap);
    }
    s->v[s->sz++] = e;
}

int top(stack * s) {
    return s->v[s->sz - 1];
}
   
int pop(stack * s) {
    return s->v[--s->sz];
} 

int is_empty(stack * s) {
    return s->sz == 0;
} 

void destroy(stack * s) {
    free(s->v);
    free(s);
}