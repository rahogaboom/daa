```

daa
===

Dynamic Array Allocator - C++
  
  - ptr to ptr to ... style arbitrary N dimensional array allocator of anything you can take sizeof()
  - arbitrary starting subscript(0, 1, -10, ...) for each separate dimension
  - allocates anything of any type(double, int, long, struct, ...)
  - one contiguous block allocation of memory for all ptr's and data(locality of
    reference and easy free())
  - array or slices of array can be passed to subroutines to any depth, modifed there,
    and be seen at all levels
  - initialization pointer argument set to instance of initialized type or NULL for no initialization
  - implemented by a recursive routine(ptr_init()) which calls two other different recursive
    routines(off() and doff())
  - see article in Embedded Systems Programming, Dec. 2000, "Flexible Dynamic Array Allocation"(included)
  - see article in The C Users Journal, Nov. 1990. "A Flexible Dynamic Array Allocator"(included)
  - 16 verification tests that show code usage examples, see daa_test.cpp
  - see cut/paste Examples section and API(das(), daa(), daav()) section below
  - a daa-compile.tar file is provided with a non-header compiled(clang, clang++, gcc, g++) version
  - for an excellent reference on this type of array(ptr to ptr to ...) access see Numerical Recipes
    in C, Press, Flannery, Teukolsky, and Vettering, Cambridge University Press, 1992, pg. 20.
  - this file is the entire documentation needed to use the library
  - Author: Richard Hogaboom, richard.hogaboom@gmail.com

Common Usage:
    a. das()/valloc()/daa() - find size necessary then allocate memory then populate allocated memory
    b. daav()               - all in one(uses valloc())

Files:

    daa.hpp - header only C++ implementation
    daa.mk  - build script, compiles/executes test code
    daa_test.cpp - test code
    daa-compile.tar - non-header compiled(clang, clang++, gcc, g++) version

Examples:


API:

/*
 *==================================================================================================
 * File: daa.hpp
 *
 *     Based on Dec. 2000 Embedded Systems Design article "Flexible Dynamic
 *     Array Allocation" by Richard A Hogaboom
 *
 * Description:
 *     these dynamic array allocators - daa() and daav() - are designed
 *     to be efficient and flexible.  daa() takes as argument a pointer
 *     to the space that the caller allocates.  the size of this space is
 *     determined by a previous call to das().  the usual sequence would
 *     be das()/valloc()/daa().  daav() takes as argument a pointer to
 *     pointer that daav() returns the pointer to the valloc()'ed space in.
 *     they can allocate arrays of up to MAX_DIM dimensions of any type that
 *     will return a size with the sizeof() C operator.  they are also very
 *     efficient from the point of view of array access.  for daav() the
 *     single valloc() ensures locality of reference that eliminates excess
 *     paging encountered with dynamic array allocation schemes that use
 *     multiple valloc()'s.  arrays of structure, enum or union type can
 *     be allocated.  a corresponding free of the allocated area must
 *     normally be done (unless you want to allocate to program termination).
 *     for daav() the sixth parameter returns the free pointer; daa() does
 *     not require a free pointer since the caller allocates the space.
 *     do not free on the function return pointer since array storage begins
 *     with the data and is followed by pointers to pointers to ..., and the
 *     first element(lowest subscript of each dimension), does not point to
 *     the beginning of allocated space.  the array is initialized to the
 *     value pointed to by the last parameter, or not initialized if NULL.
 *     the last parameter init pointer should point to something with a size
 *     the same as the size of the type in the sizeof() first argument.
 *     arrays may be allocated to have one or more dimensions with non-zero
 *     integer(+ or -) start subscripts.  thus, arrays may be one based or
 *     zero based or minus one based or any based for that matter.
 *
 * Examples:
 *     see daa_test.cpp
 *
 * Author:
 *    Richard Hogaboom
 *    richard.hogaboom@gmail.com
 *
 *==================================================================================================
 */

/*
 * Notes:
 *     1. the dimensional limit of the allocated arrays is set by MAX_DIM;
 *        increase or decrease this to adjust the maximum array dimension.
 *
 *     2. alignment of the data area is the alignment of the first argument
 *        sizeof(), while the alignment of the pointer area is sizeof(char *).
 *        the data comes first, and the assumption here is that the valloc()
 *        (for daav()) and whatever allocation routine is used between das()
 *        and daa() will align at the most stringent boundary, thus
 *        accommodating the data area alignment.  the pointer area comes
 *        second and may, depending on the total size of the data area need
 *        to be aligned on a sizeof(char *) boundary.  the beginning of the
 *        pointer area is tested for alignment, and its alignment adjusted if
 *        necessary.
 *
 *     3. for an excellent reference on this type of array access see Numerical
 *        Recipes in C, Press, Flannery, Teukolsky, and Vettering, Cambridge
 *        University Press, 1992, pg. 20.
 */

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

```

