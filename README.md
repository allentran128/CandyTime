# CandyTime

## Description
Clone of popular mobile application, Candy Crush, written in C/C++ for the desktop.

The intention of this project was to introduce principles of systems programming: defining the architecture, modules, interfaces, and data of the system.
I chose the MVC architectural pattern to decouple the application into three separate entities to achieve parallel development.
I implemented the data structure to represent the game board and designed its public interface. 
I used the GTK+ library to create a GUI of the board which inlcudes directional buttons to perform swaps with adjacent candies.
I learned how to serialize and deserialize data that is used to represent the game state over a TCP network connection.
The server application runs on memory, so data is not persisstent if the server crashes. (which is outside the scope of the project)
