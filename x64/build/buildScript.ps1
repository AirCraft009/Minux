# make sure errors stop
$ErrorActionPreference = "Stop"

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

Write-Host "Created 10MB hdd.img with bootloader + stage2"
