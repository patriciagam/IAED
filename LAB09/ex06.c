#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *v;   
    int cap;    
    int sz;     
} stack;


stack* build() {
    stack *stck = (stack*) malloc(sizeof(stack));
    stck->v = (int*) malloc(sizeof(int) * 4);
    stck->cap = 4;
    stck->sz = 0;
    return stck;
}


void push(stack * stck, int e) { 
    if (stck->sz == stck->cap) {
        stck->cap *= 2;
        stck->v = (int*) realloc(stck->v, sizeof(int) * stck->cap);
    }
    stck->v[stck->sz++] = e;
}


int top(stack * stck) {
    return stck->v[stck->sz - 1];
}

           
int pop(stack * stck) {
    return stck->v[--stck->sz];
} 


int is_empty(stack * stck) {
    return stck->sz == 0;
} 


void destroy(stack * stck) {
    free(stck->v);
    free(stck);
}


int main() {
    int i;
    char input[1000];
    stack* stck;

    if (scanf("%stck", input) == 0)
        return 0;
    stck = build();
    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == '(' || input[i] == '{' || input[i] == '[')
            push(stck, input[i]);
        else if (input[i] == ')' || input[i] == '}' || input[i] == ']') {
            char c = pop(stck);
            if ((c == '(' && input[i] != ')') ||
                (c == '{' && input[i] != '}') ||
                (c == '[' && input[i] != ']')) {
                printf("no\n");
                destroy(stck);
                return 0;
            }
        }
    }
    if (is_empty(stck))
        printf("yes\n");
    else
        printf("no\n");
    destroy(stck);
    return 0;
}
