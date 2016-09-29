#include "stdafx.h"

#include <iostream>
#include "trace_cell.hpp"


using namespace std;
using namespace cv;


// ============================================================================
// 文字列の最後についたコンマを消去する
// ============================================================================
string eraseComma(string str)
{
	for(int i = 0; i < str.size(); i++)
		if(str[i] == ',')
			str.erase(i);

	return str;
}



// ============================================================================
// 列挙子を文字列で出力
// ============================================================================
string enumStr(cell_color c)
{
	switch(c)
	{
	case GREEN:
		return "G   ";
	case RED:
		return " R  ";
	case YELLOW:
		return "  Y ";
	case BLACK:
		return "   B";
	case DIC:
		return "DIC ";
	default:
		return "none";
	}
}


// ============================================================================
// 色を判定する
// ============================================================================
cell_color determineColor(Cell& cell)
{
	int x_center = cell.getXCenter();
	int y_center = cell.getYCenter();

	Mat img[2] = {
		Cell::getImage(GREEN, cell.getT2()),
		Cell::getImage(RED, cell.getT2())
	};
	// 細胞が黄色の場合（緑と赤の両方に存在する場合）
	if(
		img[GREEN].at<uchar>(y_center, x_center) != 0 &&
		img[RED].at<uchar>(y_center, x_center) != 0
		)
	{
		cell.setColor(YELLOW);
		cell.setColorValue(img[RED].at<uchar>(y_center, x_center));
	}
	// 細胞が緑色の場合
	else if(img[GREEN].at<uchar>(y_center, x_center) != 0)
	{
		cell.setColor(GREEN);
		cell.setColorValue(img[GREEN].at<uchar>(y_center, x_center));
	}
	// 細胞が赤色の場合
	else if(img[RED].at<uchar>(y_center, x_center) != 0)
	{
		cell.setColor(RED);
		cell.setColorValue(img[RED].at<uchar>(y_center, x_center));
	}
	// 細胞が黒色の場合，もしくは，二値化によって消えた場合
	else
		cell.setColor(BLACK);

	return cell.getColor();
}


// ============================================================================
// 着色，追跡
// ============================================================================
void fillColor(int x, int y)
{
	//if(repeat_max-- < 0)
	//	return;
	if(x < 0 || x > WIDTH || y < 0 || y > HEIGHT) { return; }
	if(result_image.at<Vec3b>(y, x) != WW) { return; }
	if(gray.at<uchar>(y, x) == 0) { return; }

	if(x < x_min) { x_min = x; }
	if(x > x_max) { x_max = x; }
	if(y < y_min) { y_min = y; }
	if(y > y_max) { y_max = y; }

	if(fill_color == GREEN)
		result_image.at<Vec3b>(y, x) = GG;
	else if(fill_color == RED)
		result_image.at<Vec3b>(y, x) = RR;
	else if(fill_color == YELLOW)
		result_image.at<Vec3b>(y, x) = YY;
	else if(fill_color == BLACK)
		result_image.at<Vec3b>(y, x) = BB;

	fillColor(x - 1, y);
	fillColor(x, y - 1);
	fillColor(x + 1, y);
	fillColor(x, y + 1);
}



// ============================================================================
// 結果を出力
// ============================================================================
void result(Mat img, Cell cell)
{
	stringstream s;
	s << directory + "\\Results\\";
	s << cell.getM() << "_time_" << setw(3) << setfill('0') <<
		cell.getT2() << ".bmp";
	imwrite(s.str(), img);
}

void allResult(Mat img, int i)
{
	stringstream s;
	s << directory + "\\All\\";
	s << "all_time_" << setw(3) << setfill('0') << i << ".bmp";
	imwrite(s.str(), img);
}


void track(int thres, void*)
{
	imshow("all", all_final[thres]);
}

void progressBar(int per, int x, char c)
{
	stringstream s;
	s << "||";
	for(int i = 0; i < 40; i++)
	{
		if(i * x / 40 < per) { s << c; }
		else { s << " "; }
	}
	s << "||\r";

	cout << s.str();
}
