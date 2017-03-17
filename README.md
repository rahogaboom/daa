
```

daa
===

Dynamic Array Allocator - C/C++
  - ptr to ptr to ... style arbitrary N dimensional, implemented by a recursive routine(ptr_init()) which calls
    two other different recursive routines(off() and doff())
  - arbitrary starting subscript(0, 1, -10, ...) for each separate dimension
  - allocates anything of any type(double, int, long, struct, ...) that you can take sizeof() on
  - one contiguous block allocation of memory for all ptr's and data(locality of reference and easy free())
  - array or slices of array can be passed to subroutines to any depth, modifed there, and be seen at all levels
  - initialization pointer argument set to instance of initialized type or NULL for no initialization
  - see article in Embedded Systems Programming, Dec. 2000, "Flexible Dynamic Array Allocation"(included)
  - see article in The C Users Journal, Nov. 1990. "A Flexible Dynamic Array Allocator"(included)
  - 16 verification tests that show complex code usage examples, for C see daac_test.c and for C++ see daacpp_test.cpp


Files:

    daa.h - declaration header file for both daa.c and daa.cpp

    c directory:
        daac.mk         - build script, builds static and dynamic libraries.
                            builds and executes test program.
        daa.c           - ANSI C implementation
        daac_test.c     - daa.c test program

    cpp directory:
        daacpp.mk       - build script, builds static and dynamic libraries.
                            builds and executes test program.
        daa.cpp         - ANSI C++ implementation
        daacpp_test.cpp - daa.cpp test program


API:

/*
 *------------------------------------------------------------------------------
 * das:
 *     dynamic array size.  this routine takes four of the same arguments
 *     that daa() and daav() take and calculates the total heap allocation
 *     in bytes required to store the array.  normally used in conjunction
 *     with daa() to do das()/valloc()/daa() sequence.
 *
 * Arguments:
 *     unsigned int data_size
 *        size of the basic array data object.  this will usually be
 *        obtained from the sizeof() operator.
 *
 *     unsigned int num_dim
 *        number of array dimensions.
 *
 *     unsigned int *dim
 *        a single dimensional int array of the dimensions of the array to
 *        be allocated.
 *
 *     int *err_code
 *        index to returned error code string in daa_errs[].
 *
 * Returns:
 *     size in bytes of dynamic array that daa() and daav() will use.
 *------------------------------------------------------------------------------
 */

    int
das(
    unsigned int data_size,
    unsigned int num_dim,
    unsigned int *dim,
    int *err_code)


/*
 *------------------------------------------------------------------------------
 * daa:
 *     dynamic array allocation with memory allocated externally.
 *
 * Arguments:
 *     unsigned int data_size
 *        size of the basic array data object.  this will usually be
 *        obtained from the sizeof() operator.
 *
 *     unsigned int num_dim
 *        number of array dimensions.
 *
 *     unsigned int *dim
 *        a single dimensional int array of the dimensions of the array to
 *        be allocated.
 *
 *     int *st
 *        a single dimensional int array of integer start subscripts for
 *        each corresponding dimension of the dim array, elements may
 *        be negative.
 *
 *     int *err_code
 *        index to returned error code string in daa_errs[].
 *
 *     char *base_ptr
 *        pointer to the array space.
 *
 *     char *init_ptr
 *        initialization pointer parameter.
 *
 * Returns:
 *     error free operation returns a pointer to void that points to the start
 *     of the dynamically allocated array area and does not set *err_code.  the
 *     returned pointer will need to be cast to the type of the subsequent array
 *     references.
 *
 *     routine failure returns NULL and any of the following
 *     error codes in *err_code:
 *     ERRS_INV_DIMS - invalid number of dimensions - must be > 0 and <= MAX_DIM.
 *     ERRS_INV_REQ_SIZE - invalid request size - must be > 0.
 *     ERRS_INV_DIM - invalid dimension - must be > 0.
 *     ERRS_FAIL_ALLOC - failure to allocate memory. (daav() only)
 *
 *     failure to index the subscripts in the manner established by the starting
 *     index and the dimensional extent arrays will of course result in run time
 *     errors.  this is not an array allocation error but an array usage error,
 *     similar to any array access on a static C zero based array outside the
 *     normal 0...n-1 bounds.
 *------------------------------------------------------------------------------
 */

    void *
daa(
    unsigned int data_size,
    unsigned int num_dim,
    unsigned int *dim,
    int *st,
    int *err_code,
    char *base_ptr,
    char *init_ptr)


/*
 *------------------------------------------------------------------------------
 * daav:
 *     dynamic array allocation with memory allocated by valloc().
 *
 * Arguments:
 *     unsigned int data_size
 *        size of the basic array data object.  this will usually be
 *        obtained from the sizeof() operator.
 *
 *     unsigned int num_dim
 *        number of array dimensions.
 *
 *     unsigned int *dim
 *        a single dimensional int array of the dimensions of the array to
 *        be allocated.
 *
 *     int *st
 *        a single dimensional int array of integer start subscripts for
 *        each corresponding dimension of the dim array, elements may
 *        be negative.
 *
 *     int *err_code
 *        index to returned error code string in daa_errs[].
 *
 *     char char **free_ptr
 *        pointer to the base pointer of the valloc()'ed array
 *        space for the caller to free.
 *
 *     char *init_ptr
 *        initialization pointer parameter.
 *
 * Returns:
 *     error free operation returns a pointer to void that points to the start
 *     of the dynamically allocated array area and does not set *err_code.  the
 *     returned pointer will need to be cast to the type of the subsequent array
 *     references.
 *
 *     routine failure returns NULL and any of the following
 *     error codes in *err_code:
 *     ERRS_INV_DIMS - invalid number of dimensions - must be > 0 and <= MAX_DIM.
 *     ERRS_INV_REQ_SIZE - invalid request size - must be > 0.
 *     ERRS_INV_DIM - invalid dimension - must be > 0.
 *     ERRS_FAIL_ALLOC - failure to allocate memory. (daav() only)
 *
 *     failure to index the subscripts in the manner established by the starting
 *     index and the dimensional extent arrays will of course result in run time
 *     errors.  this is not an array allocation error but an array usage error,
 *     similar to any array access on a static C zero based array outside the
 *     normal 0...n-1 bounds.
 *------------------------------------------------------------------------------
 */

    void *
daav(
    unsigned int data_size,
    unsigned int num_dim,
    unsigned int *dim,
    int *st,
    int *err_code,
    char **free_ptr,
    char *init_ptr)

Common Usage:
    a. das()/valloc()/daa() - find size necessary then allocate memory then populate allocated memory
    b. daav()               - all in one(uses valloc())

```
