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

// �ǐՏ㉺��
extern int under_x;
extern int top_x;
extern int under_y;
extern int top_y;

// �㉺��臒l
extern double top_thresh;
extern double under_thresh;

extern int repeat_max;

extern std::string directory;
extern cell_color fill_color;
extern cv::Mat result_image;
extern cv::Mat* all_final;
extern cv::Mat gray;

// �����񂩂�s�v�ȃR���}����������
std::string eraseComma(std::string str);

// �񋓎q�𕶎���ŏo��
std::string enumStr(cell_color c);

// �F�𔻒肷��
cell_color determineColor(Cell& cell);

// ���F�C�ǐ�
void fillColor(int x, int y);

// �o��
void result(cv::Mat& img, Cell cell);
void allResult(cv::Mat& img, int i);

void track(int thres, void*);
void progressBar(int per, int x, char c);

#endif