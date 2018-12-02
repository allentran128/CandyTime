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

int main(int argc, char **argv) {
  
  cout << "Starting Vector Test" << endl;
  // Vector here
  vector<Manager*> moves;
  
  // Setup the class constructor args
  int row = 4; // use 6 times
  int * ext = (int *) malloc(sizeof(int));
  *ext = 1;

  array2dPtr arr = (array2dPtr) malloc(sizeof(array2d));
  int ** data = (int **) malloc(4*sizeof(int*));
  for (int i = 0; i < 4; i++) {
    int *payload = (int *) malloc(sizeof(int));
    *payload = 9;
    data[i] = payload;
  }
  Initiate_array2d(2, 2, (void **)data, row, arr);

  array2dPtr arr2 = (array2dPtr) malloc(sizeof(array2d));
  int ** data2 = (int **) malloc(4*sizeof(int*));
  for (int i = 0; i < 4; i++) {
    int *payload = (int *) malloc(sizeof(int));
    *payload = 9;
    data2[i] = payload;
  }
  Initiate_array2d(2, 2, (void **)data2, row, arr2);

  array2dPtr arr3 = (array2dPtr) malloc(sizeof(array2d));
  int ** data3 = (int **) malloc(4*sizeof(int*));
  for (int i = 0; i < 4; i++) {
    int *payload = (int *) malloc(sizeof(int));
    *payload = 9;
    data3[i] = payload;
  }
  Initiate_array2d(2, 2, (void **)data3, row, arr3);

  array2dPtr arr4 = (array2dPtr) malloc(sizeof(array2d));
  int ** data4 = (int **) malloc(4*sizeof(int*));
  for (int i = 0; i < 4; i++) {
    int *payload = (int *) malloc(sizeof(int));
    *payload = 9;
    data4[i] = payload;
  }
  Initiate_array2d(2, 2, (void **)data4, row, arr4);


  Manager *me = new Manager(row, row, row, 
    row, row, row, ext, arr, arr2, arr3, arr4);

  moves.push_back(me);
  
  cout << "do not clear()" << endl;
  
  cout << "manual destructor call" << endl;

  Manager *ref = moves.front();
  if(ref == me) cout <<"same before/after ref" << endl;
  cout << "pop back then destructor" <<endl;
  moves.pop_back();
  delete ref;
  if(moves.empty()) cout << "EMPTIED" << endl;
  //moves.clear();
  
  //cout << "CLEARED" << endl;
  return EXIT_SUCCESS;
}
