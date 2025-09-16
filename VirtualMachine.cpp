#include "VirtualMachine.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

using namespace std;

// Initializes a VM by loading its configuration and binary file.
VirtualMachine::VirtualMachine(const string& config_file_path) {
    load_config(config_file_path);
    load_binary();
    cpu.set_pc(0); // Set the Program Counter to the start of the instructions.
}

// Loads the VM's configuration from a file.
// The configuration file specifies parameters like the path to the binary.
void VirtualMachine::load_config(const string& config_file_path) {
    ifstream config_file(config_file_path);
    if (!config_file.is_open()) {
        cerr << "Error: Unable to open config file " << config_file_path << endl;
        return;
    }

    string line;
    while (getline(config_file, line)) {
        istringstream iss(line);
        string key, value;
        if (getline(iss, key, '=') && getline(iss, value)) {
            config[key] = value;
        }
    }
}

// Loads the machine code from the binary file specified in the configuration.
void VirtualMachine::load_binary() {
    auto it = config.find("vm_binary");
    if (it == config.end()) {
        cerr << "Error: vm_binary not found in config" << endl;
        return;
    }

    ifstream binary_file(it->second);
    if (!binary_file.is_open()) {
        cerr << "Error: Unable to open binary file " << it->second << endl;
        return;
    }

    // Read each line of the binary file into the instructions vector.
    string line;
    while (getline(binary_file, line)) {
        instructions.push_back(line);
    }
}

// Prints the configuration of the VM.
void VirtualMachine::print_config() {
    for (const auto& pair : config) {
        cout << "  " << pair.first << " = " << pair.second << endl;
    }
}

// The main execution loop of the virtual machine.
// It fetches, decodes, and executes instructions sequentially.
void VirtualMachine::run() {
    // Loop as long as the Program Counter is within the bounds of the instruction set.
    while (cpu.get_pc() < instructions.size()) {
        string instruction_line = instructions[cpu.get_pc()];
        execute_instruction(instruction_line);
        cpu.increment_pc(); // Move to the next instruction.
    }
}

// Parses and executes a single line of machine code.
void VirtualMachine::execute_instruction(const string& instruction_line) {
    string temp_line = instruction_line;
    // Replace commas with spaces to correctly tokenize operands.
    std::replace(temp_line.begin(), temp_line.end(), ',', ' ');

    istringstream iss(temp_line);
    // Tokenize the instruction line to separate the opcode and operands.
    vector<string> tokens{istream_iterator<string>{iss},
                                 istream_iterator<string>{}};

    if (tokens.empty()) {
        return; // Skip empty lines.
    }

    string opcode = tokens[0];
    tokens.erase(tokens.begin()); // The rest of the tokens are operands.

    // A simple dispatcher to call the correct processor function based on the opcode.
    if (opcode == "add") {
        cpu.op_add(tokens);
    } else if (opcode == "sub") {
        cpu.op_sub(tokens);
    } else if (opcode == "addi") {
        cpu.op_addi(tokens);
    } else if (opcode == "addiu") {
        cpu.op_addiu(tokens);
    } else if (opcode == "mul") {
        cpu.op_mul(tokens);
    } else if (opcode == "and") {
        cpu.op_and(tokens);
    } else if (opcode == "or") {
        cpu.op_or(tokens);
    } else if (opcode == "xor") {
        cpu.op_xor(tokens);
    } else if (opcode == "sll") {
        cpu.op_sll(tokens);
    } else if (opcode == "srl") {
        cpu.op_srl(tokens);
    } else if (opcode == "li") {
        cpu.op_li(tokens);
    } else if (opcode == "DUMP_PROCESSOR_STATE") {
        cpu.op_dump_processor_state();
    } else if (opcode == "addu") {
        cpu.op_addu(tokens);
    } else if (opcode == "subu") {
        cpu.op_subu(tokens);
    } else if (opcode == "andi") {
        cpu.op_andi(tokens);
    } else if (opcode == "ori") {
        cpu.op_ori(tokens);
    } else if (opcode == "mult") {
        cpu.op_mult(tokens);
    } else if (opcode == "div") {
        cpu.op_div(tokens);
    } else if (opcode == "move") {
        cpu.op_move(tokens);
    } else if (opcode == "mfhi") {
        cpu.op_mfhi(tokens);
    } else if (opcode == "mflo") {
        cpu.op_mflo(tokens);
    }
    // Future instructions will be handled by adding more 'else if' blocks here.
    else {
        cerr << "Error: Unknown instruction '" << opcode << "'" << endl;
    }
}
