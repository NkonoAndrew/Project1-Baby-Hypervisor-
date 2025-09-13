#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct CPUState {
    uint32_t GPR[16]; // General Purpose Registers
    uint32_t LR;      // Link Register
    uint32_t PC;      // Program Counter
    int IE;           // Interrupt Enable Bit
    int IRQ;          // Interrupt ReQuest
};

class Processor {
public:
    Processor();
    void op_addiu(const vector<string>& operands);
    void dumpState();

private:
    CPUState cpu_state;
};

#endif // PROCESSOR_H
