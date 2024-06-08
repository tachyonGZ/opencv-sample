#include "../inc/stdafx.h"
#include "../inc/CConnections.hpp"
#include "../inc/CContours.hpp"

cv::Mat reduce_domain(cv::Mat &&mat, const cv::Mat &mask)
{
	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			if (mask.at<uchar>(i, j) == 0)
			{
				mat.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
			}
		}
	}
	return mat;
}

void task1()
{
	cv::Mat srcMat = cv::imread("D:\\Download\\IFR\\Task\\S2_Q1\\number.png", cv::IMREAD_COLOR);
	if (srcMat.empty())
	{
		std::cerr << "���ļ�\n";
	}

	// �ָ�ͨ��
	std::vector<cv::Mat> matVec(3);
	cv::split(srcMat, matVec);

	cv::Mat& srcMatRef = matVec[1];

	cv::Mat temp(srcMatRef.size(), srcMatRef.type());

	// ��ֵ����
	cv::threshold(srcMatRef, srcMatRef, 164, 0, cv::THRESH_TOZERO_INV);
	cv::threshold(srcMatRef, srcMatRef, 44, 0, cv::THRESH_TOZERO);
	cv::threshold(srcMatRef, srcMatRef, 0, 255, cv::THRESH_BINARY);

	// �������
	cv::Mat openKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size{ 1, 3 });

	// ������
	cv::erode(srcMatRef, srcMatRef, openKernel);
	cv::dilate(srcMatRef, srcMatRef, openKernel);

	// �������
	cv::Mat closeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size{ 2, 1 });

	// ������
	cv::dilate(srcMatRef, srcMatRef, closeKernel);
	cv::erode(srcMatRef, srcMatRef, closeKernel);

	// ���ɸѡ
	cv::Mat coloredResMat = CConnections(srcMatRef).area_select(
		[](const int area){
			return area > 64 && area < 512;
		}	
	).convert_to_image();

	fast_show(coloredResMat);
}

void task2()
{
	for(const auto &i: std::filesystem::directory_iterator("D:\\Download\\IFR\\Task\\S2_Q2\\appleͼ��\\"))
	{
		cv::Mat srcMat = cv::imread(i.path().string(), cv::IMREAD_COLOR);
		fast_show(srcMat);

		// �ָ�ͨ��
		std::vector<cv::Mat> matVec(3);
		cv::split(srcMat, matVec);
		

		cv::Mat &&srcMatRef = cv::abs(matVec[2] - matVec[1]);
		// ��ֵ����
		cv::threshold(srcMatRef, srcMatRef, 255, 0, cv::THRESH_TOZERO_INV);
		cv::threshold(srcMatRef, srcMatRef, 48, 0, cv::THRESH_TOZERO);
		cv::threshold(srcMatRef, srcMatRef, 0, 255, cv::THRESH_BINARY);

		/*
		// �׶����
		cv::Mat fillMat = srcMatRef.clone();
		cv::floodFill(fillMat, cv::Point(0, 0), cv::Scalar(255));

		// ��ת
		cv::bitwise_not(fillMat, fillMat);

		// ǰ��
		srcMatRef = srcMatRef | fillMat;
		fast_show(srcMatRef);
		*/

		// ������
		cv::Mat closeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size{ 3, 3 });
		cv::dilate(srcMatRef, srcMatRef, closeKernel);
		cv::erode(srcMatRef, srcMatRef, closeKernel);

		
		// ɸ��С�������
		cv::bitwise_xor(
			CConnections(srcMatRef).area_select(
				[](const int area){
					return area > 0 && area < 512;
				}
			).threshold(), 
			srcMatRef, 
			srcMatRef
		);
		fast_show(reduce_domain(std::move(srcMat), srcMatRef));
	}
}



int main(int argc, char *argv)
{
	int i = 0;
	std::cin >> i;
	switch(i)
	{
	case 1:
		task1();
		break;
	case 2:
		task2();
		break;
	case 3:
		task3();
		break;
	case 4:
		task4();
		break;
	case 5:
		task5();
		break;
	}
	return EXIT_SUCCESS;
}
 