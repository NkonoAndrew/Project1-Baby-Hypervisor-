#include "Processor.h"
#include <iostream>
#include <stdexcept> // Required for std::invalid_argument and std::out_of_range

using namespace std;

// Initializes the processor's state, setting all registers to zero.
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


// Helper function to parse a register name (e.g., "$1") and return its index.
int Processor::get_reg_index(const std::string& reg) {
    // Make parsing much more robust. Must be at least 2 chars (e.g., "$0")
    // and the part after the '$' must be a valid number with no trailing characters.
    if (reg.length() < 2 || reg[0] != '$') {
        cerr << "Error: Invalid register format '" << reg << "'" << endl;
        return -1;
    }
    
    std::string num_part = reg.substr(1);
    try {
        size_t chars_processed = 0;
        int index = std::stoi(num_part, &chars_processed);

        // Ensure the entire string was consumed (e.g., reject "$1a")
        if (chars_processed != num_part.length()) {
            cerr << "Error: Invalid characters in register name '" << reg << "'" << endl;
            return -1;
        }

        if (index >= 0 && index < 32) {
            return index;
        }
    } catch (const std::exception& e) {
        // Catch any exception from stoi and fall through to the error message
    }

    cerr << "Error: Invalid register number '" << reg << "'" << std::endl;
    return -1; // Invalid register
}

// Dumps the current state of the CPU to the console for debugging.
void Processor::dumpState() {
    cout << "PC: " << cpu_state.PC << endl;
    cout << "LR: " << cpu_state.LR << endl;
    cout << "IE: " << cpu_state.IE << endl;
    cout << "IRQ: " << cpu_state.IRQ << endl;
    cout << "HI: " << cpu_state.HI << endl;
    cout << "LO: " << cpu_state.LO << endl;
    for (int i = 0; i < 32; ++i) {
        cout << "R" << i << "=[" << cpu_state.GPR[i] << "]" << endl;
    }
}

// Helper function to safely convert a string to a SIGNED 32-bit integer.
bool string_to_s32(const std::string& s, int32_t& out) {
    try {
        long result = std::stol(s);
        if (result > INT32_MAX || result < INT32_MIN) {
            cerr << "Error: Immediate value '" << s << "' is out of 32-bit signed range." << endl;
            return false;
        }
        out = static_cast<int32_t>(result);
        return true;
    } catch (const std::exception& e) {
        cerr << "Error: Invalid immediate value '" << s << "'" << endl;
        return false;
    }
}


// Helper function to safely convert a string to an UNSIGNED 32-bit integer.
bool string_to_u32(const std::string& s, uint32_t& out) {
    try {
        unsigned long result = std::stoul(s);
        if (result > UINT32_MAX) {
            cerr << "Error: Immediate value '" << s << "' is out of 32-bit unsigned range." << endl;
            return false;
        }
        out = static_cast<uint32_t>(result);
        return true;
    } catch (const std::exception& e) {
        cerr << "Error: Invalid immediate value '" << s << "'" << endl;
        return false;
    }
}


// --- INSTRUCTION IMPLEMENTATIONS ---

// 3-Operand Instructions
void Processor::op_add(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: add instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);

    if (dest_reg > 0 && src1_reg != -1 && src2_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] + cpu_state.GPR[src2_reg];
    }
}
void Processor::op_sub(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: sub instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);

    if (dest_reg > 0 && src1_reg != -1 && src2_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] - cpu_state.GPR[src2_reg];
    }
}
void Processor::op_addi(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: addi instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    
    int32_t immediate;
    if (!string_to_s32(operands[2], immediate)) {
        return;
    }

    if (dest_reg > 0 && src_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] + immediate;
    }
}
void Processor::op_addu(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: addu instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);

    if (dest_reg > 0 && src1_reg != -1 && src2_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] + cpu_state.GPR[src2_reg];
    }
}

void Processor::op_addiu(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: addiu instruction requires 3 operands" << endl;
        return;
    }

    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t immediate;
    if (!string_to_u32(operands[2], immediate)) {
        return;
    }

    if (dest_reg > 0 && src_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] + immediate;
    }
}

void Processor::op_subu(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: subu instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);

    if (dest_reg > 0 && src1_reg != -1 && src2_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] - cpu_state.GPR[src2_reg];
    }
}
void Processor::op_mul(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: mul instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);

    if (dest_reg > 0 && src1_reg != -1 && src2_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] * cpu_state.GPR[src2_reg];
    }
}
void Processor::op_and(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: and instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);

    if (dest_reg > 0 && src1_reg != -1 && src2_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] & cpu_state.GPR[src2_reg];
    }
}
void Processor::op_or(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: or instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);

    if (dest_reg > 0 && src1_reg != -1 && src2_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] | cpu_state.GPR[src2_reg];
    }
}
void Processor::op_xor(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: xor instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src1_reg = get_reg_index(operands[1]);
    int src2_reg = get_reg_index(operands[2]);

    if (dest_reg > 0 && src1_reg != -1 && src2_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src1_reg] ^ cpu_state.GPR[src2_reg];
    }
}
void Processor::op_andi(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: andi instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t immediate;
    if (!string_to_u32(operands[2], immediate)) {
        return;
    }

    if (dest_reg > 0 && src_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] & immediate;
    }
}
void Processor::op_ori(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: ori instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t immediate;
    if (!string_to_u32(operands[2], immediate)) {
        return;
    }

    if (dest_reg > 0 && src_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] | immediate;
    }
}
void Processor::op_sll(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: sll instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t shift_amount;
    if (!string_to_u32(operands[2], shift_amount)) {
        return;
    }

    if (dest_reg > 0 && src_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] << shift_amount;
    }
}
void Processor::op_srl(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 3) {
        cerr << "Error: srl instruction requires 3 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    uint32_t shift_amount;
    if (!string_to_u32(operands[2], shift_amount)) {
        return;
    }

    if (dest_reg > 0 && src_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg] >> shift_amount;
    }
}

// 2-Operand Instructions
void Processor::op_mult(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 2) {
        cerr << "Error: mult instruction requires 2 operands" << endl;
        return;
    }
    int src1_reg = get_reg_index(operands[0]);
    int src2_reg = get_reg_index(operands[1]);

    if (src1_reg != -1 && src2_reg != -1) {
        uint64_t result = static_cast<uint64_t>(cpu_state.GPR[src1_reg]) * cpu_state.GPR[src2_reg];
        cpu_state.HI = result >> 32;
        cpu_state.LO = result & 0xFFFFFFFF;
    }
}
void Processor::op_div(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 2) {
        cerr << "Error: div instruction requires 2 operands" << endl;
        return;
    }
    int src1_reg = get_reg_index(operands[0]);
    int src2_reg = get_reg_index(operands[1]);

    if (src1_reg != -1 && src2_reg != -1) {
        int32_t dividend = static_cast<int32_t>(cpu_state.GPR[src1_reg]);
        int32_t divisor = static_cast<int32_t>(cpu_state.GPR[src2_reg]);
        if (divisor != 0) {
            cpu_state.LO = static_cast<uint32_t>(dividend / divisor); // quotient
            cpu_state.HI = static_cast<uint32_t>(dividend % divisor); // remainder
        } else {
            cerr << "Error: Division by zero" << endl;
        }
    }
}
void Processor::op_li(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 2) {
        cerr << "Error: li instruction requires 2 operands" << endl;
        return;
    }

    int dest_reg = get_reg_index(operands[0]);
    if (dest_reg > 0) { // Also protects $zero register
        uint32_t immediate;
        if (string_to_u32(operands[1], immediate)) {
             cpu_state.GPR[dest_reg] = immediate;
        }
    }
}
void Processor::op_move(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 2) {
        cerr << "Error: move instruction requires 2 operands" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);

    if (dest_reg > 0 && src_reg != -1) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg];
    }
}

// 1-Operand Instructions
void Processor::op_mfhi(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 1) {
        cerr << "Error: mfhi instruction requires 1 operand" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);

    if (dest_reg > 0) {
        cpu_state.GPR[dest_reg] = cpu_state.HI;
    }
}
void Processor::op_mflo(const vector<string>& operands) {
    // FIX: Check size BEFORE accessing elements to prevent out-of-bounds access.
    if (operands.size() != 1) {
        cerr << "Error: mflo instruction requires 1 operand" << endl;
        return;
    }
    int dest_reg = get_reg_index(operands[0]);

    if (dest_reg > 0) {
        cpu_state.GPR[dest_reg] = cpu_state.LO;
    }
}

// Special Commands
// Dumps the processor state when called.
void Processor::op_dump_processor_state() {
    // This instruction has no operands, so no size check needed.
    dumpState();
}

