/* Allen Tran, Conner Knight
 *
 * Desc:
 *Takes a serverName and a port and turns it into a clientSocket which it
 * uses to read and write to the server, messages given by the user and
 * outputed to the user
 *
 */


#include <iostream>
#include <string>
#include "clientMessage.h"
#include <cstdlib>
#include "ClientSocket.h"

using namespace std;

ClientMessage::~ClientMessage(){
  delete _clientSocket;
}

int ClientMessage::writeMessage(string messageList){
  try{   
    _clientSocket->WrappedWrite(messageList.c_str(), messageList.length());
    return EXIT_SUCCESS;
  } catch(...){
    return EXIT_FAILURE;
  }
}


int ClientMessage::getMessage(string& result){
  int readCount;
  int tmpRead;
  try{
    //do{
      readCount = _clientSocket->WrappedRead(_buf, 199999);
      //cout << tmpRead << endl;
      //readCount += tmpRead;
    //} while(tmpRead !=0);
  } catch(...){
  cout << "this failed" << endl;
    return EXIT_FAILURE;
  }
  //cout << _buf << endl;
  int i = 0;
  _buf[readCount] = '\0';
  cout << readCount << endl;
  cout << _buf << endl;
  string str;
  int fCount = 0, bCount = 0;
  do{
    if(_buf[i] == '{'){
      fCount++;
    }
    else if (_buf[i] == '}'){
      bCount++;
    }
    str+=_buf[i];
    i++;
  }while(fCount != bCount);
  result.assign(str);
}
