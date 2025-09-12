#include "Processor.h"
#include <iostream>

Processor::Processor() : pc(0), hi(0), lo(0) {
    for (int i = 0; i < 32; ++i) {
        registers["$" + std::to_string(i)] = 0;
    }
}

void Processor::op_addiu(const std::vector<std::string>& operands) {
    // Implementation for addiu instruction
}

void Processor::dumpState() {
    std::cout << "PC: " << pc << std::endl;
    std::cout << "HI: " << hi << std::endl;
    std::cout << "LO: " << lo << std::endl;
    for (int i = 0; i < 32; ++i) {
        std::cout << "$" << i << ": " << registers["$" + std::to_string(i)] << std::endl;
    }
}
