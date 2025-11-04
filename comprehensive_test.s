# comprehensive_test.s
# This program tests all major features in one run:
# - Initial instruction execution
# - DUMP_PROCESSOR_STATE on the client
# - MIGRATE to a server
# - Resumed execution on the server
# - SNAPSHOT creation on the server
# - Final DUMP_PROCESSOR_STATE on the server

# --- This part executes on the client ---
li $1, 2
li $2, 9
li $3, 12
DUMP_PROCESSOR_STATE

# --- Migration happens here ---
MIGRATE 127.0.0.1:12345

# --- This part executes on the server ---
add $4, $1, $2
sub $5, $2, $1
addi $6, $3, 15
mul $7, $4, $5
SNAPSHOT snapshot_vm1
and $8, $2, $3
or $9, $1, $4
xor $10, $6, $8
ori $11, $4, 100
sll $12, $7, 10
srl $13, $8, 10
DUMP_PROCESSOR_STATE
