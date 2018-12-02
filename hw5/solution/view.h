/* Allen Tran, Conner Knight
 *
 * Documentation for View's methods.
 * Contains functions pertaining to the display of the game.
 */

#ifndef _VIEW_H_
#define _VIEW_H_

// Used to integrate C to C++
#ifdef __cplusplus
extern "C" {
#endif
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "array2d.h"

// Structure that contains the game state
typedef struct {
  GtkWidget * prevSelected;
  int* row;
  int* col;
  int* score;
  array2dPtr holder;
  array2dPtr gameState;
  GtkWidget ** buttons;
  int counter;
  GtkWidget* moves;
  GtkWidget* scoreBtn;
  GdkPixbuf * colors[6];
  GdkPixbuf * altColors[6];
  GdkPixbuf * newColors[6];
  void * masterPtr;
  void * serverPtr;
} boardState, *boardStatePtr;

// Launches the GTK app given a board to load
// returns the status code upon exiting the app
int startGtk(boardStatePtr board);

// Redraws the game display given a board 
void updateBoard(boardStatePtr board); 

// Ends the game cleanly depending on the situation indicated
// by identifier
void destroy_on_exit(int identifier, boardStatePtr board);

// Creates the directional arrow buttons and puts them on the grid
// passed in
int assignControlButtons(GtkWidget * grid, boardStatePtr board);

// Updates the score label in gdk with the score in board
void updateScore(boardStatePtr board);

// Update the moves made label in gdk with the number of moves made
// indicated in board
void updateMove(boardStatePtr board);

// Loads in the candy images
void processColors(boardStatePtr board);
#ifdef __cplusplus
}
#endif

#endif
