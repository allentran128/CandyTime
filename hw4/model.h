/* Allen Tran, Conner Knight
 *
 * Documentation for Model's methods.
 * Contains functions pertaining to the start up tasks of the game.
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <gdk-pixbuf/gdk-pixbuf.h>
#include "array2d.h"
#include "view.h"
#include "jansson.h"

// Description : loads in a board from a JSON file and saves it into holder
// Returns a status code that indicates success or failure
int processInit(boardStatePtr board, char *file);

// Invert the rows of the holder array
void reorder(array2dPtr holder, int row, int col);

// Loads candy images into colors
// Returns a status code to indicate success or failure
void processColors(boardStatePtr board);

// Updated swap function that checks for matching
// templates then destroys those candies and re-fills
// the board with new candy
void checkDestroy(boardStatePtr board, int direction);

// Cleans all of the objects pertaining to the deserialize proccess
void cleanInit(json_t *root, array2dPtr first, array2dPtr second, array2dPtr third, array2dPtr fourth);

// frees the payload in the array (if possible) then the array itself (if
// valid)
void deepCleanArr(array2dPtr arr);

// Exports the game state into a json file that
// can be loaded in later
int serialize(boardStatePtr board);
#ifdef __cplusplus
}
#endif

#endif
