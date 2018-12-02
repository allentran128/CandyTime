/* Allen Tran, Conner Knight
 *
 * Visual Interface
 *
 * Description : Creates and runs the main application.
 * In charge of creating the image of the board and its interface.
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <strings.h>
#include "array2d.h"
#include <stdlib.h>
#include "control.h"
#include "model.h"

#define MISSING_IMG(image) if (image == NULL) { return EXIT_FAILURE; }
  
// Redraws the game display given a board 
void updateBoard(boardStatePtr board) {
  GtkWidget * image;

  // Redraws the board
  for (int i = 0; i < board->holder->_row; i++) {
    for (int j = 0; j < board->holder->_col; j++) {
      GtkWidget *button = board->buttons[i * board->holder->_col + j];
      int* value = (int*)board->holder->_arr[i * board->holder->_col + j];
      if(*(int*)board->gameState->_arr[i * board->gameState->_col + j] == 0){ 
        image = gtk_image_new_from_pixbuf(board->colors[*value]);
      } else {
        image = gtk_image_new_from_pixbuf(board->altColors[*value]);
      }
      gtk_button_set_image((GtkButton*)button, image);
    }
  }

  // Updates the move counter
  updateMove(board);

  // Updates the score counter
  updateScore(board);
}

void updateMove(boardStatePtr board) {
  char buff[8];
  sprintf(buff, "moves: %d", board->counter);
  gtk_label_set_text((GtkLabel*)board->moves, buff);
}

void updateScore(boardStatePtr board) {
  char scor[6];
  sprintf(scor, "score: %d", *(board->score));
  gtk_label_set_text((GtkLabel*)board->scoreBtn, scor);
} 

// Customizes the view
static int activate (GtkApplication* app, gpointer aboard) { 
  boardStatePtr board = (boardStatePtr) aboard;
  
  // Setup main window
  GtkWidget *window; 
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Candy Time");

  // set custom size based on row/col
  int width, height;
  width = board->holder->_col * 50 + 100;
  height = board->holder->_row * 50;
  gtk_window_set_default_size(GTK_WINDOW(window), width, height);

  // Setup interface
  GtkWidget *button;
  GtkWidget *grid;
  GtkWidget *image;

  // Setup the grid
  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);  

  // Assigns all the buttons
  for (int i = 0; i < board->holder->_row; i++) {
    for (int j = 0; j < board->holder->_col; j++) {
      // Create a  new button
      button = gtk_toggle_button_new();
      int* numPtr = (int*)board->holder->_arr[i*board->holder->_col + j];

      // Sets it's icon image
      if(*(int*)board->gameState->_arr[i * board->gameState->_col + j] == 0){
        image = gtk_image_new_from_pixbuf(board->colors[*numPtr]);
      } else {
        image = gtk_image_new_from_pixbuf(board->altColors[*numPtr]);
      }
      // candy images are corrupted
      MISSING_IMG(image);

      gtk_button_set_image((GtkButton *)button, image);
      
      // Event upon clicking on the button
      g_signal_connect(button, "clicked", G_CALLBACK(selected_candy), (gpointer)board);

      // Add the button to the grid
      board->buttons[i * board->holder->_col + j] = (GtkWidget *) button;
      gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
    }
  }
  
  // Moves Counter Widget
  // First time init of moves and scoreBtn
  board->moves = gtk_label_new("moves: 0");
  gtk_grid_attach(GTK_GRID(grid), board->moves, 13,0,1,1);
  updateMove(board);
  
  board->scoreBtn = gtk_label_new("score: 0");
  gtk_grid_attach(GTK_GRID(grid), board->scoreBtn, 13,1,1,1);
  updateScore(board);
  
  // Create and assign the control buttons to the grid 
  int errNum = assignControlButtons(grid, board);
  if (errNum != EXIT_SUCCESS) {
    // exit out, missing direction images
    return EXIT_FAILURE;
  }

  // Update Window
  gtk_widget_show_all(window);
  return EXIT_SUCCESS;
}

// Creates and attachs the control buttons to the grid
// Returns 
// 	EXIT_FAILURE if a file couldn't be loaded
// 	EXIT_SUCCESS otherwise
int assignControlButtons(GtkWidget * grid, boardStatePtr board) { 
  // Control Buttons
  GtkWidget* buttonLeft = gtk_button_new();
  GtkWidget * image = gtk_image_new_from_file("./images/direction/left.png");
  MISSING_IMG(image);
  gtk_button_set_image((GtkButton *)buttonLeft, image);
  g_signal_connect(buttonLeft, "clicked", G_CALLBACK(moveLeft), (gpointer)board);
  gtk_grid_attach(GTK_GRID(grid), buttonLeft, 13, 2,1,1);

  GtkWidget* buttonRight = gtk_button_new();
  image = gtk_image_new_from_file("./images/direction/right.png");
  MISSING_IMG(image); 
  gtk_button_set_image((GtkButton *)buttonRight, image);
  g_signal_connect(buttonRight, "clicked", G_CALLBACK(moveRight), (gpointer)board);
  gtk_grid_attach(GTK_GRID(grid), buttonRight, 13, 3,1,1);

  GtkWidget* buttonUp = gtk_button_new();
  image = gtk_image_new_from_file("./images/direction/up.png");
  MISSING_IMG(image); 
  gtk_button_set_image((GtkButton *)buttonUp, image);
  g_signal_connect(buttonUp, "clicked", G_CALLBACK(moveUp), (gpointer)board);
  gtk_grid_attach(GTK_GRID(grid), buttonUp, 13, 4,1,1);

  GtkWidget* buttonDown = gtk_button_new();
  image = gtk_image_new_from_file("./images/direction/down.png");
  MISSING_IMG(image); 
  gtk_button_set_image((GtkButton *)buttonDown, image);
  g_signal_connect(buttonDown, "clicked", G_CALLBACK(moveDown),(gpointer) board);
  gtk_grid_attach(GTK_GRID(grid), buttonDown, 13, 5,1,1);
  return EXIT_SUCCESS;
}

int startGtk(boardStatePtr board) {
  //Manager master;
  GtkApplication *app;
  int status;

  // Makes app
  app = gtk_application_new("edu.cs.alltran", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), (gpointer) board);
  
  // Runs the app
  status = g_application_run(G_APPLICATION(app), 0, NULL);

  // Exits out of the app
  g_object_unref(app);
  return status;
}

