/* Conner Knight, Allen Tran
 *
 * Button Controls
 *
 * Description
 * Functions that control the actions of the buttons that effect the game's
 * state.
 *
 * DEPRECATED
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include "array2d.h"
#include "view.h"
#include "model.h"

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

// Control for Left swap button
void moveLeft(GtkWidget *widget, gpointer aboard) {
  boardStatePtr board = (boardStatePtr) aboard;
  if((*board->col) > 0 && (*board->row) >= 0){
      // 1 inidicates a swap to the left
      checkDestroy(board, 1);
      updateBoard(board);
  }
  resetPiece(board);
}

// Control for Right swap button
void moveRight(GtkWidget *widget, gpointer aboard) {
  boardStatePtr board = (boardStatePtr) aboard;
  // bounds check
  if((*board->col) < (board->holder->_col-1) && (*board->row)>=0 && (*board->col)>=0){
      // 2 indicates a swap to the right
      checkDestroy(board,2);
      updateBoard(board);
  }
  resetPiece(board);
}

// Control for Up swap button
void moveUp(GtkWidget *widget, gpointer aboard) {
  boardStatePtr board = (boardStatePtr) aboard;
  // bounds check
  if((*board->row) > 0 && (*board->col) >=0){
      // 3 indicates a swap up
      checkDestroy(board,3);
      updateBoard(board);
  }
  resetPiece(board);
}

// Control for Down swap button
void moveDown(GtkWidget *widget, gpointer aboard) {
  boardStatePtr board = (boardStatePtr) aboard;
  // bounds check
  if((*board->row) < (board->holder->_row+1) && (*board->row)>=0 && (*board->col)>=0){
      // 4 indiciates a swap down
      checkDestroy(board,4);
      updateBoard(board);
  }
  resetPiece(board);
}
