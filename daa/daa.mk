#!/bin/bash

# build script

set -v

if [ "$1" == 'clean' ]
then
    rm -f daa_test
    rm -f daa_test.results
    exit
fi

# compiler
#CC="clang++"
CC="g++"

# debug/optimize
O=g
#O=O

# compile test program
$CC -$O -ansi -pedantic -Wall -o daa_test daa_test.cpp

# run test program
./daa_test 2> daa_test.results

