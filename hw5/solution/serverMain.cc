/* Allen
 *
 * Main Interface
 *
 * Description
 * Starts up an open port to connect to a client
 * to play the game
 *
 * Upon exiting we produce a save file
 */
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

#include "ServerSocket.h"
#include "ClientSocket.h"
#include "serverMessage.h"

void usage(const char *exeName) {
  cout << "Usage: " << exeName << "json filename" << endl;
  exit(1);
}

int main(int argc, char *argv[]) {
  
  // Wrong arguments passed
  if ( argc != 1 && argc != 2) usage(argv[0]);
  
  // Setup port
  int port;
  hw5_net::ClientSocket *peerSocket = NULL;

  try {
    // Setting up socket
    int socketFd;
    hw5_net::ServerSocket sock(port);
    sock.BindAndListen(AF_INET, &socketFd);
    cout << "Your hosting port is at : " << sock.port() << endl;

    // wait for client connection
    int acceptedFd;
    string clientAddr;
    uint16_t clientPort;
    string clientDNSName;
    string serverAddress;
    string serverDNSName;

    sock.Accept( &acceptedFd,
		 &clientAddr,
		 &clientPort,
		 &clientDNSName,
		 &serverAddress,
		 &serverDNSName );

    // wrap connection to peer with a CientSocket
    peerSocket = new hw5_net::ClientSocket(acceptedFd);
  }catch (...) {
    cout << "Error connecting to the client, please try again" << endl;
  }

  if (peerSocket != NULL) {
    Server *server = new Server(argv[1], peerSocket);
    // init all the fields AND startup GTK
    server->getServerInfo()->serverPtr = server;
    startGtk(server->getServerInfo());
    delete server;
    delete peerSocket;
  }

  cout << "Exiting" << endl;
  return EXIT_SUCCESS;
}

