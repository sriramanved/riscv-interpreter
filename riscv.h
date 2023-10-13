/**
 * The interface for a register file containing 32 individual registers
 */
struct registers
{
    int r[32];
};
typedef struct registers registers_t;

/**
 * Initializes the internal state with the given set of register values.
 * This method is called ONCE at the very beginning of the interpreter.
 */
void init(registers_t *starting_registers);

/**
 * Free every memory previously allocated.
 */
void end();

/**
 * Evaluates the given instruction.
 * This method is called ONCE FOR EVERY instruction in the program.
 */
void step(char *instruction);
