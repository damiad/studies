1) 439954 - Damian Dąbrowski
2) hard - 1
3) bsk{33017a6f470d2e637713c994f03fd0c7}
4) solve.py

I'll write the solution for the hard version right away, which is analogous to task 5 from lab3 ROP.
Upon inspecting the code, we see that the vulnerability lies in the decrypt() function:
void decrypt() {
    puts("How long is the data?");
    size_t data_len = read_uint();
    char* data = alloca(data_len);
    printf("Gib data: ");
    readn(data, data_len);
    putchar('\n');

    puts("How long is the key?");
    size_t key_len = read_uint();
    char* key = alloca(key_len);
    printf("Gib key: ");
    readn(key, key_len);
    putchar('\n');

    puts("Here's your decrypted data:");
    for (size_t i = 0; i < key_len; ++i)
    {
        data[i] ^= key[i];
        putchar(data[i]);
    }
}

The program reads data and a key, then performs an XOR operation on each byte of data with the corresponding byte of the key. The trick is that data_len and key_len don't have to be equal. Furthermore, since XOR is used, we can manipulate stack items freely, and by inserting \x00 into the key, we can easily print arbitrary data from the stack. We execute solve.py several times and observe the program's behavior.

Another facilitation is that solve.py already provides us with the libc version: libc.so.6. We can easily find offsets. Most of the solution consists of the solve.py file itself.

Let's write down the steps:
We search for necessary instruction offsets in libc.so.6:
pop rdi; ret
/bin/sh
system
Instructions like:
gdb ./libc.so.6
info address system, or ROPgadget
The libc library loads at a different memory address each time (ASLR), but the offset from the beginning of the file to the return address from main() to __libc_start_main() is constant. By examining it, we find it to be 0x280D0.

We set data_len to 1 and key_len to 1 + something. We inspect the stack.
We search for the offset from the beginning of data to the return address from the decrypt() function to main(), and then we overwrite it with a sequence of instructions that will call /bin/sh. This will allow us to execute any command, especially cat /tmp/flag.txt. We have: x^y^y = x.

According to the note:
Note! Depending on the libc version used, an additional gadget may be needed that does nothing but adjust the stack by 8 bytes, e.g.:
msg += p64(libc + POP_RDI_OFF + 1) # pop rdi ; ret
Indeed, we need it, as the offset is 72, we wanted 3 instructions, so we'll use a no-op that simply adjusts the stack by 8 bytes (mod 16 preserved). We'll use 4 instructions.

The script is already working perfectly!
