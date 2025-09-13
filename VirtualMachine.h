#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "Processor.h"
#include <string>
#include <vector>
#include <map>

class VirtualMachine {
public:
    VirtualMachine(const std::string& config_file_path);
    void run();
    void print_config();

private:
    void load_config(const std::string& config_file_path);
    
    std::map<std::string, std::string> config;
    Processor cpu;
    // Memory, etc. will be added here
};

#endif // VIRTUAL_MACHINE_H
