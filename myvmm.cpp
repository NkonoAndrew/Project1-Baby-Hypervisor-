#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <map>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <sstream>
#include <stdexcept>

using namespace std;

// --- FORWARD DECLARATIONS ---
class Processor;
class VirtualMachine;

// --- Processor.h ---
struct CPUState {
    uint32_t GPR[32]; // General-purpose registers
    uint32_t PC;      // Program Counter
    uint32_t HI;      // High-order bits of multiplication result
    uint32_t LO;      // Low-order bits of multiplication result, or division remainder
    uint32_t LR;      // Link Register
    int IE;           // Interrupt Enable Bit
    int IRQ;          // Interrupt Request
};

class Processor {
public:
    Processor();
    void dumpState();
    uint32_t get_pc() const;
    void set_pc(uint32_t value);
    void increment_pc();
    void set_cpu_state(const CPUState& new_state);
    CPUState get_cpu_state() const;

    // Instruction implementations
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

// --- VirtualMachine.h ---
class VirtualMachine {
public:
    VirtualMachine(const string& config_file_path, const string& snapshot_file_path);
    bool run();
    void print_config();
    uint32_t get_current_pc() const;

private:
    void load_config(const string& config_file_path);
    void load_binary();
    void load_from_snapshot(const string& snapshot_file_path);
    void create_snapshot(const std::string& filename);
    bool execute_instruction(const string& instruction_line);

    map<string, string> config;
    vector<string> instructions;
    Processor cpu;
    string config_dir;
};

// --- Processor Implementation ---

// Checks if an operand is a valid register (e.g., "$5").
bool is_register(const std::string& operand) {
    if (operand.length() < 2 || operand[0] != '$') return false;
    for (size_t i = 1; i < operand.length(); ++i) {
        if (!isdigit(operand[i])) return false;
    }
    return true;
}

// Checks if an operand is a valid immediate value (a number).
bool is_immediate(const string& operand) {
    if (operand.empty() || operand[0] == '$') return false;
    try {
        stol(operand);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

// Formats an instruction for clear error messages.
string format_received_instruction(const string& opcode, const vector<string>& operands) {
    string received = opcode;
    if (!operands.empty()) {
        received += " " + operands[0];
        for (size_t i = 1; i < operands.size(); ++i) {
            received += ", " + operands[i];
        }
    }
    return received;
}

// Validates operand count and types against an expected format.
void validate_operands(const string& opcode, const vector<string>& operands,
                       const vector<char>& expected_types, const string& expected_format) {
    if (operands.size() != expected_types.size()) {
        cerr << "Error: Invalid number of operands for instruction '" << opcode << "'" << endl;
        cerr << "  Expected format: " << expected_format << endl;
        cerr << "  Received:        " << format_received_instruction(opcode, operands) << endl;
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < operands.size(); ++i) {
        bool valid = (expected_types[i] == 'R') ? is_register(operands[i]) : is_immediate(operands[i]);
        if (!valid) {
            cerr << "Error: Invalid operand type for instruction '" << opcode << "'" << endl;
            cerr << "  Expected format: " << expected_format << endl;
            cerr << "  Received:        " << format_received_instruction(opcode, operands) << endl;
            exit(EXIT_FAILURE);
        }
    }
}

// Initializes the processor state, setting all registers to zero.
Processor::Processor() {
    cpu_state.PC = 0;
    cpu_state.LR = 0;
    cpu_state.IE = 0;
    cpu_state.IRQ = 0;
    cpu_state.HI = 0;
    cpu_state.LO = 0;
    for (int i = 0; i < 32; ++i) {
        cpu_state.GPR[i] = 0;
    }
}

// Returns the current value of the Program Counter.
uint32_t Processor::get_pc() const {
    return cpu_state.PC;
}

// Sets the Program Counter to a specific value.
void Processor::set_pc(uint32_t value) {
    cpu_state.PC = value;
}

// Increments the Program Counter.
void Processor::increment_pc() {
    cpu_state.PC++;
}

void Processor::set_cpu_state(const CPUState& new_state) {
    cpu_state = new_state;
}

CPUState Processor::get_cpu_state() const {
    return cpu_state;
}

// Parses a register name (e.g., "$1") and returns its index.
int Processor::get_reg_index(const string& reg) {
    try {
        int index = stoi(reg.substr(1));
        if (index >= 0 && index < 32) return index;
    } catch (const exception& e) {}
    cerr << "Error: Invalid register number '" << reg << "'" << endl;
    exit(EXIT_FAILURE);
}

// Dumps the current state of the CPU to the console.
void Processor::dumpState() {
    cout << "\n--- DUMP_PROCESSOR_STATE ---" << endl;
    cout << "PC: " << cpu_state.PC << endl;
    cout << "LR: " << cpu_state.LR << endl;
    cout << "IE: " << cpu_state.IE << endl;
    cout << "IRQ: " << cpu_state.IRQ << endl;
    cout << "HI: " << cpu_state.HI << endl;
    cout << "LO: " << cpu_state.LO << endl;
    for (int i = 0; i < 32; ++i) {
        cout << "R" << i << "=[" << static_cast<int32_t>(cpu_state.GPR[i]) << "]" << endl;
    }
    cout << "--- END DUMP ---" << endl;
}

// Safely converts a string to a signed 32-bit integer.
int32_t string_to_s32(const string& s) {
    try {
        long result = stol(s);
        if (result > INT32_MAX || result < INT32_MIN) {
            cerr << "Error: Immediate value '" << s << "' is out of 32-bit signed range." << endl;
            exit(EXIT_FAILURE);
        }
        return static_cast<int32_t>(result);
    } catch (const std::exception& e) {
        cerr << "Error: Invalid immediate value '" << s << "'" << endl;
        exit(EXIT_FAILURE);
    }
}

// Safely converts a string to an unsigned 32-bit integer.
uint32_t string_to_u32(const std::string& s) {
    try {
        unsigned long result = std::stoul(s);
        if (result > UINT32_MAX) {
            cerr << "Error: Immediate value '" << s << "' is out of 32-bit unsigned range." << endl;
            exit(EXIT_FAILURE);
        }
        return static_cast<uint32_t>(result);
    } catch (const std::exception& e) {
        cerr << "Error: Invalid immediate value '" << s << "'" << endl;
        exit(EXIT_FAILURE);
    }
}

// --- Instruction Implementations ---

void Processor::op_add(const vector<string>& operands) {
    validate_operands("add", operands, {'R', 'R', 'R'}, "add $rd, $rs, $rt");
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] + cpu_state.GPR[src2_reg];
}

void Processor::op_sub(const vector<string>& operands) {
    validate_operands("sub", operands, {'R', 'R', 'R'}, "sub $rd, $rs, $rt");
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] - cpu_state.GPR[src2_reg];
}

void Processor::op_addi(const vector<string>& operands) {
    validate_operands("addi", operands, {'R', 'R', 'I'}, "addi $rt, $rs, immediate");
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    int32_t immediate = string_to_s32(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] + immediate;
}

void Processor::op_addu(const vector<string>& operands) {
    validate_operands("addu", operands, {'R', 'R', 'R'}, "addu $rd, $rs, $rt");
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] + cpu_state.GPR[src2_reg];
}

void Processor::op_addiu(const vector<string>& operands) {
    validate_operands("addiu", operands, {'R', 'R', 'I'}, "addiu $rt, $rs, immediate");
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t immediate = string_to_u32(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] + immediate;
}

void Processor::op_subu(const vector<string>& operands) {
    validate_operands("subu", operands, {'R', 'R', 'R'}, "subu $rd, $rs, $rt");
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] - cpu_state.GPR[src2_reg];
}

void Processor::op_mul(const vector<string>& operands) {
    validate_operands("mul", operands, {'R', 'R', 'R'}, "mul $rd, $rs, $rt");
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] * cpu_state.GPR[src2_reg];
}

void Processor::op_and(const vector<string>& operands) {
    validate_operands("and", operands, {'R', 'R', 'R'}, "and $rd, $rs, $rt");
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] & cpu_state.GPR[src2_reg];
}

void Processor::op_or(const vector<string>& operands) {
    validate_operands("or", operands, {'R', 'R', 'R'}, "or $rd, $rs, $rt");
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] | cpu_state.GPR[src2_reg];
}

void Processor::op_xor(const vector<string>& operands) {
    validate_operands("xor", operands, {'R', 'R', 'R'}, "xor $rd, $rs, $rt");
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] ^ cpu_state.GPR[src2_reg];
}

void Processor::op_andi(const vector<string>& operands) {
    validate_operands("andi", operands, {'R', 'R', 'I'}, "andi $rt, $rs, immediate");
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t immediate = string_to_u32(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] & immediate;
}

void Processor::op_ori(const vector<string>& operands) {
    validate_operands("ori", operands, {'R', 'R', 'I'}, "ori $rt, $rs, immediate");
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t immediate = string_to_u32(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] | immediate;
}

void Processor::op_sll(const vector<string>& operands) {
    validate_operands("sll", operands, {'R', 'R', 'I'}, "sll $rd, $rt, shamt");
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t shift_amount = string_to_u32(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] << shift_amount;
}

void Processor::op_srl(const vector<string>& operands) {
    validate_operands("srl", operands, {'R', 'R', 'I'}, "srl $rd, $rt, shamt");
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t shift_amount = string_to_u32(operands[2]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] >> shift_amount;
}

void Processor::op_mult(const vector<string>& operands) {
    validate_operands("mult", operands, {'R', 'R'}, "mult $rs, $rt");
    int src1_reg = get_reg_index(operands[0]);
    int src2_reg = get_reg_index(operands[1]);
    uint64_t result = static_cast<uint64_t>(cpu_state.GPR[src1_reg]) * cpu_state.GPR[src2_reg];
    cpu_state.HI = result >> 32;
    cpu_state.LO = result & 0xFFFFFFFF;
}

void Processor::op_div(const vector<string>& operands) {
    validate_operands("div", operands, {'R', 'R'}, "div $rs, $rt");
    int src1_reg = get_reg_index(operands[0]);
    int src2_reg = get_reg_index(operands[1]);
    int32_t dividend = static_cast<int32_t>(cpu_state.GPR[src1_reg]);
    int32_t divisor = static_cast<int32_t>(cpu_state.GPR[src2_reg]);
    if (divisor != 0) {
        cpu_state.LO = static_cast<uint32_t>(dividend / divisor);
        cpu_state.HI = static_cast<uint32_t>(dividend % divisor);
    } else {
        cerr << "Error: Division by zero" << endl;
        exit(EXIT_FAILURE);
    }
}

void Processor::op_li(const vector<string>& operands) {
    validate_operands("li", operands, {'R', 'I'}, "li $rt, immediate");
    int dest_reg = get_reg_index(operands[0]);
    int32_t immediate = string_to_s32(operands[1]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = immediate;
}

void Processor::op_move(const vector<string>& operands) {
    validate_operands("move", operands, {'R', 'R'}, "move $rd, $rs");
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg];
}

void Processor::op_mfhi(const vector<string>& operands) {
    validate_operands("mfhi", operands, {'R'}, "mfhi $rd");
    int dest_reg = get_reg_index(operands[0]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.HI;
}

void Processor::op_mflo(const vector<string>& operands) {
    validate_operands("mflo", operands, {'R'}, "mflo $rd");
    int dest_reg = get_reg_index(operands[0]);
    if (dest_reg > 0) cpu_state.GPR[dest_reg] = cpu_state.LO;
}

void Processor::op_dump_processor_state() {
    if (!cin.eof()) dumpState();
}

// --- VirtualMachine Implementation ---

// Helper function to trim leading/trailing whitespace from a string.
void trim(string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

// Initializes a VM by loading its configuration and binary file.
VirtualMachine::VirtualMachine(const string& config_file_path, const string& snapshot_file_path) {
    load_config(config_file_path);
    load_binary();

    if (!snapshot_file_path.empty()) {
        load_from_snapshot(snapshot_file_path);
    } else {
        cpu.set_pc(0);
    }
}

// Loads the VM's configuration from a file.
void VirtualMachine::load_config(const string& config_file_path) {
    size_t last_slash = config_file_path.find_last_of("/\\");
    config_dir = (string::npos != last_slash) ? config_file_path.substr(0, last_slash + 1) : "./";
    ifstream config_file(config_file_path);
    if (!config_file.is_open()) {
        cerr << "Error: Unable to open config file " << config_file_path << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    int line_num = 0;
    while (getline(config_file, line)) {
        line_num++;
        trim(line);
        if (line.empty() || line[0] == '#') continue;
        istringstream iss(line);
        string key, value;
        if (getline(iss, key, '=') && getline(iss, value)) {
            trim(key);
            trim(value);
            config[key] = value;
        } else {
            cerr << "Warning: Malformed line " << line_num << " in config file, skipping: \"" << line << "\"" << endl;
        }
    }
}

// Loads the machine code from the binary file specified in the configuration.
void VirtualMachine::load_binary() {
    auto it = config.find("vm_binary");
    if (it == config.end()) {
        cerr << "Error: vm_binary not found in config" << endl;
        exit(EXIT_FAILURE);
    }
    string binary_path = config_dir + it->second;
    ifstream binary_file(binary_path);
    if (!binary_file.is_open()) {
        cerr << "Error: Unable to open binary file " << binary_path << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(binary_file, line)) {
        trim(line);
        instructions.push_back(line);
    }
}

// Loads the CPU state from a snapshot file.
void VirtualMachine::load_from_snapshot(const string& snapshot_file_path) {
    ifstream snapshot_file(snapshot_file_path);
    if (!snapshot_file.is_open()) {
        cerr << "Error: Unable to open snapshot file " << snapshot_file_path << endl;
        exit(EXIT_FAILURE);
    }

    CPUState new_state = cpu.get_cpu_state(); // Start with current state as a base
    string line;
    while (getline(snapshot_file, line)) {
        trim(line);
        if (line.empty()) continue;

        size_t equals_pos = line.find('=');
        if (string::npos == equals_pos) continue;

        string key = line.substr(0, equals_pos);
        string value_str = line.substr(equals_pos + 1);
        uint32_t value = stoul(value_str);

        if (key == "PC") new_state.PC = value;
        else if (key == "HI") new_state.HI = value;
        else if (key == "LO") new_state.LO = value;
        else if (key == "LR") new_state.LR = value;
        else if (key == "IE") new_state.IE = value;
        else if (key == "IRQ") new_state.IRQ = value;
        else if (key[0] == 'R') {
            try {
                int reg_index = stoi(key.substr(1));
                if (reg_index >= 0 && reg_index < 32) {
                    new_state.GPR[reg_index] = value;
                }
            } catch (const exception& e) {
                // Ignore malformed register keys
            }
        }
    }
    cpu.set_cpu_state(new_state);
    cout << "\n--- SNAPSHOT LOAD ---" << endl;
    cout << "VM state loaded from: " << snapshot_file_path << endl;
    cout << "--- END SNAPSHOT LOAD ---" << endl;
}

// Creates a snapshot of the current CPU state and saves it to a file.
void VirtualMachine::create_snapshot(const std::string& filename) {
    // Construct the full path for the snapshot file relative to the config directory
    string snapshot_path = config_dir + filename;
    ofstream snapshot_file(snapshot_path);
    if (!snapshot_file.is_open()) {
        cerr << "Error: Unable to open snapshot file " << snapshot_path << " for writing." << endl;
        return;
    }

    CPUState state = cpu.get_cpu_state();

    snapshot_file << "PC=" << state.PC << endl;
    snapshot_file << "HI=" << state.HI << endl;
    snapshot_file << "LO=" << state.LO << endl;
    snapshot_file << "LR=" << state.LR << endl;
    snapshot_file << "IE=" << state.IE << endl;
    snapshot_file << "IRQ=" << state.IRQ << endl;
    for (int i = 0; i < 32; ++i) {
        snapshot_file << "R" << i << "=" << state.GPR[i] << endl;
    }
    snapshot_file.close();
    cout << "Snapshot created: " << snapshot_path << endl;
}

// Prints the configuration of the VM.
void VirtualMachine::print_config() {
    for (const auto& pair : config) {
        cout << "  " << pair.first << " = " << pair.second << endl;
    }
}

// Main execution loop of the virtual machine.
bool VirtualMachine::run() {
    while (cpu.get_pc() < instructions.size()) {
        string instruction_line = instructions[cpu.get_pc()];
        if (instruction_line.empty()) return true; // End of program
        if (!execute_instruction(instruction_line)) return false; // Execution error
        cpu.increment_pc();
    }
    return true;
}

// Parses and executes a single line of machine code.
bool VirtualMachine::execute_instruction(const string& instruction_line) {
    string temp_line = instruction_line;
    std::replace(temp_line.begin(), temp_line.end(), ',', ' ');
    istringstream iss(temp_line);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
    if (tokens.empty() || tokens[0].find("#") == 0) return true; // Skip comments and empty lines
    string opcode = tokens[0];
    tokens.erase(tokens.begin());

    // Dispatch to the correct instruction handler
    if (opcode == "add") cpu.op_add(tokens);
    else if (opcode == "sub") cpu.op_sub(tokens);
    else if (opcode == "addi") cpu.op_addi(tokens);
    else if (opcode == "addiu") cpu.op_addiu(tokens);
    else if (opcode == "mul") cpu.op_mul(tokens);
    else if (opcode == "and") cpu.op_and(tokens);
    else if (opcode == "or") cpu.op_or(tokens);
    else if (opcode == "xor") cpu.op_xor(tokens);
    else if (opcode == "sll") cpu.op_sll(tokens);
    else if (opcode == "srl") cpu.op_srl(tokens);
    else if (opcode == "li") cpu.op_li(tokens);
    else if (opcode == "DUMP_PROCESSOR_STATE") cpu.op_dump_processor_state();
    else if (opcode == "addu") cpu.op_addu(tokens);
    else if (opcode == "subu") cpu.op_subu(tokens);
    else if (opcode == "andi") cpu.op_andi(tokens);
    else if (opcode == "ori") cpu.op_ori(tokens);
    else if (opcode == "mult") cpu.op_mult(tokens);
    else if (opcode == "div") cpu.op_div(tokens);
    else if (opcode == "move") cpu.op_move(tokens);
    else if (opcode == "mfhi") cpu.op_mfhi(tokens);
    else if (opcode == "mflo") cpu.op_mflo(tokens);
    else if (opcode == "SNAPSHOT") {
        if (tokens.size() != 1) {
            cerr << "Error: At line " << cpu.get_pc() + 1 << ": SNAPSHOT instruction requires exactly one filename argument." << endl;
            return false;
        }
        create_snapshot(tokens[0]);
    }
    else {
        cerr << "Error: At line " << cpu.get_pc() + 1 << ": Unknown instruction '" << opcode << "'" << endl;
        return false;
    }
    return true;
}

// Returns the current program counter of the VM's CPU.
uint32_t VirtualMachine::get_current_pc() const {
    return cpu.get_pc();
}

// --- Main Application Logic ---

// Holds information needed to launch a VM, including config and optional snapshot.
struct VMInfo {
    string config_file;
    string snapshot_file;
};

int main(int argc, char *argv[]) {
    vector<VMInfo> vm_infos;
    int opt;

    // Parse command-line arguments for VM configuration and snapshot files.
    while ((opt = getopt(argc, argv, "v:s:")) != -1) {
        switch (opt) {
            case 'v':
                // A new VM is defined. Store its config file.
                vm_infos.push_back({string(optarg), ""});
                break;
            case 's':
                // A snapshot is specified for the most recently defined VM.
                if (vm_infos.empty()) {
                    cerr << "Error: -s flag must be preceded by a -v flag." << endl;
                    return EXIT_FAILURE;
                }
                vm_infos.back().snapshot_file = optarg;
                break;
            default:
                cerr << "Usage: myvmm -v <config_file> [-s <snapshot_file>] ..." << endl;
                return EXIT_FAILURE;
        }
    }

    if (vm_infos.empty()) {
        cerr << "Error: At least one config file must be provided with -v." << endl;
        return EXIT_FAILURE;
    }

    // Create and initialize virtual machines.
    vector<VirtualMachine> vms;
    for (const auto& vm_info : vm_infos) {
        vms.emplace_back(vm_info.config_file, vm_info.snapshot_file);
    }

    // Run the virtual machines sequentially.
    cout << "\nStarting VM execution..." << endl;
    for (size_t i = 0; i < vms.size(); ++i) {
        cout << "Starting VM " << i + 1 << " execution..." << endl;
        if (vms[i].run()) {
            cout << "VM " << i + 1 << " completed." << endl;
        } else {
            cout << "VM " << i + 1 << " failed due to an execution error at pc = " << vms[i].get_current_pc() << "." << endl;
        }
        if (i < vms.size() - 1) {
            cout << "--------------------" << endl;
        }
    }
    cout << "All VM executions finished." << endl;
    return 0;
}
