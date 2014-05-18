daa
===

Dynamic Array Allocator C/C++
  - ptr to ptr to ... style arbitrary N dimensional
  - arbitrary starting subscript for each separate dimension
  - allocates anything of any type(double, int, long, struct, ...) that you can take sizeof() on
  - one contiguous block allocation of memory for all ptr's and data
  - array or slices of array can be passed to subroutines to any depth, modifed there, and be seen at all levels 
