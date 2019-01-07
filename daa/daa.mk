#!/bin/bash

# build script

set -v

if [ "$1" == 'clean' ]
then
    rm -f *.o
    rm -f daa_test
    rm -f daa_test.results
    rm -f libdaa.*
    exit
fi

# compiler
CC=clang
#CC="clang++"
#CC="gcc"
#CC="g++"

# debug/optimize
O=g
#O=O

# add current directory to library path for shared libraries
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# compile
$CC -$O -c -fpic -ansi -pedantic -Wall -o daa.o daa.cpp

# build static library
ar rcv libdaa.a daa.o
ar ts libdaa.a

# build dynamic library and set up links
$CC -shared -Wl,-soname,libdaa.so.1 -o libdaa.so.1.0.1 daa.o -lc
ln -sf libdaa.so.1.0.1 libdaa.so.1
ln -sf libdaa.so.1.0.1 libdaa.so

# compile test program and link with shared library
$CC -$O -ansi -pedantic -Wall -o daa_test daa_test.cpp -L. -ldaa -lm

# run test program
./daa_test 2> daa_test.results

