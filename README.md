# Simple Stack VM

This project contains the code for a simple stack based process VM. The VM assembles assembly into a bytecode and executes the bytecode.

#### Instruction Set

The instruction set used is an extremely simple instruction set. There are only 5 operations: PUSH, POP, ADD, SUB, and HALT. The PUSH and POP instructions are used to put and remove items from the underlying stack that is used to store the operands for the ADD and SUB operations. The HALT instruction is used to exit the VM.

Each instruction is 16 bits wide. The 8 most significant bits, bits 15-8, are reserved for the op code. The 8 least significant bits, bits 7-0, are for the operand for instructions that have operands. With this being a simple instruction set, the only op code that uses an operand is the PUSH instruction.
