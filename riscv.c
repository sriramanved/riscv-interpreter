#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "riscv.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int U_TYPE = 3;
const int UNKNOWN_TYPE = 4;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
static int get_op_type(char *op)
{
    const char *r_type_op[] = {"add", "sub", "and", "or", "xor", "slt", "sll", "sra"};
    const char *i_type_op[] = {"addi", "andi"};
    const char *mem_type_op[] = {"lw", "lb", "sw", "sb"};
    const char *u_type_op[] = {"lui"};
    for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(r_type_op[i], op) == 0)
        {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(i_type_op[i], op) == 0)
        {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(mem_type_op[i], op) == 0)
        {
            return MEM_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(u_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(u_type_op[i], op) == 0)
        {
            return U_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

#define HEX_BASE_NUM 16
#define MAX_IMM_PLACE 2047
#define SIGN_EXTENDER_BITMASK 0xFFFFF000
#define LUI_SHIFT_AMOUNT 12
#define MAX_BYTE_PLACE 0x80
#define BYTE_SIGN_EXTENDER_BITMASK 0xffffff00
#define LEAST_BYTE_BITMASK 0xFF
#define FIRST_BYTE_BITMASK 0x0000FF00
#define SECOND_BYTE_BITMASK 0x00FF0000 
#define THIRD_BYTE_BITMASK 0xFF000000
#define THIRD_BLOCK_BYTE_OFFSET 3
#define SECOND_BLOCK_BYTE_OFFSET 2
#define THIRD_BYTE_SHIFT_AMT 24
#define SECOND_BYTE_SHIFT_AMT 16
#define FIRST_BYTE_SHIFT_AMT 8
#define BUCKET_SIZE 480

registers_t *registers;
hashtable_t *memory;

void init(registers_t *starting_registers)
{
    registers = starting_registers;
    memory = ht_init(BUCKET_SIZE);
}

void end()
{
    free(registers);
    ht_free(memory);
}

/**
 * Return a pointer to the first non-space and non-'x' character in the string.
 */
char *ltrim(char *s)
{
    while (s[0] == ' ' || s[0] == 'x')
        s++;
    return s;
}


/**
 * Returns an int after taking in a pointer to a string and converting to a numerical immediate value
 */
int immediate_converter(char *imm)
{
    int int_imm = 0;
    if (imm[1] == 'x') 
    {
        int_imm = (int) strtol(imm, NULL, HEX_BASE_NUM);

        if (int_imm <= MAX_IMM_PLACE) 
        {
            // If the 11th bit is not set (not a 1), then it will be less than 2^11 = 2047. Do not sign extend.
            return int_imm;
        }
        else 
        {
            // If the 11th bit is set, we must OR the immediate with all 1's in binary in order to sign extend using a bitmask.
            return int_imm | SIGN_EXTENDER_BITMASK;
        }
    }
    else
    {
        // If the immediate is not a string representing a hex value, we simply make the string representing decimal and integer in decimal.
        int_imm = atoi(strsep(&imm, ""));
    }

    return int_imm;
}

void step(char *instruction)
{
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE)
    {
        return;
    }

    if (op_type == R_TYPE)
    {
        // Trim the instruction to get the desired register values. Then convert to numerical values.
        char *rd = ltrim(strsep(&instruction, ","));
        char *rs1 = ltrim(strsep(&instruction, ","));
        char *rs2 = ltrim(strsep(&instruction, ""));

        int rd_num = atoi(strsep(&rd, ""));
        int rs1_num = atoi(strsep(&rs1, ""));
        int rs2_num = atoi(strsep(&rs2, ""));

        if (!rd_num)
        {
            return;
        }

        if (strcmp(op, "add") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] + 
            registers->r[rs2_num];
        }
        else if (strcmp(op, "sub") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] - 
            registers->r[rs2_num];
        }
        else if (strcmp(op, "and") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] & 
            registers->r[rs2_num];
        }
        else if (strcmp(op, "or") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] | 
            registers->r[rs2_num];
        }
        else if (strcmp(op, "xor") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] ^ 
            registers->r[rs2_num];
        }
        else if (strcmp(op, "slt") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] < 
            registers->r[rs2_num];
        }
        else if (strcmp(op, "sll") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] << 
            registers->r[rs2_num];
        }
        else if (strcmp(op, "sra") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] >> 
            registers->r[rs2_num];
        }
    }
    if (op_type == I_TYPE)
    {
        // Trim the instruction to get the desired register values. Then convert to numerical values.
        char *rd = ltrim(strsep(&instruction, ","));
        char *rs1 = ltrim(strsep(&instruction, ","));
        char *imm = ltrim(strsep(&instruction, ""));

        int rd_num = atoi(strsep(&rd, ""));
        int rs1_num = atoi(strsep(&rs1, ""));
        int imm_num = immediate_converter(imm);

        if (!rd_num)
        {
            return;
        }

        if (strcmp(op, "addi") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] + imm_num;
        }
        else if (strcmp(op, "andi") == 0)
        {
            registers->r[rd_num] = registers->r[rs1_num] & imm_num;
        }
    }
    if (op_type == MEM_TYPE)
    {
        // Trim the instruction to get the desired register and address values. Then convert to numerical values.
        char *rs2 = ltrim(strsep(&instruction, ","));
        char *offset = ltrim(strsep(&instruction, "("));
        char *rs1 = ltrim(strsep(&instruction, ")"));

        int rs2_num = atoi(strsep(&rs2, ""));
        int offset_num = immediate_converter(offset);
        int rs1_num = atoi(strsep(&rs1, ""));

        if (strcmp(op, "sw") == 0)
        {
            // To get the relevant byte, use a bitmask
            ht_add(memory, registers->r[rs1_num] + 
            offset_num, registers->r[rs2_num] & LEAST_BYTE_BITMASK);

            ht_add(memory, registers->r[rs1_num] + 
            offset_num + 1, (registers->r[rs2_num] & FIRST_BYTE_BITMASK) >> FIRST_BYTE_SHIFT_AMT);

            ht_add(memory, registers->r[rs1_num] + offset_num + SECOND_BLOCK_BYTE_OFFSET, (registers->r[rs2_num] & SECOND_BYTE_BITMASK) >> SECOND_BYTE_SHIFT_AMT);

            ht_add(memory, registers->r[rs1_num] + offset_num + THIRD_BLOCK_BYTE_OFFSET, (registers->r[rs2_num] & THIRD_BYTE_BITMASK) >> THIRD_BYTE_SHIFT_AMT);
        }
        else if (strcmp(op, "sb") == 0)
        {
            ht_add(memory, registers->r[rs1_num] + 
            offset_num, registers->r[rs2_num] & LEAST_BYTE_BITMASK);
        }
        else if (strcmp(op, "lw") == 0)
        {
            if (!rs2_num)
            {
                return;
            }

            int temp = ht_get(memory, registers->r[rs1_num] + offset_num);

            temp += ht_get(memory, (registers->r[rs1_num] + offset_num + 1)) << FIRST_BYTE_SHIFT_AMT;

            temp += ht_get(memory, (registers->r[rs1_num] + offset_num + SECOND_BLOCK_BYTE_OFFSET)) << SECOND_BYTE_SHIFT_AMT;

            temp += ht_get(memory, (registers->r[rs1_num] + offset_num + THIRD_BLOCK_BYTE_OFFSET)) << THIRD_BYTE_SHIFT_AMT;

            registers->r[rs2_num] = temp;
        }
        else if (strcmp(op, "lb") == 0)
        {
            if (!rs2_num)
            {
                return;
            }

            int temp = ht_get(memory, registers->r[rs1_num] + offset_num);

            if(temp >= MAX_BYTE_PLACE)
            {
                // Sign extend before putting into a register in case the 7th bit is set.
                temp = temp | BYTE_SIGN_EXTENDER_BITMASK;
            }

            registers->r[rs2_num] = temp;
        }
    }
    if (op_type == U_TYPE)
    {
        // Trim the instruction to get the desired register values. Then convert to numerical values.
        char *rd = ltrim(strsep(&instruction, ","));
        char *imm = ltrim(strsep(&instruction, ""));

        int rd_num = atoi(strsep(&rd, ""));

        if (!rd_num)
        {
            return;
        }

        int int_imm = 0;
        if (imm[1] == 'x') {
            int_imm = (int) strtol(imm, NULL, HEX_BASE_NUM);
        }
        else
        {
            int_imm = atoi(strsep(&imm, ""));
        }

        if (strcmp(op, "lui") == 0)
        {
            registers->r[rd_num] = int_imm << LUI_SHIFT_AMOUNT;
        }
    }
}