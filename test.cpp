#include <iostream>
#include <cstdio>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <name>" << endl;
        return 1;
    }
    cout << argv[0] << " says Hello, " << argv[1] << "!" << endl;
}