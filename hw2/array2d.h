/*Allen Tran, Conner Knight
 *CSE 333 Hw2 
 *
 *the header for the array2d.c file, includes the array2d struct and definitions
 *for all the functions in array2d.c and descriptions of what they do and what they
 * input and output.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _HW2_ARRAY2D_H_
#define _HW2_ARRAY2D_H_
/*status code chart:
 *0: function has succeeded in it's purpose
 *1: generic error
 *2: invalid row/col
 *3: invalid payload
 *4: invalid array2dPtr
 *5: malloc failure
each function outputs a int which represents an  int status code, if the function
suceeded it will output zero, if it fails it will output a non zero integer,
the description for what number represents what type of error is shown above.

 */
typedef struct {
  int _row;
  int _col;
  void ** _arr;
} array2d, *array2dPtr;

//desc: Allocates a array2d structure and mallocs enough space for an
//array of the given rows and columns with values of void *, as well as
//a array2d struct
//input: the row size and column size of the new array2d to be allocated,
//as well as a pointer to the  array2d struct that will be created
//output: the status code(ref to top)
int Allocate_array2d(int row, int col, array2dPtr holder);

//desc:Allocates a array2d structure, and then uses the "farr" 1d array
//of void * to fill in the rows and columns of the new array2d.
//Pattern of doing this is it fills up the array2d by column, with the
//contents of the 1d array
//input:takes in the row size and column size of new array, as well as a
//1d Array that the new array2d will be based on, and a pointer to the
//array2d struct that will be created
//output: the status code(ref to top)
int Initiate_array2d(int row, int col, void** farr, int size, array2dPtr holder);
//copies information in original array into the copy array
int copyArray(array2dPtr orig,array2dPtr copy);

//desc:gets a pointer to the value at a "row" and "col" of a array2d
// called holder and puts it into the payloadPtr variable passed in
//input:takes in the desired row and column to get value from, as well as
//the pointer to the desired array2d the user wishes to get the value from,
//and also the pointer to the payloadptr that we will be the result of the function
//output: the status code(ref to top)
int get(int row, int col, array2dPtr holder, void ** payloadPtr);

//desc:sets the value of a array2d element at a "row" and "col" to a new
//value of g the user passes in
//input:takes in the desired row and col, and the value to replace the old
//value at the given row and col for a array2d, the pointer to this array 
//is passed in as "holder"
//output: the status code(ref to top)
int set(int row, int col, void * g, array2dPtr holder);

//desc:swaps a pair of values for a array2d with pointer "holder". The
//elements swapped are the one's at [r1,c1] and [r2,c2]
//If any set of indexes are out of bounds, then swap is not performed
//input:takes in 2 rows and columns that will be swapped, as well as the
//array2dPtr of the array2d that will have it's elments swapped
//output: the status code(ref to top)
int swap(int r1, int c1, int r2, int c2, array2dPtr holder);

//desc:frees all the fields of the passed in array2dPtr
//input:takes in a pointer to the 2dArray to be deleted
//output: the status code(ref to top)
int destroyArray2d(array2dPtr holder);
#endif

#ifdef __cplusplus
}
#endif
