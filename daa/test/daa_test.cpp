
//  daa_test.cpp

//
//          Copyright Richard Allen Hogaboom 1997 - 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// Boost Software License - Version 1.0 - August 17th, 2003
//

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
    using namespace Daa;

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
     * TEST 2
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[4] = {3, 5, 4, 2}; /* dimensions */
        int st[4] = {-1, -5, 10, 0}; /* starting subscripts */
        double ****array = NULL; /* array pointer */

        fprintf(stderr, "\nTEST 2");
        fprintf(stderr, "\n    4 dimensional array of double");
        fprintf(stderr, "\n        dimensions: 3, 5, 4, 2");
        fprintf(stderr, "\n        starting subscripts: -1, -5, 10, 0");
        fprintf(stderr, "\n        init: NULL\n");
        fprintf(stderr, "\n    initializes four dimensional array[][][][] to integer values");
        fprintf(stderr, "\n    which are the sum of the dimension subscripts\n\n");

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
                            fprintf(stderr, "array[%2d][%2d][%2d][%2d] = %5.1f\n"
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
     * TEST 3
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[4] = {3, 5, 4, 2}; /* dimensions */
        int st[4] = {-1, -5, 10, 0}; /* starting subscripts */
        double init = 123.;
        double ****array = NULL; /* array pointer */

        fprintf(stderr, "\nTEST 3");
        fprintf(stderr, "\n    4 dimensional array of double");
        fprintf(stderr, "\n        dimensions: 3, 5, 4, 2");
        fprintf(stderr, "\n        starting subscripts: -1, -5, 10, 0");
        fprintf(stderr, "\n        init: 123.\n");
        fprintf(stderr, "\n    initializes four dimensional array[][][][] to integer values");
        fprintf(stderr, "\n    which are the sum of the dimension subscripts\n\n");

        asize = das(sizeof(double), 4, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (double ****) daa(sizeof(double), 4, d, st, &err_code, mem_ptr, (char *)&init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            init4darray(d, st, array);

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
                            fprintf(stderr, "array[%2d][%2d][%2d][%2d] = %5.1f\n"
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
     * TEST 4
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[1] = {10}; /* dimensions */
        int st[1] = {1}; /* starting subscripts */
        double init = 10.10;
        double *array = NULL; /* array pointer */

        fprintf(stderr, "\nTEST 4");
        fprintf(stderr, "\n    1 dimensional array of double");
        fprintf(stderr, "\n        dimensions: 10");
        fprintf(stderr, "\n        starting subscripts: 1");
        fprintf(stderr, "\n        init: 10.10\n");
        fprintf(stderr, "\n    set array[10] = 4.5\n\n");

        asize = das(sizeof(double), 1, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (double *) daa(sizeof(double), 1, d, st, &err_code, mem_ptr, (char *)&init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            array[10] = 4.5;

            fprintf(stderr, "sizeof(double) = %ld\n", sizeof(double));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                fprintf(stderr, "array[%2d] = %e\n", i, array[i]);
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
     * TEST 6
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[1] = {10}; /* dimensions */
        int st[1] = {1}; /* starting subscripts */
        struct s {
            double d;
            int l;
        } *array = NULL, s_init = {1.25, 5}; /* array pointer */

        fprintf(stderr, "\nTEST 6");
        fprintf(stderr, "\n    1 dimensional array of struct");
        fprintf(stderr, "\n        dimensions: 1");
        fprintf(stderr, "\n        starting subscripts: 1");
        fprintf(stderr, "\n        init: struct s { double d; int l; } s_init = {1.25, 5}\n");
        fprintf(stderr, "\n    set array[1].d= .5");
        fprintf(stderr, "\n    set array[1].l= 10");
        fprintf(stderr, "\n    set array[10].d = 4.5");
        fprintf(stderr, "\n    set array[10].l = 4\n\n");

        asize = das(sizeof(struct s), 1, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (struct s *) daa(sizeof(struct s), 1, d, st, &err_code, mem_ptr, (char *)&s_init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            array[1].d= .5;
            array[1].l= 10;
            array[10].d = 4.5;
            array[10].l = 4;

            fprintf(stderr, "sizeof(struct s) = %ld\n", sizeof(struct s));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                fprintf(stderr, "array[%2d].d = %f    ", i, array[i].d);
                fprintf(stderr, "array[%2d].l = %2d\n", i, array[i].l);
            }
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 7
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[2] = {5, 5}; /* dimensions */
        int st[2] = {0, 0}; /* starting subscripts */
        struct s {
            double d;
            int l;
        } **array = NULL, s_init = {1, 5}; /* array pointer */

        fprintf(stderr, "\nTEST 7");
        fprintf(stderr, "\n    2 dimensional struct array");
        fprintf(stderr, "\n        dimensions: 5, 5");
        fprintf(stderr, "\n        starting subscripts: 0, 0");
        fprintf(stderr, "\n        init: struct s { double d; int l; } s_init = {1, 5}\n");
        fprintf(stderr, "\n    set array[0][0].d = .25");
        fprintf(stderr, "\n    set array[0][0].l = 10");
        fprintf(stderr, "\n    set array[4][4].d = 4.5");
        fprintf(stderr, "\n    set array[4][4].l = 4\n\n");

        asize = das(sizeof(struct s), 2, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (struct s **) daa(sizeof(struct s), 2, d, st, &err_code, mem_ptr, (char *)&s_init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            array[0][0].d = .25;
            array[0][0].l = 10;
            array[4][4].d = 4.5;
            array[4][4].l = 4;

            fprintf(stderr, "sizeof(struct s) = %ld\n", sizeof(struct s));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                {
                    fprintf(stderr, "array[%2d][%2d].d = %f    ", i, j, array[i][j].d);
                    fprintf(stderr, "array[%2d][%2d].l = %2d\n", i, j, array[i][j].l);
                }
            }
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 8
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[3] = {2, 5, 6}; /* dimensions */
        int st[3] = {0, 0, 0}; /* starting subscripts */
        struct s {
            char c;
            int l;
            double d;
        } ***array = NULL, s_init = {0, 1, 10}; /* array pointer */

        fprintf(stderr, "\nTEST 8");
        fprintf(stderr, "\n    3 dimensional struct array");
        fprintf(stderr, "\n        dimensions: 2, 5, 6");
        fprintf(stderr, "\n        starting subscripts: 0, 0, 0");
        fprintf(stderr, "\n        init: struct s { char c; int l; double d; } s_init = {0, 1, 10}\n");
        fprintf(stderr, "\n    set array[1][4][5].c = 1");
        fprintf(stderr, "\n    set array[1][4][5].l = 15");
        fprintf(stderr, "\n    set array[1][4][5].d = 7.5\n\n");

        asize = das(sizeof(struct s), 3, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (struct s ***) daa(sizeof(struct s), 3, d, st, &err_code, mem_ptr, (char *)&s_init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            array[1][4][5].c = 1;
            array[1][4][5].l = 15;
            array[1][4][5].d = 7.5;

            fprintf(stderr, "sizeof(struct s) = %ld\n", sizeof(struct s));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                {
                    for (int k=st[2] ; k<st[2]+int(d[2]) ; k++)
                    {
                        fprintf(stderr, "array[%d][%d][%d].c = %2d    ", i, j, k, array[i][j][k].c);
                        fprintf(stderr, "array[%d][%d][%d].l = %2d    ", i, j, k, array[i][j][k].l);
                        fprintf(stderr, "array[%d][%d][%d].d = %e\n", i, j, k, array[i][j][k].d);
                    }
                }
            }
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 9
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[3] = {1, 2, 3}; /* dimensions */
        int st[3] = {-1, -1, -1}; /* starting subscripts */
        struct s {
            double d;
            int l;
            char c;
        } ***array = NULL, s_init = {1, 10, 0}; /* array pointer */

        fprintf(stderr, "\nTEST 9");
        fprintf(stderr, "\n    3 dimensional struct array");
        fprintf(stderr, "\n        dimensions: 1, 2, 3");
        fprintf(stderr, "\n        starting subscripts: -1, -1, -1");
        fprintf(stderr, "\n        init: struct s { double d; int l; char c; } s_init = {1, 10, 0}\n");
        fprintf(stderr, "\n    set array[-1][0][1].d = 7.5");
        fprintf(stderr, "\n    set array[-1][0][1].l = 15");
        fprintf(stderr, "\n    set array[-1][0][1].c = 2\n\n");

        asize = das(sizeof(struct s), 3, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (struct s ***) daa(sizeof(struct s), 3, d, st, &err_code, mem_ptr, (char *)&s_init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            array[-1][0][1].d = 7.5;
            array[-1][0][1].l = 15;
            array[-1][0][1].c = 2;

            fprintf(stderr, "sizeof(struct s) = %ld\n", sizeof(struct s));
            fprintf(stderr, "array size = %d\n\n", asize);
            fprintf(stderr, "array[-1][0][0].d = %e"
                            " array[-1][0][0].l = %d"
                            " array[-1][0][0].c = %d\n",
               array[-1][0][0].d, array[-1][0][0].l, array[-1][0][0].c);
            fprintf(stderr, "array[-1][0][1].d = %e"
                            " array[-1][0][1].l = %d"
                            " array[-1][0][1].c = %d\n",
               array[-1][0][1].d, array[-1][0][1].l, array[-1][0][1].c);
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 10
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[1] = {10}; /* dimensions */
        int st[1] = {1}; /* starting subscripts */
        int init = 12;
        int *array = NULL; /* array pointer */

        fprintf(stderr, "\nTEST 10");
        fprintf(stderr, "\n    1 dimensional int array");
        fprintf(stderr, "\n        dimensions: 10");
        fprintf(stderr, "\n        starting subscripts: 1");
        fprintf(stderr, "\n        init: 12\n");
        fprintf(stderr, "\n    set array[10] = 4\n\n");

        asize = das(sizeof(int), 1, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (int *) daa(sizeof(int), 1, d, st, &err_code, mem_ptr, (char *)&init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            array[10] = 4;

            fprintf(stderr, "sizeof(int) = %ld\n", sizeof(int));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0]; i<st[0]+int(d[0]) ; i++)
            {
                fprintf(stderr, "array[%2d] = %d\n", i, array[i]);
            }
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 11
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[2] = {2, 2}; /* dimensions */
        int st[2] = {-20, 20}; /* starting subscripts */
        int data;
        int init = 0;
        int **array = NULL; /* array pointer */

        fprintf(stderr, "\nTEST 11");
        fprintf(stderr, "\n    2 dimensional int array");
        fprintf(stderr, "\n        dimensions: 2, 2");
        fprintf(stderr, "\n        starting subscripts: -20, 20");
        fprintf(stderr, "\n        init: 0\n");
        fprintf(stderr, "\n    set array[i][j] = data++ each time array is accessed\n\n");

        asize = das(sizeof(int), 2, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (int **) daa(sizeof(int), 2, d, st, &err_code, mem_ptr, (char *)&init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            data = 1;
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                {
                    array[i][j] = data++;
                }    
            }

            fprintf(stderr, "sizeof(int) = %ld\n", sizeof(int));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                {
                    fprintf(stderr, "array[%3d][%3d] = %d\n"
                        , i, j, array[i][j]);
                }    
            }
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 12
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[3] = {3, 3, 3}; /* dimensions */
        int st[3] = {-10, -20, -30}; /* starting subscripts */
        int data;
        int init = 2;
        int ***array = NULL; /* array pointer */

        fprintf(stderr, "\nTEST 12");
        fprintf(stderr, "\n    3 dimensional int array");
        fprintf(stderr, "\n        dimensions: 3, 3, 3");
        fprintf(stderr, "\n        starting subscripts: -10, -20, -30");
        fprintf(stderr, "\n        init: 2\n");
        fprintf(stderr, "\n    set array[i][j][k] = data++ each time array is accessed\n\n");

        asize = das(sizeof(int), 3, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (int ***) daa(sizeof(int), 3, d, st, &err_code, mem_ptr, (char *)&init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            data = 0;
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                {
                    for (int k=st[2] ; k<st[2]+int(d[2]) ; k++)
                    {
                        array[i][j][k] = data++;
                    }
                }    
            }

            fprintf(stderr, "sizeof(int) = %ld\n", sizeof(int));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                {
                    for (int k=st[2] ; k<st[2]+int(d[2]) ; k++)
                    {
                        fprintf(stderr, "array[%3d][%3d][%3d] = %d\n"
                            , i, j, k, array[i][j][k]);
                    }
                }    
            }
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 13
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[3] = {2, 5, 6}; /* dimensions */
        int st[3] = {0, 0, 0}; /* starting subscripts */
        union s {
            int l;
            double d;
        } ***array = NULL, s_init = {5}; /* array pointer */

        fprintf(stderr, "\nTEST 13");
        fprintf(stderr, "\n    3 dimensional union array");
        fprintf(stderr, "\n        dimensions: 2, 5, 6");
        fprintf(stderr, "\n        starting subscripts: 0, 0, 0");
        fprintf(stderr, "\n        init: union s { int l; double d; } s_init = {5}\n");
        fprintf(stderr, "\n    print array[1][4][4].l");
        fprintf(stderr, "\n    set array[1][4][5].l = 7 then print");
        fprintf(stderr, "\n    set array[1][4][5].d = 7.5 then print\n\n");

        asize = das(sizeof(union s), 3, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (union s ***) daa(sizeof(union s), 3, d, st, &err_code, mem_ptr, (char *)&s_init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            fprintf(stderr, "sizeof(union s) = %ld\n", sizeof(union s));
            fprintf(stderr, "array size = %d\n\n", asize);

            fprintf(stderr, "array[1][4][4].l = %d\n", array[1][4][4].l);

            array[1][4][5].l = 7;
            fprintf(stderr, "array[1][4][5].l = %d\n", array[1][4][5].l);

            array[1][4][5].d = 7.5;
            fprintf(stderr, "array[1][4][5].d = %e\n", array[1][4][5].d);
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 14
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[3] = {2, 5, 6}; /* dimensions */
        int st[3] = {0, 0, 0}; /* starting subscripts */
        enum e {a,b,c} ***array = NULL, e_init = {c}; /* array pointer */

        fprintf(stderr, "\nTEST 14");
        fprintf(stderr, "\n    3 dimensional enum array");
        fprintf(stderr, "\n        dimensions: 2, 5, 6");
        fprintf(stderr, "\n        starting subscripts: 0, 0, 0");
        fprintf(stderr, "\n        init: enum e {a,b,c} e_init = {c}\n");
        fprintf(stderr, "\n    print array[1][4][4] - should be 2");
        fprintf(stderr, "\n    set array[1][4][5] = a - should be 0");
        fprintf(stderr, "\n    set array[1][4][5] = b - should be 1\n\n");

        asize = das(sizeof(enum e), 3, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (enum e ***) daa(sizeof(enum e), 3, d, st, &err_code, mem_ptr, (char *)&e_init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            fprintf(stderr, "sizeof(enum e) = %ld\n",sizeof(enum e));
            fprintf(stderr, "array size = %d\n\n", asize);

            fprintf(stderr, "array[1][4][4] = %d\n", array[1][4][4]);

            array[1][4][5] = a;
            fprintf(stderr, "array[1][4][5] = %d\n", array[1][4][5]);

            array[1][4][5] = b;
            fprintf(stderr, "array[1][4][5] = %d\n", array[1][4][5]);

            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 15
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[2] = {15, 5}; /* dimensions */
        int st[2] = {-1, -1}; /* starting subscripts */
        short init = 11;
        short **array = NULL; /* array pointer */

        fprintf(stderr, "\nTEST 15");
        fprintf(stderr, "\n    2 dimensional short array");
        fprintf(stderr, "\n        dimensions: 15, 5");
        fprintf(stderr, "\n        starting subscripts: -1, -1");
        fprintf(stderr, "\n        init: 11\n");
        fprintf(stderr, "\n    set array[5][3] = 4\n\n");

        asize = das(sizeof(short), 2, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (short **) daa(sizeof(short), 2, d, st, &err_code, mem_ptr, (char *)&init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            array[5][3] = 4;

            fprintf(stderr, "sizeof(short) = %ld\n", sizeof(short));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1]; j<st[1]+int(d[1]) ; j++)
                {
                    fprintf(stderr, "array[%2d][%2d] = %d\n"
                        , i, j, array[i][j]);
                }
            }
            fprintf(stderr, "err_code = %d\n\n", err_code);
            free(mem_ptr);
        }
    }

    /*
     * TEST 16
     */
    {
        int err_code = 0;
        int asize = 0;
        char *mem_ptr;

        unsigned int d[2] = {4, 5}; /* dimensions */
        int st[2] = {0, 1}; /* starting subscripts */
        unsigned short init = 13;
        unsigned short **array = NULL; /* array pointer */

        fprintf(stderr, "\nTEST 16");
        fprintf(stderr, "\n    2 dimensional unsigned short array");
        fprintf(stderr, "\n        dimensions: 4, 5");
        fprintf(stderr, "\n        starting subscripts: 0, 1");
        fprintf(stderr, "\n        init: 13\n");
        fprintf(stderr, "\n    set array[3][5] = 5\n\n");

        asize = das(sizeof(unsigned short), 2, d, &err_code);

        mem_ptr = (char *)malloc(asize);

        array = (unsigned short **) daa(sizeof(unsigned short), 2, d, st, &err_code, mem_ptr, (char *)&init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            array[3][5] = 5;

            fprintf(stderr, "sizeof(unsigned short) = %ld\n", sizeof(unsigned short));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                {
                    fprintf(stderr, "array[%2d][%2d] = %d\n"
                        , i, j, array[i][j]);
                }
            }
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

    /*
     * TEST 18
     */
    {
        int err_code = 0;
        int asize = 0;

        unsigned int d[2] = {3, 3}; /* dimensions */
        int st[2] = {1, 1}; /* starting subscripts */
        double init = 1.5;
        double **array = NULL; /* array pointer */

        char stack[1024]; /* stack area for array(as opposed to heap)) */

        fprintf(stderr, "\nTEST 18");
        fprintf(stderr, "\n    2 dimensional double array(on stack)");
        fprintf(stderr, "\n        dimensions: 3, 3");
        fprintf(stderr, "\n        starting subscripts: 1, 1");
        fprintf(stderr, "\n        init: 1.5\n");
        fprintf(stderr, "\n    allocate array on stack");
        fprintf(stderr, "\n    print array with default value(1.5)\n\n");

        asize = das(sizeof(double), 2, d, &err_code);

        array = (double **) daa(sizeof(double), 2, d, st, &err_code, stack, (char *)&init);

        if (array == NULL)
        {
            fprintf(stderr, "daa: error on dynamic allocation. %s\n"
                , daa_errs[err_code]);
        }
        else
        {
            fprintf(stderr, "sizeof(double) = %ld\n", sizeof(double));
            fprintf(stderr, "array size = %d\n\n", asize);
            for (int i=st[0] ; i<st[0]+int(d[0]) ; i++)
            {
                for (int j=st[1] ; j<st[1]+int(d[1]) ; j++)
                {
                    fprintf(stderr, "array[%2d][%2d] = %5.1f\n", i, j, array[i][j]);
                }
            }
            fprintf(stderr, "err_code = %d\n\n", err_code);
        }
    }
}

