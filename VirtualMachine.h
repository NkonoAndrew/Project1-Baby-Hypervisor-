#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "Processor.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class VirtualMachine {
public:
    // Constructor that takes the path to a configuration file.
    VirtualMachine(const std::string& config_file_path);

    // The main execution loop for the VM.
    void run();

    // Prints the loaded configuration.
    void print_config();

private:
    // Loads the configuration from the given file path.
    void load_config(const std::string& config_file_path);

    // Loads the binary instructions from the file specified in the config.
    void load_binary();

    // Parses and executes a single instruction.
    void execute_instruction(const std::string& instruction_line);

    // Stores the configuration key-value pairs.
    std::map<std::string, std::string> config;

    // Stores the instructions loaded from the binary file.
    std::vector<std::string> instructions;

    // The processor (CPU) for this virtual machine.
    Processor cpu;


    // Future additions will include memory management, etc.
};

#endif
