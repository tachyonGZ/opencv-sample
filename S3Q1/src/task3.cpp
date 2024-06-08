#include "../inc/stdafx.h"
#include "../inc/CContours.hpp"
#include "../inc/CRotatedRect.hpp"

void task3()
{
	for (const auto& i : std::filesystem::directory_iterator("D:\\Download\\IFR\\Task\\S2_Q3\\pictures\\"))
	{
		cv::Mat srcMat = cv::imread(i.path().string(), cv::IMREAD_COLOR);
		fast_show(srcMat);
		// 分割通道
		std::vector<cv::Mat> matVec(3);
		cv::split(srcMat, matVec);


		cv::Mat &srcMatRef = matVec[0];

		// 阈值处理
		cv::threshold(srcMatRef, srcMatRef, 128, 0, cv::THRESH_TOZERO);
		cv::threshold(srcMatRef, srcMatRef, 0, 255, cv::THRESH_BINARY);

		// 筛选最小外接矩形
		CContours contour(srcMatRef);
		std::list<CRotatedRect> rectList;
		contour.get_min_area_rect(std::back_insert_iterator(rectList));

		// 最小外接矩形的面积
		CRotatedRect::select_area(
			rectList,
			[](const double area){
				return area > 16500;
			}
		);

		// 绘制矩形
		CRotatedRect::draw_on_image(srcMat, rectList, cv::Scalar(0, 0, 255), 2, cv::LineTypes::LINE_8);

		fast_show(srcMat);
	}
}