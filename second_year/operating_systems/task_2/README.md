# Task 2

## Requirements
Open: Monday, April 3, 2023, 00:00
Due: Saturday, April 22, 2023, 23:59

## Distributed Stack Machine

Implement a simulator of a distributed stack machine in x86_64 assembly language. The machine consists of N cores, numbered from 0 to N-1, where N is a constant determined during the compilation of the simulator. The simulator will be used with the C language by running N threads, and each thread will invoke the function:

```c
uint64_t core(uint64_t n, char const *p);
```

The parameter `n` represents the core number, and the parameter `p` is a pointer to an ASCIIZ string that defines the computation to be performed by the core. The computation consists of operations performed on a stack, which is initially empty. The characters in the string `p` are interpreted as follows:

- '+' - Pop two values from the stack, calculate their sum, and push the result onto the stack.
- '*' - Pop two values from the stack, calculate their product, and push the result onto the stack.
- '-' - Negate the value on top of the stack.
- '0' to '9' - Push the corresponding value (0 to 9) onto the stack.
- 'n' - Push the core number onto the stack.
- 'B' - Pop a value from the stack. If the value on top of the stack is nonzero, treat the popped value as a two's complement number and perform a number of operations equal to that value.
- 'C' - Pop a value from the stack and discard it.
- 'D' - Duplicate the value on top of the stack.
- 'E' - Swap the top two values on the stack.
- 'G' - Push the value obtained from calling the function `uint64_t get_value(uint64_t n)` (implemented elsewhere in the C language) onto the stack.
- 'P' - Pop a value from the stack (let's denote it as 'w') and call the function `void put_value(uint64_t n, uint64_t w)` (implemented elsewhere in the C language).
- 'S' - Synchronize the cores. Pop a value from the stack, treat it as the core number 'm', wait for the core 'm' to perform an 'S' operation with the core number 'n' popped from the stack, and swap the values on the tops of the stacks of cores 'm' and 'n'.

After completing the computation, the core should return the value from the top of the stack as the result of the `core` function. All operations are performed on 64-bit numbers modulo 2 to the power of 64.

It can be assumed that the provided core number is valid. The computation is assumed to be valid as well, meaning it only contains the characters described above, ends with a null byte, does not attempt to access a value from an empty stack, and does not lead to a deadlock. The behavior of a core for an invalid computation is undefined.

The hardware stack of the processor should be used as the stack used for the described computations. No libraries should be used. The synchronization of cores, i.e., the 'S' operation, should be implemented using some variant of a spin lock.

There are no upper limits on the value of N, except those imposed by the processor's architecture and available memory.

## Compilation
The solution will be compiled using the following command:

```bash
nasm -DN=XXX -f elf64 -w+all -w+error -o core.o core.asm
```

where XXX specifies the value of the constant N.