## Build Project

````commandline
    ./buildScript.ps1
````
## Run

```commandline
qemu-system-i386.exe -drive file=hdd.img,format=raw,if=ide -boot c
```
