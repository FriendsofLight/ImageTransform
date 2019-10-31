#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
int main(int argc, char** argv) {
	// load images
	Mat src = imread("road.jpg");
	if (!src.data) {
		printf("could not load image...\n");
		return -1;
	}
	// show images
	namedWindow("input image", CV_WINDOW_AUTOSIZE);
	imshow("input image", src);
	Mat replaceImg = imread("U23_Thuong_Chau.jpg");
	imshow("adv content", replaceImg);
	// 定义两个平面上四个角坐标 Dinh nghia 4 dinh cua 2 mat phang
	vector<Point> src_corners(4);
	vector<Point> dst_corners(4);
	// 4 diem tren mat phang goc
	src_corners[0] = Point(0, 0);
	src_corners[1] = Point(replaceImg.cols, 0);
	src_corners[2] = Point(0, replaceImg.rows);
	src_corners[3] = Point(replaceImg.cols, replaceImg.rows);
	// 4 diem tren mat phang can xoay den
	dst_corners[0] = Point(218, 50);
	dst_corners[1] = Point(557, 30);
	dst_corners[2] = Point(273, 236);
	dst_corners[3] = Point(559, 215);
	// Tinh toan ma tran bien doi giua 2 mat phang
	Mat h = findHomography(src_corners, dst_corners);
	Mat output_img;
	warpPerspective(replaceImg, output_img, h, src.size());

	// create mask
	Mat m1 = Mat::zeros(replaceImg.size(), CV_8UC1);
	m1 = Scalar(255);
	Mat mask_output;
	warpPerspective(m1, mask_output, h, src.size());
	imshow("Mask Result", mask_output);
	// use mask 
	Mat result1;
	/*add(output_img, output_img, result1, mask_output);
	imshow("output_img", result1);
	cvWaitKey(5000);*/
	Mat result2;
	bitwise_not(mask_output, mask_output);
	add(src, output_img, result2, mask_output);
	// put them together
	Mat result;
	add(output_img, result2, result);
	imshow("Final Result", result);
	imwrite("result.jpg", result);
	waitKey(0);
	return 0;
}