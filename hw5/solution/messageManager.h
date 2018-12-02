/* Allen Tran, Conner Knight
 *
 * Desc: Takes in messages from the user and deserializes them and executes
 * the correct things based on what the message says. Also serializes the
 * state of the board and sends it to the user
 *
 */

#ifndef _MESSAGEMANAGER_H_
#define _MESSAGEMANAGER_H_
#include <iostream>
#include <string>
#include <cstdlib>
#include "manager.h"
#include "manager.h"
extern "C"{
#include "array2d.h"
#include "jansson.h"
}

using namespace std;

class MessageManager{
  public:
    MessageManager(){}

    ~MessageManager();
    //getMasterPtr
    Manager* getMasterPtr() const { return this->_masterPtr;}
     
    //outputs a spring that represents a json file with the format
    //{"action":"hello"}
    string serializeHello();
    
    //outputs a string that represents a json file that contains all
    //the information about the state of the game, with the format
    //{"action":"update" "gameinstance":...} where the state of the
    //game is in gameinstance
    string serializeState();
    
    //takes in a json object that the server sent that contains the state
    //of the board  and creates a manager object from that with the
    //correct fields that correspond to the state of the board indicated
    //by the json object
    int deserializeState(json_t * input);
    
    //takes in a character array representing the information recieved 
    //from the server about what action should be performed, and then
    //performs that action, if it needs to change manager then changes
    //the passed in manager pointer
    int deserializeAction(const char * entry, Manager*& masterPtr);
    
  private:
    Manager* _masterPtr;

    //reorders a array2d so that the rows are flipped, so that the board
    //is in the right state
    void reorder(array2dPtr holder, int row, int col);

    //goes through array2d's and the json object and frees the memory it
    //occupies
    void cleanInit(json_t *root, array2dPtr first, array2dPtr second, array2dPtr third, array2dPtr fourth);

    //frees 2d Arrays
    void deepCleanArr(array2dPtr arr);

};
#endif
