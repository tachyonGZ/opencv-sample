#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
	cv::Mat img = cv::imread("D:\\Download\\img1.jpg");
	cv::namedWindow("image");
	cv::imshow("image", img);
	cv::waitKey(0);
	cv::destroyWindow("image");
	return 0;
}