**Damian Dąbrowski task 2 BSK**

Firstly, we attempt to earn the first point. To achieve this, I launch x64dbg and the retask application, then connect it to x64dbg. I also start IDA Free with retask to analyze the code.

Browsing through the available functions, I found:
- `load_pc_resources`
- Numerous functions prefixed with `pc_` and `doPC_`
- Notably, `doPC_attemptPassword`

In `pc_keypress`, which handles character input, there's a call to `pc_putch`. After analyzing:
```assembly
mov word ptr [rcx+rdx], 2Ah ; '*'
```
This replaces a character with a '*'. By modifying it to:
```assembly
mov word ptr [rcx+rdx], r9w
```
and ensuring alignment (`ret;`), I successfully earned the first point after several hours!

Next, finding the password. In `doPC_attemptPassword`, `pc_check_luks_password` is invoked. Within it:
```c
if (dword_140011B5C == 8)
```
If the password length is eight characters:
```c
LODWORD(v0) = 0;
LODWORD(v1) = 145;
```
Set the lower 32 bits of `v0` and `v1` to 0 and 145 respectively.

Continuing through the function, we encounter:
```c
return hash_password(v0, v1, 8i64) == 0x3E4C3F5BDF80E7D3i64;
```
Here, `hash_password` is called with `v0 = 8`, `v1 = 145/(2^8) = 0`, and `8i64`.

Analyzing `hash_password`:
```assembly
loc_1400028E1:
movzx   edx, byte ptr [r9+r10+11B48h]
inc     r9
movzx   ecx, al
xor     rdx, rcx
shr     rax, 8
xor     rax, ds:rva qword_140009730[r10+rdx*8]
cmp     r9, r8
jb      short loc_1400028E1
```
Here, careful not to exceed the `qword_140009730` array bounds.

To reverse the hashing:
- First byte shifts `rax` right, zeroing the first byte, then `xor` with hardcoded array element.
- The result `0x3E4C3F5BDF80E7D3i64` in little-endian form guides the process.

Calculating `rax`:
- 1. `00 00 00 00 00 00 00 00`
- 2. `58 63 db f1 e3 ac 9d ec`
- 3. `9c ae 39 c0 c7 6c d9 b3`
- 4. `a0 b8 39 f3 94 a6 01 11`
- 5. `3a 89 a3 3d 7a a9 cf 8c`
- 6. `e9 b5 bc 97 4a d7 98 68`
- 7. `09 b3 7c 8e 0d 8e 6b 03`
- 8. `c8 40 c6 1b df d0 13 ec`

Resulting characters:
- 1. `112 CHARACTER - p`
- 2. `52 CHARACTER - 4`
- 3. `53 CHARACTER - 5`
- 4. `53 CHARACTER - 5`
- 5. `119 CHARACTER - w`
- 6. `48 CHARACTER - 0`
- 7. `52 CHARACTER - 4`
- 8. `100 CHARACTER - d`

Password: `p455w04d`, verified and functional, time to move on to the next parts.

Now, modifying the code to simulate the game state as if started by mom. Within `SDL_main`, adding:
```assembly
xor eax, eax
or eax, 0x747
or qword ptr ds:[0x00007FF70C2A1AC8], rax
```
Necessary adjustments around `mark` and a few `nop` for alignment.

Great, it works! Time to proceed to the fourth part and resolve the battle with Gary. After multiple battles, noticed he regenerates full HP after defeat. Need to prevent cheating.

In `start_battle`, `update_battle`, `battle_keypress`, and `go_execute` within `strategy_endless_healing`, uses `predict`. To ensure fair play, replace:
```assembly
jne 0x00007FF6F3036A60
```
With:
```assembly
jmp 0x00007FF6F3036A60
```
Ensuring `jmp` to `strategy_status` for an honest duel.

Successfully completed each subpoint.