/*
 * utility.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: tim
 */
#include "utility.h"

//static const int kInfinity = pow(2,sizeof(int)*8-2);
//static const int kInfinity = 16777216;
static const int kInfinity = 20000000;



static const int kNegativeInfinity = kInfinity*-1;

static const int kEmpty = 0;
static const int kUserPiece = 1;
static const int kAiPiece = 2;
static const int kDrawUtility = 0;

namespace utility {

bool d = false;

//determines if board is full
bool IsBoardFull(const MatrixType& board) {
	int rows = board.shape()[0];
	int slots = board.shape()[1];
	for (int slot_index=0;slot_index<slots;slot_index++) {
		if (board[rows-1][slot_index]==kEmpty) {
			return false;
		}
	}
	return true;
}
//evaluates a board
int EvaluateBoardHeauristic(const MatrixType& board) {
//	std::cout<<"HAD TO USE HEURISTIC"<<std::endl;
	return 0;
}
bool WinnerFound(const int N,const MatrixType& board,bool* ai_won) {
	//for each piece on the board check its neighbors to see if there are N
	//in a row
	int rows = board.shape()[0];
	int slots = board.shape()[1];


	//find a piece
	for (int row_cursor=0;row_cursor<rows;row_cursor++) {
		for (int slot_cursor=0;slot_cursor<slots;slot_cursor++) {
			int target_piece = board[row_cursor][slot_cursor];
			if (target_piece==kEmpty) continue;
			//check up,right,upright,upleft for N target_piece pieces

			//check up
			//check if possible
			if (row_cursor+(N-1) < rows) {
				//it is possible
				bool winner = true;
				for (int connected=1;connected<N;connected++) {
					if (board[row_cursor+connected][slot_cursor]!=target_piece) {
						//interupted a sequence
						winner = false; break;
					}
				}
				if (winner) {
					*ai_won = (target_piece==kAiPiece); return true;
				}
			}

			//check right
			//check if possible
			if (slot_cursor+(N-1) < slots) {
				//it is possible
				bool winner = true;
				for (int connected=1;connected<N;connected++) {
					if (!(board[row_cursor][slot_cursor+connected]==target_piece)) {
						//interupted a sequence
						winner = false; break;
					}
				}
				if (winner) {
					*ai_won = (target_piece==kAiPiece); return true;
				}
			}

			//check upright
			//check if possible
			if (slot_cursor+(N-1) < slots && row_cursor+(N-1) < rows) {
				//it is possible
				bool winner = true;
				for (int connected=1;connected<N;connected++) {
					if (!(board[row_cursor+connected][slot_cursor+connected]==target_piece)) {
						//interupted a sequence
						winner = false; break;
					}
				}
				if (winner) {
					*ai_won = (target_piece==kAiPiece); return true;
				}
			}

			//check upleft
			//check if possible
			if (((slots-1-slot_cursor)+(N-1)) < slots && (row_cursor+(N-1)) < rows) {
				//it is possible
				bool winner = true;
				for (int connected=1;connected<N;connected++) {
					if (!(board[row_cursor+connected][slot_cursor-connected]==target_piece)) {
						//interupted a sequence
						winner = false; break;
					}
				}
				if (winner) {
					*ai_won = (target_piece==kAiPiece); return true;
				}
			}
		}
	}
	return false;
}

//returns the heuristic value of node and sets slot to the optimum move
int Minimax(int depth,bool use_ab,const int N,int color,MatrixType& board,int* slot) {
	int slots = board.shape()[1];
	int rows = board.shape()[0];
	bool ai_won=false;
	*slot = -1;
	int minimum_utility = -1;
	if (depth==0) {
		//out of time, return heuristic of node
//		std::cout<<"D"<<depth<<" out of time"<<std::endl;
		return EvaluateBoardHeauristic(board);
	} else if (WinnerFound(N,board,&ai_won)) {

		if (ai_won) {
//			std::cout<<"D"<<depth<<" AI won"<<std::endl;
			return color*(kInfinity-depth);
		} else {
//			std::cout<<"D"<<depth<<" user won"<<std::endl;
			//return color*(kLossUtility-depth);
			return color*-1*(kInfinity-depth);
		}
	} else if (IsBoardFull(board)) {
		//tie, no score
//		std::cout<<"D"<<depth<<" tie"<<std::endl;
		return kDrawUtility;
	} else{
//		std::cout<<"D"<<depth<<" minimax"<<std::endl;
		//set best possible value
		int best_utility = kNegativeInfinity;
		int best_at = 0;
		int branch_sum = 0;
		*slot = -2;
		//get child utilities
		for (int slot_index=0;slot_index<slots;slot_index++) {
			//if this slot has an open space
			if (board[rows-1][slot_index]==kEmpty) {
//				std::cout<<"D"<<depth<<" agent "<<color<<" try "<<slot_index<<std::endl;
				if(use_ab && slot_index!=0) {
					if(best_utility < minimum_utility) {
						continue;
					}
				}
				EvaluateChild(board,rows,slot_index,color,depth,use_ab,N,slot,best_utility,best_at);
//				std::cout<<"D"<<depth<<" agent "<<color<<" untry "<<slot_index<<" best_utility "<<best_utility<<std::endl;
				if(use_ab && slot_index==0) {
					minimum_utility=best_utility;
				}
			}
		}
		*slot = best_at;
		//return branch_sum;
		return best_utility;
	}
}

void EvaluateChild(MatrixType& board,int rows,int slot_index,int color,int depth,bool use_ab,int N,int* slot,int& best_utility,int& best_at) {
	//find the empty row in this slot
	int empty_row=0;
	for (;empty_row<rows;empty_row++) {
		if (board[empty_row][slot_index]==kEmpty) {
			break;
		}
	}

	//mark place in slot,row
	board[empty_row][slot_index] = (color==1) ? kAiPiece:kUserPiece;
	//get utility
	int this_utility = -1*Minimax(depth-1,use_ab,N,color*-1,board,slot);
	//unmark place
	board[empty_row][slot_index] = kEmpty;

	//check if this is the best place
	if (this_utility > best_utility) {
		best_utility = this_utility;
		best_at = slot_index;
	}

}




bool GetFlags(int argc, char* argv[],helper* settings) {
	bool force = false;
	for (int arg=1;arg<argc;arg++) {
		if (strcmp(argv[arg],"-d")==0 || strcmp(argv[arg],"-D")==0) {
			//custom depth
			//advance arg cursor to arg
			arg++;
			settings->depth = atoi(argv[arg]);
		}
		else if (strcmp(argv[arg],"-A")==0 || strcmp(argv[arg],"-a")==0) {
			//use AB pruning
			settings->use_ab=true;
		}
		else if (strcmp(argv[arg],"-n")==0 || strcmp(argv[arg],"-N")==0) {
			//custom goal
			//advance arg cursor to arg
			arg++;
			settings->N = atoi(argv[arg]);
		}
		else if (strcmp(argv[arg],"-c")==0 || strcmp(argv[arg],"-C")==0) {
			//custom cols
			//advance arg cursor to arg
			arg++;
			settings->cols = atoi(argv[arg]);
		}
		else if (strcmp(argv[arg],"-r")==0 || strcmp(argv[arg],"-R")==0) {
			//custom cols
			//advance arg cursor to arg
			arg++;
			settings->rows = atoi(argv[arg]);
		}
		else if (strcmp(argv[arg],"-h")==0 || strcmp(argv[arg],"-H")==0) {
			std::cout<<std::endl<<"Usage: ./main [-d <DEPTH>] [-a] [-n <GOAL LENGTH>] [-c <COLS>] [-r <ROWS>] [-u] [-f]"<<std::endl<<std::endl;
			return false;
		}
		else if (strcmp(argv[arg],"-f")==0 || strcmp(argv[arg],"-F")==0) {
			//force playing large boards
			force = true;
		}
		else if (strcmp(argv[arg],"-u")==0 || strcmp(argv[arg],"-U")==0) {
			//let the AI go first
			settings->ai_first = true;
		}
		else {
			std::cout<<argv[arg]<<std::endl;
			//bad flag
			std::cout<<std::endl<<"Usage: ./main [-d <DEPTH>] [-a] [-n <GOAL LENGTH>] [-c <COLS>] [-r <ROWS>] [-u] [-f]"<<std::endl<<std::endl;
			return false;
		}
	}
	if (!force && (settings->rows>7 || settings->cols>8)) {
		std::cout<<settings->rows<<" rows by "<<settings->cols<<" cols is a little excessive for your screen, I don't want to play. Use -f to force me to play."<<std::endl;
		return false;
	}
	return true;
}
}
