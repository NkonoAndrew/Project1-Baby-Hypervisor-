#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "Processor.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class VirtualMachine {
public:
    VirtualMachine(const string& config_file_path);
    void run();
    void print_config();

private:
    void load_config(const string& config_file_path);
    void load_binary();
    void execute_instruction(const string& instruction_line);

    map<string, string> config;
    vector<string> instructions;
    Processor cpu;
    string config_dir;
};

#endif
