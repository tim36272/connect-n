#ifndef CONNECT_H_
#define CONNECT_H_
#include "types.h"
#include "utility.h"
#include <opencv2/opencv.hpp>
#include <boost/multi_array.hpp>

class Connect {
public:
	Connect(); //default construction disallowed
	Connect(const helper& settings);
	bool run() {return run_;}
	void Display();
private:
	DISALLOW_COPY_AND_ASSIGN(Connect);
	bool use_ab_;
	int depth_;
	int N_;
	int cols_;
	int rows_;
	bool run_;
	bool winner_;
	cv::Point active_piece_position_;
	static const cv::Scalar background_color_;
	static const cv::Scalar user_piece_color_;
	static const cv::Scalar ai_piece_color_;
	cv::VideoWriter writer;


	cv::Mat board_visual_;
	MatrixType board;


	static void OnMouse(int event, int x, int y, int flags, void* userdata);
	void Drop(int slot);
	void PositionPieceVisual(int x);
	void DrawPiece(int row,int col,const cv::Scalar& color);
	void DrawPiece(const cv::Point& location,const cv::Scalar& color);

	void TakeAiTurn();

};

#endif /* CONNECT_H_ */
