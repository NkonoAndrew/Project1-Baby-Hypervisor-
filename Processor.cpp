#include "Processor.h"
#include <iostream>

using namespace std;

Processor::Processor() {
    cpu_state.PC = 0;
    cpu_state.LR = 0;
    cpu_state.IE = 0;
    cpu_state.IRQ = 0;
    for (int i = 0; i < 16; ++i) {
        cpu_state.GPR[i] = 0;
    }
}

void Processor::op_addiu(const vector<string>& operands) {
    // Implementation for addiu instruction
    
}

void Processor::dumpState() {
    cout << "PC: " << cpu_state.PC << endl;
    cout << "LR: " << cpu_state.LR << endl;
    cout << "IE: " << cpu_state.IE << endl;
    cout << "IRQ: " << cpu_state.IRQ << endl;
    for (int i = 0; i < 16; ++i) {
        cout << "GPR[" << i << "]: " << cpu_state.GPR[i] << endl;
    }
}
