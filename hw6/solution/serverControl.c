/* Conner Knight, Allen Tran
 *
 * Button Controls
 *
 * Description
 * Functions that control the actions of the buttons that effect the game's
 * state.
 */

#include <stdlib.h>
#include <stdio.h>

#include "controlTransfer.h"

#include "jansson.h"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "array2d.h"
#include "view.h"

// On button click, ensures that atmost one button is toggled and
// sets the selected button's row, col and pointer.
void selected_candy(GtkWidget *widget, gpointer aboard) {
  boardStatePtr board = (boardStatePtr) aboard;
  if(board->prevSelected != NULL){
    gtk_toggle_button_set_active((GtkToggleButton *)board->prevSelected, FALSE);
  }
  // Update previously selected button
  board->prevSelected = widget;

  // Sets the row and col of the previously selected button
  for (int i = 0; i < board->holder->_row; i++) {
    for (int j = 0; j < board->holder->_col; j++) {
      if (board->buttons[i * board->holder->_col + j] == (void *)widget) {
        (*board->row) = i;
        (*board->col) = j;
      }
    }
  }
}

// Untoggles the previously toggled button
void resetPiece(boardStatePtr board) {
  (*board->row) = (*board->col) = -1;
  gtk_toggle_button_set_active((GtkToggleButton *)board->prevSelected, FALSE);
}

// Makes a move json to send to client given a direction.
void packageJson(boardStatePtr board, int dir, json_t *root) {
  // ENCODE a json to send to model
  // First param
  json_t *move = json_string("move");
  json_object_set_new(root, "action", move);
  // Second Param
  json_t *row = json_integer(board->holder->_row - (*board->row+ 1));
  json_object_set_new(root, "row", row);
  // Third Param
  json_t *col = json_integer(*board->col);
  json_object_set_new(root, "column", col);
  // Direction Param
  json_t *direction = json_integer(dir);
  json_object_set_new(root, "direction", direction);
}

// General procedure of a button
void packageDeliver(boardStatePtr board, int direction) {
  if (*board->row > -1 && *board->col > -1) {
    json_t *root = json_object();
    packageJson(board, direction, root);
    passOnControl(board->serverPtr, root);
    json_decref(root);
  }
}

// Button callback for left arrow
void serverMoveLeft(GtkWidget *widget, gpointer server) {
  boardStatePtr board = (boardStatePtr)server;
  packageDeliver(board, 0);
}

// Button callback for right arrow
void serverMoveRight(GtkWidget *widget, gpointer server) {
  boardStatePtr board = (boardStatePtr)server;
  packageDeliver(board, 1);
}

// Button callback for up arrow
void serverMoveUp(GtkWidget *widget, gpointer server) {
  boardStatePtr board = (boardStatePtr)server;
  packageDeliver(board, 2);
}

// Button callback for down arrow
void serverMoveDown(GtkWidget *widget, gpointer server) {
  boardStatePtr board = (boardStatePtr)server;
  packageDeliver(board, 3); 
}
