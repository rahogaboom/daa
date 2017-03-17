#!/bin/bash

# C++ build script

set -v

if [ "$1" == 'clean' ]
then
    rm -f *.o
    rm -f daacpp_test
    rm -f daacpp_test.results
    rm -f libdaacpp.*
    exit
fi

# pick compiler
#CC="clang++"
CC="g++"

# add current directory to library path for shared libraries
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# build static library
rm -rf libdaacpp.a
$CC -c -fpic -g -ansi -pedantic -Wall -o daacpp.o daa.cpp
ar rcv libdaacpp.a daacpp.o
ar ts libdaacpp.a

# build dynamic library and set up links
rm -rf libdaacpp.so.1.0.1
$CC -shared -Wl,-soname,libdaacpp.so.1 -o libdaacpp.so.1.0.1 daacpp.o -lc
ln -sf libdaacpp.so.1.0.1 libdaacpp.so.1
ln -sf libdaacpp.so.1.0.1 libdaacpp.so

# compile test program and link with shared library
$CC -g -ansi -pedantic -Wall -o daacpp_test daacpp_test.cpp -L. -ldaacpp -lm

# run test program
./daacpp_test 2> daacpp_test.results

