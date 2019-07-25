```

Library:
    daa.hpp

Dynamic Array Allocator - C++ header file only library

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

    - no library dependencies(except standard header files)

    - initialization pointer argument set to instance of initialized type or NULL for no initialization

    - implemented by a recursive routine(ptr_init()) which calls two other different recursive
      routines(off() and doff())

    - see article in Embedded Systems Programming, Dec. 2000, "Flexible Dynamic Array Allocation"(included)

    - see article in The C Users Journal, Nov. 1990. "A Flexible Dynamic Array Allocator"(included)

    - 18 verification tests that show code usage examples, see daa_test.cpp

    - see cut/paste Examples section and API(das(), daa()) section below

    - a daa-compile.tar file is provided with a non-header compiled(clang, clang++, gcc, g++) version

    - for an excellent reference on this type of array(ptr to ptr to ...) access see Numerical Recipes
      in C, Press, Flannery, Teukolsky, and Vettering, Cambridge University Press, 1992, pg. 20-23.
      Ch. 1 Preliminaries -> 1.2 Some C Conventions for Scientific Computing -> Matrices and
      Two-Dimensional Arrays.  From the above reference read:
          'Here is our bottom line: We avoid the fixed-size two-dimensional arrays of C as
           being unsuitable data structures for representing matrices in scientific computing. We
           adopt instead the convention “pointer to array of pointers,” with the array elements
           pointing to the first element in the rows of each matrix.'

    - this file is the entire documentation needed to use the library

    - Author: Richard Hogaboom, richard.hogaboom@gmail.com

Usage:
    - das() - find size necessary
      malloc()(or whatever memory allocation routine) - allocate memory
      daa() - populate allocated memory

    - allocate auto stack or static area storage e.g. unsigned char mem[SIZE]
      das() - determine if array will fit in mem[]
      daa() - populate allocated memory

Files:

    daa-compile.tar - non-header compiled(clang, clang++, gcc, g++) version
    daa.doc         - documentation for library
    daa.hpp         - header only C++ implementation
    daa.mk          - build script, compiles/executes test code
    daa_test.cpp    - test code

Examples:

    An excerpt of TESTs 1, 5 and 17 from daa_test.cpp
        TEST 1  - demonstrate passing slices of arrays to subroutines and
                  visibility of changes at any level throughout
        TEST 5  - demonstrate 10 dimensional array with different starting
                  subscripts for each dimension
        TEST 17 - demonstrate single dimensional array with elements that
                  are 4 megabyte strings

    #include <cstdio>
    #include <cstring>

    #include "daa.hpp"

    /*
     * test code for das()/daa().  all the tests are completely independent,
     * that is, any test may be eliminated or moved elsewhere and run
     * independently.  cut/paste tests into your code and modify as needed.
     */

    /*
     * used in TESTs 1, 2, 3.  initializes four dimensional array[][][][] to integer
     * values which are the sum of the dimension subscripts.
     */

        void
    init4darray(
        unsigned int d[4],
        int st[4],
        double ****array)
    {
        int i, j, k, l;


        for (i=st[0] ; i<st[0]+int(d[0]) ; i++)
        {
            for (j=st[1] ; j<st[1]+int(d[1]) ; j++)
            {
                for (k=st[2] ; k<st[2]+int(d[2]) ; k++)
                {
                    for (l=st[3] ; l<st[3]+int(d[3]) ; l++)
                    {
                        array[i][j][k][l] = i + j + k + l;
                    }
                }
            }
        }
    }

    /*
     * used in TEST 1 to demonstrate passing and modification of slices of arrays in subroutines
     */

    void f2(
        double *array)
    {
        array[0] = 2.72;
        array[1] = 2.72;
    }

    /*
     * used in TEST 1 to demonstrate passing and modification of slices of arrays in subroutines
     */

    void f1(
        double ***array)
    {
        array[-5][10][0] = 3.14;

        f2(array[-1][13]);
    }

       int
    main()
    {
        using namespace boost;

        /*
         * TEST 1
         */
        {
            int err_code = 0;
            int asize = 0;
            char *mem_ptr;

            unsigned int d[4] = {3, 5, 4, 2}; /* dimensions */
            int st[4] = {-1, -5, 10, 0}; /* starting subscripts */
            double ****array = NULL; /* array pointer */

            fprintf(stderr, "\nTEST 1");
            fprintf(stderr, "\n    4 dimensional array of double");
            fprintf(stderr, "\n        dimensions: 3, 5, 4, 2");
            fprintf(stderr, "\n        starting subscripts: -1, -5, 10, 0");
            fprintf(stderr, "\n        init: NULL\n");
            fprintf(stderr, "\n    initializes four dimensional array[][][][] to integer values");
            fprintf(stderr, "\n    which are the sum of the dimension subscripts.");
            fprintf(stderr, "\n    descend into two subroutines(f1() -> f2()) and set values there:");
            fprintf(stderr, "\n        set array[-1][-5][10][0] = 3.14 in f1();");
            fprintf(stderr, "\n        set array[-1][-1][13][0] = 2.72 in f2();");
            fprintf(stderr, "\n        set array[-1][-1][13][1] = 2.72 in f2();\n\n");

            asize = das(sizeof(double), 4, d, &err_code);

            mem_ptr = (char *)malloc(asize);

            array = (double ****) daa(sizeof(double), 4, d, st, &err_code, mem_ptr, NULL);

            if (array == NULL)
            {
                fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                    , daa_errs[err_code]);
            }
            else
            {
                init4darray(d, st, array);

                f1(array[-1]);

                fprintf(stderr, "sizeof(double) = %ld\n", sizeof(double));
                fprintf(stderr, "array size = %d\n\n", asize);
                for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
                {
                    for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                    {
                        for (int k=st[2] ; k<st[2]+int(d[2]) ; k++)
                        {
                            for (int l=st[3] ; l<st[3]+int(d[3]) ; l++)
                            {
                                fprintf(stderr, "array[%2d][%2d][%2d][%2d] = %5.2f\n"
                                    , i, j, k, l, array[i][j][k][l]);
                            }
                        }
                    }
                }
                fprintf(stderr, "err_code = %d\n\n", err_code);
                free(mem_ptr);
            }
        }

        /*
         *  TEST 5
         */
        {
            int err_code = 0;
            int asize = 0;
            char *mem_ptr;

            unsigned int d[10] = {3, 3, 2, 4, 5, 4, 4, 4, 4, 4}; /* dimensions */
            int st[10] = {-4, -3, -2, -1, 0, 1, 2, 3, 4, 5}; /* starting subscripts */
            double data;
            double init = 0;
            double **********array = NULL; /* array pointer */

            fprintf(stderr, "\nTEST 5");
            fprintf(stderr, "\n    10 dimensional double array");
            fprintf(stderr, "\n        dimensions: 3, 3, 2, 4, 5, 4, 4, 4, 4, 4");
            fprintf(stderr, "\n        starting subscripts: -4, -3, -2, -1, 0, 1, 2, 3, 4, 5");
            fprintf(stderr, "\n        init: 0\n");
            fprintf(stderr, "\n    this is by far the largest allocation test.  only the first and");
            fprintf(stderr, "\n    last array elements are printed out.  they should be 1.0 and");
            fprintf(stderr, "\n    368640.0 respectively.  the last number is obtained by taking");
            fprintf(stderr, "\n    the product of all the dimensions(d[])\n\n");

            asize = das(sizeof(double), 10, d, &err_code);

            mem_ptr = (char *)malloc(asize);

            array = (double **********) daa(sizeof(double), 10, d, st, &err_code, mem_ptr, (char *)&init);

            if (array == NULL)
            {
                fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                    ,daa_errs[err_code]);
            }
            else
            {
                data = 0.;
                for (int i0=st[0] ; i0<st[0]+int(d[0]) ; i0++)
                {
                   for (int i1=st[1] ; i1<st[1]+int(d[1]) ; i1++)
                   {
                      for (int i2=st[2] ; i2<st[2]+int(d[2]) ; i2++)
                      {
                         for (int i3=st[3] ; i3<st[3]+int(d[3]) ; i3++)
                         {
                            for (int i4=st[4] ; i4<st[4]+int(d[4]) ; i4++)
                            {
                               for (int i5=st[5] ; i5<st[5]+int(d[5]) ; i5++)
                               {
                                  for (int i6=st[6] ; i6<st[6]+int(d[6]) ; i6++)
                                  {
                                     for (int i7=st[7] ; i7<st[7]+int(d[7]) ; i7++)
                                     {
                                         for (int i8=st[8] ; i8<st[8]+int(d[8]) ; i8++)
                                         {
                                            for (int i9=st[9] ; i9<st[9]+int(d[9]) ; i9++)
                                            {
                                                array[i0][i1][i2][i3][i4]
                                                     [i5][i6][i7][i8][i9] = ++data;
                                            }
                                         }
                                     }
                                  }
                               }
                            }
                         }
                      }
                   }
                }

                fprintf(stderr, "sizeof(double) = %ld\n", sizeof(double));
                fprintf(stderr, "array size = %d\n\n", asize);
                fprintf(stderr, "array[-4][-3][-2][-1][0][1][2][3][4][5] = %e\n"
                    , array[-4][-3][-2][-1][0][1][2][3][4][5]);
                fprintf(stderr, "array[-2][-1][-1][2][4][4][5][6][7][8] = %e\n"
                    , array[-2][-1][-1][2][4][4][5][6][7][8]);
                fprintf(stderr, "err_code = %d\n\n", err_code);
                free(mem_ptr);
            }
        }

        /*
         * TEST 17
         */
        {
            int err_code = 0;
            int asize = 0;
            char *mem_ptr;

            unsigned int d[1] = {3}; /* dimensions */
            int st[1] = {0}; /* starting subscripts */

            const int FOUR_MEGS = 4*1024*1024;

            /* four megs of unsigned char */
            typedef struct
            {
                unsigned char s[FOUR_MEGS];
            } STRING_FOUR_MEGS;

            STRING_FOUR_MEGS *array = NULL; /* array pointer */

            fprintf(stderr, "\nTEST 17");
            fprintf(stderr, "\n    1 dimensional four meg char string array");
            fprintf(stderr, "\n        dimensions: 3");
            fprintf(stderr, "\n        starting subscripts: 0");
            fprintf(stderr, "\n        init: NULL\n");
            fprintf(stderr, "\n    set array[0] = \"AXXXXXXXXX\"");
            fprintf(stderr, "\n    set array[1] = \"XXXXBXXXXX\"");
            fprintf(stderr, "\n    set array[2] = \"XXXXXXXXXC\"\n\n");

            asize = das(sizeof(STRING_FOUR_MEGS), 1, d, &err_code);

            mem_ptr = (char *)malloc(asize);

            array = (STRING_FOUR_MEGS *) daa(sizeof(STRING_FOUR_MEGS), 1, d, st, &err_code, mem_ptr, (char *)NULL);

            if (array == NULL)
            {
                fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                    , daa_errs[err_code]);
            }
            else
            {
                memset((unsigned char *)&array[0], 0, FOUR_MEGS);
                memset((unsigned char *)&array[1], 1, FOUR_MEGS);
                memset((unsigned char *)&array[2], 2, FOUR_MEGS);

                strcpy((char *)&array[0], "AXXXXXXXXX");
                strcpy((char *)&array[1], "XXXXBXXXXX");
                strcpy((char *)&array[2], "XXXXXXXXXC");

                fprintf(stderr, "sizeof(STRING_FOUR_MEGS) = %ld\n", sizeof(STRING_FOUR_MEGS));
                fprintf(stderr, "array size = %d\n\n", asize);
                for (int i=st[0]; i<st[0]+int(d[0]) ; i++)
                {
                    fprintf(stderr, "array[%2d] = %s\n", i, (unsigned char *)&array[i]);
                }
                fprintf(stderr, "err_code = %d\n\n", err_code);
                free(mem_ptr);
            }
        }
    }

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
 *     will return a size with the sizeof() function.  it is also very
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
 *     zero based or minus one based or any based for that matter.  when the
 *     array is to be deallocated, the pointer returned by malloc()(or whatever)
 *     is passed to free().
 *
 * Examples:
 *     see daa_test.cpp
 *
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
 *        total size of the data area, need to be aligned on a sizeof(char *)
 *        boundary.  the beginning of the  pointer area is tested for
 *        alignment, and its alignment adjusted if necessary.
 *
 *     3. for an excellent reference on this type of array access see Numerical
 *        Recipes in C, Press, Flannery, Teukolsky, and Vettering, Cambridge
 *        University Press, 1992, pg. 20.
 *
 * Author:
 *    Richard Hogaboom
 *    richard.hogaboom@gmail.com
 *
 *==================================================================================================
 */

/*
 * das:
 *     dynamic array size.  this routine takes four of the same arguments
 *     that daa() takes and calculates the total space allocation in bytes
 *     required to store the array.  normally used in conjunction with
 *     daa() to do a das()/malloc()/daa() sequence.
 *
 * Arguments:
 *     unsigned int data_size
 *        size of the basic array data object.  this will usually be
 *        obtained from the sizeof() function.
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
 *        obtained from the sizeof() function.
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
 *     index and the dimensional extent will of course result in run time errors.
 *     this is not an array allocation error but an array usage error, similar
 *     to any array access on a static C zero based array outside the normal
 *     0...n-1 bounds.
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

```
