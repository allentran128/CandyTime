/*Allen Tran, Conner Knight
 *CSE 333 Hw2 
 *
 *The class for the array2d struct, includes all the function implimentations
 *descriptions for what these methods do is included in the .h file.
 */

#include <stdio.h>
#include <stdlib.h>

#include "array2d.h"


int Allocate_array2d(int row, int col, array2dPtr holder){
  if (row <= 0 || col <=0){
    return EXIT_FAILURE;
  }
  void ** arr = (void **) malloc(row * col * sizeof(void *));
  if (arr == NULL || holder == NULL){
    return EXIT_FAILURE;
  }
  holder->_arr = arr;
  holder->_row = row;
  holder->_col = col;
  return EXIT_SUCCESS;
}

int Initiate_array2d(int row, int col, void ** farr,int size, array2dPtr holder){
  int errNum;
  if(row <= 0 || col <= 0){
    return EXIT_FAILURE;
  }

  errNum = Allocate_array2d(row, col, holder);
  if(errNum != 0) {
    return errNum;
  }

  // Init values
  if(size != row * col){
    return EXIT_FAILURE;
  }
  for(int r = 0; r < row; r++){
    for(int c = 0; c < col; c++){
      holder->_arr[r*col+c] = farr[r*col+c];
    }
  }
  return EXIT_SUCCESS;
}

int copyArray(array2dPtr orig,array2dPtr copy){
  void ** tempData = (void **) malloc(orig->_row * orig->_col * sizeof(void *));;
  for(int r = 0; r < orig->_row ; r++){
    for(int c = 0; c < orig->_col ; c++){
      int * payload = (int *) malloc(sizeof(int));
      *payload =  * (int *)orig->_arr[r * orig->_col + c];
      tempData[r* orig->_col + c] = (void *) payload;
      //*(int *)copy->_arr[r * orig->_col + c] = * (int *)orig->_arr[r * orig->_col + c];
    }
  }
  int errNum = Initiate_array2d(orig->_row, orig->_col,tempData,orig->_row * orig->_col, copy);
  free(tempData);
  return errNum;
}

int get(int row, int col, array2dPtr holder, void ** payloadPtr){
  //checking array2d
  if(holder == NULL){
    return 4;
  }
  //bounds check
  if (row < holder->_row && col < holder->_col) {
    *payloadPtr = holder->_arr[row*holder->_col+col];
    return 0;
  }
  return 2;
}


int set(int row, int col, void * g, array2dPtr holder){
  if(holder == NULL){
    return 4;
  }
  // Bounds Check
  if (row < holder->_row && col < holder->_row) {
    // Payload Check
    if (g != NULL) {
      holder->_arr[row*holder->_col+col] = g;
      return 0;
    }
    return 3;
  }
  return 2;
}

int swap(int r1, int c1, int r2, int c2, array2dPtr holder){
  // Array Check
  if(holder == NULL) {
    return 4;
  }

  if(r1 >= holder->_row || r2 >= holder->_row || 
		  c1 >= holder->_col || c2 >= holder->_col) {
    return 2;
  }
  void * temp = holder->_arr[r1*holder->_col+c1];
  holder->_arr[r1*holder->_col+c1] = holder->_arr[r2*holder->_col+c2];
  holder->_arr[r2*holder->_col+c2] = temp;
  return 0;
}

int destroyArray2d(array2dPtr holder){
  if(holder == NULL){
    return 4;
  }
  for (int i = 0; i < holder->_row * holder->_col;i++){
    free(holder->_arr[i]);
  }
  free(holder->_arr);
  return 0;
}

