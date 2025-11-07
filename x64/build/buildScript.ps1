# make sure errors stop
$ErrorActionPreference = "Stop"

$root = Resolve-Path ../

nasm "$root/boot/MBR.asm" -o "MBR.bin"
(Get-Item "MBR.bin").Length
nasm "$root/boot/Bootloader.asm" -o "$root/build/Bootloader.bin"

# read both binaries as byte arrays (no text conversion!)
[byte[]]$boot  = [System.IO.File]::ReadAllBytes("x64/build/MBR.bin")
[byte[]]$stage = [System.IO.File]::ReadAllBytes("x64/build/Bootloader.bin")

# combine into one array
[byte[]]$combined = New-Object byte[] ($boot.Length + $stage.Length)
[Array]::Copy($boot, 0, $combined, 0, $boot.Length)
[Array]::Copy($stage, 0, $combined, $boot.Length, $stage.Length)

# create or resize HDD image (10 MB example)
$size = 10MB
$fs = [System.IO.File]::Open("x64/build/hdd.img", [System.IO.FileMode]::Create)
$fs.SetLength($size)
$fs.Write($combined, 0, $combined.Length)
$fs.Close()
$hdd = [System.IO.File]::ReadAllBytes("x64/build/hdd.img")
$hdd[0x1BE] = 0x80
[System.IO.File]::WriteAllBytes("x64/build/hdd.img", $hdd)
Copy-Item "$root/build/hdd.img" -Destination "$root/testing/hdd.img"

Write-Host "Created 10MB hdd.img with bootloader + stage2"
