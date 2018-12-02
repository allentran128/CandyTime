/* Allen Tran, Conner Knight
 *
 * Desc:Model part of the process, makes the board from a json file and processes
 * all the possible colors from files
 *
 * Description :
 * Contains startup tasks.
 */

#include <stdio.h>
#include <stdlib.h>
#include "array2d.h"
#include "jansson.h"
#include "view.h"
#include "model.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

#define NULL_CHECK(ptr) if (ptr == NULL) return EXIT_FAILURE;

// Description : loads in a board from a JSON file and saves it into holder
// Returns a status code that indicates success or failure
int processInit(array2dPtr holder, char *file) {
  if (holder != NULL && file != NULL) {
    int row, col, errNum;
    void ** darr;
    
    size_t index;
    json_t *value;
    json_error_t error;
    json_t *root = json_load_file(file, 0, &error);
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

    // Copies over JSON data to the data array
    json_array_foreach(data_obj, index, value) {
      int *payload = (int *) malloc(sizeof(int *));
      NULL_CHECK(payload);
      *payload = json_integer_value(value);
      darr[index] = (void *) payload;
    }

    // Checks if there is an error making the array2d object,
    // if so it will free the previously malloc'd data and exit
    errNum = Initiate_array2d(row, col, darr, row * col, holder);

    if (errNum != 0) {
      // hit an error, free
      for (int i = 0; i < index; i++) {
        free(darr[i]);
      }
      free(darr);
      return EXIT_FAILURE;
    }
    
    // Reorders the array2d object to match the layout of the board
    reorder(holder, row, col);
    free(darr);
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

// Converts the order of the holder array to match the board's layout
void reorder(array2dPtr holder, int row, int col){
  for(int i = 0; i < (row/2); i++){
    for(int j = 0; j < col; j++){
      swap(i,j,(row-1)-i,j,holder);
    }
  }
}

// Loads candy images into colors
// Returns a status code to indicate success or failure
void processColors(boardStatePtr board) {
    board->colors[0] = gdk_pixbuf_new_from_file("./images/40x40/blue.png", NULL);
    board->colors[1] = gdk_pixbuf_new_from_file("./images/40x40/green.png", NULL);
    board->colors[2] = gdk_pixbuf_new_from_file("./images/40x40/orange.png", NULL);
    board->colors[3] = gdk_pixbuf_new_from_file("./images/40x40/purple.png", NULL);
    board->colors[4] = gdk_pixbuf_new_from_file("./images/40x40/red.png", NULL);
    board->colors[5] = gdk_pixbuf_new_from_file("./images/40x40/yellow.png", NULL);
}
