```
 daa-1.0.9.tar.gz - 
    a. checked against compilers:
           g++     - g++ (GCC) 9.1.1 20190503 (Red Hat 9.1.1-1)
           clang++ - clang version 8.0.0 (Fedora 8.0.0-1.fc30)

 daa-1.0.8.tar.gz - 
    a. added one test to demonstrate passing and modification of
       array slices in subroutines and subsequent visibility throught.
    b. updates Examples section in daa.doc to use only three examples,
       copied exactly, from daa_test.cpp.

 daa-1.0.7.tar.gz - 
    a. all changes to code in daa_test.cpp to print more
       informative test output.  this allows reading and verifying
       the test output without referencing the test code.

 daa-1.0.6.tar.gz - 
    a. minor doc changes
    b. directory structure changes
    c. add doc/html/index.html file

 daa-1.0.5.tar.gz - 
    a. minor code and doc changes

 daa-1.0.4.tar.gz - 
    a. minor doc changes in several places.
    b. rename README to daa.doc to better indicate where doc is.
    c. checked against compilers:
           g++     - g++ (GCC) 8.2.1 20181215 (Red Hat 8.2.1-6)
           clang++ - clang version 7.0.0 (Fedora 7.0.0-2.fc29)
    d. added test to store allocated array to stack(as opposed to heap)

 daa-1.0.3.tar.gz - 
    a. converted to daa.hpp header file only implementation.
    b. removed daav() routine.  valloc() is obsolete. the das()/malloc()/daa()
       usage is more flexible for memory allocation.
    c. updated test suite.
    d. updated documentation.  all doc is in README file.
    e. added tar file with C only(compiles with gcc/g++/clang/clang++)
       compiled in routines.

 daa-1.0.2.tar.gz - 
    a. Remove daa.tar.xz file.

 daa-1.0.1.tar.gz - 
    a. Add const modifiers to eliminate warnings
       (because of newer compilers).

 daa-1.0.0.tar.gz - 
    a. init.

```
