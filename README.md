# Baby Hypervisor

This project implements a simple Virtual Machine Monitor (VMM), also known as a hypervisor, capable of running custom assembly programs. It supports a MIPS-like instruction set, state snapshots, and a full cold migration feature that allows a running Virtual Machine (VM) to be transferred from one VMM instance to another over the network.

## Features

- **Virtual Machine Execution**: Runs programs written in a custom MIPS-like assembly language.
- **Multi-VM Execution**: Run multiple VMs sequentially in a single command.
- **State Dumps**: The `DUMP_PROCESSOR_STATE` instruction prints the current state of all CPU registers.
- **Snapshots**: The `SNAPSHOT <filename>` instruction saves the current CPU state to a file. VMs can also be started from a snapshot.
- **Cold Migration**: The `MIGRATE <IP:PORT>` instruction serializes the entire VM state and transfers it to a listening server, which then resumes execution.

## How to Compile

The project uses a standard `Makefile`. To compile, simply run `make` in the project directory.

```bash
make
```

This will produce an executable named `myvmm`.

## How to Run & Test

The VMM has two primary modes of operation: **Client Mode** (for running VMs locally) and **Server Mode** (for receiving migrated VMs).

### Testing the Full Migration Feature

This is the primary test case for the cold migration feature. You will need **two terminals**.

---

#### **Terminal 1: The Server**

Start the VMM in server mode to listen for the migration.

1.  **Run the command:**
    ```bash
    ./myvmm -p 12345
    ```
2.  The server will start and wait. Leave this terminal open.

---

#### **Terminal 2: The Client**

Start the VM that will execute the `MIGRATE` instruction.

1.  **Run the command:**
    ```bash
    ./myvmm -v config_comprehensive_test.txt
    ```
2.  The client VM will run, print its state, send the migration data to the server, and then exit. The server will then resume execution.

---

### Testing Legacy Features (Multi-VM and Snapshots)

These tests verify the original functionality of running multiple VMs and loading from snapshots. These tests only require a single terminal.

#### 1. Run Two VMs from Scratch

This test runs two separate virtual machines in sequence.

-   **Command:**
    ```bash
    ./myvmm -v config_file_vm1.txt -v config_file_vm2.txt
    ```

#### 2. Run One VM from a Snapshot

This test loads `vm1` directly from the state saved in `snapshot_vm1`, skipping the initial instructions.

-   **Command:**
    ```bash
    ./myvmm -v config_file_vm1.txt -s snapshot_vm1
    ```

#### 3. Run a Mixed Sequence

This test runs the first VM from a snapshot and the second VM from scratch.

-   **Command:**
    ```bash
    ./myvmm -v config_file_vm1.txt -s snapshot_vm1 -v config_file_vm2.txt
    ```


