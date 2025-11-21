# Minux
## prerequisites:
- for running
   - qemu
- for testing
   - bochs or qemu with gdb


## Build Project

Go to the build directory x32/build\
There run.
````commandline
    ./buildScript.ps1
````
## Run
Still in build directory run.
```commandline
qemu-system-i386.exe -drive file=hdd.img,format=raw,if=ide -boot c
```
## Testing
Go to x32/testing\


