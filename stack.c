#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define MULTIPLIER 2

stack *st_new(void){
    stack *st = (stack *)malloc(sizeof(stack));

    if (st){
        st->size  = 4;
        st->count = 0;
        st->data  = (int *)malloc(sizeof(int) * st->size);
    }

    return st;
}

void st_rm(stack *st) {
	if (st) {
		if (st->data)
			free(st->data);

		st->count = 0;
		st->size  = 0;
		st->data  = NULL;
	}
}

int resize(stack *st) {
	int err = 0;

	if (st && st->count >= st->size) {
		int *tmp;

		if (!st->size)
			st->size++;

		tmp = (int *)realloc(st->data, st->size * sizeof(int) * MULTIPLIER);

		if (tmp) {
			st->data  = tmp;
			st->size *= MULTIPLIER;
		}
		else
			err = 1;
	}

	return err;
}

int push (stack *st, int val){
	int err = 0;

	if (st) {
		err = resize(st);

		if (!err) {
			st->data[st->count] = val;
			st->count++;
		}
	}

	return err;
}

int pop(stack *st, int *val) {
	int err = top(st, val);

	if (st && st->count)
		st->count--;

	return err;
}

int top(const stack *st, int *val) {
	int err = 0;

    if (st && st->count) {
		if (val)
			*val = st->data[st->count - 1];
	}
	else
		err = 1;

	return err;
}
