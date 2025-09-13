#include "VirtualMachine.h"
#include <fstream>
#include <iostream>
#include <sstream>

VirtualMachine::VirtualMachine(const std::string& config_file_path) {
    load_config(config_file_path);
    std::cout << "Configuration for " << config_file_path << ":" << std::endl;
    print_config();
}

void VirtualMachine::load_config(const std::string& config_file_path) {
    std::ifstream config_file(config_file_path);
    if (!config_file.is_open()) {
        std::cerr << "Error: Unable to open config file " << config_file_path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(config_file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            config[key] = value;
        }
    }
}

void VirtualMachine::print_config() {
    for (const auto& pair : config) {
        std::cout << "  " << pair.first << " = " << pair.second << std::endl;
    }
}

void VirtualMachine::run() {
    // VM execution logic will go here
}
