#include "stdafx.h"

#include "cell.hpp"

using namespace std;
using namespace cv;



// ============================================================================
// 静的メンバ変数
// ============================================================================
int Cell::t0;
int Cell::t1;
int Cell::total;

Mat* Cell::green_image;
Mat* Cell::red_image;
Mat* Cell::dic_image;



// ============================================================================
// コンストラクタ
// ============================================================================
Cell::Cell(
	int _m, int _t2, int _x0, int _y0, 
	int _x1, int _y1, cell_color _color)
	: m(_m), t2(_t2), x0(_x0), y0(_y0), x1(_x1), y1(_y1), color(_color)
{}



// ============================================================================
// ゲッタ
// ============================================================================
int Cell::getM() const { return m; }
int Cell::getTotal() { return total; }

int Cell::getT0() { return t0; }
int Cell::getT1() { return t1; }
int Cell::getT2() const { return t2; }

int Cell::getX0() const { return x0; }
int Cell::getY0() const { return y0; }
int Cell::getX1() const { return x1; }
int Cell::getY1() const { return y1; }
int Cell::getXCenter() const { return x_center; }
int Cell::getYCenter() const { return y_center; }
int Cell::getCenter() const { return center; }
Point Cell::getPoint0() const { return Point(getX0(), getY0()); }
Point Cell::getPoint1() const { return Point(getX1(), getY1()); }

int Cell::getColorValue() const { return color_value; }
cell_color Cell::getColor() const { return color; }



// ============================================================================
// 読み込み画像の取得
// _color：画像の種類
// _t    ：取得する画像の時刻
// ============================================================================
Mat Cell::getImage(cell_color _color, int _t)
{
	switch(_color)
	{
	case GREEN:
		return green_image[_t];
	case RED:
		return red_image[_t];
	case DIC:
		return dic_image[_t];
	}
}

Mat Cell::getGreenImage(int _t) { return green_image[_t]; }
Mat Cell::getRedImage(int _t) { return red_image[_t]; }
Mat Cell::getDicImage(int _t) { return dic_image[_t]; }



// ============================================================================
// セッタ
// ============================================================================
void Cell::setM(int _m) { m = _m; }
void Cell::setTotal(int _total) { total = _total; }

void Cell::setT0(int _t0) { t0 = _t0; }
void Cell::setT1(int _t1) { t1 = _t1; }
void Cell::setT2(int _t2) { t2 = _t2; }

void Cell::setX0(int _x0) { x0 = _x0; }
void Cell::setY0(int _y0) { y0 = _y0; }
void Cell::setX1(int _x1) { x1 = _x1; }
void Cell::setY1(int _y1) { y1 = _y1; }

void Cell::setXCenter() { x_center = x0 + (x1 - x0) / 2; }
void Cell::setXCenter(int _x_center) { x_center = _x_center; }
void Cell::setYCenter() { y_center = y0 + (y1 - y0) / 2; }
void Cell::setYCenter(int _y_center) { y_center = _y_center; }

void Cell::setCenter() 
{
	setXCenter();
	setYCenter();
	center = x_center + y_center * WIDTH;
}
void Cell::setCenter(int _center) { center = _center; }

void Cell::setPoint0(int _x0, int _y0) { setX0(_x0); setY0(_y0); }
void Cell::setPoint1(int _x1, int _y1) { setX1(_x1); setY1(_y1); }

void Cell::setColorValue(int _color_value) { color_value = _color_value; }
void Cell::setColor(cell_color _color) { color = _color; }



// ============================================================================
// ファイル名から画像を読み込む
// ============================================================================
void Cell::setImage(string* name)
{
	setGreenImage(name[GREEN]);
	//cout << "------------------------------------------------------------------\n";
	setRedImage(name[RED]);
	//cout << "------------------------------------------------------------------\n";
	setDicImage(name[DIC]);
}

void Cell::setGreenImage(string name)
{
	green_image = new Mat[t1 + 1];
	stringstream str;
	for(int i = 0; i <= t1; i++)
	{
		str << name << setw(3) << setfill('0') << i << ".tif";
		green_image[i] = imread(str.str(), 0);
		if(green_image[i].data == NULL)
		{
			cout << "ファイルが存在しません．\n";
			cout << "Enterキーを押してください．\n";
			getchar();
			exit(-1);
		}

		str.str("");
		str.clear(stringstream::goodbit);
	}
}

void Cell::setRedImage(string name)
{
	red_image = new Mat[t1 + 1];
	stringstream str;
	for(int i = 0; i <= t1; i++)
	{
		str << name << setw(3) << setfill('0') << i << ".tif";
		red_image[i] = imread(str.str(), 0);
		if(red_image[i].data == NULL)
		{
			cout << "ファイルが存在しません．\n";
			cout << "Enterキーを押してください．\n";
			getchar();
			exit(-1);
		}

		str.str("");
		str.clear(stringstream::goodbit);
	}
}

void Cell::setDicImage(string name)
{
	dic_image = new Mat[t1 + 1];
	stringstream str;
	for(int i = 0; i <= t1; i++)
	{
		str << name << setw(3) << setfill('0') << i << ".tif";
		dic_image[i] = imread(str.str(), 0);
		if(dic_image[i].data == NULL)
		{
			cout << "ファイルが存在しません．\n";
			cout << "Enterキーを押してください．\n";
			getchar();
			exit(-1);
		}

		str.str("");
		str.clear(stringstream::goodbit);
	}
}



// ============================================================================
// 抽出子
// ============================================================================
istream& operator >> (istream& is, Cell& cell)
{
	char c;
	is >>
		cell.m >> c >> cell.t2 >> c >>
		cell.x0 >> c >> cell.y0 >> c >> 
		cell.x1 >> c >> cell.y1;
	
	cell.setCenter();

	return is;
}



// ============================================================================
// 挿入子
// ============================================================================
ostream& operator << (ostream& os, Cell& cell)
{
	os <<
		setw(3) << cell.m << ", " << setw(3) << cell.t2 << ", " <<
		setw(3) << cell.x0 << ", " << setw(3) << cell.y0 << ", " <<
		setw(3) << cell.x1 << ", " << setw(3) << cell.y1;

	return os;
}