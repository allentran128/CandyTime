/* Allen Tran, Conner Knight
 *
 * Documentation for Model's methods.
 * Contains functions pertaining to the start up tasks of the game.
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <gdk-pixbuf/gdk-pixbuf.h>
#include "array2d.h"
#include "view.h"

// Description : loads in a board from a JSON file and saves it into holder
// Returns a status code that indicates success or failure
int processInit(array2dPtr holder, char *file);

// Invert the rows of the holder array
void reorder(array2dPtr holder, int row, int col);

// Loads candy images into colors
// Returns a status code to indicate success or failure
void processColors(boardStatePtr board);
#endif
