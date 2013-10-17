#include "Connect.h"

//board values
static const int kEmpty = 0;
static const int kUserPiece = 1;
static const int kAiPiece = 2;

static const int kBoardScale = 100;

static const cv::Scalar kLineColor(128,128,128);
static const std::string window_name("board");

const cv::Scalar Connect::background_color_(0,0,0);
const cv::Scalar Connect::user_piece_color_(255,0,0);
const cv::Scalar Connect::ai_piece_color_(0,0,255);

Connect::Connect(const helper& settings)
	: board(boost::extents[settings.rows][settings.cols]),
	  board_visual_(kBoardScale*(settings.rows+1),kBoardScale*settings.cols,CV_8UC3,cv::Scalar(0)),
	  active_piece_position_(kBoardScale/2,kBoardScale/2){
	writer.open("output.avi",CV_FOURCC('D','I','V','X'),23,cv::Size(kBoardScale*(settings.rows+1),kBoardScale*settings.cols),true);


	use_ab_ = settings.use_ab;
	depth_ = settings.depth;
	N_ = settings.N;
	cols_ = settings.cols;
	rows_ = settings.rows;
	run_ = true;
	winner_ = false;

	//register callback
	cv::namedWindow(window_name);
	cv::setMouseCallback(window_name,Connect::OnMouse,this);

	//initialize internal board
	for(int y_cursor=0;y_cursor<rows_;y_cursor++) {
		for(int x_cursor=0;x_cursor<cols_;x_cursor++) {
			board[y_cursor][x_cursor] = kEmpty;
		}
	}

	//Draw on board
	//put gridlines on lower part
	cv::Point upper_reference(0,kBoardScale);
	cv::Point lower_reference(0,board_visual_.rows);

	for(int line_index=0;line_index<cols_;line_index++) {
		cv::line(board_visual_,
				 upper_reference+cv::Point(kBoardScale*line_index),
				 lower_reference+cv::Point(kBoardScale*line_index),
				 kLineColor,1);
	}
	////////////////temp
	//stack the board
	/*
	board[0][0]=kAiPiece;
	DrawPiece(0,0,ai_piece_color_);
	board[1][1]=kAiPiece;
	DrawPiece(1,1,ai_piece_color_);
	board[2][2]=kAiPiece;
	DrawPiece(2,2,ai_piece_color_);
	board[1][0]=kUserPiece;
	DrawPiece(1,0,user_piece_color_);
	board[0][1]=kUserPiece;
	DrawPiece(0,1,user_piece_color_);
	board[2][0]=kUserPiece;
	DrawPiece(2,0,user_piece_color_);
	board[2][1]=kUserPiece;
	DrawPiece(2,1,user_piece_color_);
	board[0][2]=kUserPiece;
	DrawPiece(0,2,user_piece_color_);
	board[1][2]=kUserPiece;
	DrawPiece(1,2,user_piece_color_);
	board[3][0]=kAiPiece;
	DrawPiece(3,0,ai_piece_color_);
	board[3][1]=kUserPiece;
	DrawPiece(3,1,user_piece_color_);
	board[0][3]=kUserPiece;
	DrawPiece(0,3,user_piece_color_);
	board[1][3]=kUserPiece;
	DrawPiece(1,3,user_piece_color_);
	board[2][3]=kUserPiece;
	DrawPiece(2,3,user_piece_color_);
	*/
/*
	board[0][0]=kAiPiece;
	DrawPiece(0,0,ai_piece_color_);
	board[0][1]=kAiPiece;
	DrawPiece(0,1,ai_piece_color_);
	board[0][2]=kUserPiece;
	DrawPiece(0,2,user_piece_color_);
	board[0][3]=kUserPiece;
	DrawPiece(0,3,user_piece_color_);

	board[1][0]=kAiPiece;
	DrawPiece(1,0,ai_piece_color_);
	board[1][1]=kAiPiece;
	DrawPiece(1,1,ai_piece_color_);
	board[1][2]=kAiPiece;
	DrawPiece(1,2,ai_piece_color_);
	board[1][3]=kEmpty;
//	DrawPiece(1,3,ai_piece_color_);

	board[2][0]=kUserPiece;
	DrawPiece(2,0,user_piece_color_);
	board[2][1]=kUserPiece;
	DrawPiece(2,1,user_piece_color_);
	board[2][2]=kUserPiece;
	DrawPiece(2,2,user_piece_color_);
	board[2][3]=kEmpty;

	board[3][0]=kEmpty;
//	DrawPiece(3,0,user_piece_color_);
	board[3][1]=kEmpty;
//	DrawPiece(3,1,user_piece_color_);
	board[3][2]=kEmpty;
	board[3][3]=kEmpty;
*/




	///////////////////

	//if ai_first take a turn right away
	if(settings.ai_first) {
		TakeAiTurn();
	}

}

void Connect::Display(){
	cv::imshow(window_name,board_visual_);
	cv::Mat temp(board_visual_);
	if(cv::waitKey(33)=='q') {
		run_ = false;
	}
}

void Connect::OnMouse(int event, int x, int y, int flags, void* board_in) {
	Connect* pboard = static_cast<Connect*>(board_in);

	//if clicked drop piece
	if(event==cv::EVENT_LBUTTONUP) {
		//decode position
		int slot = x/kBoardScale;
		pboard->Drop(slot);
	}
	else if(event==cv::EVENT_MOUSEMOVE) {
		pboard->PositionPieceVisual(x);
	}
}
void Connect::DrawPiece(int row,int col,const cv::Scalar& color) {
	cv::Point location((col+1)*kBoardScale-kBoardScale/2,board_visual_.rows-row*(kBoardScale+1)-kBoardScale/2);
	cv::circle(board_visual_,location,kBoardScale/2-2,color,CV_FILLED);
}
void Connect::DrawPiece(const cv::Point& location,const cv::Scalar& color) {
	cv::circle(board_visual_,location,kBoardScale/2-2,color,CV_FILLED);
}
void Connect::TakeAiTurn() {
	if(winner_) return;
	int slot;
	std::cout<<"Start mm: "<<N_<<std::endl;
	int utility = utility::Minimax(depth_,use_ab_,N_,1,board,&slot);
	std::cout<<"end mm: "<<slot<<std::endl;
	assert(slot < cols_);
	assert(slot > -2);
	if(slot==-1) {
		//the game is over
		winner_ = true;
		std::cout<<"there was a winner!"<<std::endl;
		return;
	}
	//make the move
	int row_index=0;
	for(;row_index<rows_;row_index++) {
		//this doesn't need to be static_cast'ed but it makes eclipse happy
		std::cout<<"rs: "<<static_cast<int>(board[row_index][slot])<<" "<<std::endl;
		if(board[row_index][slot]==kEmpty) {
			board[row_index][slot] = kAiPiece;
			break;
		}
	}
	std::cout<<"row_index: "<<row_index<<std::endl;
	//update board visual
	DrawPiece(row_index,slot,ai_piece_color_);
	std::cout<<"                   AI droped at "<<slot<<std::endl;
	std::cout<<"utility: "<<utility<<std::endl;
}
void Connect::Drop(int slot) {
	if(winner_) return;
	//decide if the drop is valid
	//check the internal board for an open position on top
	//if the top is open we know it is valid
	if(board[rows_-1][slot]!=kEmpty) {
		//drop is not valid
		return;
	}

	//find where it should go
	int row=0;
	while(board[row][slot]!=kEmpty) {
		row++;
	}
	//modify board
	board[row][slot] = kUserPiece;

	//update board visual
	DrawPiece(row,slot,user_piece_color_);
	std::cout<<"                   User droped at "<<slot<<std::endl;

	//take computer's turn
	TakeAiTurn();
}

void Connect::PositionPieceVisual(int x) {
	if(x<kBoardScale/2 || x>board_visual_.cols-kBoardScale/2) {
		return;
	}
	//erase the old piece
	//draw a background_color_ circle over it

	DrawPiece(active_piece_position_,background_color_);

	//set new piece position
	active_piece_position_.x = x;
	//y never has to change because it is just the top row

	//draw the new piece
	DrawPiece(active_piece_position_,user_piece_color_);
}
