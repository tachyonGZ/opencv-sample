#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <chrono>

#include <desuwa.h>

void task3();
void task4();
void task5();

inline void debug_show(const cv::Mat& image)
{
	static bool init = false;
	if (!init)
	{
		cv::namedWindow("DEBUG", cv::WINDOW_NORMAL);
		init = true;
	}
	cv::imshow("DEBUG", image);
	cv::waitKey(0);
}

inline void fast_show(const cv::Mat& image)
{
	static bool init = false;
	if (!init)
	{
		cv::namedWindow("FAST", cv::WINDOW_AUTOSIZE);
		init = true;
	}
	cv::imshow("FAST", image);
	cv::waitKey(0);
}