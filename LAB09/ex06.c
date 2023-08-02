/* iaed-23 - ist1107245 - lab09/ex06 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *v;     /* contents of the stack */
    int cap;    /* capacity of v, i.e. how many elements can fit in v */
    int sz;     /* number of elements currently stored in v */
} stack;


/* builds a new empty stack with initial capacity 4 */
stack* build() {
    stack *stck = (stack*) malloc(sizeof(stack));
    stck->v = (int*) malloc(sizeof(int) * 4);
    stck->cap = 4;
    stck->sz = 0;
    return stck;
}

/* pushes integer on top of the stack (reallocate v if necessary) */
void push(stack * stck, int e) { 
    if (stck->sz == stck->cap) {
        stck->cap *= 2;
        stck->v = (int*) realloc(stck->v, sizeof(int) * stck->cap);
    }
    stck->v[stck->sz++] = e;
}

/* returns top element in the stack */
int top(stack * stck) {
    return stck->v[stck->sz - 1];
}

/* removes top element from the stack and returns it (not necessary to reallocate v) */                
int pop(stack * stck) {
    return stck->v[--stck->sz];
} 

/* returns 1 if stck represents the empty stack, returns 0 otherwise */
int is_empty(stack * stck) {
    return stck->sz == 0;
} 

/* frees the memory associated with the stack */
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