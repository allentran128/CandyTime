/* Allen Tran, Conner Knight
 *
 * Desc: Takes in messages from the user and deserializes them and executes
 * the correct things based on what the message says. Also serializes the
 * state of the board and sends it to the user
 *
 */
#include <iostream>
#include <string>
#include <cstdlib>
#include "manager.h"
#include "messageManager.h"
extern "C"{
#include "array2d.h"
#include "jansson.h"
}
#define NULL_CHECK(ptr) (ptr == NULL)

using namespace std;

MessageManager::~MessageManager(){
  delete this->_masterPtr;
}

void MessageManager::cleanInit(json_t *root, array2dPtr first, array2dPtr second, array2dPtr third, array2dPtr fourth) {
  if(!NULL_CHECK(root)) {
    json_decref(root);
  }
  deepCleanArr(first);
  deepCleanArr(second);
  deepCleanArr(third);
  deepCleanArr(fourth);
}
 
void MessageManager::deepCleanArr(array2dPtr arr) {
  if(!NULL_CHECK(arr)) {
    if (!NULL_CHECK(arr->_arr)) {
      destroyArray2d(arr);
    }
    free(arr);
  }
}

int MessageManager::deserializeAction(const char * entry,Manager*& masterPtr){
  json_error_t error;
  json_t *root = json_loads(entry,0,&error);
  if (root == NULL){
    return EXIT_FAILURE;
  }
  //action
  json_t *action_obj = json_object_get(root, "action");
  string action = json_string_value(action_obj);
  if(!action.compare("helloack")){
    deserializeState(root);
    json_decref(root);
    masterPtr = _masterPtr; 
    return EXIT_SUCCESS;
  }
  else if(!action.compare("bye")){
    json_decref(root);
    return 1;
  } else if(!action.compare("move")){
    //row
    json_t *row_obj = json_object_get(root, "row");
    int row = _masterPtr->getCandyBoard()->_row - 1 - json_integer_value(row_obj);
    _masterPtr->setRow(row);
    //col
    json_t *col_obj = json_object_get(root, "column");
    _masterPtr->setCol(json_integer_value(col_obj));
    //direction
    json_t *direction_obj = json_object_get(root, "direction");
    int direction = json_integer_value(direction_obj) + 1;
    _masterPtr->searchBoard(direction);
    //search successfull
    json_decref(root);
    //fclose (jFile);
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}

// The deserializing task, initializes the Manager given a properly 
// malloc'd boardStatePtr
int MessageManager::deserializeState(json_t * input){
  //FILE * jFile;
  //jFile = fopen ("myfile.bin", "wb");
  //fwrite (str, sizeof(char), str.length, jFile);
  //json_error_t error;
  json_t *root = json_object_get(input, "gameinstance");
  if (NULL_CHECK(root)) {
    //cleanInit(NULL, extColor, defState, gameState, candyboard);
    return EXIT_FAILURE;
  }
  if ( input != NULL) {
    // Declares and mallocs variables
    int gameid,EXT_row, EXT_col, colors,errNum;
    // Check for NULLS
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
      cleanInit(root, extColor, defState, gameState, candyboard);
      return EXIT_FAILURE;
    }

    json_array_foreach(EXT_data_obj, index, value){
      int *payload = (int *) malloc(sizeof(int *));
      if (NULL_CHECK(payload)) {
        cleanInit(root, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      *payload = json_integer_value(value);
      EXT_data[index] = (void *) payload;
    }

    errNum = Initiate_array2d(EXT_row, EXT_col, (void **)EXT_data, EXT_row * EXT_col, extColor);
    free(EXT_data);
    if(errNum != 0){
      cleanInit(root, extColor, defState, gameState, candyboard);
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
      cleanInit(root, extColor, defState, gameState, candyboard);
      return EXIT_FAILURE;
    }
    
    json_array_foreach(DEF_data_obj, index, value) {
      int *payload = (int *) malloc(sizeof(int *));
      if (NULL_CHECK(payload) ) {
        cleanInit(root, extColor, defState, gameState, candyboard);
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
    
    // Checks if there is an error making the array2d object,
    // if so it will free the previously malloc'd data and exit
    json_t *gamestate = json_object_get(root, "gamestate");
    // Checks if there is a previously saved game before making one
    int *ext_offset;
    int movesmade = 0, score = 0;
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
        cleanInit(root, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }

      json_t * data_objs[CAN_row * CAN_col];
      json_array_foreach(CAN_data_obj, index, value) {
        data_objs[index] = json_object_get(value, "color"); 
        int *payload = (int *) malloc(sizeof(int *));
        if (NULL_CHECK(payload)) {
          cleanInit(root, extColor, defState, gameState, candyboard);
          return EXIT_FAILURE;
        }
        *payload = json_integer_value(data_objs[index]);
        CAN_data[index] = (void *) payload;
      }

      errNum = Initiate_array2d(CAN_row, CAN_col, CAN_data, CAN_row * CAN_col, candyboard);
      free(CAN_data);

      if (errNum != 0) {
        cleanInit(root, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      //board->holder = candyboard;
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
        cleanInit(root, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }

      json_array_foreach(STA_data_obj, index, value){
        int *payload = (int *) malloc(sizeof(int *));
        if (NULL_CHECK(payload)) {
          cleanInit(root, extColor, defState, gameState, candyboard);
          return EXIT_FAILURE;
        }
        
        *payload = json_integer_value(value);
        STA_data[index] = (void *) payload;
      }

      errNum = Initiate_array2d(STA_row, STA_col, STA_data, STA_row * STA_col, gameState);
      free(STA_data);
      if(errNum != 0){
        cleanInit(root, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      
      //board->gameState = gameState;
      // Board state (jellies) is assigned

      index = 0;
      //movesmade
      json_t *movesmade_obj = json_object_get(gamestate, "movesmade");
      movesmade = json_integer_value(movesmade_obj);
      //board->counter = movesmade;
      // board->counter is assigned

      //currentscore
      json_t *score_obj = json_object_get(gamestate, "currentscore");
      score = json_integer_value(score_obj);
      //*board->score = currentscore;
      // board->score is assigned

      //extension offset
      json_t *extensionoffset_obj = json_object_get(gamestate, "extensionoffset");
      ext_offset = (int*)malloc(candyboard->_col *sizeof(int));
      if (NULL_CHECK(ext_offset)) {
        cleanInit(root, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }

      json_array_foreach(extensionoffset_obj, index, value){
        ext_offset[index] = json_integer_value(value) % extColor->_row;
      }
    }else {
      // Set default values to start a new game(state)
      //board->counter = 0;
      //*board->score = 0;

      // Use the game def's default jelly board
      void **DEFAULT_data = (void**)malloc(DEF_row * DEF_col *sizeof(void *));
      if (NULL_CHECK(DEFAULT_data)) {
        cleanInit(root, extColor, defState, gameState, candyboard);
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
        cleanInit(root, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      //board->gameState = gameState;
      // Default Extended offset
      ext_offset = (int*)malloc(DEF_col * sizeof(int));      
      if (NULL_CHECK(ext_offset)) {
        cleanInit(root, extColor, defState, gameState, candyboard);
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
        cleanInit(root, extColor, defState, gameState, candyboard);
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
        cleanInit(root, extColor, defState, gameState, candyboard);
        return EXIT_FAILURE;
      }
      //board->holder = candyboard;
    }
    // Reorders the board to match desired layout 
    reorder(extColor, extColor->_row, extColor->_col);
    reorder(gameState, gameState->_row,gameState->_col);
    reorder(candyboard, candyboard->_row, candyboard->_col);

    // Creates an instance of the manager
    Manager * masterPtr = new Manager(-1, -1, gameid, colors,movesmade, score, ext_offset, extColor,defState,gameState,candyboard);

    masterPtr->gravity();
    masterPtr->searchBoard(-1);
    //board->masterPtr = (void*)masterPtr;
    _masterPtr = masterPtr;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
//writes hello
string MessageManager::serializeHello() {
  json_t *root = json_object();
  
  //json_t *action = json_object();
  json_object_set_new(root, "action",json_string("hello"));
  char* oldRes = json_dumps(root,0);
  string result;
  result.assign(oldRes);
  free(oldRes);
  json_decref(root);
  return result;

}
// Writes out the game state to a save file
string MessageManager::serializeState() {
  
  array2dPtr extColor = _masterPtr->getExtendedBoard();
  array2dPtr gameState = _masterPtr->getGameState();
  array2dPtr candyboard = _masterPtr->getCandyBoard();
  reorder(extColor, extColor->_row, extColor->_col);
  reorder(gameState, gameState->_row,gameState->_col);
  reorder(candyboard, candyboard->_row, candyboard->_col);
 
  size_t index;
  json_t *value;
  json_error_t error;
  
  // Prepares creates json objects top down then assigns bottom up
  json_t *root = json_object();

  //int movesmade = _masterPtr->getMovesMade();
  //json_t *action = json_object();
  //json_t * actionName = json_string("update");
  //json_object_set_new(root, "action", actionName);
  json_object_set_new(root, "action",json_string("update"));
  json_t *gameInstance = json_object();
  // Game Def portion
  json_t *gamedef = json_object();
  
  // Game State portion
  json_t *gamestate = json_object();
  //gameid
  int gameid_int = _masterPtr->getGameId();
  json_t* gameid_obj = json_integer(gameid_int);
  json_object_set_new(gamedef, "gameid", gameid_obj);
  //extensioncolor
  json_t *extensioncolor_obj = json_object();
  // extension color row
  array2dPtr boardExtended = _masterPtr->getExtendedBoard();
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
  array2dPtr boardDef = _masterPtr->getDefState();
  int DEF_row_int = boardDef->_row;
  int DEF_col_int = boardDef->_col;
  
  // Size of this board is the extended board
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
  int colors_int = _masterPtr->getColors();
  json_object_set_new(gamedef, "colors", json_integer(colors_int));
  
  // DONE WITH GAME DEF
  
  
  // Gamestate
  //boardcandies of current gameplay
  json_t *boardcandies = json_object();
  // boardcandies row
  array2dPtr jellyBoard = _masterPtr->getCandyBoard();
  int candyrow = jellyBoard->_row;
  json_object_set_new(boardcandies, "rows", json_integer(candyrow));
  // boardcandies col
  int candycol = jellyBoard->_col;
  json_object_set_new(boardcandies, "columns", json_integer(candycol));
  
  json_t * candydata = json_array();
  // Size is same as game board, holds the info on the candies
  for (int l = 0; l < (_masterPtr->getCandyBoard())->_row * (_masterPtr->getCandyBoard())->_col; l++) {
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
  array2dPtr usedJellyBoard = _masterPtr->getGameState();
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
  int movesmade = _masterPtr->getMovesMade();
  json_object_set_new(gamestate, "movesmade", json_integer(movesmade));
  
  // current score
  int currentscore = _masterPtr->getScore();
  json_object_set_new(gamestate, "currentscore", json_integer(currentscore));
  
  json_t *extoffset = json_array();
  // Size of this array is 1 x col
  int * currExt = _masterPtr->getCurrentExtendedOffset();
  for (int i = 0; i < (_masterPtr->getCandyBoard())->_col; i++) {
    int value = currExt[i];
    json_t *entry = json_pack("i", value);
    json_array_append(extoffset, entry);
    json_decref(entry);
  }
 
  json_object_set_new(gamestate, "extensionoffset", extoffset);
  
  // Add back to root
  json_object_set_new(gameInstance, "gamedef", gamedef);
  json_object_set_new(gameInstance, "gamestate", gamestate);
  json_object_set_new(root, "gameinstance", gameInstance);
  char* oldRes = json_dumps(root,0);
  string result;
  result.assign(oldRes);
  free(oldRes);
  json_decref(root);
  reorder(extColor, extColor->_row, extColor->_col);
  reorder(gameState, gameState->_row,gameState->_col);
  reorder(candyboard, candyboard->_row, candyboard->_col);
  return result;
}

// Converts the order of the holder array to match the board's layout
void MessageManager::reorder(array2dPtr holder, int row, int col){
  for(int i = 0; i < (row/2); i++){
    for(int j = 0; j < col; j++){
      swap(i,j,(row-1)-i,j,holder);
    }
  }
}
