#include "manager.h"
extern "C" {
#include "array2d.h"
}
//deconstructor
Manager::~Manager(){
  free(_ext_offset);
  destroyArray2d(_extColor);
  free(_extColor);
  destroyArray2d(_defState);
  free(_defState);
  destroyArray2d(_gameState);
  free(_gameState);
}

//checks every element in the board to see if it needs to
//be replaced and if so then brings the board pieces above it down to
//take it's spot
void Manager::gravity(){
  //applies gravity to every piece on board
  for(int i = 0; i < this->_candyboard->_col; i++){
    for(int k = (this->_candyboard->_row-1); k >=0; k--){
      // find a candy slot that needs to be filled in
      if(A2DintGet(k,i,this->_candyboard) == -1){
        //pulls down if candy has been destroyed
        pullDown(k, i);
      }
    }
  }
}

//takes the row and column of a array2d _candyboard and replaces that element with a
//piece that still exists, if there are no board pieces above it to swap with
//go into the extended board to get a piece.
void Manager::pullDown(int row, int col){
  int index = row-1;
  int found = 0;
  //goes until piece to pull down is found
  while(found == 0){
    if(index >= 0){
      //case for if there is a piece in the board that can fall down to the spot
      //of the piece that has been destroyed
      if(A2DintGet(index, col,this->_candyboard) != -1){
        swap(row, col, index, col, this->_candyboard);
        found = 1;
      } else {
        index--;
      }
    } else {
      //case in which no piece above to pull down was found so bring down
      //element from extended array to destroyed element in board
      xSwap(row, col);
      found = 1;
    }
  }
}

void Manager::xSwap(int row, int col){
  //accesses a game piece in extended board to put in board
  *(int*)this->_candyboard->_arr[row * this->_candyboard->_col + col] = A2DintGet(this->_extColor->_row - this->_ext_offset[col]-1,col,this->_extColor);
  //either moves up the extended offset for that row, or sets it back to
  //0 if it reached the end of the offset board
  if(this->_ext_offset[col] < this->_extColor->_row-1){
    this->_ext_offset[col]++;
  } else{
    this->_ext_offset[col] = 0;
  }
}

int Manager::A2DintGet(int row, int col, array2dPtr holder){
  return *(int*)holder->_arr[row * holder->_col + col];
}

//searches each of the directions of the board
//takes in a parameter that represents which direction to swap
int Manager::searchBoard(int direction){
  //whether the move the user made results in destroyed candies
  int matched = 0;
  int masterFound = 0;
  directionSwap(direction);
  //keeps going until there is no more candies matching
  do{
    masterFound = 0; 
    masterFound += searchVert(4);
    masterFound += searchHori(4);
    masterFound += searchVert(3);
    masterFound += searchHori(3);
    if(masterFound){
      gravity();
      matched =1;
    }
  }while(masterFound > 0);
  //if there was no matches found, then reverses the swap the user made
  if(matched == 0){
    directionSwap(direction);
  }
  //returns whether the user's swap resulted in a candy match
  return matched;
}

//takes in a parameter that represents which direction to swap
//left: 1, right: 2, up: 3, down:4, no direction: -1

void Manager::directionSwap(int direction){
  switch(direction){
    case -1:
      break;
    case 1 :
      swap(*_row,*_col,*_row,(*_col)-1, this->_candyboard);
      break;
    case 2 :
      swap(*_row, *_col,*_row,(*_col)+1, this->_candyboard);
      break;
    case 3 :
      swap(*_row, *_col,(*_row)-1, *_col, this->_candyboard);
      break;
    case 4 :
      swap(*_row, *_col, (*_row) + 1, *_col, this->_candyboard);
      break;
  }
}

//determines whether the user should recieve a point for their move
void Manager::score(int row, int col){
  if(A2DintGet(row, col, this->_gameState) > 0){
            *(int*)this->_gameState->_arr[row * this->_gameState->_col + col] -= 1;
            *(this->_score) += 1;
          }
}

//searches and destroys any horizontal matches of length interv
int Manager::searchHori(int interv){
  //whether there was at least one match
  int masterFound = 0;
  int found = 1;
  //goes through and checks to see if there is a match among interv amount
  //of candies in a horizontal line
  for(int i = 0; i < this->_candyboard->_row; i++){
    for(int k = 0; k < this->_candyboard->_col-interv+1; k++){
      for(int p = k; p < (k + interv-1); p++){
        //if there is a mismatch set found = 0 for this set of candies
        if(A2DintGet(i,p,this->_candyboard) != A2DintGet(i, p+1,this->_candyboard) || A2DintGet(i,p,this->_candyboard)==-1){
          found = 0;
        }
      }
      //if a match is found then destroy those candies and find out whether
      //the user scores
      if(found == 1){
        masterFound = 1;
        for(int g = k; g < (k + interv); g ++){
          *(int*)this->_candyboard->_arr[i * this->_candyboard->_col + g] = -1;
          score(i,g);
        }
      }
      found = 1;
    } 
  }
  //returns whether at least one match found
  return masterFound;
}

//searches and destroys any vertical matches of length interv
int Manager::searchVert(int interv){
  //whether there was at least one match
  int masterFound = 0;
  int found = 1;
  //goes through and checks to see if there is a match among interv amount
  //of candies in a horizontal line
  for(int k = 0; k < this->_candyboard->_col; k++){
    for(int i = 0; i < this->_candyboard->_row-interv+1; i++){
      for(int p = i; p < (i + interv-1); p++){
        //if there is a mismatch set found = 0 for this set of candies
        if(A2DintGet(p,k,this->_candyboard) != A2DintGet(p+1,k,this->_candyboard) || A2DintGet(p,k,this->_candyboard)==-1){
          found = 0;
        }
      }
      //if a match is found then destroy those candies and find out whether
      //the user scores
      if(found == 1){
        masterFound = 1;
        for(int g = i; g < (i + interv); g++){
          *(int*)this->_candyboard->_arr[g * this->_candyboard->_col + k] = -1;
            score(g,k);
        }
      }
      found = 1;
    }
  }
  //returns whether at least one match found
  return masterFound;
}
