#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#include "stack.h"

enum {
    cmd_rm,
    cmd_push,
    cmd_pop,
    cmd_count,
    cmd_top
};

void help(void) {
	if (isatty(0) && isatty(1)) {
		printf("Commands: \n");
		printf("0 - delete all elements from the stack;\n");
		printf("1 - push a number into stack; \n");
		printf("2 - pop a number from stack; \n");
		printf("3 - show a number of items in stack; \n");
		printf("4 - show a top element in stack. \n");
	}
}

void buy(void) {
	if (isatty(0) && isatty(1))
		printf("\n");
}

void greet(void) {
	if (isatty(0) && isatty(1))
		printf(">>> ");
}

int parent(int ind, int outd) {
	int command;
    int value;
    int error;

    help();

	greet();
    while (scanf("%d", &command) == 1) {
		write(outd, &command, sizeof(command));

        if (command == cmd_push) {
			scanf("%d", &value);
			write(outd, &value, sizeof(value));
		}

        read(ind, &error, sizeof(error));
        read(ind, &value, sizeof(value));

        switch (command) {
			case cmd_top:
				if (!error)
					printf("At top:\t%d\n", value);
				else
					printf("Stack is empty\n");

				break;

			case cmd_pop:
				if (!error)
					printf("Popped:\t%d\n", value);
				else
					printf("Stack is empty\n");

				break;

			case cmd_push:
				if (!error)
					printf("Pushed:\t%d\n", value);
				else
					printf("Error occured\n");

				break;

			case cmd_count:
				printf("Count:\t%d\n", value);
				break;

			case cmd_rm:
				printf("Cleared\n");
				break;

			default:
				printf("Unknown command\n");
				break;
		}
		greet();
    }
	buy();

    close(outd);
    close(ind);

	wait(NULL);
}

int child(int ind, int outd) {
    int    c_num;
	stack *st = st_new();

	if (st) {
		while (read(ind, &c_num, sizeof(c_num)) > 0) {
			int value = 0;
			int error = 0;

			switch (c_num) {
				case cmd_rm:
					st_rm(st);
					break;

				case cmd_push:
					read(ind, &value, sizeof(value));
					error = push(st, value);
					break;

				case cmd_pop:
					error = pop(st, &value);
					break;

				case cmd_count:
					value = st->count;
					break;

				case cmd_top:
					error = top(st, &value);
					break;

			}
			write(outd, &error, sizeof(error));
			write(outd, &value, sizeof(value));
		}
	}

	if (st) {
		st_rm(st);
		free (st);
	}

    close(ind);
    close(outd);

	return 0;
}

int main() {
    // We use two pipes
    // First pipe to send input string from parent
    // Second pipe to send concatenated string from child

    int fd1[2]; // Used to store two ends of first pipe
    int fd2[2]; // Used to store two ends of second pipe
    int er1 = pipe(fd1);
    int er2 = pipe(fd2);

    if (er1 == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (er2 == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    pid_t p = fork();

    if (p < 0) {
        fprintf(stderr, "fork Failed");
        er1 = 1;
    } else if (p > 0) {
		close(fd1[0]);
		close(fd2[1]);
		er1 = parent(fd2[0], fd1[1]);
    } else {
		close(fd1[1]);
		close(fd2[0]);
        er1 = child(fd1[0], fd2[1]);
    }

    return er1;
}
