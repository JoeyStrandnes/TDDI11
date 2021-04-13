#!/bin/bash

clear

gcc main.c -std=c99

> outputLog.txt

./a.out > ./displayLog.txt

cmp ./outputLog.txt ./outputLog_Correct.txt 