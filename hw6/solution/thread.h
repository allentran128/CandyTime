/* Allen Tran
 *
 * Thread header
 *
 * Thread uses a breath first search technique to find the optimal moves to make on the
 * board and records it into the bestMove global variable for model to use in making
 * moves on the board.
 */

#ifndef _THREAD_H_
#define _THREAD_H_

#include "manager.h"
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

typedef struct {
  int row, col, dir;
}Moves;

class Path {
  public: 
    // Constructors
    Path(){}
    
    Path(Manager *currBoard,vector<Moves> currMoves) 
        : board(currBoard), moves(currMoves) {}

    Path(Path *orig) {
      try {
        this->board = new Manager(orig->getBoard());
      } catch (...) {
        cout << "bad alloc" << endl;
      }
      this->moves = vector<Moves>(orig->getMoves());
    }

    // Destructor
    ~Path() {
      delete board;
    }

    // Get methods
    Manager *getBoard() const { return this->board; }
    vector<Moves> getMoves() const { return this->moves; }

  private:
    Manager *board;
    vector<Moves> moves;
};

// Thread Safe Breadth First Search Algorithm 
void BFS(int id);

// Returns a vector of valid Paths that are one move ahead
// of p.
vector<Path*> applyMoves(Path* p);

// Evaluate the score of the move relative to the board. 
float evaluateScore(Path* p, int dataScore);

#endif

