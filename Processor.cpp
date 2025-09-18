#include "Processor.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

using namespace std;

// --- HELPER FUNCTIONS FOR VALIDATION ---

// Checks if an operand is in the register format (e.g., "$5").
bool is_register(const std::string& operand) {
    if (operand.length() < 2 || operand[0] != '$') {
        return false;
    }
    // Check if the rest are digits
    for (size_t i = 1; i < operand.length(); ++i) {
        if (!isdigit(operand[i])) return false;
    }
    return true;
}

// Checks if an operand is in the immediate format (a number).
bool is_immediate(const std::string& operand) {
    if (operand.empty()) {
        return false;
    }
    // An immediate value cannot look like a register.
    if (operand[0] == '$') {
        return false;
    }
    try {
        // Attempt to convert to a long to check if it's a number.
        std::stol(operand);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

// Formats the received instruction for clear error messages.
std::string format_received_instruction(const std::string& opcode, const std::vector<std::string>& operands) {
    std::string received = opcode;
    if (!operands.empty()) {
        received += " " + operands[0];
        for (size_t i = 1; i < operands.size(); ++i) {
            received += ", " + operands[i];
        }
    }
    return received;
}

// Main validation function to check operand count and types.
void validate_operands(const std::string& opcode, const std::vector<std::string>& operands,
                       const std::vector<char>& expected_types, const std::string& expected_format) {
    if (operands.size() != expected_types.size()) {
        cerr << "Error: Invalid number of operands for instruction '" << opcode << "'" << endl;
        cerr << "  Expected format: " << expected_format << endl;
        cerr << "  Received:        " << format_received_instruction(opcode, operands) << endl;
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < operands.size(); ++i) {
        bool valid = false;
        if (expected_types[i] == 'R') {
            valid = is_register(operands[i]);
        } else if (expected_types[i] == 'I') {
            valid = is_immediate(operands[i]);
        }

        if (!valid) {
            cerr << "Error: Invalid operand type for instruction '" << opcode << "'" << endl;
            cerr << "  Expected format: " << expected_format << endl;
            cerr << "  Received:        " << format_received_instruction(opcode, operands) << endl;
            exit(EXIT_FAILURE);
        }
    }
}

// --- PROCESSOR INITIALIZATION AND STATE ---

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

uint32_t Processor::get_pc() const { return cpu_state.PC; }
void Processor::set_pc(uint32_t value) { cpu_state.PC = value; }
void Processor::increment_pc() { cpu_state.PC++; }

// --- PARSING AND CONVERSION HELPERS ---

int Processor::get_reg_index(const std::string& reg) {
    // Validation is handled by validate_operands, this just extracts the number.
    try {
        int index = std::stoi(reg.substr(1));
        if (index >= 0 && index < 32) {
            return index;
        }
    } catch (const std::exception& e) {
        // This should not be reached if validation passes, but is a safeguard.
    }
    cerr << "Error: Invalid register number '" << reg << "'" << std::endl;
    exit(EXIT_FAILURE);
}

void Processor::dumpState() {
    cout << "PC: " << cpu_state.PC << endl;
    cout << "LR: " << cpu_state.LR << endl;
    cout << "IE: " << cpu_state.IE << endl;
    cout << "IRQ: " << cpu_state.IRQ << endl;
    cout << "HI: " << cpu_state.HI << endl;
    cout << "LO: " << cpu_state.LO << endl;
    for (int i = 0; i < 32; ++i) {
        cout << "R" << i << "=[" << static_cast<int32_t>(cpu_state.GPR[i]) << "]" << endl;
    }
}

int32_t string_to_s32(const std::string& s) {
    try {
        long result = std::stol(s);
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

// --- INSTRUCTION IMPLEMENTATIONS ---

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
    if (dest_reg > 0) {
        cpu_state.GPR[dest_reg] = immediate;
    }
}

void Processor::op_move(const vector<string>& operands) {
    validate_operands("move", operands, {'R', 'R'}, "move $rd, $rs");
    int dest_reg = get_reg_index(operands[0]);
    int src_reg = get_reg_index(operands[1]);
    if (dest_reg > 0) {
        cpu_state.GPR[dest_reg] = cpu_state.GPR[src_reg];
    }
}

void Processor::op_mfhi(const vector<string>& operands) {
    validate_operands("mfhi", operands, {'R'}, "mfhi $rd");
    int dest_reg = get_reg_index(operands[0]);
    if (dest_reg > 0) {
        cpu_state.GPR[dest_reg] = cpu_state.HI;
    }
}

void Processor::op_mflo(const vector<string>& operands) {
    validate_operands("mflo", operands, {'R'}, "mflo $rd");
    int dest_reg = get_reg_index(operands[0]);
    if (dest_reg > 0) {
        cpu_state.GPR[dest_reg] = cpu_state.LO;
    }
}

void Processor::op_dump_processor_state() {
    if (!cin.eof()) {
        dumpState();
    }
}
