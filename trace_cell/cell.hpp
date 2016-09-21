#ifndef ___CELL_HPP
#define ___CELL_HPP

#include "stdafx.h"

#include <opencv2\\opencv.hpp>
#include <opencv\\highgui.h>

#include <iomanip>

#define WIDTH  340 // �摜�̉��T�C�Y
#define HEIGHT 256 //       �c

enum cell_color { GREEN, RED, DIC, BLACK, YELLOW, NONE };


class Cell
{
	int m;        // �זE�ԍ�
	int t2;       // �זE�̍ŏI����

	int x0;       // �זE�̍���� x ���W
	int y0;       // �זE�̍���� y ���W
	int x1;       // �זE�̉E���� x ���W
	int y1;       // �זE�̉E���� y ���W
	int x_center; // x ���W�̒��S
	int y_center; // y ���W�̒��S
	int center;   // �g�̒��S

	static int t0;      // �J�n����
	static int t1;      // �I������
	static int total;   // �זE�̑���

	cell_color color;   // �זE�̐F

	static cv::Mat* green_image;  // �΂̉摜
	static cv::Mat* red_image;    // �Ԃ̉摜
	static cv::Mat* dic_image;    // �ʑ����摜

public:
	// �R���X�g���N�^
	Cell(
		int _m = 0, int _t2 = 0,
		int _x0 = 0, int _y0 = 0,
		int _x1 = 0, int _y1 = 0,
		cell_color _color = GREEN
	);
	
	// �Q�b�^
	int getM() const;
	static int getTotal();

	static int getT0();
	static int getT1();
	int getT2() const;

	int getX0() const;
	int getY0() const;
	int getX1() const;
	int getY1() const;
	int getXCenter() const;
	int getYCenter() const;
	int getCenter() const;
	cv::Point getPoint0() const;
	cv::Point getPoint1() const;


	cell_color getColor() const;

	static cv::Mat getImage(cell_color _color, int _t);
	static cv::Mat getGreenImage(int _t);
	static cv::Mat getRedImage(int _t);
	static cv::Mat getDicImage(int _t);


	// �Z�b�^
	void setM(int _m);
	static void setTotal(int _total);

	static void setT0(int _t0);
	static void setT1(int _t1);
	void setT2(int _t2);

	void setX0(int _x0);
	void setY0(int _y0);
	void setX1(int _x1);
	void setY1(int _y1);
	void setXCenter();
	void setXCenter(int _x_center);
	void setYCenter();
	void setYCenter(int _y_center);
	void setCenter();
	void setCenter(int _center);
	void setPoint0(int _x0, int _y0);
	void setPoint1(int _x1, int _y1);

	void setColor(cell_color _color);

	// �t�@�C��������摜��ǂݍ���
	static void setImage(std::string* name);
	static void setGreenImage(std::string name);
	static void setRedImage(std::string name);
	static void setDicImage(std::string name);


	// ���o�q
	friend std::istream& operator >> (std::istream& is, Cell& cell);

	// �}���q
	friend std::ostream& operator << (std::ostream& os, Cell& cell);
};

#endif