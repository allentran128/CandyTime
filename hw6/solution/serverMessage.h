/* Allen
 *
 * Server class
 * holds relevant fields
 *
 * Methods
 *
      // INTERPRET the .JSON file -> game state, # moves, # score.
      //
      // PERFORM the task asked by the UPDATE BOARD (redraw)
      //
      // WAIT for a move specified
      //
      // WRITE to a .JSON file
      //
      // SEND to Client
      //
      // REPLACE this with what we want to write to peer
 */
#ifndef _SERVER_MESSAGE_H_
#define _SERVER_MESSAGE_H_

#ifdef __cplusplus
#include <cstdlib>
#include <string>
#include "ClientSocket.h"
#include "manager.h"
#include <iostream>
#endif
using namespace std;
#include "jansson.h"
#include "view.h"
#include "array2d.h"

#define NULL_CHECK(ptr) (ptr == NULL)

class Server {
  public :
    // Constructor
    Server(char *filename, hw5_net::ClientSocket *peer) : _peerSocket(peer) {
      // Contact client first
      char buf[2048];
      int readCount = this->_peerSocket->WrappedRead(buf, 2047);
      buf[readCount] = '\0';
      helloack(buf, filename);

      // Wait for a game instance to update view with
      readCount = this->_peerSocket->WrappedRead(buf, 2047);      
      buf[readCount] = '\0';
      json_t *root = readMessageInServer(buf);

      // construct boardStatePtr
      this->_serverInfo = (boardStatePtr) malloc(sizeof(boardState));
      if (NULL_CHECK(this->_serverInfo)) {
        // exit after doing :
        // clearing peersocket
      }
      
      // Malloc the fields of board
      boardInit(this->_serverInfo); 

      // load in candy images
      processColors(this->_serverInfo);
      
      // load in file to board
      processInit(root);
      json_decref(root); 
      // Malloc GtkWidget buttons array 
      int row = this->_serverInfo->holder->_row;
      int col = this->_serverInfo->holder->_col;
      this->_serverInfo->buttons = (GtkWidget **)malloc(row*col*sizeof(GtkWidget *));   
      if(this->_serverInfo->buttons == NULL) {
        // end game
      }
      // Link Manager from board
      this->_clientInfo = (Manager *) this->_serverInfo->masterPtr;
    }

    // Destructor
    ~Server() {
      // Serialize board, exports a save file
      serialize(this->_clientInfo);
      // free everything in the board, not shared with manager
      destroyBoard(this->_serverInfo);
      // free the manager
      this->_clientInfo->Manager::~Manager();
      // then call bye 
      bye();
    };

    // Methods
    void helloack(char *buf, char *filename);
    void bye();
    void cleanBoard();
    int processInit(json_t *root);
    int boardInit(boardStatePtr board);
    json_t * readMessageInServer(char * buf);
    void sendMessageToClient(json_t *root);
    void cleanInit(json_t *root, array2dPtr first, array2dPtr second, array2dPtr third, array2dPtr fourth);
    void destroyBoard(boardStatePtr board);
    void deepCleanArr(array2dPtr arr);
    void reorder(array2dPtr holder, int row, int col);
    int serialize(Manager * masterPtr); 

    // Get methods
    boardStatePtr getServerInfo() const { return this->_serverInfo; }
    Manager *getClientInfo() const { return this->_clientInfo; }
    hw5_net::ClientSocket *getPeerSocket() const { return this->_peerSocket; }

  private :
    boardStatePtr _serverInfo;
    Manager *_clientInfo;
    hw5_net::ClientSocket *_peerSocket;
};

#endif
