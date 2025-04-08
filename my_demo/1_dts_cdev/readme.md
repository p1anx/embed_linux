# issues
1. x86 compiles the `blinkApp.c` file to `blink`, then send it to `pi5`. running `blink` will show error. 
2. But sending the `blinkApp.c` file to `pi5`, compiling it on the board, it's no problem.
3. Solution: 
    If you want to compile the `blinkApp.c` in x86 platform, you can run `aarch64-linux-gnu-gcc blinkApp.c -o blinkApp` command, then send it to `arm` platform.