#!/bin/bash

# C build script

set -v

# add current directory to library path for shared libraries
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# build static library
rm -rf libdaac.a
gcc -c -fpic -g -ansi -pedantic -Wall -o daac.o daa.c
ar rcv libdaac.a daac.o
ar ts libdaac.a

# build dynamic library and set up links
rm -rf libdaac.so.1.0.1
gcc -shared -Wl,-soname,libdaac.so.1 -o libdaac.so.1.0.1 daac.o -lc
ln -sf libdaac.so.1.0.1 libdaac.so.1
ln -sf libdaac.so.1.0.1 libdaac.so

# compile test program and link with shared library
gcc -g -ansi -pedantic -Wall -o daac_test daac_test.c -L. -ldaac -lm

# run test program
./daac_test 2> daac_test.results

