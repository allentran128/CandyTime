/* Allen Tran, Conner Knight
 *
 * Documentation for View's methods.
 * Contains functions pertaining to the display of the game.
 */

#ifndef _VIEW_H_
#define _VIEW_H_

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "array2d.h"

// Structure that contains the game state
typedef struct {
  GtkWidget * prevSelected;
  int row;
  int col;
  array2dPtr holder;
  GtkWidget ** buttons;
  int counter;
  GtkWidget* moves;
  GdkPixbuf * colors[6];
} boardState, *boardStatePtr;

// Redraws the game display given a board 
void updateBoard(boardStatePtr board); 

// Ends the game cleanly depending on the situation indicated
// by identifier
void destroy_on_exit(int identifier, boardStatePtr board);

#endif
