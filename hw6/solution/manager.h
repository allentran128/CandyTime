/* Allen Tran
 * ** Currently a template for writing other classes ** 
 * Client Manager
 *
 * Description :
 * Holds all the pertaining information to the updated version of the game
 */
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <iostream>
#include <cstdlib>
#include "array2d.h"

#define NULL_CHECKS(ptr) (ptr == NULL)

using namespace std;
class Manager {
  public:
    // Constructor
    Manager();
    Manager(
      int row, int col, 
      int gameid, int colors, int movesmade, 
      int score, int* ext_offset, 
      array2dPtr extColor, array2dPtr defState, 
      array2dPtr gameState, array2dPtr candyboard) :
      _row(row), _col(col), _gameid(gameid), _colors(colors),
      _movesmade(movesmade), _score(score),_ext_offset(ext_offset), 
      _extColor(extColor), _defState(defState), _gameState(gameState), _candyboard(candyboard) {}
    
    Manager(Manager * orig) { 
      this->_row = orig->getSelectedRow();
      this->_col = orig->getSelectedCol();
      this->_gameid = orig->getGameId();
      this->_colors = orig->getColors();
      this->_movesmade = orig->getMovesMade();
      this->_score = orig->getScore();

      try {
        this->_extColor = (array2dPtr) malloc(sizeof(array2d));
        if (NULL_CHECKS(this->_extColor)) {
          throw 1;
        }
        this->_defState = (array2dPtr) malloc(sizeof(array2d));
        if (NULL_CHECKS(this->_defState)) {
          throw 1;
        }
        this->_candyboard = (array2dPtr) malloc(sizeof(array2d));
        if (NULL_CHECKS(this->_candyboard)) {
          throw 1;
        }  
        this->_gameState = (array2dPtr) malloc(sizeof(array2d));
        if (NULL_CHECKS(this->_gameState)) {
          throw 1;
        }

        if(copyArray(orig->getExtendedBoard(),this->_extColor) == EXIT_FAILURE) throw 2;
        if(copyArray(orig->getDefState(),this->_defState) == EXIT_FAILURE) throw 2;
        if(copyArray(orig->getCandyBoard(),this->_candyboard) == EXIT_FAILURE) throw 2;
        if(copyArray(orig->getGameState(),this->_gameState) == EXIT_FAILURE) throw 2;

        this->_ext_offset  = (int*)malloc(this->_candyboard->_col * sizeof(int));
        if (NULL_CHECKS(this->_ext_offset)) {
          throw 1;
        }
        int* old_ext_offset = orig->getCurrentExtendedOffset();
        for(int i = 0; i < this->_candyboard->_col ; i++){
          this->_ext_offset[i] = old_ext_offset[i];
        }
      } catch (int e) {
        if (e == 1) {
          cout << "New Manager Malloc fields failed" << endl;
        } else {
          cout << "copyArray failed" << endl;
        }
      }  
    }

    // Destructor
    ~Manager();
    
    // Board function logic

    // checks to see if there is candies missing in the board and if there
    // is then fills it up with the candies above it, if there are no 
    // candies above then takes candies from the extended board
    void gravity();

    //searches the board for 4 and 3 in a row of vertical and horzontal
    //match of candies and eliminates those candies from the board
    int searchBoard(int direction);
    
    // Get fields
    int getSelectedRow() const { return this->_row; }
    int getSelectedCol() const { return this->_col; }
    array2dPtr getCandyBoard() const { return this->_candyboard; }
    array2dPtr getExtendedBoard() const { return this->_extColor; }
    array2dPtr getDefState() const { return this->_defState; }
    int getMovesMade() const { return this->_movesmade; }
    int getColors() const { return this->_colors; }
    int getGameId() const { return this->_gameid; }
    int getScore() const { return this->_score; }
    array2dPtr getGameState() const { return this->_gameState; }
    int * getCurrentExtendedOffset() const { return this->_ext_offset; }
    //set fields
    void setRow(int rowValue){ _row = rowValue;}
    void setCol(int colValue){ _col = colValue;}
 private:
    // Fields
    //GameDefBoard seed;
    //GameStateBoard current;
    int _row,_col,_score,_gameid, _colors, _movesmade;
    int* _ext_offset;
    array2dPtr _extColor;
    array2dPtr _defState;
    array2dPtr _gameState;
    array2dPtr  _candyboard;

    //helper function of gravity pulls down a candy for the missing candy
    //at row and col
    void pullDown(int row, int col);

    //helper function of gravity, gets a candy in extended board for the
    //board spot at row and col
    void xSwap(int row, int col);

    //helper function of the searchBoard, swaps two candies in the candyboard,
    //the one indicated at _row and_col and the other one is dependent on
    //the direction where
    //1:left 2:right 3:up 4:down -1: no direction
    int directionSwap(int direction);

    //helper function of searchBoard searches through the board looking for
    //a horizontal match of length interv
    int searchHori(int interv);

    //helper function of searchBoard searches through the board looking for
    // a vertical match of length interv
    int searchVert(int interv);

    //helper function of searchBoard gives user a point given the gamepiece
    //is in a initial location
    void score(int row,int col);

    //gets the int value from an array2dPtr at given row and col
    int A2DintGet(int row, int col, array2dPtr holder);
};

#endif //_MANAGER_H_  
