#include "opencv2/opencv.hpp"
#include <string>
#include <iostream>

using namespace cv;

void help();

int main()
{
	const int FPS = 15;
	int WIDTH = 1280;
	int HEIGHT = 720;
	std::string tlo = "jamajka.jpg";

	VideoCapture cap(0);
	cap.set(CAP_PROP_FRAME_WIDTH, WIDTH);
	cap.set(CAP_PROP_FRAME_HEIGHT, HEIGHT);

	if (!cap.isOpened())
		return -1;

	Mat background;
	cap >> background;

	Mat secondBackgroung = imread(tlo);
	if (!secondBackgroung.data)
	{
		return -2;
	}

	int foregroundThreshold = 0;

	VideoWriter record;
	
	bool isRecording = false;
	int option = CV_THRESH_BINARY;

	help();

	for (;;)
	{
		Mat frame;
		Mat differenceImage;
		Mat grayscaleDifferenceImage;
		Mat thresholdedDifferenceImage;
		Mat foregroundMask;
		Mat backgroundimg;
		
		cap >> frame;
		
		absdiff(frame, background, differenceImage);

		cvtColor(differenceImage, grayscaleDifferenceImage, CV_BGR2GRAY);
		threshold(grayscaleDifferenceImage, thresholdedDifferenceImage, foregroundThreshold, 255, option);

		morphologyEx(thresholdedDifferenceImage, differenceImage, MORPH_OPEN, Mat(), Point(-1, -1), 1);
		morphologyEx(differenceImage, differenceImage, MORPH_CLOSE, Mat(), Point(-1, -1), 2);

		cvtColor(differenceImage, foregroundMask, CV_GRAY2BGR);

		frame = frame & foregroundMask;

		backgroundimg = secondBackgroung - foregroundMask;

		Mat result = frame | backgroundimg;

		if (isRecording == true) {
			record << result;

			circle(result, Point(WIDTH - 50, 50), 10, Scalar(0, 0, 255), 5, 8, 0);
		}

		imshow("CamController", result);

		char ch = 0;
		ch = waitKey(1000/30);
		switch (ch)
		{
		case '1': option = CV_THRESH_BINARY; break;
		case '2': option = CV_THRESH_BINARY_INV; break;
		case '3': option = CV_THRESH_TOZERO; break;
		case '4': option = CV_THRESH_TOZERO_INV; break;
		case '5': option = CV_THRESH_TRUNC; break;

		case '6': option = CV_THRESH_OTSU | CV_THRESH_BINARY; break;
		case '7': option = CV_THRESH_OTSU | CV_THRESH_BINARY_INV; break;
		case '8': option = CV_THRESH_OTSU | CV_THRESH_TOZERO; break;
		case '9': option = CV_THRESH_OTSU | CV_THRESH_TOZERO_INV; break;
		case '0': option = CV_THRESH_OTSU | CV_THRESH_TRUNC; break;

		case 'y': option = CV_THRESH_TRIANGLE | CV_THRESH_BINARY; break;
		case 'u': option = CV_THRESH_TRIANGLE | CV_THRESH_BINARY_INV; break;
		case 'i': option = CV_THRESH_TRIANGLE | CV_THRESH_TOZERO; break;
		case 'o': option = CV_THRESH_TRIANGLE | CV_THRESH_TOZERO_INV; break;
		case 'p': option = CV_THRESH_TRIANGLE | CV_THRESH_TRUNC; break;

		case '+': foregroundThreshold++; break;
		case '-': foregroundThreshold--; break;
		case 'r': 
		{
			isRecording = true;
			std::string name =  std::to_string(rand()) + "_video.avi";
			record.open(name, 0, FPS, Size(WIDTH, HEIGHT));
			break;
		}
		case 's': 
			isRecording = false; 
			record.release();
			break;
		case 'b': cap >> background; break;
		case 'n': 
			std::cout << "Enter the path to new background. The size of picutre must be: " << WIDTH << "x" << HEIGHT << ": "; 
			std::cin >> tlo;
			secondBackgroung.deallocate();
			secondBackgroung = imread(tlo);
			if (!secondBackgroung.data)
			{
				return -2;
			}
			break;
		case 'h': help(); break;
		case 27: return 0; break;
		default: break;
		}
	}
	return 0;
}

void help()
{
	std::cout << "[1] - CV_THRESH_BINARY\n"
		<< "[2] - CV_THRESH_BINARY_INV\n"
		<< "[3] - CV_THRESH_TOZERO\n"
		<< "[4] - CV_THRESH_TOZERO_INV\n"
		<< "[5] - CV_THRESH_TRUNC\n"
		<< "[6] - CV_THRESH_OTSU | CV_THRESH_BINARY\n"
		<< "[7] - CV_THRESH_OTSU | CV_THRESH_BINARY_INV\n"
		<< "[8] - CV_THRESH_OTSU | CV_THRESH_TOZERO\n"
		<< "[9] - CV_THRESH_OTSU | CV_THRESH_TOZERO_INV\n"
		<< "[0] - CV_THRESH_OTSU | CV_THRESH_TRUNC\n"
		<< "[y] - CV_THRESH_TRIANGLE | CV_THRESH_BINARY\n"
		<< "[u] - CV_THRESH_TRIANGLE | CV_THRESH_BINARY_INV\n"
		<< "[i] - CV_THRESH_TRIANGLE | CV_THRESH_TOZERO\n"
		<< "[o] - CV_THRESH_TRIANGLE | CV_THRESH_TOZERO_INV\n"
		<< "[p] - CV_THRESH_TRIANGLE | CV_THRESH_TRUNC\n"
		<< "[r] - start recording\n"
		<< "[s] - stop recording\n"
		<< "[h] - help\n"
		<< "[b] - update background\n"
		<< "[n] - upload new background\n"
		<< "[+] - foregroundThreshold++\n"
		<< "[-] - foregroundThreshold--\n"
		<< "[ESC] - exit\n";
}