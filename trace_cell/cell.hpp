#ifndef ___CELL_HPP
#define ___CELL_HPP

#include "stdafx.h"

#include <opencv2\\opencv.hpp>
#include <opencv\\highgui.h>

#include <iomanip>

#define WIDTH  340 // 画像の横サイズ
#define HEIGHT 256 //       縦

enum cell_color { GREEN, RED, DIC, BLACK, YELLOW, NONE };


class Cell
{
	int m;        // 細胞番号
	int t2;       // 細胞の最終時刻

	int x0;       // 細胞の左上の x 座標
	int y0;       // 細胞の左上の y 座標
	int x1;       // 細胞の右下の x 座標
	int y1;       // 細胞の右下の y 座標
	int x_center; // x 座標の中心
	int y_center; // y 座標の中心
	int center;   // 枠の中心

	static int t0;      // 開始時刻
	static int t1;      // 終了時刻
	static int total;   // 細胞の総数

	cell_color color;   // 細胞の色

	static cv::Mat* green_image;  // 緑の画像
	static cv::Mat* red_image;    // 赤の画像
	static cv::Mat* dic_image;    // 位相差画像

public:
	// コンストラクタ
	Cell(
		int _m = 0, int _t2 = 0,
		int _x0 = 0, int _y0 = 0,
		int _x1 = 0, int _y1 = 0,
		cell_color _color = GREEN
	);
	
	// ゲッタ
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


	// セッタ
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

	// ファイル名から画像を読み込む
	static void setImage(std::string* name);
	static void setGreenImage(std::string name);
	static void setRedImage(std::string name);
	static void setDicImage(std::string name);


	// 抽出子
	friend std::istream& operator >> (std::istream& is, Cell& cell);

	// 挿入子
	friend std::ostream& operator << (std::ostream& os, Cell& cell);
};

#endif