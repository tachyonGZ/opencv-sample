#include "../inc/stdafx.h"
#include "../inc/CContours.hpp"
#include "../inc/CRotatedRect.hpp"

void task3()
{
	for (const auto& i : std::filesystem::directory_iterator("D:\\Download\\IFR\\Task\\S2_Q3\\pictures\\"))
	{
		cv::Mat srcMat = cv::imread(i.path().string(), cv::IMREAD_COLOR);
		fast_show(srcMat);
		// �ָ�ͨ��
		std::vector<cv::Mat> matVec(3);
		cv::split(srcMat, matVec);


		cv::Mat &srcMatRef = matVec[0];

		// ��ֵ����
		cv::threshold(srcMatRef, srcMatRef, 128, 0, cv::THRESH_TOZERO);
		cv::threshold(srcMatRef, srcMatRef, 0, 255, cv::THRESH_BINARY);

		// ɸѡ��С��Ӿ���
		CContours contour(srcMatRef);
		std::list<CRotatedRect> rectList;
		contour.get_min_area_rect(std::back_insert_iterator(rectList));

		// ��С��Ӿ��ε����
		CRotatedRect::select_area(
			rectList,
			[](const double area){
				return area > 16500;
			}
		);

		// ���ƾ���
		CRotatedRect::draw_on_image(srcMat, rectList, cv::Scalar(0, 0, 255), 2, cv::LineTypes::LINE_8);

		fast_show(srcMat);
	}
}