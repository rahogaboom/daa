/*
 *==============================================================================
 * File: daa.cpp
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
 *     see daacpp_test.cpp
 *
 * Author:
 *    Richard Hogaboom
 *
 *==============================================================================
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
 * include files
 *------------------------------------------------------------------------------
 */

#include <malloc.h>

#include "daa.h"

/*
 *------------------------------------------------------------------------------
 * constants
 *------------------------------------------------------------------------------
 */

/* global constants */
const unsigned int MAX_DIM = 256; /* maximum number of array dimensions */

/* error msg indices */
const unsigned int ERRS_INV_DIMS = 0;
const unsigned int ERRS_INV_REQ_SIZE = 1;
const unsigned int ERRS_INV_DIM = 2;
const unsigned int ERRS_FAIL_ALLOC = 3;

/*
 *------------------------------------------------------------------------------
 * data types
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * global variables
 *------------------------------------------------------------------------------
 */

/* error msgs */
const char *daa_errs[] =
{
    "daa: invalid number of dimensions - must be > 0 and <= MAX_DIM.",
    "daa: invalid request size - must be > 0.",
    "daa: invalid dimension - must be > 0.",
    "daa: failure to allocate memory.",
};

/*
 *------------------------------------------------------------------------------
 * local function prototypes
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * functions
 *------------------------------------------------------------------------------
 */



/*
 *------------------------------------------------------------------------------
 * doff:
 *     data offset routine that calculates the offset in basic data
 *     units of the location of the data item specified by the
 *     current settings in dim_ind[].  this multiplied by the size of
 *     a basic data unit plus the offset of the start of the area
 *     gives the address of the data item.  a basic data unit may be
 *     either a pointer(sizeof(char *) bytes long) or array data
 *     item(data_size bytes long).
 *
 * Arguments:
 *     unsigned int level
 *        the array level to initialize pointers for.  the first
 *        dimension is level 0, the second level 1, etc., always
 *        starts at zero.
 *
 *     int *dim_ind
 *        dimension index array of current call, starts zeroed.
 *
 *     int dim_prod
 *        product of dimensions from zero to level.
 *
 *     unsigned int *dp
 *        array of dimension products.
 *
 * Returns:
 *     int offset in basic data units from the start of the data area(base_ptr)
 *     to data item specified by dim_ind[].  byte offset is given by:
 *     doff(level, dim_ind, dim_prod, dp) * data_size.
 *------------------------------------------------------------------------------
 */

    static int
doff(
    unsigned int level,
    int *dim_ind,
    int dim_prod,
    unsigned int *dp)
{
    if (level == 0)
    {
        return 0;
    }
    else
    {
        return doff(level-1, dim_ind, dim_prod, dp) +
            (dim_prod/dp[level-1]) * dim_ind[level-1];
    }
}



/*
 *------------------------------------------------------------------------------
 * off:
 *     offset routine that calculates the offset in pointer units(char *)
 *     of the start of the pointer array for that level.  each level has a
 *     pointer array associated with that level that points to the next level
 *     and is pointed to by the previous level.
 *
 * Arguments:
 *     unsigned int level
 *        the array level to initialize pointers for.  the first dimension is
 *        level 0, the second level 1, etc., always starts at zero.
 *
 *     unsigned int *dp
 *        array of dimension products.
 *
 * Returns:
 *     int offset in pointer units(char *) of the pointer array for that level.
 *     byte offset is given by off(level, dp) * sizeof(char *).
 *------------------------------------------------------------------------------
 */

    static int
off(
    unsigned int level,
    unsigned int *dp)
{
    if (level == 0)
    {
        return 0;
    }
    else
    {
        return dp[level-1] + off(level-1, dp);
    }
}



/*
 *------------------------------------------------------------------------------
 * ptr_init:
 *     this routine uses the space pointed to by the "base_ptr" pointer.
 *     it constructs the several levels of pointers needed to reference
 *     elements of a multidimensional array.
 *
 * Arguments:
 *     unsigned int level
 *        the array level to initialize pointers for.  the first dimension
 *        is level 0, the second level 1, etc., always starts at zero.
 *
 *     int *dim_ind
 *        dimension index array of current call, starts zeroed.
 *
 *     unsigned int data_size
 *        size of the basic array data object.  this will usually be
 *        obtained from the sizeof() operator.
 *
 *     unsigned int num_dim
 *        number of array dimensions.
 *
 *     char *data_ptr
 *        points to base of allocated array.
 *
 *     char *ptr_ptr
 *        points to base of pointers to pointers to ... to data
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
 *     unsigned int *dp
 *        array of dimension products.
 *
 * Returns:
 *     pointer to next level of indirection.
 *------------------------------------------------------------------------------
 */

    static char *
ptr_init(
    unsigned int level,
    int *dim_ind,
    unsigned int data_size,
    unsigned int num_dim,
    char *data_ptr,
    char *ptr_ptr,
    unsigned int *dim,
    int *st,
    unsigned int *dp)
{
    char		**ptrs;
    unsigned long	i;



    /*
     * if level is not final level get pointer to array of pointers
     * for that level and recursively call ptr_init() to return pointers to
     * fill the array of pointers.  if level is final level calculate
     * position of data and return pointer to level-1.
     */

    if (level < (num_dim - 1))
    {
        /*
         * ptrs points to the start of a subarray of pointers to the
         * next dimension level of pointers or data
         */

        ptrs = (char **) (ptr_ptr + off(level, dp) * sizeof(char *) +
            ((level == 0)?0:doff(level, dim_ind,
            dp[level], dp) * sizeof(char *)));

        /* fill the array of pointers that ptrs points to */
        for (i=0 ; i<dim[level] ; i++)
        {
            dim_ind[level] = i;

            /*
             * the recursive call to ptr_init() returns a pointer to the
             * next level and is then adjusted by st[] for a
             * non-zero based subscript
             */

            ptrs[i] = ptr_init(level+1, dim_ind, data_size, num_dim, data_ptr,
                ptr_ptr, dim, st, dp) - st[level+1] *
                ((level+1 == num_dim-1)?data_size:sizeof(char *));
        }

        /*
         * adjust zeroth level pointer that is passed back to root
         * of recursive tree call
         */

        if (level == 0)
        {
            ptrs -= st[0];
        }
    }
    else
    {
        /*
         * calculate pointers to data subarrays.  if one dimensional
         * array and non-zero subscripting adjust returned pointer
         */

        ptrs = (char **) (data_ptr + doff(level, dim_ind, dp[level], dp) *
            data_size - ((level == 0)?(st[0] * data_size):0));
    }

    return (char *) ptrs;
}



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
{
    unsigned int i;

    /* product of dimensions from 0 to index, dim[0]*dim[1]...dim[index] */
    unsigned int dp[MAX_DIM];



    if (num_dim < 1 || num_dim > MAX_DIM)
    {
        *err_code = ERRS_INV_DIMS;
        return -1;
    }

    if (data_size < 1)
    {
        *err_code = ERRS_INV_REQ_SIZE;
        return -1;
    }

    dp[0] = dim[0];
    for (i=0 ; i<num_dim ; i++)
    {
        if (dim[i] <= 0)
        {
            *err_code = ERRS_INV_DIM;
            return -1;
        }

        if (i > 0)
        {
            dp[i] = dp[i-1] * dim[i];
        }
    }

    return(dp[num_dim-1] * data_size + off(num_dim-1, dp) * sizeof(char *) +
        sizeof(char *));
}



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
{
    unsigned int		i, j;

    /* array of current dimension indices */
    int		dim_ind[MAX_DIM];

    char	*p_data, /* pointer to array data */
        	*p;      /* tmp pointer */

    /* product of dimensions from 0 to index, dim[0]*dim[1]...dim[index] */
    unsigned int		dp[MAX_DIM];

    /* points to base of pointers to pointers to ... to data */
    char	*ptr_ptr;



    if (num_dim < 1 || num_dim > MAX_DIM)
    {
        *err_code = ERRS_INV_DIMS;
        return NULL;
    }

    if (data_size < 1)
    {
        *err_code = ERRS_INV_REQ_SIZE;
        return NULL;
    }

    /*
     * set dim_ind to all zeros, the multiple invocations of the recursive
     * array allocation routine ptr_init() will pass changed by 1 dim_ind arrays
     * to each invocation of ptr_init().  set dp[] from dim[] input array.
     */

    dp[0] = dim[0];
    for (i=0 ; i<num_dim ; i++)
    {
        dim_ind[i] = 0;
        if (dim[i] <= 0)
        {
            *err_code = ERRS_INV_DIM;
            return NULL;
        }

        if (i > 0)
        {
            dp[i] = dp[i-1] * dim[i];
        }
    }

    /*
     * calculate address of start of pointers.  If not
     * sizeof(char *) aligned make it so.
     */
    ptr_ptr = base_ptr + dp[num_dim-1] * data_size;

    for (i=0 ; i<sizeof(char *) ; i++, ++ptr_ptr)
    {
        if (((unsigned long)ptr_ptr)%sizeof(char *) == 0)
        {
            break;
        }
    }

    /* if init_ptr is NULL skip initialization */
    if (init_ptr != NULL)
    {
        /* set p_data to point to the start of the data area */
        p_data = base_ptr;
     
        /* initialize the array */
        for (i=0 ; i<dp[num_dim-1] ; i++)
        {
            p = init_ptr;
            for (j=0 ; j<data_size ; j++)
            {
                *p_data++ = *p++;
            }
        }
    }

    /* do array setup i.e. all the pointer stuff */
    return ptr_init(0, dim_ind, data_size, num_dim, base_ptr,
        ptr_ptr, dim, st, dp);
}



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
{
    unsigned int		i, j;

    /* array of current dimension indices */
    int dim_ind[MAX_DIM];

    char	*p_data, /* pointer to array data */
        	*p;      /* tmp pointer */

    /* points to base of allocated array */
    char	*base_ptr;

    /* points to base of pointers to pointers to ... to data */
    char	*ptr_ptr;

    /* product of dimensions from 0 to index, dim[0]*dim[1]...dim[index] */
    unsigned int		dp[MAX_DIM];



    if (num_dim < 1 || num_dim > MAX_DIM)
    {
        *err_code = ERRS_INV_DIMS;
        return NULL;
    }

    if (data_size < 1)
    {
        *err_code = ERRS_INV_REQ_SIZE;
        return NULL;
    }

    /*
     * set dim_ind to all zeros, the multiple invocations of the recursive
     * array allocation routine ptr_init() will pass changed by 1 dim_ind arrays
     * to each invocation of ptr_init().  set dp[] from dim[] input array.
     */

    dp[0] = dim[0];
    for (i=0 ; i<num_dim ; i++)
    {
        dim_ind[i] = 0;
        if (dim[i] <= 0)
        {
            *err_code = ERRS_INV_DIM;
            return NULL;
        }

        if (i > 0)
        {
            dp[i] = dp[i-1] * dim[i];
        }
    }

    /*
     * allocate enough memory for the data(dp[num_dim-1]*data_size) plus all
     * the array pointers(off(num_dim-1, dp)*sizeof(char *)) plus an extra
     * sizeof(char *) to allow for possible realignment of start of pointer
     * area on pointer alignment boundary.
     */

    if ((base_ptr = (char *) valloc(dp[num_dim-1] * data_size +
           off(num_dim-1, dp) * sizeof(char *) + sizeof(char *))) == NULL)
    {
        *err_code = ERRS_FAIL_ALLOC;
        return NULL;
    }

    /*
     * calculate address of start of pointers.  If not
     * sizeof(char *) aligned make it so.
     */
    ptr_ptr = base_ptr + dp[num_dim-1] * data_size;

    for (i=0 ; i<sizeof(char *) ; i++, ++ptr_ptr)
    {
        if (((unsigned long)ptr_ptr)%sizeof(char *) == 0)
        {
            break;
        }
    }

    /* return allocated space pointer that caller should use to free space */
    *free_ptr = base_ptr;

    /* if init_ptr is NULL skip initialization */
    if (init_ptr != NULL)
    {
        /* set p_data to point to the start of the data area */
        p_data = base_ptr;
     
        /* initialize the array */
        for (i=0 ; i<dp[num_dim-1] ; i++)
        {
            p = init_ptr;
            for (j=0 ; j<data_size ; j++)
            {
                *p_data++ = *p++;
            }
        }
    }

    /* do array setup i.e. all the pointer stuff */
    return ptr_init(0, dim_ind, data_size, num_dim, base_ptr,
        ptr_ptr, dim, st, dp);
}

