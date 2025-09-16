# Baby Hypervisor

This project is a lightweight, text-based hypervisor that simulates the execution of multiple Virtual Machines (VMs) on a single host. It is designed to demonstrate the core concepts of virtualization, including VM management, configuration parsing, and isolated instruction execution.

Each VM runs a program written in a simplified, MIPS-like assembly language. The hypervisor reads configuration files to set up the environment for each VM and then executes its binary instruction file.

## Key Features

- **Multi-VM Management**: Can load and run multiple VMs concurrently, specified via command-line arguments.
- **Configuration Files**: Each VM is configured with a simple `.txt` file that specifies its binary program.
- **MIPS-like Instruction Set**: The processor for each VM supports a subset of MIPS arithmetic and logical instructions.
- **Relative Pathing**: VM binaries can be referenced with paths relative to their configuration files, making the project portable.

## How to Use

### 1. Build the Program

Compile the project using the provided `Makefile`. Open a terminal in the project directory and run:

```bash
make
```

This command compiles the C++ source files and creates an executable named `myvmm`.

### 2. Run the Hypervisor

Execute the program from your terminal, using the `-v` flag to specify the configuration file for each VM you want to run.

**To run two VMs:**
```bash
./myvmm -v config_file_vm1.txt -v config_file_vm2.txt
```

**To run a single VM:**
```bash
./myvmm -v config_file_vm1.txt
```

The program will print the state of each VM's registers upon completion of its instruction set.

## Supported MIPS Instructions

The simulator supports the following arithmetic and logical instructions:

| Instruction | Example                  | Description                                       |
|-------------|--------------------------|---------------------------------------------------|
| `li`        | `li $1, 100`             | Load Immediate: Loads a constant into a register. |
| `add`       | `add $3, $1, $2`         | Add: Adds two registers and stores in a third.    |
| `sub`       | `sub $4, $2, $1`         | Subtract: Subtracts two registers.                |
| `addi`      | `addi $5, $1, 25`        | Add Immediate: Adds a register and a constant.    |
| `mul`       | `mul $7, $1, $2`         | Multiply: Multiplies two registers.               |
| `and`       | `and $8, $1, $2`         | Bitwise AND (Register).                           |
| `or`        | `or $9, $1, $2`          | Bitwise OR (Register).                            |
| `xor`       | `xor $10, $1, $2`        | Bitwise XOR (Register).                           |
| `ori`       | `ori $11, $2, 100`       | Bitwise OR (Immediate).                           |
| `sll`       | `sll $12, $1, 2`         | Shift Left Logical.                               |
| `srl`       | `srl $13, $1, 2`         | Shift Right Logical.                              |
| `move`      | `move $1, $2`            | Move: Copies the value of one register to another.|
| `mult`      | `mult $1, $2`            | Multiply: Stores 64-bit result in `HI`/`LO`.      |
| `div`       | `div $1, $2`             | Divide: Stores quotient in `LO`, remainder in `HI`.|
| `mfhi`      | `mfhi $3`                | Move From HI: Copies `HI` to a register.          |
| `mflo`      | `mflo $3`                | Move From LO: Copies `LO` to a register.          |

Additionally, the custom command `DUMP_PROCESSOR_STATE` can be used to print the current register values at any point in a program.
