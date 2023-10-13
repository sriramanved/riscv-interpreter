################################################################################
#             Makefile for the CS 3410 RISC-V Interpreter Project              #
################################################################################

# Additional flags for the compiler
CFLAGS := -std=c99 -D_DEFAULT_SOURCE -Wall -g

# Default target to run, which creates a `riscv_interpreter` executable
all: riscv_interpreter

# Compiles the student linkedlist.c into an object file
# Then, combines the object file into a single `linkedlist` executable
linkedlist: linkedlist.o linkedlist_main.o
	gcc $(CFLAGS) -o $@ $^

# Compiles the student linkedlist.c, hashtable.c into object files
# Then, combines the object files into a single `hashtable` executable
hashtable: linkedlist.o hashtable.o hashtable_main.o
	gcc $(CFLAGS) -o $@ $^

# Compiles the student linkedlist.c, hashtable.c, and riscv.c into object files
# Then, combines the object files into a single `riscv_interpreter` executable
riscv_interpreter: linkedlist.o hashtable.o riscv.o riscv_interpreter.o
	gcc $(CFLAGS) -Werror -o $@ $^

# Wildcard rule that allows for the compilation of a *.c file to a *.o file
%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

# Removes any executables and compiled object files
clean:
	rm -f linkedlist hashtable riscv_interpreter *.o
