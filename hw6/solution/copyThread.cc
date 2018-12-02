#include "thread.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>

using namespace std;

int id;

int evaluateScore(Path* p){
  int evaluateScore = p->getBoard()->getScore() / p->getBoard()->getMovesMade();
  return evaluateScore;
}

vector<Path*> applyMoves(Path* p){//Manager * board, vector<Moves> m){
  vector<Path*>result;
  for(int i = 0; i < p->getBoard()->getCandyBoard()->_row ; i++){
    for(int j = 0; j < p->getBoard()->getCandyBoard()->_col; j++){
      for(int k = 1; k < 4; k++){
        // Constructs a deep copy of Manager
        Manager * btmp = new Manager(p->getBoard());
        btmp->setRow(i);
        btmp->setCol(j);
        if(!btmp->searchBoard(k)){
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
  id = pid;
  //std::unique_lock<std::mutex> lck(lk);
  //cout << "[" << id << "]  " << "thread started" << endl; 
  while (1){
    //lock_guard<mutex> lock(lk);
    Path *data;
    int localMoves;
    Path *localBestMove;
    {
      unique_lock<mutex> lck(lk);
      //cout << "[" << id << "]  " << "gets lock" << endl;
      while(q.empty() & !done){
        //cout << "[" << id << "]  " << "waits" << endl;
        cv.wait(lck);
      }
      //cout << "[" << id << "]  " << "is notified" << endl;
      if(done){
        lck.unlock();
        break;
      }
      //cout << "[" << id << "]  " << "accesses queue" << endl;
      data = new Path(q.front());
      // unqueue here
      q.pop();
      localMoves = numMoves;
      if(bestMove != NULL){
        localBestMove = new Path(bestMove);
      }else {
        localMoves = 0;
        localBestMove = NULL;
      }
    }
    int badPath = 0;
    for(int i = 0;i < localMoves ; i++){
      //has to be more sophisticated
      if(data->getMoves()[i].row != localBestMove->getMoves()[i].row || data->getMoves()[i].col != localBestMove->getMoves()[i].col || data->getMoves()[i].dir != localBestMove->getMoves()[i].dir) {
        badPath = 1;
        break;
      }
    }
    if(badPath){
      //cout << "[" << id << "]  " << "bad path, unlock, end" << endl;
    } else{
      //cout << "[" << id << "]  " << "unlocks and processes data" << endl;
      vector<Path*> newPaths = applyMoves(data);
      int locBScore = 0;
      Path* locBest;
      for(auto p: newPaths){
        int tempScore = evaluateScore(p);
        if (tempScore > locBScore){
          locBScore = tempScore;
          locBest = p;
        }
      }
      {
        unique_lock<mutex> lck(lk);
        if(numMoves == localMoves) {
          if(locBScore > bestScore){
            bestScore = locBScore;
            bestMove = locBest;
          }
          for(auto p: newPaths){
            q.push(p);
          }
        }
      }
      movesExplored++;
      cv.notify_all();
    }
  delete data;
  }
}
