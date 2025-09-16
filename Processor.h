#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

using namespace std;

// Represents the state of the CPU, including registers and control bits.
struct CPUState {
    uint32_t GPR[32]; // General Purpose Registers 0-31
    uint32_t PC;      // Program Counter
    uint32_t HI;      // High-order bits of multiplication result
    uint32_t LO;      // Low-order bits of multiplication result, or division remainder
    uint32_t LR;      // Link Register ($ra)
    int IE;           // Interrupt Enable Bit
    int IRQ;          // Interrupt ReQuest
};

// The Processor class simulates a MIPS-like CPU.
// It contains the CPU state and methods to execute MIPS instructions.
class Processor {
public:
    Processor();
    void dumpState();

    uint32_t get_pc() const;
    void set_pc(uint32_t value);
    void increment_pc();

    void op_add(const vector<string>& operands);
    void op_sub(const vector<string>& operands);
    void op_addi(const vector<string>& operands);
    void op_addu(const vector<string>& operands);
    void op_subu(const vector<string>& operands);
    void op_addiu(const vector<string>& operands);
    void op_mul(const vector<string>& operands);
    void op_and(const vector<string>& operands);
    void op_or(const vector<string>& operands);
    void op_xor(const vector<string>& operands);
    void op_andi(const vector<string>& operands);
    void op_ori(const vector<string>& operands);
    void op_sll(const vector<string>& operands);
    void op_srl(const vector<string>& operands);

    void op_mult(const vector<string>& operands);
    void op_div(const vector<string>& operands);
    void op_li(const vector<string>& operands);
    void op_move(const vector<string>& operands);

    void op_mfhi(const vector<string>& operands);
    void op_mflo(const vector<string>& operands);

    void op_dump_processor_state();

private:
    int get_reg_index(const std::string& reg);
    CPUState cpu_state;
};

#endif // PROCESSOR_H

