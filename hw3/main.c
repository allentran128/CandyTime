/* Conner's work
 * 
 * Allen's (rough) Description
 * main reads in the JSON file and converts it into an array2d object
 * the array2d object is reordered to match the layout of the VIEW
 * This file contains the methods of the swap buttons for the VIEW
 */
#include <stdio.h>
#include <stdlib.h>
#include "array2d.h"

#define NULL_CHECK(ptr) if (ptr == NULL) return EXIT_FAILURE;

// ISSUE : how are we going to return the array2d object?
int main (int argc, char * argv[]) {
  
  if (argc == 2){
    int row, col, errNum;
    void ** darr;
    array2dPtr holder;

    json_error_t error;
    json_t *root = json_load_file(argv[1],0,&error);
    NULL_CHECK(root);

    // Reads the JSON file
    json_t *row_obj = json_object_get(root, "rows");
    json_t *col_obj = json_object_get(root, "columns");
    json_t *data_obj = json_object_get(root, "data");

    // Extracts data from the JSON file
    row = json_integer_value(row_obj);
    col = json_integer_value(col_obj);

    // Initializes the data array to allocate into array2d object
    darr = (void **) malloc(row * col * sizeof(void *));
    NULL_CHECK(darr);

    size_t index;
    json_t *value;

    // Copies over JSON data to the temp data array
    json_array_foreach(data_obj, index, value){
      int *payload = (int *) malloc(sizeof(int *));
      NULL_CHECK(payload);
      *payload = (int) json_integer_value(value);
      darr[index] = (void *) payload;
    }

    // Checks if there is an eror making the array2d object,
    // if so it will free the previously malloc'd data and exit
    errNum = Initiate_array2d(row, col, darr, row * col, holder);
    free(darr);
    if(errNum != 0){
      for(int i = 0; i <= index; i++){
        free(farr[i]);
      }
      return EXIT_FAILURE;
    }

    // Reorders the array2d object to match the layout of the VIEW
    reorder(holder, row, col); 
    return EXIT_SUCCESS;
  }
}

// Inverts the array rows of the array2d object
int reorder(array2dPtr holder,int row, int col){
  //int size = row * col;
  for(int i = 0; i < (col/2); i++){
    for(int j = 0; j < row; j++){
      swap(i,j,(row-1)-i,j,holder);
    }
  }
  return 0;
}

// Control for Left swap button
int moveLeft(array2dPtr holder, int row, int col ){
  if(col > 0){
    return swap(row, col, row, col-1, holder); 
  }
  return -1;
}

// Control for Right swap button
int moveRight(array2dPtr holder, int row, int col ){
  if(col < (holder->_col-1)){
    return swap(row, col, row, col+1, holder);
  }
  return -1;
}

// Control for Up swap button
int moveUp(array2dPtr holder, int row, int col ){
  if(row > 0){
    return swap(row, col, row+1, col, holder);
  }
  return -1;
}

// Control for Down swap button
int moveDown(array2dPtr holder, int row, int col ){
  if(row < (holder->_row-1)){
    return swap(row, col, row-1, col, holder);
  }
  return -1;
}
