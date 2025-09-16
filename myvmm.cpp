#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>

#include "VirtualMachine.h"

using namespace std;

int main(int argc, char *argv[]) {
    vector<string> config_files;
    int opt;

    while ((opt = getopt(argc, argv, "v:")) != -1) {
        switch (opt) {
            case 'v':
                config_files.push_back(optarg);
                break;
            default:
                cerr << "Usage: myvmm -v config_file_vm1 [-v config_file_vm2 ...]" << endl;
                return EXIT_FAILURE;
        }
    }

    if (config_files.empty()) {
        cerr << "Error: At least one config file must be provided." << endl;
        return EXIT_FAILURE;
    }

    vector<VirtualMachine> vms;
    for (const auto& config_file : config_files) {
        vms.emplace_back(config_file);
    }

    cout << "\nStarting VM execution..." << endl;
    for (size_t i = 0; i < vms.size(); ++i) {
        cout << "Starting VM " << i + 1 << " execution..." << endl;
        vms[i].run();
        cout << "VM " << i + 1 << " completed." << endl;
        if (i < vms.size() - 1) {
            cout << "--------------------" << endl;
        }
    }
    cout << "All VM executions finished." << endl;

    return 0;
}