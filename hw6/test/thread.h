/* Allen Tran
 *
 * Thread header
 *
 * Defines class and struct we use.
 */

#ifndef _THREAD_H_
#define _THREAD_H_
#include <cstdlib>
#include "manager.h"
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

typedef struct {
  int row, col, dir;
}Moves;

class Path {
  public: 
    // Constructor
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
      //while (!moves.empty()) {
        //free(&(moves.back()));
        //moves.pop_back();
      //}
      /*
      this->board->~Manager();
      // frees each Move struct
      while(!this->moves.empty()) {
        free(&this->moves.back());
        this->moves.pop_back();
      }
      this->moves->~vector();
      */
    }

    // Get methods
    Manager *getBoard() 
        const { return this->board; }
    vector<Moves> getMoves() 
        const { return this->moves; }

  private:
    Manager *board;
    vector<Moves> moves;
};

void BFS(int id);

vector<Path*> applyMoves(Path* p);

int evaluateScore(Path* p);

#endif

