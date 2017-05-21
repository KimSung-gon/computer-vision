#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

void calc_histogram_gray(Mat&, double[]);
Mat draw_histogram_gray(double[]);
void draw_binary_photo(Mat&, double t);
int otsu_algo(double hist[]);
//void fill(Mat&, int row, int col, int &count);

int main() {

	double hist[256];
	Mat otsu;

	Mat img = imread("C:/Users/KimSunggon/Desktop/cameraman.jpg", 1);
	cvtColor(img, otsu, CV_BGR2GRAY);
	namedWindow("입력 영상");
	imshow("입력 영상", otsu);
	
	calc_histogram_gray(otsu, hist);
	Mat bar = draw_histogram_gray(hist);
		
	double t = otsu_algo(hist);
	draw_binary_photo(otsu,t);
	namedWindow("오츄알고리즘");
	imshow("오츄알고리즘", otsu);

	//fill(otsu, );
	//namedWindow("fill");
	//imshow("fill", otsu);

	waitKey(0);

	return 0;
}

void calc_histogram_gray(Mat& src, double hist[]) {

	int i, j, bin, ihist[256];

	for (i = 0; i < 256; i++)	ihist[i] = 0;

	if (src.channels() != 1)	cout << "ERR: 명암 영상을 입력하세요.";
	else {
		for (j = 0; j < src.rows; j++)
			for (i = 0; i < src.cols; i++) {
				bin = src.at<uchar>(j, i);
				ihist[bin]++;
			}
		for (i = 0; i < 256; i++)
			hist[i] = ihist[i];
	}
}

Mat draw_histogram_gray(double hist[]) {

	double vmax = -1.0;
	Mat himg(256, 256, CV_8U, Scalar(255));

	for (int i = 0; i < 256; i++)	vmax = (vmax > hist[i]) ? vmax : hist[i];

	for (int i = 0; i < 256; i++) {
		Point p1(i, 256), p2(i, 256 - (int)((hist[i] / vmax)*(0.8 * 256)));
		line(himg, p1, p2, Scalar(0));
	}

	return himg;
}

void draw_binary_photo(Mat& src, double t) {

	int i, j;

	if (src.channels() != 1)	cout << "ERR: 명암 영상을 입력하세요.";
	else {
		for (j = 0; j < src.rows; j++)
			for (i = 0; i < src.cols; i++) {
				if (src.at<uchar>(j, i) >= t)
					src.at<uchar>(j, i) = 255;
				else
					src.at<uchar>(j, i) = 0;
			}
	}
}

int otsu_algo(double hist[]) {
	int t=0;
	int k = 256*256;
	double sum1=0, sum2=0;
	double w1,w2,u1,u2,v1,v2;
	for (int i = 0; i < 256; i++) 
		sum2 += hist[i];

	double max = -100;
	
	for (int i = 1; i < 255; i++) {
		w1 = 0; w2 = 0;
		u1 = 0; u2 = 0;
		v1 = 0; v2 = 0;

		sum1 += hist[i];
		sum2 -= hist[i];

		w1 = sum1 / k;
		w2 = sum2 / k;
		
		for (int j = 0; j <= i; j++) 
			u1 += j * hist[j];		

		for (int j = i + 1; j < 256; j++) 
			u2 += j * hist[j];
		
		u1 /= sum1;
		u2 /= sum2;
		
		for (int j = 0; j <= i; j++) 
			v1 += abs(pow((j-u1),2.0)) * hist[j];
		
		for (int j = i + 1; j < 256; j++) 
			v2 += abs(pow((j-u2), 2.0)) * hist[j];
		
		v1 /= sum1;
		v2 /= sum2;
		if (w1*v1 + w2*v2 > max) {
			max = w1*v1 + w2*v2;
			t = u2;
		}
		
	}
	
	return t;
}

/*void fill(Mat otsu, int row, int col, int &count) {

	otsu[row][col] = '+';
	count++;

	if ((array[row][col + 1] == '+') && (array[row + 1][col] == '+') && (array[row][col - 1] == '+') && (array[row - 1][col] == '+')
		&& (array[row][col + 1] != NULL) && (array[row][col - 1] != NULL) && (array[row + 1][col] != NULL) && (array[row - 1][col] != NULL))
		return;

	if ((array[row][col + 1] != 255) && (array[row][col + 1] != NULL))
		fill(array, row, col + 1, count);
	if ((array[row + 1][col] != 255) && (array[row + 1][col] != NULL))
		fill(array, row + 1, col, count);
	if ((array[row][col - 1] != 255) && (array[row][col - 1] != NULL))
		fill(array, row, col - 1, count);
	if ((array[row - 1][col] != 255) && (array[row - 1][col] != NULL))
		fill(array, row - 1, col, count);
	return;
}*/