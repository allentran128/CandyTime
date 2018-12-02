/* Allen Tran
 *
 * Thread header
 *
 * Defines class and struct we use.
 */

#ifndef _THREAD_H_
#define _THREAD_H_

#include "manager.h"
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

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
      this->board = new Manager(orig->getBoard());
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

