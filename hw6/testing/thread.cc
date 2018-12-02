#include "thread.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>

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
  float evaluateScore = (p->getBoard()->getScore() - dataScore) + (1.0f / (1.0f + p->getBoard()->getMovesMade() - numMoves));
  return evaluateScore;
}

// Finds the valid next moves and returns them in a vector.
vector<Path*> applyMoves(Path* p){
  vector<Path*>result;
  int row = p->getBoard()->getCandyBoard()->_row;
  int col = p->getBoard()->getCandyBoard()->_col;
  for(int i = 0; i < row ; i++){
    for(int j = 0; j < col; j++){
      for(int k = 1; k < 4; k += 2){
        // Constructs a deep copy of Manager
        Manager * btmp = new Manager(p->getBoard());
        btmp->setRow(i);
        btmp->setCol(j);
        if(!btmp->searchBoard(k)) {
          vector<Moves> mtmp = vector<Moves>(p->getMoves());
          mtmp.push_back(Moves{i,j,k});
          Path* ptmp = new Path(btmp,mtmp); 
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
      while(q.empty() && !done){
        cv.wait(lck);
      }
      if(done){
        lck.unlock();
        break;
      }
      
      localMoves = numMoves;

      data = new Path(q.front());
      delete q.front();
      q.pop(); 
    }
   
    // Process Path 
    vector<Path*> newPaths = applyMoves(data);
    int dataScore = data->getBoard()->getScore();
    float locBScore = 0.0f;
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
      if (numMoves <= localMoves) {
        relevant = true;
        if (locBScore > bestScore) {
          delete bestMove;
          cout << "found new best" << endl;
          cout << locBScore << endl;
          bestScore = locBScore;
          bestMove = new Path(locBest);
        }
        for(auto p: newPaths){
          if (evaluateScore(p, dataScore) > 0) {
            q.push(p);
          }else {
            delete p;
          }
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
