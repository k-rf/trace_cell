#include "stdafx.h"

#include <iostream>
#include "trace_cell.hpp"


using namespace std;
using namespace cv;


// ============================================================================
// ������̍Ō�ɂ����R���}����������
// ============================================================================
string eraseComma(string str)
{
	for(int i = 0; i < str.size(); i++)
		if(str[i] == ',')
			str.erase(i);

	return str;
}



// ============================================================================
// �񋓎q�𕶎���ŏo��
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
	default:
		return "none";
	}
}


// ============================================================================
// �F�𔻒肷��
// ============================================================================
cell_color determineColor(Cell& cell)
{
	int x_center = cell.getXCenter();
	int y_center = cell.getYCenter();

	Mat img[2] = {
		Cell::getImage(GREEN, cell.getT2()),
		Cell::getImage(RED, cell.getT2())
	};
	// �זE�����F�̏ꍇ�i�΂ƐԂ̗����ɑ��݂���ꍇ�j
	if(
		img[GREEN].at<uchar>(y_center, x_center) != 0 &&
		img[RED].at<uchar>(y_center, x_center) != 0
		)
		cell.setColor(YELLOW);

	// �זE���ΐF�̏ꍇ
	else if(img[GREEN].at<uchar>(y_center, x_center) != 0)
		cell.setColor(GREEN);

	// �זE���ԐF�̏ꍇ
	else if(img[RED].at<uchar>(y_center, x_center) != 0)
		cell.setColor(RED);

	// �זE�����F�̏ꍇ�C�������́C��l���ɂ���ď������ꍇ
	else
		cell.setColor(BLACK);

	return cell.getColor();
}


// ============================================================================
// ���F�C�ǐ�
// ============================================================================
Point fillColor(Mat img, Cell cell, int x, int y)
{
	if(x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
		return Point(cell.getXCenter(), cell.getYCenter());

	if(x < x_min)
		x_min = x;
	if(x > x_max)
		x_max = x;
	if(y < y_min)
		y_min = y;
	if(y > y_max)
		y_max = y;

	cell_color color = cell.getColor();
	if(color == YELLOW)
		//return;
		color = RED;
	if(color == BLACK)
		return Point(cell.getXCenter(), cell.getYCenter());
	    //color = DIC;

	if(color == GREEN && (cell.getT2() == 22 || cell.getT2() == 0))
		return Point(cell.getXCenter(), cell.getYCenter());
	if(color == RED && cell.getT2() == 0)
		return Point(cell.getXCenter(), cell.getYCenter());

	if(Cell::getImage(color, cell.getT2()).at<uchar>(y, x) == 0)
		return Point(cell.getXCenter(), cell.getYCenter());

	else if(
		img.at<Vec3b>(y, x) == Vec3b(0, 255, 0) ||
		img.at<Vec3b>(y, x) == Vec3b(0, 0, 255) ||
		img.at<Vec3b>(y, x) == Vec3b(0, 255, 255) ||
		img.at<Vec3b>(y, x) == Vec3b(0, 0, 0)
		)
		return Point(cell.getXCenter(), cell.getYCenter());
	else
	{
		switch(cell.getColor())
		{
		case GREEN:
			img.at<Vec3b>(y, x) = Vec3b(0, 255, 0);
			break;
		case RED:
			img.at<Vec3b>(y, x) = Vec3b(0, 0, 255);
			break;
		case YELLOW:
			img.at<Vec3b>(y, x) = Vec3b(0, 255, 255);
			break;
		}
	}
	
	fillColor(img, cell, x - 1, y);
	fillColor(img, cell, x, y + 1);
	fillColor(img, cell, x + 1, y);
	fillColor(img, cell, x, y - 1);
}



// ============================================================================
// ���ʂ��o��
// ============================================================================
void result(Mat img, Cell cell)
{
	stringstream s;
	s << ".\\original_images\\CO2_2008_20_pos-1\\Results\\";
	s << cell.getM() << "_time_" << setw(3) << setfill('0') <<
		cell.getT2() << ".bmp";
	imwrite(s.str(), img);
}