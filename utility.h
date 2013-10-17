#ifndef UTILITY_H_
#define UTILITY_H_
#include <cstring>
#include "types.h"

namespace utility {
bool GetFlags(int argc, char* argv[],helper* settings);

//determines if board is full
bool IsBoardFull(const MatrixType& board);
//evaluates a board
int EvaluateBoardHeauristic(const MatrixType& board);
//if the board is finished return true and if the ai won
bool WinnerFound(const int N,const MatrixType& board,bool* ai_won);
//returns a slot to put the piece in
int Minimax(int depth,bool use_ab,int color,const int N,MatrixType& board,int* slot);
void EvaluateChild(MatrixType& board,int rows,int slot_index,int color,int depth,bool use_ab,int N,int* slot,int& best_utility,int& best_at);
}


#endif /* UTILITY_H_ */
