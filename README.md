# Baby Hypervisor

This project is a lightweight, text-based hypervisor that simulates the execution of multiple Virtual Machines (VMs). It demonstrates core virtualization concepts, including VM management, configuration parsing, isolated instruction execution, and state snapshotting.

Each VM runs a program in a simplified, MIPS-like assembly language. The hypervisor can start a VM from scratch or restore its exact state from a previously saved snapshot file.

## Key Features

- **Multi-VM Management**: Load and run multiple VMs, specified via command-line arguments.
- **Configuration Files**: Each VM is defined by a simple `.txt` file that points to its binary program.
- **State Snapshots**:
  - **Save State**: A running VM can execute a `SNAPSHOT` instruction to save its complete CPU state (registers, PC) to a file.
  - **Load State**: The hypervisor can launch a VM directly from a snapshot file, resuming execution exactly where it left off.
- **MIPS-like Instruction Set**: The VM's processor supports a subset of MIPS arithmetic and logical instructions.
- **Robust Error Handling**: The instruction parser validates formats and operand types, exiting on errors to prevent undefined behavior.

## How to Use

### 1. Build the Program

Compile the project using the provided `Makefile`. This creates the `myvmm` executable.

```bash
make
```

### 2. Run the Hypervisor

Execute the program from your terminal, using flags to specify configurations and optional snapshots.

**Syntax:**
```bash
./myvmm -v <config_file> [-s <snapshot_file>] ...
```

- `-v <config_file>`: **(Required)** Specifies a VM to run via its configuration file.
- `-s <snapshot_file>`: **(Optional)** Loads the VM state from a snapshot. This flag applies to the most recent `-v` flag.

**Examples:**

- **Run two VMs from scratch:**
  ```bash
  ./myvmm -v config_file_vm1.txt -v config_file_vm2.txt
  ```

- **Run one VM from a snapshot:**
  ```bash
  ./myvmm -v config_file_vm1.txt -s snapshot_vm1
  ```

- **Run two VMs—one from a snapshot, one from scratch:**
  ```bash
  ./myvmm -v config_file_vm1.txt -s snapshot_vm1 -v config_file_vm2.txt
  ```

## Supported MIPS Instructions

The simulator supports the following instructions. Invalid instruction formats or operand counts will cause the program to exit with an error.

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

### Special Instructions

| Instruction              | Example                             | Description                                           |
|--------------------------|-------------------------------------|-------------------------------------------------------|
| `DUMP_PROCESSOR_STATE`   | `DUMP_PROCESSOR_STATE`              | Prints the current CPU register values to the console.|
| `SNAPSHOT`               | `SNAPSHOT my_snapshot.snap`         | Saves the current CPU state to the specified file.    |

