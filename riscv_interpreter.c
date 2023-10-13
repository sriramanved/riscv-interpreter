#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "riscv.h"

const char *COMMENT_START = "## start";
const int BUFFER_SIZE = 256;
int DEBUG = 0;

/**
 * Prints the values of the 32 registers to stderr, one per line.
 * Since the values are printed to stderr, students can use:
 *
 *     printf("r0 = %d\n", registers->r[0]);
 *     // above is equivalent to fprintf(stdout, "r0 = %d\n", registers->r[0])
 *
 * for debugging purposes, since printf outputs to stdout.
 */
void print_registers(registers_t *registers)
{
    for (int i = 0; i < 32; i++)
    {
        fprintf(stderr, "r[%d] = 0x%x\n", i, registers->r[i]);
    }
}

/**
 * Converts all the characters to lowercase in the given string
 */
void strlower(char *s)
{
    // If the string is not null-terminated, the length is bound by BUFFER_SIZE
    int len = strnlen(s, BUFFER_SIZE);
    for (int i = 0; i < len; i++)
    {
        s[i] = tolower(s[i]);
    }
}

/**
 * Return a pointer to the first non-space character in the string
 */
char *ltrim_spaces(char *s)
{
    while (isspace(*s))
        s++;
    return s;
}

/**
 * Handles the start comment, which initializes a register to a value.
 *
 *     ## start[<r>] = <v>
 *
 * sets the register <r> to the value <v>
 */
void handle_start(char *s, registers_t *registers)
{
    char *start = strstr(s, COMMENT_START);
    strsep(&start, "[");
    int r = atoi(strsep(&start, "]"));
    strsep(&start, "=");
    int v = (int)strtol(start, NULL, 0);
    if (r > 0 && r < 32)
    {
        registers->r[r] = v;
    }
}

int main(int argc, char *argv[])
{
    // If -d or --debug is passed as a command line argument, enable DEBUG mode
    if (argc > 1 && (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--debug") == 0))
    {
        DEBUG = 1;
    }
    // Allocate memory for 32 registers and return a pointer to the memory
    registers_t *registers = (registers_t *)calloc(1, sizeof(registers_t));
    // Call student init() code with the allocated registers
    init(registers);
    // Use a temporary buffer to read a string into
    char buffer[BUFFER_SIZE];
    // Keep track of the current line number
    int line_no = 0;
    // Read from stdin until an EOF (sent using Ctrl+D)
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        // Convert the newline at the end of a line to a null-terminator
        buffer[strcspn(buffer, "\r\n")] = 0;
        // Increment the line number
        line_no++;
        // Print the line if in DEBUG mode (-d or --debug)
        if (DEBUG)
        {
            printf("(DEBUG:%d)> %s\n", line_no, buffer);
        }
        // Handles the comment which initializes a register to a value
        char *start = strstr(buffer, COMMENT_START);
        if (start)
        {
            handle_start(buffer, registers);
            continue;
        }
        // Find the first occurrence of a comment, and terminate string there
        char *comment = strchr(buffer, '#');
        if (comment)
        {
            *comment = '\0';
        }
        // Normalize our input by converting the string to lowercase
        strlower(buffer);

        char *instruction = ltrim_spaces(buffer);
        if (strlen(instruction) == 0)
        {
            continue;
        } else {
            // Call student step() code with the string representing the instruction
            step(instruction);
        }
    }
    // After entire program is read from stdin, print the register values
    print_registers(registers);
    end();
}
