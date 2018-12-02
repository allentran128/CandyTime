/*Allen Tran, Conner Knight
 *CSE 333 Hw2 
 *
 *Example use case of our array2d library.
 */

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <stdlib.h>
#include "array2d.h"

#define NULL_CHECK(ptr) if (ptr == NULL) return EXIT_FAILURE;

// Print the status of the array
int status(array2dPtr holder) {
  printf("Array is %d x %d\n", holder->_row, holder->_col);
  for (int i = 0; i < holder->_row * holder->_col; i++) {
    printf("%d ", *((int *)holder->_arr[i]));
  }
  printf("\n\n");
  return EXIT_SUCCESS;
}

// Free the array
int destroy(array2dPtr holder) {
  printf("destroy array\n");
  for (int i = 0; i < holder->_row * holder->_col; i++) {
    printf("          destroyed %d\n", *((int *)holder->_arr[i]));
  }
  destroyArray2d(holder);
  free(holder);
  return EXIT_SUCCESS;
}

// Write the JSON object
int serialize(array2dPtr holder) {
  NULL_CHECK(holder);
  json_t *root;
  json_t *row;
  json_t *col;
  json_t *data;

  row = json_pack("i", holder->_row);
  col = json_pack("i", holder->_col);
  data = json_array();
    
  for (int i = 0; i < holder->_row * holder->_col; i++) {
    int value = *((int *)holder->_arr[i]);
    json_t *entry = json_pack("i", value);
    printf("          serialized %d\n", value);
    json_array_append(data, entry);
    json_decref(entry);
  }

  root = json_object();

  json_object_set_new(root, "rows", row);
  json_object_set_new(root, "columns", col);
  json_object_set_new(root, "data", data);
  json_dump_file(root, "json.out", JSON_COMPACT);

  json_decref(root);
  return EXIT_SUCCESS;
}

// Read the JSON object
int deserialize(json_t* root, json_error_t error, array2dPtr holder) {
  int row, col, errNum;
  void **farr;

  NULL_CHECK(root);

  // get data from JSON file
  json_t *row_obj = json_object_get(root, "rows");
  json_t *col_obj = json_object_get(root, "columns");
  json_t *data_obj = json_object_get(root, "data");

  row = json_integer_value(row_obj);
  col = json_integer_value(col_obj);
 
  // parse data obj to an array 
  farr = (void **) malloc(row * col * sizeof(void *));
  NULL_CHECK(farr);

  size_t index;
  json_t *value;
  
  json_array_foreach(data_obj, index, value) {
    int *payload = (int *) malloc(sizeof(int *));
    NULL_CHECK(payload);
    *payload = (int) json_integer_value(value);
    printf("          deserialized %d\n", (int) *payload);
    farr[index] = (void *) payload; 
  }

  errNum = Initiate_array2d(row, col, farr, row*col, holder);

  if (errNum != 0) {
    // failed to initialize array
    // free the data array
    for (int i = 0; i <= index; i++) {
      free(farr[i]);
    }
  }

  free(farr);

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
  int statusCode; 
  if (argc == 2) {
    // Get root
    json_error_t error;
    json_t *root = json_load_file(argv[1], 0, &error);
    NULL_CHECK(root); // cannot find the file

    // Init and Allocate Array
    array2dPtr holder = (array2dPtr) malloc(sizeof(array2d));
    NULL_CHECK(holder);

    printf("Deserialize %s\n", argv[1]); 
    statusCode = deserialize(root, error, holder);
    
    status(holder);
    json_decref(root);

    // Set [1][1] to 100
    int *value = (int *) malloc(sizeof(int *));
    NULL_CHECK(value);
    *value = 100;
    
    void ** payloadPtr = (void **) malloc(sizeof(void **));
    NULL_CHECK(payloadPtr);

    //if implimenter wants to deal with out of bounds, check the returned int of get
    get(1, 1, holder, payloadPtr);
    free(*payloadPtr);
    free(payloadPtr);    
    printf("Set [1][1] to 100\n");

    //if implimenter wants to deal with invalid payload or out of bounds, check
    //the returned int of set
    set(1, 1, (void *) value, holder);
    status(holder);

    // Swap [1][1] and [0][0]
    printf("swap [1][1] and [0][0]\n");

    //if implimenter wants to deal with invalid payload or out of bounds, check
    //the returned int of swap
    swap(1, 1, 0, 0, holder);
    status(holder);

    // Try swapping [1][1] with [10][10]
    printf("swap [1][1] and [10][10]\n");

    //if implimenter wants to deal with invalid payload or out of bounds, check
    //the returned int of swap
    swap(1, 1, 10, 10, holder);
    status(holder);

    // Serialize current array to "json.out"
    printf("serialize array to file json.out\n");
    
    statusCode = serialize(holder);
    if(statusCode != 0){
      return EXIT_FAILURE;
    }

    // Free array2d
    destroy(holder);

    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
