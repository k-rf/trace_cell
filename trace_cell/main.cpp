#include "stdafx.h"

#include "trace_cell.hpp"
#include "cell.hpp"

using namespace std;
using namespace cv;

int x_max = 0, x_min = 0;
int y_max = 0, y_min = 0;
string directory;

int main(int argc, char* argv[])
{
	directory = argv[1];
	string input_file_name = directory + "\\input.csv";
	string image_name[3];
	image_name[GREEN] = directory +	"\\Green\\time_";
	image_name[RED]   = directory +	"\\Red\\time_";
	image_name[DIC]   = directory + "\\DIC\\time_";
	
	ifstream ifs;
	ifs.open(input_file_name);
	if(ifs.fail())
	{
		cout << "ファイルが開けません．\n";
		cout << "Enterキーを押してください．\n";
		getchar();
		return -1;
	}

	// データセット名を読み込む
	string line;  
	ifs >> line;
	cout << "データセット名：" + eraseComma(line) + "\n";
	
	// 開始時刻 t0 を読み込む
	ifs >> line;
	Cell::setT0(stoi(eraseComma(line)));
	cout << "開始時刻：" << Cell::getT0() << "\n";

	// 終了時刻 t1 を読み込む
	ifs >> line;
	Cell::setT1(stoi(eraseComma(line)));
	cout << "終了時刻：" << Cell::getT1() << "\n";

	// 細胞の総数 total を読み込む
	ifs >> line;
	Cell::setTotal(stoi(eraseComma(line)));
	cout << "細胞総数：" << Cell::getTotal() << "\n";

	// 必要な画像をすべて読み込む
	Cell::setImage(image_name);

	// Cellクラスのインスタンスを作成
	Cell* cell = new Cell[Cell::getTotal()];

	// 各細胞の情報（m, t2, x0, y0, x1, y1）を読み込む
	for(int i = 0; i < Cell::getTotal(); i++)
		ifs >> cell[i];
	
	// 二値化，膨張，収縮
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j <= Cell::getT1(); j++)
		{
			threshold(
				Cell::getImage(static_cast<cell_color>(i), j),
				Cell::getImage(static_cast<cell_color>(i), j),
				0, 200, cv::THRESH_BINARY | cv::THRESH_OTSU
			);
			erode(
				Cell::getImage(static_cast<cell_color>(i), j),
				Cell::getImage(static_cast<cell_color>(i), j),
				Mat(), Point(-1, -1), 3
			);
			dilate(
				Cell::getImage(static_cast<cell_color>(i), j),
				Cell::getImage(static_cast<cell_color>(i), j),
				Mat(), Point(-1, -1), 3
			);

		}
	}

	//for(int i = 0; i < 3; i++)
	//	for(int j = 0; j <= Cell::getT1(); j++)
	//		imshow(to_string(i) + ", " + to_string(j), 
	//			Cell::getImage(static_cast<cell_color>(i), j));
	
	// 結果出力用
	Mat result_image(
		Cell::getImage(GREEN, 0).rows,
		Cell::getImage(GREEN, 0).cols,
		CV_8UC3
	);

	for(int i = 0; i < WIDTH; i++)
		for(int j = 0; j < HEIGHT; j++)
			result_image.at<Vec3b>(j, i) = Vec3b(255, 255, 255);


	for(int m = 0; m < Cell::getTotal(); m++)
	{
		for(int t = cell[m].getT2(); t >= cell[m].getT0(); t--)
		{
			// m 番目，t 時刻の細胞の色を判定する
			determineColor(cell[m]);

			cell[m].setCenter();
			int x_center = cell[m].getXCenter();
			int y_center = cell[m].getYCenter();

			x_max = x_min = x_center;
			y_max = y_min = y_center;

			// 着色
			fillColor(result_image, cell[m], x_center, y_center);

			cell[m].setPoint0(x_min, y_min);
			cell[m].setPoint1(x_max, y_max);

			imshow("RESULT" + to_string(m), result_image);
			waitKey(20);
			
			// 結果を出力
			result(result_image, cell[m]);
			
			// 一つ前の時刻に設定
			cell[m].setT2(t - 1);

			// 出力用画像のリセット
			result_image = Scalar(255, 255, 255);
		}
		destroyWindow("RESULT" + to_string(m));
		cout <<
			"|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
	}
	
	cout << "END\n";

	waitKey(0);

	
	
	return 0;
}

