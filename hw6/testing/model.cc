/* Allen Tran, Conner Knight
 *
 * Desc:
 * Model part of the process, makes the board from a 
 * json file and processes all the possible colors from files
 *
 */

#include "manager.h"
#include "messageManager.h"
#include "clientMessage.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include "ClientSocket.h"
#include <queue>
#include "thread.h"
extern "C"{
#include "array2d.h"
#include "jansson.h"
}

// Globals
queue<Path*> q;
Path* bestMove;
mutex lk;
int bestScore;
int movesExplored;
int numMoves;
bool done;
condition_variable cv;

using namespace std;

#define NULL_CHECK(ptr) (ptr == NULL)

void cleanQ() {
  while(!q.empty()) {
    delete q.front();
    q.pop();
  }
}

int main(int argc, char **argv) {
  // File arg check
  if(argc !=3){
    return EXIT_FAILURE;
  }

  // Setup the globals
  // q is empty
  // bestMove is empty
  // lk is init already
  bestScore = 0;
  movesExplored = 0;
  numMoves = 0;
  done = false;
  // End Globals
  
  // Start up threads
  vector<thread> threads;
  for (int i = 0; i < 8; i++) {
    threads.push_back(thread(BFS,i));
  } // Each thread should shortly fall into wait


  // Setting up Socket Connection
  int errNum;
  int exit = 0;
  string hostName = argv[1];
  int serverPort;
  try{
     serverPort = stoi(argv[2]);
  } catch(...){
    return EXIT_FAILURE;
  }
  //set up the Client Socket with the given hostname and port
  ClientMessage message(hostName, serverPort);
  //sets up a manager for sending and recieving messages
  MessageManager msgManager;
  //writes a hello message
  string sent = msgManager.serializeHello();
  //cout << sent << endl;
  string recieved;
  //sends message to server
  if(message.writeMessage(sent))
    return EXIT_FAILURE;
  // SEND-RECIEVE Loop
  while(1){
    cout << "in while" << endl;
    //cout << recieved << endl;
    //gets message from server
    if(message.getMessage(recieved))
      return EXIT_FAILURE;
    cout << recieved << endl;
    //deserializes the servers message
    int check;
    { 
      unique_lock<mutex> lck(lk);
      check = msgManager.deserializeAction(recieved.c_str());
      cv.notify_one();
    }

    if(check == 0){
      // bye case
      cout << "formal bye msg" << endl;
      break;
    } else if(check== -1) {
      // error in msg sent
      cout << "msg corrupt" << endl;
      return EXIT_FAILURE;
    } else if(check == 1) {
      // helloack
    } else {
      // else, send the board
      recieved.clear();
      {
        cout << "proper recieved, on begin" << endl;
        unique_lock<mutex> lck(lk);
        // Setup and send the message to server
        sent = msgManager.serializeState();
        if(message.writeMessage(sent))
          return EXIT_FAILURE;
        cout << sent << endl;
        // Update global
        bestScore = 0;
        numMoves++;
        cleanQ();

        // Restart search
        vector<Moves> next;
        Manager *start = new Manager(msgManager.getMasterPtr());
        Path *begin = new Path(start,next);
        q.push(begin);

        delete bestMove;
        bestMove = new Path(begin);

        sent.clear();
        cv.notify_one();
        cout << "proper recieved, on exit" << endl;
      }
    }      
  }

  // Closing Procedures
  {
    unique_lock<mutex> lck(lk);
    done = true;
  }
  cv.notify_all(); // threads should start finishing up
  int joined = 0;
  for (auto & th: threads) {
    th.join();
    joined++;
  }
  threads.clear();

  delete bestMove; 
  cleanQ();
  return EXIT_SUCCESS;
}


