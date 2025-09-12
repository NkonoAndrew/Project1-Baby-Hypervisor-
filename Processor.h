#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class Processor {
public:
    Processor();
    void op_addiu(const std::vector<std::string>& operands);
    void dumpState();

private:
    std::map<std::string, int32_t> registers;
    uint32_t pc;
    int32_t hi;
    int32_t lo;
};

#endif // PROCESSOR_H
