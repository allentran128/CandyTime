/* Allen
 *
 * Server (view) interface
 *
 * Recieves "hello", "update" messages from the client side
 *
 * Sends "helloack", "move", "bye" messages to client side
 *
 * Responsible for sending a gameinstance, moves and a bye msg
 *
 */
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

#include "ClientSocket.h"
#include "serverMessage.h"
extern "C" {
#include "manager.h"
#include "array2d.h"
#include "view.h"
#include "jansson.h"
}

// Mallocs a new BoardStatePtr
int Server::boardInit(boardStatePtr board) {
  //Malloc Checks
  board->prevSelected = NULL;
  
  board->row = (int*)malloc(sizeof(int*));
  if (NULL_CHECK(board->row)) {
    destroy_on_exit(7, board);
    return EXIT_FAILURE;
  }

  board->col = (int*)malloc(sizeof(int*));
  if (NULL_CHECK(board->col)) {
    destroy_on_exit(6, board);
    return EXIT_FAILURE;
  }

  board->score = (int*)malloc(sizeof(int*));
  if (NULL_CHECK(board->score)) {
    destroy_on_exit(5, board);
    return EXIT_FAILURE;
  }

  *(board->row) = *(board->col) = -1;
  
  return EXIT_SUCCESS;
}

// Sends a message to the client passed in as a parameter
// Then catches a response and updates the board
void Server::sendMessageToClient(json_t *root) {
  char buf[2048];
  char *msg = json_dumps(root, JSON_COMPACT);

  json_decref(root);
  string actionMsg(msg);
  try {
    this->_peerSocket->WrappedWrite(actionMsg.c_str(), actionMsg.length());
  } catch (...) {
    cout << "Could not contanct the client, please exit" << endl;
  }
  free(msg);
 
  // Catch a response
  int readCount = this->_peerSocket->WrappedRead(buf, 2047);
  buf[readCount] = '\0';
  json_t *newroot = readMessageInServer(buf);

  // Update the board's view
  cleanBoard();
  processInit(newroot);
  json_decref(newroot);
  this->_clientInfo = (Manager *)this->_serverInfo->masterPtr;
  updateBoard(this->_serverInfo);
}

// Cleans the boardState
void Server::cleanBoard() {
  *(this->_serverInfo->row) = *(this->_serverInfo->col) = -1;
  this->_serverInfo->prevSelected = NULL;
  this->_serverInfo->masterPtr = NULL;
  delete this->_clientInfo;
} 

// Reads in the buffer and assigns newroot the contents
json_t * Server::readMessageInServer(char *buf) {
  string msg(buf);
  json_error_t error;
  return json_loads(msg.c_str(), 0, &error);
}

// Will read in auth of peer and send out gameInstance
void Server::helloack(char *buf, char *filename) {
  if (buf != NULL && filename != NULL) {
    // Authenticate communication
    json_error_t error;
    json_t *root = json_loads(buf,0, &error);
    if (root == NULL) {
      // exit
      cout << "No response from client" << endl;
    }

    // Message confirmation 
    json_t *msg = json_object_get(root, "action");
    if (msg == NULL) {
      cout << "Invalid response from client" << endl;
    }

    string hellomsg = json_string_value(msg);
    string key("hello");
    if(key.compare(hellomsg) != 0) { 
      cout << "Invalid helloack, wrong client" << endl;
    }
    // free the json file 
    json_decref(root);

    // Success, parse JSON to buf and send to client
    root = json_load_file(filename, 0, &error);
    if (root == NULL) {
      // cannot load file
      cout << "Invalid file to start the game" << endl;
    }else {
      // Modify root 
      json_t *newroot = json_object();
      json_object_set_new(newroot, "action", json_string("helloack"));
      json_object_set_new(newroot, "gameinstance", root);
      char * data = json_dumps(newroot,0);
      string gameInstance(data);
      (this->_peerSocket)->WrappedWrite(gameInstance.c_str(), gameInstance.length());
      free(data);
      json_decref(newroot);
    }
  }
  //failed
}

// Bye message
void Server::bye() {
  json_t *root = json_object();
  json_object_set_new(root, "action", json_string("bye"));
  char *byeMsg = json_dumps(root, JSON_COMPACT);
  string msg(byeMsg);
  this->_peerSocket->WrappedWrite(msg.c_str() , msg.length());
  free(byeMsg);
  json_decref(root);
}

// Writes out the game state to a save file
int Server::serialize(Manager * masterPtr) {
  array2dPtr extColor = masterPtr->getExtendedBoard();
  array2dPtr gameState = masterPtr->getGameState();
  array2dPtr candyboard = masterPtr->getCandyBoard();

  reorder(extColor, extColor->_row, extColor->_col);
  reorder(gameState, gameState->_row,gameState->_col);
  reorder(candyboard, candyboard->_row, candyboard->_col);

  size_t index;
  json_t *value;
  json_error_t error;
  
  // Prepares creates json objects top down then assigns bottom up
  json_t *root = json_object();
  
  // Game Def portion
  json_t *gamedef = json_object();
  
  // Game State portion
  json_t *gamestate = json_object();
  
  //gameid
  int gameid_int = masterPtr->getGameId();
  json_t* gameid_obj = json_integer(gameid_int);
  json_object_set_new(gamedef, "gameid", gameid_obj);
  
  //extensioncolor
  json_t *extensioncolor_obj = json_object();
  // extension color row
  array2dPtr boardExtended = masterPtr->getExtendedBoard();
  int EXT_row_int = boardExtended->_row;
  json_object_set_new(extensioncolor_obj, "rows", json_integer(EXT_row_int));
  // extension color col
  int EXT_col_int = boardExtended->_col;
  json_object_set_new(extensioncolor_obj, "columns", json_integer(EXT_col_int));
  
  // extension color data
  json_t *EXT_data_arr = json_array();
  // Size of this board is the game board, with default 1,0's to clear
  for (int j = 0; j < boardExtended->_row * boardExtended->_col; j++) {
    int value = *((int *)(boardExtended->_arr[j]));
    json_t *entry = json_pack("i", value);
    json_array_append(EXT_data_arr, entry);
    json_decref(entry);
  }
  json_object_set_new(extensioncolor_obj, "data", EXT_data_arr);
  
  // Add extended colors back into the wrapper object
  json_object_set_new(gamedef, "extensioncolor", extensioncolor_obj);
  
  //boardstate
  json_t *DEF_boardstate_obj = json_object();
  // board state row
  //
  array2dPtr boardDef = masterPtr->getDefState();
  int DEF_row_int = boardDef->_row;
  int DEF_col_int = boardDef->_col;

  //Size of this board is the extended board
  json_t *DEF_data_arr = json_array();
  for (int k = 0; k < boardDef->_row * boardDef->_col; k++) {
    int value = *((int *)(boardDef->_arr[k]));
    json_t *entry = json_pack("i", value);
    json_array_append(DEF_data_arr, entry);
    json_decref(entry);
  }
  
  json_object_set_new(DEF_boardstate_obj, "rows", json_integer(DEF_row_int));
  json_object_set_new(DEF_boardstate_obj, "columns", json_integer(DEF_col_int));
  json_object_set_new(DEF_boardstate_obj, "data", DEF_data_arr);
  // Add boardstate back into wrapper object
  json_object_set_new(gamedef, "boardstate", DEF_boardstate_obj);

  //colors
  int colors_int = masterPtr->getColors();
  json_object_set_new(gamedef, "colors", json_integer(colors_int));

  // DONE WITH GAME DEF

  // Gamestate
  //boardcandies of current gameplay
  json_t *boardcandies = json_object();
  // boardcandies row
  array2dPtr jellyBoard = masterPtr->getCandyBoard();
  int candyrow = jellyBoard->_row;
  json_object_set_new(boardcandies, "rows", json_integer(candyrow));
  // boardcandies col
  int candycol = jellyBoard->_col;
  json_object_set_new(boardcandies, "columns", json_integer(candycol));

  json_t * candydata = json_array();
  // Size is same as game board, holds the info on the candies
  for (int l = 0; l < candyrow * candycol; l++) {
    int color = *((int*)jellyBoard->_arr[l]);
    json_t *entrycolor = json_integer(color);
    json_t *entrypair = json_object();
    json_t *entrytype = json_integer(0);
    json_object_set_new(entrypair, "color", entrycolor);
    json_object_set_new(entrypair, "type", entrytype);
    json_array_append(candydata, entrypair);
    json_decref(entrypair);
  }
  json_object_set_new(boardcandies, "data", candydata);

  // boardcandies done
  json_object_set_new(gamestate, "boardcandies", boardcandies);

  // Start boardstate
  json_t *boardstate = json_object();
  array2dPtr usedJellyBoard = masterPtr->getGameState();
  // board state row
  int staterow = usedJellyBoard->_row;
  json_object_set_new(boardstate, "rows", json_integer(staterow));

  // board state col
  int statecol = usedJellyBoard->_col;
  json_object_set_new(boardstate, "columns", json_integer(statecol));

  // board state data
  json_t *statedata = json_array();
  // Size of this array is size of the baord, holds 1, 0's
  for (int j = 0; j < usedJellyBoard->_row * usedJellyBoard->_col; j++) {
    int value = *((int *)(usedJellyBoard->_arr[j]));
    json_t *entry = json_pack("i", value);
    json_array_append(statedata, entry);
    json_decref(entry);
  }
  json_object_set_new(boardstate, "data", statedata);

  json_object_set_new(gamestate, "boardstate", boardstate);
  // boardstate is done

  // moves made
  int movesmade = masterPtr->getMovesMade();
  json_object_set_new(gamestate, "movesmade", json_integer(movesmade));

  // current score
  int currentscore = masterPtr->getScore();
  json_object_set_new(gamestate, "currentscore", json_integer(currentscore));

  json_t *extoffset = json_array();
  // Size of this array is 1 x col
  int * currExt = masterPtr->getCurrentExtendedOffset();
  for (int i = 0; i < masterPtr->getExtendedBoard()->_col; i++) {
    int value = currExt[i];
    json_t *entry = json_pack("i", value);
    json_array_append(extoffset, entry);
    json_decref(entry);
  }

  json_object_set_new(gamestate, "extensionoffset", extoffset);

  // Add back to root
  json_object_set_new(root, "gamedef", gamedef);
  json_object_set_new(root, "gamestate", gamestate);
  
  json_dump_file(root, "savedGame.out", JSON_INDENT(5));
  json_decref(root);
}

// Destroys the board without interfereing with the manager
void Server::destroyBoard(boardStatePtr board) {
  for (int i = 0; i < 6; i++){
    g_object_unref(board->colors[i]);
    g_object_unref(board->altColors[i]);
    g_object_unref(board->newColors[i]);
  }
  free(board->buttons);
  free(board->score);
  free(board->col);
  free(board->row);
  free(board);
}

// Helper method that frees the parameters 
void Server::cleanInit(json_t *root, array2dPtr first, array2dPtr second, array2dPtr third, array2dPtr fourth) {
  if(!NULL_CHECK(root)) {
    json_decref(root);
  }
  deepCleanArr(first);
  deepCleanArr(second);
  deepCleanArr(third);
  deepCleanArr(fourth);
}
 
// Helper method that frees the array2dPtr
void Server::deepCleanArr(array2dPtr arr) {
  if(!NULL_CHECK(arr)) {
    if (!NULL_CHECK(arr->_arr)) {
      destroyArray2d(arr);
    }
    free(arr);
  }
}

// Converts the order of the holder array to match the board's layout
void Server::reorder(array2dPtr holder, int row, int col){
  for(int i = 0; i < (row/2); i++){
    for(int j = 0; j < col; j++){
      swap(i,j,(row-1)-i,j,holder);
    }
  }
}

// The deserializing task, initializes the Manager given a
// json newroot object
int Server::processInit(json_t *newroot) {
  boardStatePtr board = this->_serverInfo;
  if (board != NULL && newroot != NULL) {

    // Declares and mallocs variables
    int gameid, EXT_row, EXT_col, colors,errNum;

    // Malloc checks
    array2dPtr extColor = (array2dPtr) malloc(sizeof(array2d));
    if (NULL_CHECK(extColor)) {
      // Nothing to free
      return EXIT_FAILURE;
    }

    array2dPtr defState = (array2dPtr) malloc(sizeof(array2d));
    if (NULL_CHECK(defState)) {
      cleanInit(NULL, extColor, NULL, NULL, NULL);
      return EXIT_FAILURE;
    }

    array2dPtr gameState = (array2dPtr) malloc(sizeof(array2d));
    if (NULL_CHECK(gameState)) {
       cleanInit(NULL, extColor, defState, NULL, NULL);
       return EXIT_FAILURE;
    }

    array2dPtr candyboard = (array2dPtr) malloc(sizeof(array2d));
    if (NULL_CHECK(candyboard)) {
        cleanInit(NULL, extColor, defState, gameState, NULL);
        return EXIT_FAILURE;
    }
    

    // Json variables
    size_t index;
    json_t *value;
    json_error_t error;

    if (NULL_CHECK(newroot)) {
      cleanInit(NULL, extColor, defState, gameState, candyboard);
      return EXIT_FAILURE;
    }
    
    json_t *root = json_object_get(newroot, "gameinstance");
    if (root == NULL) {
      // in the first time loading case
      root = newroot;
    }


    // Reads the JSON file - Do we need to deref json_t* from reads?
    //gamedef
    json_t *gamedef = json_object_get(root, "gamedef");

    //gameid
    json_t *gameid_obj = json_object_get(gamedef, "gameid");
    gameid = json_integer_value(gameid_obj);
    /* gameid is now assigned */

    //extensioncolor
    json_t *extensioncolor_obj = json_object_get(gamedef, "extensioncolor");
    json_t *EXT_row_obj = json_object_get(extensioncolor_obj,"rows");
    json_t *EXT_col_obj = json_object_get(extensioncolor_obj,"columns");
    json_t *EXT_data_obj = json_object_get(extensioncolor_obj,"data");

    //boardstate
    json_t *DEF_boardstate_obj = json_object_get(gamedef, "boardstate");
    json_t *DEF_row_obj = json_object_get(DEF_boardstate_obj,"rows");
    json_t *DEF_col_obj = json_object_get(DEF_boardstate_obj,"columns");
    json_t *DEF_data_obj = json_object_get(DEF_boardstate_obj,"data");

    //colors
    json_t *colors_obj = json_object_get(gamedef, "colors");
    colors = json_integer_value(colors_obj);
    /* colors is now assigned */

    //extension color 2d array set up
    EXT_row = json_integer_value(EXT_row_obj);
    EXT_col = json_integer_value(EXT_col_obj);
    void** EXT_data = (void **) malloc(EXT_row * EXT_col * sizeof(void *));
    if (NULL_CHECK(EXT_data)) {
      cleanInit(newroot, extColor, defState, gameState, candyboard);
      return EXIT_FAILURE;
    }
    json_array_foreach(EXT_data_obj, index, value){
      int *payload = (int *) malloc(sizeof(int *));
      if (NULL_CHECK(payload)) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      *payload = json_integer_value(value);
      EXT_data[index] = (void *) payload;
    }
    errNum = Initiate_array2d(EXT_row, EXT_col, (void **)EXT_data, EXT_row * EXT_col, extColor);
    free(EXT_data);
    if(errNum != 0){
      cleanInit(newroot, extColor, defState, gameState, candyboard);
      return EXIT_FAILURE;
    }
    /* Extended board is now set up */

    index = 0;
    //boardstate 2d array set up
    //Board Definition
    int DEF_row = json_integer_value(DEF_row_obj);
    int DEF_col = json_integer_value(DEF_col_obj);
    // Make the game definition board
    void** DEF_data = (void **) malloc(DEF_row * DEF_col * sizeof(void *));
    if (NULL_CHECK(DEF_data) ) {
      cleanInit(newroot, extColor, defState, gameState, candyboard);
      return EXIT_FAILURE;
    }
    json_array_foreach(DEF_data_obj, index, value) {
      int *payload = (int *) malloc(sizeof(int *));
      if (NULL_CHECK(payload) ) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      *payload = json_integer_value(value);
      DEF_data[index] = (void *) payload;
    }
    errNum = Initiate_array2d(DEF_row, DEF_col, DEF_data, DEF_row * DEF_col, defState);
    free(DEF_data);
    if (errNum != 0) {
      cleanInit(root, extColor, defState, gameState, candyboard);
      return EXIT_FAILURE;
    }
    /* Board def is now set up */

    json_t *gamestate = json_object_get(root, "gamestate");
    // Checks if there is a previously saved game before making one
    int *ext_offset;
    if (gamestate != NULL) {
      //boardcandies
      json_t *boardcandies = json_object_get(gamestate, "boardcandies");
      json_t *CAN_row_obj = json_object_get(boardcandies, "rows");
      json_t *CAN_col_obj = json_object_get(boardcandies, "columns");
      json_t *CAN_data_obj = json_object_get(boardcandies, "data");
      int CAN_row = json_integer_value(CAN_row_obj);
      int CAN_col = json_integer_value(CAN_col_obj);

      void** CAN_data = (void **) malloc(CAN_row * CAN_col * sizeof(void *));
      if (NULL_CHECK(CAN_data)) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      json_t * data_objs[CAN_row * CAN_col];
      json_array_foreach(CAN_data_obj, index, value) {
        data_objs[index] = json_object_get(value, "color");
        int *payload = (int *) malloc(sizeof(int *));
        if (NULL_CHECK(payload)) {
          cleanInit(newroot, extColor, defState, gameState, candyboard);
          return EXIT_FAILURE;
        }
        *payload = json_integer_value(data_objs[index]);
        CAN_data[index] = (void *) payload;
      }
      errNum = Initiate_array2d(CAN_row, CAN_col, CAN_data, CAN_row * CAN_col, candyboard);
      free(CAN_data);

      if (errNum != 0) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      board->holder = candyboard;
      /* game saved state = candy board is assigned */

      index = 0;
      //boardstate
      json_t *boardstate = json_object_get(gamestate, "boardstate");
      json_t *STA_row_obj = json_object_get(boardstate, "rows");
      json_t *STA_col_obj = json_object_get(boardstate, "columns");
      json_t *STA_data_obj = json_object_get(boardstate, "data");
      int STA_row = json_integer_value(STA_row_obj);
      int STA_col = json_integer_value(STA_col_obj);
      void** STA_data = (void **) malloc(STA_row * STA_col * sizeof(void *));
      if (NULL_CHECK(STA_data)) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      json_array_foreach(STA_data_obj, index, value){
        int *payload = (int *) malloc(sizeof(int *));
        if (NULL_CHECK(payload)) {
          cleanInit(newroot, extColor, defState, gameState, candyboard);
          return EXIT_FAILURE;
        }
        *payload = json_integer_value(value);
        STA_data[index] = (void *) payload;
      }

      errNum = Initiate_array2d(STA_row, STA_col, STA_data, STA_row * STA_col, gameState);
      free(STA_data);
      if(errNum != 0){
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      board->gameState = gameState;
      // Board state (jellies) is assigned

      index = 0;
      //movesmade
      json_t *movesmade_obj = json_object_get(gamestate, "movesmade");
      int movesmade = json_integer_value(movesmade_obj);
      board->counter = movesmade;
      // board->counter is assigned

      //currentscore
      json_t *score_obj = json_object_get(gamestate, "currentscore");
      int currentscore = json_integer_value(score_obj);
      *board->score = currentscore;
      // board->score is assigned

      //extension offset
      json_t *extensionoffset_obj = json_object_get(gamestate, "extensionoffset");
      ext_offset = (int*)malloc(candyboard->_col *sizeof(int));
      if (NULL_CHECK(ext_offset)) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }

      json_array_foreach(extensionoffset_obj, index, value){
        ext_offset[index] = json_integer_value(value);
      }
    }else {
      // Set default values to start a new game(state)
      board->counter = 0;
      *board->score = 0; 
      // Use the game def's default jelly board 
      void **DEFAULT_data = (void**)malloc(DEF_row * DEF_col *sizeof(void *));
      if (NULL_CHECK(DEFAULT_data)) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      int entry;
      // hard copy over jelly board
      for (int i = 0; i < DEF_row * DEF_col; i++) {
        entry = *((int *)defState->_arr[i]);
        int *centry = (int *) malloc(sizeof(int *));
        *centry = entry;
        DEFAULT_data[i] =(void*) centry;
      }

      // New game state
      errNum = Initiate_array2d(DEF_row, DEF_col,DEFAULT_data, DEF_row * DEF_col, gameState);
      free(DEFAULT_data);

      if (errNum != 0){
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }

      board->gameState = gameState;
      // Default Extended offset
      ext_offset = (int*)malloc(DEF_col * sizeof(int));
      if (NULL_CHECK(ext_offset)) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }

      // Set default values of 0's
      for (int j = 0; j < gameState->_col; j++) {
        ext_offset[j] = 0;
      }

      // Create the default board->holder from Definition
      // Then we update the extended offsets
      //
      // Holder
      // need to make a struct 
      entry = -1;
      void **CAN_holder = (void**)malloc(DEF_row * DEF_col * sizeof(void *));
      if (NULL_CHECK(CAN_holder)) {
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }

      for(int j = 0; j < DEF_row * DEF_col; j++){
        int *centry = (int *) malloc(sizeof(int *));
        *centry = entry;
        CAN_holder[j] = (void*)centry;
      }
      errNum = Initiate_array2d(DEF_row, DEF_col,CAN_holder, DEF_row * DEF_col, candyboard);
      free(CAN_holder);
      if (errNum != 0){
        cleanInit(newroot, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      board->holder = candyboard;
    }
    // Reorders the board to match desired layout 
    reorder(extColor, extColor->_row, extColor->_col);
    reorder(gameState, gameState->_row,gameState->_col);
    reorder(board->holder, board->holder->_row, board->holder->_col);
    // Creates an instance of the manager
    Manager * master = new Manager(*board->row, *board->col, gameid, colors, board->counter, *board->score, ext_offset, extColor,defState,board->gameState,board->holder);
    board->masterPtr = (void*)master;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
