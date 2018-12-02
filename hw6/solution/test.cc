/* Allen 
 *
 * debugging Vector's constructor and destructor
 *
 *
 */

#include <cstdlib>
#include "thread.h"
#include <vector>

using namespace std;
queue<Path*> q;
Path* bestMove;
mutex lk;
int bestScore;
int movesExplored;
int numMoves;
bool done;
condition_variable cv; // need this to call ontify_one, notify_all


int main(int argc, char **argv) {
  
  cout << "Starting Vector Test" << endl;
  // Vector here
 
  // Setup the class constructor args
  int row = 4; // use 6 times
  int * ext = (int *) malloc(sizeof(int));
  *ext = 1;

  array2dPtr arr = (array2dPtr) malloc(sizeof(array2d));
  int ** data = (int **) malloc(4*sizeof(int*));
  for (int i = 0; i < 4; i++) {
    int *payload = (int *) malloc(sizeof(int));
    *payload = 1;
    data[i] = payload;
  }
  Initiate_array2d(2, 2, (void **)data, row, arr);
  free(data);

  array2dPtr arr2 = (array2dPtr) malloc(sizeof(array2d));
  int ** data2 = (int **) malloc(4*sizeof(int*));
  for (int i = 0; i < 4; i++) {
    int *payload = (int *) malloc(sizeof(int));
    *payload = 1;
    data2[i] = payload;
  }
  Initiate_array2d(2, 2, (void **)data2, row, arr2);
  free(data2);

  array2dPtr arr3 = (array2dPtr) malloc(sizeof(array2d));
  int ** data3 = (int **) malloc(4*sizeof(int*));
  for (int i = 0; i < 4; i++) {
    int *payload = (int *) malloc(sizeof(int));
    *payload = 1;
    data3[i] = payload;
  }
  Initiate_array2d(2, 2, (void **)data3, row, arr3);
  free(data3);

  array2dPtr arr4 = (array2dPtr) malloc(sizeof(array2d));
  int ** data4 = (int **) malloc(4*sizeof(int*));
  for (int i = 0; i < 4; i++) {
    int *payload = (int *) malloc(sizeof(int));
    *payload = 1;
    data4[i] = payload;
  }
  Initiate_array2d(2, 2, (void **)data4, row, arr4);
  free(data4);

  // MANAGER
  Manager *me = new Manager(row, row, row, 
    row, row, row, ext, arr, arr2, arr3, arr4);

  // MOVES
  //
  Moves * m1 = (Moves *) malloc(sizeof(Moves));
  m1->row = m1->col = m1->dir = 11;
  Moves * m2 = (Moves *) malloc(sizeof(Moves));
  m2->row = m2->col = m2->dir = 12;


  vector<Moves> moves;
  moves.push_back(*m1);
  moves.push_back(*m2);
  delete m1;
  delete m2;

 
  Path * p = new Path(me, moves);
  vector<Path *> test = applyMoves(p);
  delete p;
  //cout << "CLEARED" << endl;
  return EXIT_SUCCESS;
}
