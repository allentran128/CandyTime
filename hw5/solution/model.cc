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
#include "ClientSocket.h"

extern "C"{
#include "array2d.h"
#include "jansson.h"
}

using namespace std;

#define NULL_CHECK(ptr) (ptr == NULL)

int main(int argc, char **argv) {
  // File arg check
  if(argc !=3){
    return EXIT_FAILURE;
  }
  int errNum;
  Manager* masterPtr;
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
  string recieved;
  //writes a hello message
  string sent = msgManager.serializeHello();
  //sends message to server
  if(message.writeMessage(sent))
    return EXIT_FAILURE;
  while(1){
    //gets message from server
    if(message.getMessage(recieved))
      return EXIT_FAILURE;
    //deserializes the servers message
    int check = msgManager.deserializeAction(recieved.c_str(), masterPtr);
    if(check){
      break;
    } else if(check==-1) {
      return EXIT_FAILURE;
    }
    recieved.clear();
    //serializes the state of the board and sets it as a string 
    sent = msgManager.serializeState();
    //sends the serialized state to the server
    if(message.writeMessage(sent))
      return EXIT_FAILURE;
    sent.clear();
    //repeat until either something goes wrong or user closes out gtk
  }

  return EXIT_SUCCESS;
}
