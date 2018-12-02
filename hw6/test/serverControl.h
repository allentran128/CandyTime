/* Allen 
 *
 * Server Controls
 *
 * Description
 * Sends instructions upon direction click on GTK to
 * the server
 *
 */
#ifndef _SERVER_CONTROLS_H_
#define _SERVER_CONTROLS_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "gtk/gtk.h"
#include "view.h"

// Updates the game state upon a candy button click
void selected_candy(GtkWidget *widget, gpointer aboard);

// Untoggles the previously selected button
void resetPiece(boardStatePtr board);

// Makes and send the instructions to the server
void packageDeliver(boardStatePtr board, int direction);

// Sends the instructions to swap to the left to the server
void serverMoveLeft(GtkWidget *widget, gpointer aboard);

// Sends the instructions to swap to the right to the server
void serverMoveRight(GtkWidget *widget, gpointer aboard);

// Sends the instructions to swap up to the server
void serverMoveUp(GtkWidget *widget, gpointer aboard);

// Sends the instructions to swap down to the server
void serverMoveDown(GtkWidget *widget, gpointer aboard);
#ifdef __cplusplus
}
#endif

#endif 
