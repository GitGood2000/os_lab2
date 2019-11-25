#ifndef D_STACK_H
#define D_STACK_H

typedef struct {
    size_t size;
    size_t count;
    int   *data;
} stack;

stack *st_new(void);
void   st_rm (stack       *st);
int    push  (stack       *st, int  val);
int    pop   (stack       *st, int *val);
int    top   (const stack *st, int *val);

#endif //D_STACK_H
