/* Conner, Allen
 *
 * BFS for threads
 *
 * Description : 
 * Breadth First Search Algorithm for Candy Time
 * to explore the possible paths to arrive at the
 * best score. The algorithm runs concurrently
 * among threads with safety procedures for accessing
 * shared data.
 */

#include "thread.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <exception>

using namespace std;
extern queue<Path*> q;
extern Path* bestMove;
extern mutex lk;
extern float bestScore;
extern int movesExplored;
extern int numMoves;
extern bool done;
extern condition_variable cv;


float evaluateScore(Path* p, int dataScore){
  float evaluateScore = (float)(p->getBoard()->getScore() - dataScore) + (1.0f / (1.0f + p->getBoard()->getMovesMade() - numMoves));
  evaluateScore++;
  return evaluateScore;
}

// Finds the valid next moves and returns them in a vector.
vector<Path*> applyMoves(Path* p){
  vector<Path*>result;
  for(int i = 0; i < p->getBoard()->getCandyBoard()->_row ; i++){
    for(int j = 0; j < p->getBoard()->getCandyBoard()->_col; j++){
      for(int k = 1; k < 4; k += 2){
        // Constructs a deep copy of Manager
        Manager * btmp = new Manager(p->getBoard());
        btmp->setRow(i);
        btmp->setCol(j);
        if(!btmp->searchBoard(k)) {
          vector<Moves> mtmp = vector<Moves>(p->getMoves());
          mtmp.push_back(Moves{i,j,k});
          Path* ptmp;
          try {
            ptmp = new Path(btmp,mtmp); 
          } catch (bad_alloc) {
            cout << "bad alloc" << endl;
          }
          result.push_back(ptmp);
        } else {
          delete btmp; 
        }
      }
    }
  }
  return result;
}

void BFS(int pid){
  int id = pid;
  while (1){
    Path *data;
    int localMoves;
    {
      unique_lock<mutex> lck(lk);
      //if there is nothing in the queue then wait till there is
      while(q.empty() && !done){
        cv.wait(lck);
      }
      //if the game is done then unlock and get out of while loop
      if(done){
        lck.unlock();
        break;
      }
      
      localMoves = numMoves;
      
      try {
        data = new Path(q.front());
      } catch (...) {
        cout << "bad alloc" << endl;
      }
      delete q.front();
      q.pop(); 
    }
   
    // Process Path 
    vector<Path*> newPaths = applyMoves(data);
    int dataScore = data->getBoard()->getScore();
    float locBScore = 0;
    Path* locBest;

    // Check for the local best out of the group
    for(auto p: newPaths){
      float tempScore = evaluateScore(p, dataScore);
      if (tempScore > locBScore){
        locBScore = tempScore;
        locBest = p;
      }
    }

    // Have to delete the Path *'s that arent used anymore
    bool relevant = false;
    {
      unique_lock<mutex> lck(lk);
      // Check if the work we did eaerlier is still relavent
      if (numMoves == localMoves) {
        relevant = true;
        if (locBScore > bestScore) {
          delete bestMove;
          bestScore = locBScore;
          try {
            bestMove = new Path(locBest);
          } catch (...) {
            cout << "bad alloc" << endl;
          }
        }
        for(auto p: newPaths){
          q.push(p);
        }
        cv.notify_all();
      }
    }
    
    movesExplored++;
    if (!relevant) {
      // Delete everything
      while(!newPaths.empty()) {
        delete newPaths.back();
        newPaths.pop_back();
      }
    }
  delete data;
  }
}
