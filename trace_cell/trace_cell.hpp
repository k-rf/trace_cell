#ifndef ___TRACE_CELL_HPP
#define ___TRACE_CELL_HPP

#include "stdafx.h"

#include <opencv2\\opencv.hpp>
#include <opencv\\highgui.h>

#include <fstream>
#include <iostream>

#include "cell.hpp"

extern int x_max;
extern int x_min;
extern int y_max;
extern int y_min;

// 文字列から不要なコンマを消去する
std::string eraseComma(std::string str);

// 列挙子を文字列で出力
std::string enumStr(cell_color c);

// 色を判定する
cell_color determineColor(Cell& cell);

// 着色，追跡
cv::Point fillColor(cv::Mat img, Cell cell, int x, int y);

// 出力
void result(cv::Mat img, Cell cell);

#endif