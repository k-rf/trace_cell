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
		cout << "�t�@�C�����J���܂���D\n";
		cout << "Enter�L�[�������Ă��������D\n";
		getchar();
		return -1;
	}

	// �f�[�^�Z�b�g����ǂݍ���
	string line;  
	ifs >> line;
	cout << "�f�[�^�Z�b�g���F" + eraseComma(line) + "\n";
	
	// �J�n���� t0 ��ǂݍ���
	ifs >> line;
	Cell::setT0(stoi(eraseComma(line)));
	cout << "�J�n�����F" << Cell::getT0() << "\n";

	// �I������ t1 ��ǂݍ���
	ifs >> line;
	Cell::setT1(stoi(eraseComma(line)));
	cout << "�I�������F" << Cell::getT1() << "\n";

	// �זE�̑��� total ��ǂݍ���
	ifs >> line;
	Cell::setTotal(stoi(eraseComma(line)));
	cout << "�זE�����F" << Cell::getTotal() << "\n";

	// �K�v�ȉ摜�����ׂēǂݍ���
	Cell::setImage(image_name);

	// Cell�N���X�̃C���X�^���X���쐬
	Cell* cell = new Cell[Cell::getTotal()];

	// �e�זE�̏��im, t2, x0, y0, x1, y1�j��ǂݍ���
	for(int i = 0; i < Cell::getTotal(); i++)
		ifs >> cell[i];
	
	// ��l���C�c���C���k
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
	
	// ���ʏo�͗p
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
			// m �ԖځCt �����̍זE�̐F�𔻒肷��
			determineColor(cell[m]);

			cell[m].setCenter();
			int x_center = cell[m].getXCenter();
			int y_center = cell[m].getYCenter();

			x_max = x_min = x_center;
			y_max = y_min = y_center;

			// ���F
			fillColor(result_image, cell[m], x_center, y_center);

			cell[m].setPoint0(x_min, y_min);
			cell[m].setPoint1(x_max, y_max);

			imshow("RESULT" + to_string(m), result_image);
			waitKey(20);
			
			// ���ʂ��o��
			result(result_image, cell[m]);
			
			// ��O�̎����ɐݒ�
			cell[m].setT2(t - 1);

			// �o�͗p�摜�̃��Z�b�g
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

