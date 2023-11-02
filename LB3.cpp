#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include<string>
using namespace cv;
using namespace std;
/// <summary>
/// Получение объекта Mat из файла
/// </summary>
/// <returns></returns>
Mat GetImageFromFile() {
	Mat img = Mat();
	while (img.empty()) {
		std::cout << "Enter path to an image: ";
		std::string path;
		std::cin >> path;
		img = imread(path, IMREAD_COLOR);
		if (img.empty()) {
			std::cout << "Error reading the image. Try another one.\n";
		}
	}
	return img;
}
void demo1();
void demo2();
void task1(Mat sourceImage, std::string histogramTitle = "Histogram of intensity", std::string imageTitle = "Image", bool waitKeyFlag = true);
void task2();
void task3();
void task4();
Mat getImageFromHistogram(Mat hist, float scaleX = 1, float scaleY = 1, Scalar
	fillColor = Scalar(255, 255, 255)) {
	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);
	int rows = 64; //default height size
	int cols = hist.rows; //get the width size from the histogram
	Mat histImg = Mat::zeros(rows * scaleX, cols * scaleY, CV_8UC3);
	//for each bin
	for (int i = 0; i < cols - 1; i++) {
		float histValue = hist.at<float>(i, 0);
		float nextValue = hist.at<float>(i + 1, 0);
		Point pt1 = Point(i * scaleX, rows * scaleY);
		Point pt2 = Point(i * scaleX + scaleX, rows * scaleY);
		Point pt3 = Point(i * scaleX + scaleX, (rows - nextValue * rows /
			maxVal) * scaleY);
		Point pt4 = Point(i * scaleX, (rows - nextValue * rows / maxVal) * scaleY);
		int numPts = 5;
		Point pts[] = { pt1, pt2, pt3, pt4, pt1 };
		fillConvexPoly(histImg, pts, numPts, fillColor);
	}
	return histImg;
}
int main(int argc, char** argv) {
	std::cout << "OPENCV HISTOGRAM\n\n"
		<< "Demos:\n"
		<< "1. Histogram of a RGB image.\n"
		<< "2. Histogram equalization of a grayscale image.\n\n"
		<< "Tasks:\n"
		<< "3. Histogram of intensity.\n"
		<< "4. Histogram of a video.\n"
		<< "5. Histogram equalization of an RGB image.\n"
		<< "6. Adaptive histogram equalization.\n";
	std::cout << "Which program would you like to run?\n";
	int task = 0;
	std::cin >> task;
	std::cout << "\n";
		switch (task) {
		case 1:
			demo1();
			break;
		case 2:
			demo2();
			break;
		case 3:
			task1(GetImageFromFile());
			break;
		case 4:
			task2();
			break;
		case 5:
			task3();
			break;
		case 6:
			task4();
			break;
		}
	std::cin.get();
	return 0;
}

void demo1() {
	Mat img = GetImageFromFile();
	MatND hist;
	Mat histImg;
	int nbins = 256; // lets hold 256 levels
	int hsize[] = { nbins }; // just one dimension
	float range[] = { 0, 255 };
	const float* ranges[] = { range };
	int chnls[] = { 0 };
	std::vector<Mat> channels;
	split(img, channels);
	calcHist(&channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges);
	histImg = getImageFromHistogram(hist, 3, 3, Scalar(255, 0, 0));
	imshow("Blue", histImg);
	calcHist(&channels[1], 1, chnls, Mat(), hist, 1, hsize, ranges);
	histImg = getImageFromHistogram(hist, 3, 3, Scalar(0, 255, 0));
	imshow("Green", histImg);
	calcHist(&channels[2], 1, chnls, Mat(), hist, 1, hsize, ranges);
	histImg = getImageFromHistogram(hist, 3, 3, Scalar(0, 0, 255));
	imshow("Red", histImg);
	imshow("Image", img);
	waitKey(0);
}
void demo2() {
	Mat img = GetImageFromFile();
	cvtColor(img, img, COLOR_BGR2GRAY);
	Mat image_equalized;
	equalizeHist(img, image_equalized);
	imshow("Original image", img);
	imshow("Equalized image", image_equalized);
	waitKey(0);
}
void task1(Mat sourceImage, std::string histogramTitle, std::string imageTitle, bool waitKeyFlag) {
	Mat bgrImg = sourceImage;
	// Declare an empty Mat for dst image
	cv::Mat ycrcbImg;

	// Convert to ycrcb
	cv::cvtColor(bgrImg, ycrcbImg, cv::COLOR_BGR2YCrCb);
	MatND hist;
	Mat histImg;
	int nbins = 256; // lets hold 256 levels
	int hsize[] = { nbins }; // just one dimension
	float range[] = { 0, 255 };
	const float* ranges[] = { range };
	int chnls[] = { 0 };
	std::vector<Mat> channels;
	split(ycrcbImg, channels);
	calcHist(&channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges);
	histImg = getImageFromHistogram(hist, 3, 3, Scalar(255, 255, 255));
	imshow(histogramTitle, histImg);
	imshow(imageTitle, bgrImg);
	if (waitKeyFlag == true) {
		waitKey(0);
	}
}
void task2() {
	// получаем любую подключённую камеру
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "cannot open camera";
	}
	while (true)
	{
		Mat image;
		cap >> image;
		task1(image, "HistogramOfIntansityFromCamera", "web-cam", false);
		waitKey(25);
	}
}
Mat equalizeIntensity(const Mat& inputImage)
{
	if (inputImage.channels() >= 3)
	{
		Mat ycrcb;

		cv::cvtColor(inputImage, ycrcb, cv::COLOR_BGR2YCrCb);

		std::vector<Mat> channels;
		split(ycrcb, channels);

		equalizeHist(channels[0], channels[0]);

		Mat result;
		merge(channels, ycrcb);

		cvtColor(ycrcb, result, COLOR_YCrCb2BGR);

		return result;
	}
	return Mat();
}
void task3() {
	Mat img = GetImageFromFile();
	Mat equaliedImage = equalizeIntensity(img);
	task1(img, "Histogram of intensity source","Source image", false);
	task1(equaliedImage, "Histogram of intensity equalized", "Equalized image",false);
	waitKey(0);
}
void task4() {
	Mat bgr_image = GetImageFromFile();
	cv::Mat lab_image;
	cv::cvtColor(bgr_image, lab_image, COLOR_BGR2Lab);

	// Extract the L channel
	std::vector<cv::Mat> lab_planes(3);
	cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

	// apply the CLAHE algorithm to the L channel
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(4);
	cv::Mat dst;
	clahe->apply(lab_planes[0], dst);

	// Merge the the color planes back into an Lab image
	dst.copyTo(lab_planes[0]);
	cv::merge(lab_planes, lab_image);

	// convert back to RGB
	cv::Mat image_clahe;
	cv::cvtColor(lab_image, image_clahe, COLOR_Lab2BGR);

	// display the results  (you might also want to see lab_planes[0] before and after).
	cv::imshow("image original", bgr_image);
	cv::imshow("image CLAHE", image_clahe);
	cv::waitKey();
}