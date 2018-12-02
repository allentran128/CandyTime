/* Allen Tran, Conner Knight
 *
 * Documentation for Control's methods.
 * Contains the callback functions for the buttons.
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "gtk/gtk.h"
#include "view.h"

// Updates the game state upon a candy button click
void selected_candy(GtkWidget *widget, gpointer aboard);

// Untoggles the previously selected button
void resetPieces(boardStatePtr board);

// Switches the element are "row" and "col" for the array2d with the
// element to it's left (if valid)
void moveLeft(GtkWidget *widget, gpointer aboard);

// Switches the element are "row" and "col" for the array2d with the
// element to it's right (if valid)
void moveRight(GtkWidget *widget, gpointer aboard);

// Switches the element are "row" and "col" for the array2d with the
// element above it (if valid)
void moveUp(GtkWidget *widget, gpointer aboard);

// Switches the element are "row" and "col" for the array2d with the
// element below it (if valid)
void moveDown(GtkWidget *widget, gpointer aboard);

#endif
