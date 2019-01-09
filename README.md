```

daa
===

Dynamic Array Allocator - C++
  
  - ptr to ptr to ... style arbitrary N dimensional array allocator of anything you can take sizeof()

  - arbitrary starting subscript(0, 1, -10, ...) for each separate dimension - Note: for each array
    dimension the starting subscript and dimension size determine the valid subscripts for that
    dimension - any index outside that range is invalid and will give unpredictable results

  - allocates anything of any type(double, int, long, struct, ...)

  - one contiguous block allocation of memory for all ptr's and data(locality of
    reference and easy free())

  - array or slices of array can be passed to subroutines to any depth, modified there,
    and be seen at all levels

  - simple library, two routines(das()(calculate space needed) and daa()(allocate ptr to ptr ... array))

  - initialization pointer argument set to instance of initialized type or NULL for no initialization

  - implemented by a recursive routine(ptr_init()) which calls two other different recursive
    routines(off() and doff())

  - see article in Embedded Systems Programming, Dec. 2000, "Flexible Dynamic Array Allocation"(included)

  - see article in The C Users Journal, Nov. 1990. "A Flexible Dynamic Array Allocator"(included)

  - 16 verification tests that show code usage examples, see daa/daa_test.cpp

  - see cut/paste Examples section and API(das(), daa()) section below

  - a daa-compile.tar file is provided with a non-header compiled(clang, clang++, gcc, g++) version

  - for an excellent reference on this type of array(ptr to ptr to ...) access see Numerical Recipes
    in C, Press, Flannery, Teukolsky, and Vettering, Cambridge University Press, 1992, pg. 20.

  - this file is the entire documentation needed to use the library

  - Author: Richard Hogaboom, richard.hogaboom@gmail.com

Usage:
    das()/malloc()(or whatever memory allocation routine)/daa() - find size necessary(das()), then allocate
    memory, then populate allocated memory(daa())

Files:

    daa-compile.tar - non-header compiled(clang, clang++, gcc, g++) version
    daa.hpp         - header only C++ implementation
    daa.mk          - build script, compiles/executes test code
    daa_test.cpp    - test code
    README          - documentation for library

Examples:

    All examples are from the test code in daa/daa_test.cpp.

    From Test 1
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[4] = {3, 5, 4, 2}; /* dimension */
        int st[4] = {-1, -5, 10, 0}; /* start subscript */
        double ****array = NULL; /* array pointer */

        asize = das(sizeof(double), 4, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (double ****) daa(sizeof(double), 4, d, st, &err_code, mem_ptr, NULL);

        array[1][1][1][1] = 2;

        fprintf(stderr, "array[1][1][1][1] = %5.1f\n" array[1][1][1][1]);

        free(mem_ptr);

    From Test 3
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[1] = {10}; /* dimension */
        int st[1] = {1}; /* start subscript */
        double init = 10.10;
        double *array = NULL; /* array pointer */

        asize = das(sizeof(double), 1, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (double *) daa(sizeof(double), 1, d, st, &err_code, mem_ptr, (char *)&init);

        array[10] = 4.5;

        fprintf(stderr, "array size = %d\n", asize);

        fprintf(stderr, "array[10] = %e\n", array[10]);

        fprintf(stderr, "err_code = %d\n", err_code);

        free(mem_ptr);

    From Test 4
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[10] = {3, 3, 2, 4, 5, 4, 4, 4, 4, 4}; /* dimension */
        int st[10] = {-4, -3, -2, -1, 0, 1, 2, 3, 4, 5}; /* start subscript */
        double init = 0;
        double **********array = NULL; /* array pointer */

        asize = das(sizeof(double), 10, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (double **********) daa(sizeof(double), 10, d, st, &err_code, mem_ptr, (char *)&init);

        array[-4][-3][-2][-1][0][1][2][3][4][5] = 1.5e2;

        fprintf(stderr, "array size = %d\n", asize);

        fprintf(stderr, "array[-4][-3][-2][-1][0][1][2][3][4][5] = %e\n" , array[-4][-3][-2][-1][0][1][2][3][4][5]);

        fprintf(stderr, "array[-4][-3][-2][-1][0][1][2][3][4][6] = %e\n" , array[-4][-3][-2][-1][0][1][2][3][4][6]);

        fprintf(stderr, "err_code = %d\n", err_code);

        free(mem_ptr);

    From Test 5
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[1] = {10}; /* dimension */
        int st[1] = {1}; /* start subscript */
        struct s {
            double d;
            int l;
        } *array = NULL, s_init = {1.25, 5}; /* array pointer */

        asize = das(sizeof(struct s), 1, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (struct s *) daa(sizeof(struct s), 1, d, st, &err_code, mem_ptr, (char *)&s_init);

        array[1].l= 10;
        array[1].d= .5;
        array[10].l = 4;
        array[10].d = 4.5;

        fprintf(stderr, "sizeof(struct s) = %ld\n", sizeof(struct s));

        fprintf(stderr, "array size = %d\n", asize);

        fprintf(stderr, "array[1].d = %f\n", array[1].d);
        fprintf(stderr, "array[10].l = %2d\n", array[10].l);

        fprintf(stderr, "err_code = %d\n", err_code);

        free(mem_ptr);

    From Test 13
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[3] = {2, 5, 6}; /* dimension */
        int st[3] = {0, 0, 0}; /* start subscript */
        enum e {a,b,c} ***array = NULL, e_init = {c}; /* array pointer */

        asize = das(sizeof(enum e), 3, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (enum e ***) daa(sizeof(enum e), 3, d, st, &err_code, mem_ptr, (char *)&e_init);

        fprintf(stderr, "sizeof(enum e) = %ld\n",sizeof(enum e));

        fprintf(stderr, "array size = %d\n", asize);

        fprintf(stderr, "array[1][4][4] = %d\n", array[1][4][4]);

        array[1][4][5] = a;

        fprintf(stderr, "array[1][4][5] = %d\n", array[1][4][5]);

        array[1][4][5] = b;

        fprintf(stderr, "array[1][4][5] = %d\n", array[1][4][5]);

        fprintf(stderr, "err_code = %d\n", err_code);

        free(mem_ptr);

    From Test 16
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[1] = {3}; /* dimension */
        int st[1] = {0}; /* start subscript */

        #define FOUR_MEGS (4*1024*1024)

        /* four megs of unsigned char */
            typedef struct
            {
                unsigned char s[FOUR_MEGS];
            } STRING_FOUR_MEGS;

        STRING_FOUR_MEGS *array = NULL; /* array pointer */

        asize = das(sizeof(STRING_FOUR_MEGS), 1, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (STRING_FOUR_MEGS *) daa(sizeof(STRING_FOUR_MEGS), 1, d, st, &err_code, mem_ptr, (char *)NULL);

        memset((unsigned char *)&array[0], 0, FOUR_MEGS);
        memset((unsigned char *)&array[1], 1, FOUR_MEGS);
        memset((unsigned char *)&array[2], 2, FOUR_MEGS);

        strcpy((char *)&array[0], "AXXXXXXXXX");
        strcpy((char *)&array[1], "XXXXBXXXXX");
        strcpy((char *)&array[2], "XXXXXXXXXC");

        fprintf(stderr, "sizeof(STRING_FOUR_MEGS) = %ld\n" , sizeof(STRING_FOUR_MEGS));

        fprintf(stderr, "array size = %d\n", asize);

        for (int i=st[0]; i<st[0]+int(d[0]) ; i++)
        {
            fprintf(stderr, "array[%2d] = %s\n", i, (unsigned char *)&array[i]);
        }

        fprintf(stderr, "err_code = %d\n", err_code);

        free(mem_ptr);

API:

/*
 *==================================================================================================
 * File: daa.hpp
 *
 *     Based on Dec. 2000 Embedded Systems Design article "Flexible Dynamic
 *     Array Allocation" by Richard A Hogaboom
 *
 * Description:
 *     this dynamic array allocator - das()/daa() - is designed to be efficient
 *     and flexible.  daa() takes as argument a pointer to the space that the
 *     caller allocates.  the size of this space is determined by a previous
 *     call to das().  the usual sequence would be das()/malloc()/daa().  
 *     it can allocate arrays of up to MAX_DIM dimensions of any type that
 *     will return a size with the sizeof() C operator.  it is also very
 *     efficient from the point of view of array access.  arrays of structure,
 *     enum or union type can be allocated.  a corresponding free of the
 *     allocated space must normally be done (unless you want to allocate to
 *     program termination).  do not free on the function return pointer since
 *     array storage begins with the data and is followed by ptrs to ptrs to
 *     ..., and the first element(lowest subscript of each dimension), does
 *     not point to the beginning of allocated space.  the array is initialized
 *     to the value pointed to by the last parameter, or not initialized if
 *     NULL.  the last parameter init pointer should point to something with a
 *     size the same as the size of the type in the sizeof() first argument.
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
 *        the data comes first, and the assumption here is that whatever
 *        allocation routine is used between das() and daa() will align at
 *        the most stringent boundary, thus accommodating the data area
 *        alignment.  the pointer area comes second and may, depending on the
 *        total size of the data area need to be aligned on a sizeof(char *)
 *        boundary.  the beginning of the  pointer area is tested for
 *        alignment, and its alignment adjusted if necessary.
 *
 *     3. for an excellent reference on this type of array access see Numerical
 *        Recipes in C, Press, Flannery, Teukolsky, and Vettering, Cambridge
 *        University Press, 1992, pg. 20.
 */

/*
 * das:
 *     dynamic array size.  this routine takes four of the same arguments
 *     that daa() takes and calculates the total heap allocation in bytes
 *     required to store the array.  normally used in conjunction with
 *     daa() to do das()/malloc()/daa() sequence.
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
 *     size in bytes of dynamic array that daa() will use.
 */

    inline int
das(
    unsigned int data_size,
    unsigned int num_dim,
    unsigned int *dim,
    int *err_code)

/*
 * daa:
 *     dynamic array allocator
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
 *
 *     failure to index the subscripts in the manner established by the starting
 *     index and the dimensional extent arrays will of course result in run time
 *     errors.  this is not an array allocation error but an array usage error,
 *     similar to any array access on a static C zero based array outside the
 *     normal 0...n-1 bounds.
 */

    inline void *
daa(
    unsigned int data_size,
    unsigned int num_dim,
    unsigned int *dim,
    int *st,
    int *err_code,
    char *base_ptr,
    char *init_ptr)
}

```
