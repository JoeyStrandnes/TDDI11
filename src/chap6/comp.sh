#!/bin/bash

clear

make clean all
make -o0

export MTOOLSRC=mtools.conf

mcopy embedded.bin a: -o
cp floppy.img floppy_copy.img
netstat | grep ":50152"
qemu-system-i386 -drive format=raw,file=floppy.img,if=floppy -serial tcp:127.0.0.1:50152,server,nowait &
sleep 1
qemu-system-i386 -drive format=raw,file=floppy_copy.img,if=floppy -serial tcp:127.0.0.1:50152 &