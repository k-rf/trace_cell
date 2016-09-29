#ifndef ___TRACE_CELL_HPP
#define ___TRACE_CELL_HPP

#include "stdafx.h"

#include <opencv2\\opencv.hpp>
#include <opencv\\highgui.h>

#include <fstream>
#include <iostream>
#include <direct.h>

#include "cell.hpp"

#define GG Vec3b(0, 255, 0)
#define RR Vec3b(0, 0, 255)
#define YY Vec3b(0, 255, 255)
#define BB Vec3b(0, 0, 0)
#define WW Vec3b(255, 255, 255)

extern int x_max;
extern int x_min;
extern int y_max;
extern int y_min;

// 追跡上下限
extern int under_x;
extern int top_x;
extern int under_y;
extern int top_y;

// 上下の閾値
extern double top_thresh;
extern double under_thresh;

extern int repeat_max;

extern std::string directory;
extern cell_color fill_color;
extern cv::Mat result_image;
extern cv::Mat* all_final;
extern cv::Mat gray;

// 文字列から不要なコンマを消去する
std::string eraseComma(std::string str);

// 列挙子を文字列で出力
std::string enumStr(cell_color c);

// 色を判定する
cell_color determineColor(Cell& cell);

// 着色，追跡
void fillColor(int x, int y);

// 出力
void result(cv::Mat& img, Cell cell);
void allResult(cv::Mat& img, int i);

void track(int thres, void*);
void progressBar(int per, int x, char c);

#endif