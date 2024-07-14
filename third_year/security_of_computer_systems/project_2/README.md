# Reverse Engineering Module Assignment

## Task Description

The provided file is a prototype game for the Windows platform that needs to be analyzed and modified to accomplish the following tasks:

[1 point] The game starts with a computer boot attempt. Modify the game so that instead of displaying asterisks when entering a password for the disk, it shows the actual characters of the password. Describe in the report how this modification was achieved.

[4 points] The game verifies the correctness of the entered disk password. Describe the algorithm used for password verification. Based on gathered information, find the correct password.

[2 points] Constantly being forced to restart the game with Mom is somewhat annoying, right? Modify the game so that its initial state is as if you attempted to start the computer with Mom. Describe how the current "phase" of the game is determined.

[3 points] Modify the game to win the battle against Gary. Try not to rely solely on brute force, such as reducing Gary's HP or increasing his attack value, which won't earn full points. Investigate what unfair advantage the opponent has and balance the odds by applying an appropriate modification.

When submitting your solution, include:

- A report detailing the solution process for each task. The report should preferably be in plaintext format (.txt) or Markdown (.md).
- The modified .exe file with all changes implemented. One .exe file with all modifications is sufficient.
- The assignment can be completed using the software used in the RE module labs (mentioned in the introductory email) or their equivalents. Thus, using Ghidra instead of the built-in IDA Free decompiler is acceptable.

Additional tips for solving the assignment:

- Ensure Microsoft Visual C++ Redistributable for Visual Studio 2019 x64 is installed on your Windows environment (installation file: [vc_redist.x64.exe](https://aka.ms/vs/17/release/vc_redist.x64.exe)).
- The application was tested on Windows 10 and Windows 11. In case of startup issues, please email the OS version and error message (screenshots are also acceptable).
- Unzip all files from the .zip archive before running the application and beginning the analysis.
- The task includes a .pdb file (symbols). Make sure they are available to you during analysis (you can see function names and clicked Yes in the IDA window "Do you want to look for this file at the specified path and the Microsoft Symbol Server?").
- SDL libraries contain TLS callbacks that x64dbg sets breakpoints on. In Options -> Preferences, leave only "Entry Breakpoint" enabled and disable "System Breakpoint" and "TLS Callbacks".
- You can apply patches in x64dbg via File -> Patch file (CTRL+P). Instructions can be changed using the built-in assembler (spacebar) or previously assembled in NASM and added binary-wise via PPM -> Edit -> Binary Edit (CTRL+E). Remember, not the entire section of the file is reflected (file alignment is 0x200, and pages are 0x1000). If not all patches were applied and you receive a message like 8/14 patches applied (instead of 14/14), it means some patches exceeded the section's available range in the file.
- Addresses shown in IDA have a different base address than in x64dbg. However, offsets relative to the beginning of the mapped file are the same. Use this when searching for instructions in IDA and x64dbg. The "Go to" window supports RVA, so to find 0x140005D47 in x64dbg, you can simply input :$0x5D47.
