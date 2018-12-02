/* Allen Tran, Conner Knight
 *
 * Desc:
 *Takes a serverName and a port and turns it into a clientSocket which it
 * uses to read and write to the server, messages given by the user and
 * outputed to the user
 *
 */

#ifndef _CLIENTMESSAGE_H_
#define _CLIENTMESSAGE_H_
#include <iostream>
#include <string>
#include <cstdlib>
#include "ClientSocket.h"

using namespace std;

class ClientMessage {
  public:
    ClientMessage(string hostName, int serverPort){
      hw5_net::ClientSocket *clientSocket = new hw5_net::ClientSocket(hostName, serverPort);
      _clientSocket = clientSocket;
    }
    ~ClientMessage();
    
    //takes in a string a and writes a message to the server with it
    int writeMessage(string messageList);
    
    //gets a message sent from the server and puts it in a string to result
    int getMessage(string &result);
  private:
    hw5_net::ClientSocket *_clientSocket;
    char _buf[200000];
};
#endif
