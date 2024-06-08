#include "../inc/stdafx.h"
#include "../inc/CAutoSizedTemplates.hpp"
#include "../inc/CTemplateMatching.hpp"

void task4()
{
	// 读取模板, 构建自适应模板
	CAutoSizedTemplates autoSizedTemplates(
		cv::imread(
			"D:\\Download\\IFR\\Task\\S3_Q2\\橄榄球模板\\template.png",
			cv::IMREAD_GRAYSCALE
		), 
		10
	);

	for (const auto& i : std::filesystem::directory_iterator("D:\\Download\\IFR\\Task\\S3_Q2\\橄榄球\\"))
	{
		// 读取待处理的图像
		cv::Mat srcColorMat = cv::imread(i.path().string(), cv::IMREAD_COLOR);
		cv::Mat srcGrayMat(srcColorMat.size(), CV_8UC1);
		cv::cvtColor(srcColorMat, srcGrayMat, cv::COLOR_RGB2GRAY);
		fast_show(srcColorMat);

		//自适应模板匹配
		cv::Rect matchRect = CTemplateMatching::auto_size_match(srcGrayMat, autoSizedTemplates);

		cv::rectangle(srcColorMat, cv::Point(matchRect.x, matchRect.y), cv::Point(matchRect.x + matchRect.width, matchRect.y + matchRect.height), cv::Scalar(0, 255, 0), 2, cv::LINE_8);
		
		fast_show(srcColorMat);
	}
}