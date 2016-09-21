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

// �����񂩂�s�v�ȃR���}����������
std::string eraseComma(std::string str);

// �񋓎q�𕶎���ŏo��
std::string enumStr(cell_color c);

// �F�𔻒肷��
cell_color determineColor(Cell& cell);

// ���F�C�ǐ�
cv::Point fillColor(cv::Mat img, Cell cell, int x, int y);

// �o��
void result(cv::Mat img, Cell cell);

#endif