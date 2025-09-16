# Baby Hypervisor

This project is a simplified simulation of a **hypervisor**, which is a program that can run multiple, separate "virtual" computers on a single physical machine. Think of it like having several different computers running inside your one computer, each with its own set of tasks.

## What It Does

The Baby Hypervisor reads configuration files to set up one or more **Virtual Machines (VMs)**. Each VM is given a "binary" file containing a list of simple instructions, and the hypervisor executes these instructions one by one.

This project demonstrates the basic principles of virtualization:
- **Isolation**: Each VM runs independently.
- **Management**: The hypervisor controls the VMs, starting and running them as needed.

## How to Use It

### 1. Build the Program

To get the hypervisor ready, you'll need to compile it. You can do this by opening a terminal and running the `make` command:

```bash
make
```

This will create an executable file named `myvmm`.

### 2. Run the Hypervisor

Once the program is built, you can run it and tell it which virtual machines to start. You do this by using the `-v` flag, followed by the path to a configuration file.

For example, to run two VMs using their respective configuration files, you would use this command:

```bash
./myvmm -v config_file_vm1.txt -v config_file_vm2.txt
```

You can also run a single VM:

```bash
./myvmm -v config_file_vm1.txt
```

### What to Expect

When you run the program, you will see output on your screen showing:
1.  The configuration details for each VM being started.
2.  A message indicating that the VMs are starting their execution.
3.  A final message when all the tasks are complete.

This project provides a foundational look into how virtualization works, all in a simple, easy-to-understand package.
