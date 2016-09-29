#include "stdafx.h"

#include "trace_cell.hpp"
#include "cell.hpp"
#include "Timer.hpp"

#include <random>

using namespace std;
using namespace cv;

int x_max = 0, x_min = 0;
int y_max = 0, y_min = 0;

int under_x, top_x;
int under_y, top_y;

double top_thresh;
double under_thresh;

int repeat_max = 0;

string directory;
cell_color fill_color;
Mat result_image(HEIGHT, WIDTH, CV_8UC3);
Mat* all_final;
Mat gray;

int main(int argc, char* argv[])
{
	Timer process_time;
	process_time.restart();

	if(argc < 2)
	{
		cout << "入力ファイルがありません．\n";
		cout << "Enterキーを押してください．\n";
		getchar();
		return -1;
	}

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
			Mat img = Cell::getImage(static_cast<cell_color>(i), j);


			//adaptiveThreshold(img, img, 200, CV_ADAPTIVE_THRESH_GAUSSIAN_C,
			//	CV_THRESH_BINARY, 3, 5);
			//medianBlur(img, img, 35);
			switch(static_cast<cell_color>(i))
			{
			case DIC:
				threshold(img, img, 95, 0, CV_THRESH_TOZERO);
				//Laplacian(img, img, CV_8U, 7);
				//adaptiveThreshold(img, img, 200, CV_ADAPTIVE_THRESH_GAUSSIAN_C,
				//	CV_THRESH_BINARY_INV, 3, 5);
				//threshold(img, img, 60, 0, CV_THRESH_TOZERO | CV_THRESH_OTSU);
				threshold(img, img, 90, 200, CV_THRESH_BINARY | CV_THRESH_OTSU);

				//dilate(img, img, Mat(), Point(-1, -1), 1);
				erode(img, img, Mat(), Point(-1, -1), 1);
				dilate(img, img, Mat(), Point(-1, -1), 3);
				//erode(img, img, Mat(), Point(-1, -1), 2);
				//threshold(img, img, 90, 200, CV_THRESH_BINARY | CV_THRESH_OTSU);

				break;

			default:
				threshold(img, img, 30, 0, CV_THRESH_TOZERO | CV_THRESH_OTSU);
				erode(img, img, Mat(), Point(-1, -1), 1);
				dilate(img, img, Mat(), Point(-1, -1), 1);
				threshold(img, img, 90, 200, CV_THRESH_BINARY | CV_THRESH_OTSU);
				erode(img, img, Mat(), Point(-1, -1), 3);
				dilate(img, img, Mat(), Point(-1, -1), 3);
				break;
			}
			progressBar(j * 100 / Cell::getT1(), Cell::getT1(), '*');
		}
	}
		
	//for(int j = 0; j <= Cell::getT1(); j++)
	//{
	//	for(int i = 0; i < 3; i++)
	//	{
	//		namedWindow(enumStr(static_cast<cell_color>(i)) + to_string(j));
	//		imshow(
	//			enumStr(static_cast<cell_color>(i)) + to_string(j),
	//			Cell::getImage(static_cast<cell_color>(i), j)
	//		);
	//	}
	//	waitKey(0);
	//	destroyAllWindows();
	//}

	//waitKey(0);
	//exit(0);

	// 結果出力用
	
	
	all_final = new Mat[Cell::getT1() + 1];

	for(int i = 0; i < WIDTH; i++)
		for(int j = 0; j < HEIGHT; j++)
			result_image.at<Vec3b>(j, i) = Vec3b(255, 255, 255);

	for(int i = 0; i <= Cell::getT1(); i++)
		all_final[i] = result_image.clone();


	std::random_device rnd_x, rnd_y;
	int random_x[200], random_y[200];
	int white_count = 0;

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

			int x_margin = (cell[m].getX1() - cell[m].getX0()) * 2;
			int y_margin = (cell[m].getY1() - cell[m].getY0()) * 2;
			under_x = x_center - (x_margin / 2);
			under_y = y_center - (y_margin / 2);
			top_x = x_center + (x_margin / 2);
			top_y = y_center + (y_margin / 2);

			//top_thresh = cell[m].getColorValue() * 1.35;
			//under_thresh = cell[m].getColorValue() * 0.95;
			repeat_max = x_margin * y_margin;

			for(int i = 0; i < 200; i++)
			{
				random_x[i] = under_x + (rnd_x() % x_margin);
				random_y[i] = under_y + (rnd_y() % y_margin);
				if(random_x[i] < 0) { random_x[i] = -random_x[i]; }
				if(random_y[i] < 0) { random_y[i] = -random_y[i]; }
				if(random_y[i] < 7) { random_y[i] += 7; }
				if(random_y[i] > HEIGHT - 7) { random_y[i] -= 7; }
			}
			
			if(cell[m].getColor() == BLACK)
			{
				gray = Cell::getImage(DIC, cell[m].getT2());
				fill_color = BLACK;
			}
			else if(cell[m].getColor() == YELLOW)
			{
				gray = Cell::getImage(RED, cell[m].getT2());
				fill_color = YELLOW;
			}
			else
			{
				gray = Cell::getImage(cell[m].getColor(), cell[m].getT2());
				fill_color = cell[m].getColor();
			}
			
			for(int i = 0; i < 200; i++)
				if(gray.at<uchar>(random_y[i], random_x[i]) != 0)
					white_count++;
			
			//Mat tmp = gray.clone();
			//	rectangle(
			//		tmp,
			//		cell[m].getPoint0(),
			//		cell[m].getPoint1(),
			//		Scalar(255, 0, 0),
			//		1,
			//		4
			//	);

			//imshow("gr", gray);
			//waitKey(0);
			//cout << cell[m] << "\n";
			if(
				fill_color == BLACK ?
				(white_count > 100 || white_count < 5) : 
				(white_count > 150 || white_count < 5)
				)
			{
				cell[m].setT2(t - 1);
				white_count = 0;
				break;
			}
			else
			{
				fillColor(x_center, y_center);
				if(x_min != x_max)
					cell[m].setPoint0(x_min, y_min);
				if(y_min != y_max)
					cell[m].setPoint1(x_max, y_max);


				//if(cell[m].getPoint0() != cell[m].getPoint1())
				//	rectangle(
				//		result_image,
				//		cell[m].getPoint0(),
				//		cell[m].getPoint1(),
				//		Scalar(255, 0, 0),
				//		1,
				//		4
				//	);

				//imshow("RESULT" + to_string(m), result_image);
				//waitKey(0);

				for(int i = cell[m].getX0(); i < cell[m].getX1(); i++)
					for(int j = cell[m].getY0(); j < cell[m].getY1(); j++)
						if(result_image.at<Vec3b>(j, i) != Vec3b(255, 255, 255))
							all_final[t].at<Vec3b>(j, i) = result_image.at<Vec3b>(j, i);
			}
			// 結果を出力
			result(result_image, cell[m]);

			// 一つ前の時刻に設定
			cell[m].setT2(t - 1);

			// 出力用画像のリセット
			result_image = Scalar(255, 255, 255);
			white_count = 0;
		}
		progressBar(m * 100 / Cell::getTotal(), Cell::getTotal(), '=');
	}
	
	cout << "\nEND\n";

	cout << process_time.elapsed() << " msec\n";
	
	for(int i = 0; i <= Cell::getT1(); i++)
	{
		allResult(all_final[i], i);
	}

	int value = 0;
	namedWindow("all");
	createTrackbar("time", "all", &value, Cell::getT1(), track);
	setTrackbarPos("time", "all", 0);

	waitKey(0);

	return 0;
}

