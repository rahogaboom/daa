#ifndef DAA_H
#define DAA_H

/*
 *==============================================================================
 * File: daa.h
 *
 * VERSION:
 *
 * Description:
 *     header file for daa.c - dynamic array allocator
 *     header file for daa.cpp - dynamic array allocator
 *
 *==============================================================================
 */

/*
 *------------------------------------------------------------------------------
 * include files
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * constants
 *------------------------------------------------------------------------------
 */

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

extern char *daa_errs[];

/*
 *------------------------------------------------------------------------------
 * function prototypes
 *------------------------------------------------------------------------------
 */

   int
das(
   unsigned int data_size,
   unsigned int num_dim,
   unsigned int *dim,
   int *err_code);

   void *
daa(
   unsigned int data_size,
   unsigned int num_dim,
   unsigned int *dim,
   int *st,
   int *err_code,
   char *base_ptr,
   char *init_ptr);

   void *
daav(
   unsigned int data_size,
   unsigned int num_dim,
   unsigned int *dim,
   int *st,
   int *err_code,
   char **free_ptr,
   char *init_ptr);

#endif /* !DAA_H */

