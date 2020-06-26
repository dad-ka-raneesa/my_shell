#! /bin/bash

executable=$1

mkdir -p outputs
rm -rf *.o
gcc -c *.c
gcc -o outputs/$1 *.o && outputs/$1
rm -rf *.o