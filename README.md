# CandyTime

## Description
I implemented a clone of the popular mobile application, Candy Crush, in C/C++. All of the data (game state) is serialized in JSON and sent over a network between the server and client.

## Development
First I implemented a linked list and 2D array as the underlying data structures for this project. 

Then desgined the view interface of the board. Used a mapping of integers to colors to represent the candies on the board. Utilized the GTK+ library to create a GUI of the board. Added functionality to select a candy on the board and directional buttons to allow swapping a selected candy in that direction.

Next I implemented the game logic; when 3+ of the same candy are in a row or column they are erased and the candies above them fall down, updating the score, generating new candies at the top of the board, and so on.

Then I encoded the representation of the game into a game definition and a game state in JSON. Defined the model to be the client and the view to be the server. Then established a connection between them using TCP protocol. The game initialized after a handshake between the client and the server.
