#!/bin/bash

clear

make clean

make

export MTOOLSRC=./mtools.conf

mcopy embedded.bin a: -o

qemu-system-i386 -drive format=raw,file=floppy.img,if=floppy &