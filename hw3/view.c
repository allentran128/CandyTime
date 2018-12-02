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

// Redraws the game display given a board 
void updateBoard(boardStatePtr board) {
  GtkWidget * image;

  // Redraws the board
  for (int i = 0; i < board->holder->_row; i++) {
    for (int j = 0; j < board->holder->_col; j++) {
      GtkWidget *button = board->buttons[i * board->holder->_col + j];
      int* value = board->holder->_arr[i * board->holder->_col + j]; 
      image = gtk_image_new_from_pixbuf(board->colors[*value]);
      gtk_button_set_image((GtkButton*)button, image);
    }
  }

  // Updates the move counter
  board->counter++;
  char buff[8];
  sprintf(buff, "moves: %d", board->counter);
  gtk_label_set_text((GtkLabel*)board->moves, buff);
}

// Free dynamic memory in proper order
void destroy_on_exit(int identifier, boardStatePtr board) { 
  switch (identifier) {
    case 0 :
      for (int i = 0; i < 6; i++){
        g_object_unref(board->colors[i]);
      }
    case 1 :
      free(board->buttons);
    case 2 :
      destroyArray2d(board->holder);
    case 3 : 
      free(board->holder);
    case 4 :
      free(board);
      gtk_main_quit();
  }
}


// Customizes the view
static void activate (GtkApplication* app, gpointer aboard) {
  boardStatePtr board = (boardStatePtr) aboard;
  
  // Setup main window
  GtkWidget *window; 
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Candy Time");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

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
      int* numPtr =board-> holder->_arr[i*board->holder->_col + j];
      // Sets it's icon image
      image = gtk_image_new_from_pixbuf(board->colors[*numPtr]);
      gtk_button_set_image((GtkButton *)button, image);
      // Event upon clicking on the button
      g_signal_connect(button, "clicked", G_CALLBACK(selected_candy), (gpointer)board);
      // Add the button to the grid
      board->buttons[i * board->holder->_col + j] = (GtkWidget *) button;
      gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
    }
  }
  
  // Moves Counter Widget
  board->counter = 0;
  board->moves = gtk_label_new("moves: 0");
  gtk_grid_attach(GTK_GRID(grid), board->moves, 13,0,1,1);

  // Control Buttons
  GtkWidget* buttonLeft = gtk_button_new();
  image = gtk_image_new_from_file("./images/direction/left.png");
  gtk_button_set_image((GtkButton *)buttonLeft, image);
  g_signal_connect(buttonLeft, "clicked", G_CALLBACK(moveLeft), (gpointer)board);
  gtk_grid_attach(GTK_GRID(grid), buttonLeft, 13, 1,1,1);

  GtkWidget* buttonRight = gtk_button_new();
  image = gtk_image_new_from_file("./images/direction/right.png");
  gtk_button_set_image((GtkButton *)buttonRight, image);
  g_signal_connect(buttonRight, "clicked", G_CALLBACK(moveRight), (gpointer)board);
  gtk_grid_attach(GTK_GRID(grid), buttonRight, 13, 2,1,1);

  GtkWidget* buttonUp = gtk_button_new();
  image = gtk_image_new_from_file("./images/direction/up.png");
  gtk_button_set_image((GtkButton *)buttonUp, image);
  g_signal_connect(buttonUp, "clicked", G_CALLBACK(moveUp), (gpointer)board);
  gtk_grid_attach(GTK_GRID(grid), buttonUp, 13, 3,1,1);

  GtkWidget* buttonDown = gtk_button_new();
  image = gtk_image_new_from_file("./images/direction/down.png");
  gtk_button_set_image((GtkButton *)buttonDown, image);
  g_signal_connect(buttonDown, "clicked", G_CALLBACK(moveDown),(gpointer) board);
  gtk_grid_attach(GTK_GRID(grid), buttonDown, 13, 4,1,1);

  // Update Window
  gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
  GtkApplication *app;
  int status, errNum;

  // Initialize game state
  boardStatePtr board= (boardStatePtr) malloc(sizeof(boardState));
  if(board == NULL){
    return EXIT_FAILURE;
  }
  board->prevSelected = NULL;
  board->row = board->col = -1;

  // Checks if holder is allocated correctly
  board->holder = (array2dPtr) malloc(sizeof(array2d));
  if (board->holder == NULL) {
    destroy_on_exit(4, board);
    return EXIT_FAILURE;
  }

  // Checks if file correctlyinitiated to holder correctly
  errNum = processInit(board->holder,argv[1]);
  if (errNum != 0) {
    // free, exit
    destroy_on_exit(3, board);
    return EXIT_FAILURE;
  }

  // Checks if buttons is allocated correctly
  board->buttons = (GtkWidget **) malloc(board->holder->_row * board->holder->_col * sizeof(GtkWidget*));
  if(board->buttons == NULL) {
    destroy_on_exit(2, board);
  }

  // Sets the colors array of board
  processColors(board);

  // Makes app
  app = gtk_application_new("edu.cs.alltran", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), (gpointer) board);
  
  // Runs the app
  status = g_application_run(G_APPLICATION(app), 0, NULL);

  // Free, return
  destroy_on_exit(0,board);
  g_object_unref(app);
  return status;
}

