#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <cctype>
#include <stdexcept>

using namespace std;

// --- Data Structures and Functions to be tested ---

// CPU State structure (copied from myvmm.cpp)
struct CPUState {
    uint32_t GPR[32];
    uint32_t PC;
    uint32_t HI;
    uint32_t LO;
    uint32_t LR;
    int IE;
    int IRQ;
};

// Helper function to trim whitespace (copied from myvmm.cpp)
void trim(string& s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); }));
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), s.end());
}

// Serialization function (copied from myvmm.cpp)
string serialize_state(const CPUState& state) {
    stringstream ss;
    ss << "PC=" << state.PC << "\n";
    ss << "HI=" << state.HI << "\n";
    ss << "LO=" << state.LO << "\n";
    ss << "LR=" << state.LR << "\n";
    ss << "IE=" << state.IE << "\n";
    ss << "IRQ=" << state.IRQ << "\n";
    for (int i = 0; i < 32; ++i) {
        ss << "R" << i << "=" << state.GPR[i] << "\n";
    }
    return ss.str();
}

// Deserialization function (copied from myvmm.cpp)
CPUState deserialize_state(const string& data) {
    CPUState state = {}; // Zero-initialize
    stringstream ss(data);
    string line;

    while (getline(ss, line)) {
        trim(line);
        if (line.empty()) continue;

        size_t equals_pos = line.find('=');
        if (string::npos == equals_pos) {
            cerr << "Warning: Malformed line in serialized data, skipping: \"" << line << "\"" << endl;
            continue;
        }

        string key = line.substr(0, equals_pos);
        string value_str = line.substr(equals_pos + 1);
        
        try {
            uint32_t value = stoul(value_str);

            if (key == "PC") state.PC = value;
            else if (key == "HI") state.HI = value;
            else if (key == "LO") state.LO = value;
            else if (key == "LR") state.LR = value;
            else if (key == "IE") state.IE = value;
            else if (key == "IRQ") state.IRQ = value;
            else if (key[0] == 'R') {
                try {
                    int reg_index = stoi(key.substr(1));
                    if (reg_index >= 0 && reg_index < 32) {
                        state.GPR[reg_index] = value;
                    } else {
                        cerr << "Warning: Invalid register index in serialized data: " << key << endl;
                    }
                } catch (const exception& e) {
                    cerr << "Warning: Malformed register key in serialized data, skipping: " << key << endl;
                }
            }
        } catch (const std::exception& e) {
            cerr << "Warning: Invalid value in serialized data for key '" << key << "', skipping: " << value_str << endl;
        }
    }
    return state;
}

// Main function for running the tests
int main() {
    cout << "--- Running Serialization/Deserialization Tests ---" << endl;

    // 1. Create and populate a CPUState object
    CPUState original_state = {};
    original_state.PC = 100;
    original_state.HI = 50;
    original_state.LO = 25;
    original_state.GPR[5] = 12345;
    original_state.GPR[10] = 67890;

    // 2. Serialize the state
    string serialized_data = serialize_state(original_state);
    cout << "Serialized State:\n" << serialized_data << endl;

    // 3. Deserialize the state
    CPUState deserialized_state = deserialize_state(serialized_data);

    // 4. Compare the original and deserialized states
    bool success = true;
    if (original_state.PC != deserialized_state.PC) { success = false; cerr << "PC mismatch!" << endl; }
    if (original_state.HI != deserialized_state.HI) { success = false; cerr << "HI mismatch!" << endl; }
    if (original_state.LO != deserialized_state.LO) { success = false; cerr << "LO mismatch!" << endl; }
    for (int i = 0; i < 32; ++i) {
        if (original_state.GPR[i] != deserialized_state.GPR[i]) {
            success = false;
            cerr << "GPR[" << i << "] mismatch!" << endl;
        }
    }

    if (success) {
        cout << "Test 1 (Correct Data): SUCCESS" << endl;
    } else {
        cout << "Test 1 (Correct Data): FAILED" << endl;
    }

    // 5. Test with corrupted data
    cout << "\n--- Testing Error Handling ---" << endl;
    string corrupted_data = "PC=200\nHI=abc\nR5=500\nR99=999\nLO=300";
    cout << "Testing with corrupted data:\n" << corrupted_data << endl;
    CPUState error_state = deserialize_state(corrupted_data);

    // Check if the valid parts were parsed correctly
    cout << "Parsed PC from corrupted data: " << error_state.PC << " (Expected: 200)" << endl;
    cout << "Parsed R5 from corrupted data: " << error_state.GPR[5] << " (Expected: 500)" << endl;
    cout << "Parsed LO from corrupted data: " << error_state.LO << " (Expected: 300)" << endl;
    cout << "Parsed HI from corrupted data: " << error_state.HI << " (Expected: 0, due to error)" << endl;
    
    cout << "--- End of Tests ---\n" << endl;

    return 0;
}
