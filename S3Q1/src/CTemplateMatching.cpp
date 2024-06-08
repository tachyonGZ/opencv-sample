#include "../inc/CTemplateMatching.hpp"
/*
double CTemplateMatching::get_correlation(const cv::Mat &imgA, const CResizedTemplate &resizedTemplate)
{
	if(imgA.size != resizedTemplate.m_template.size)
	{
		throw std::exception("size not same");
	}

	cv::Mat imgHist;
	const int static channels[] = { 0 };
	const int static histSize[] = { 256 };
	const float static pRanges[] = { 0.0f, 255.0f };
	const float static *ranges[] = { pRanges };
	cv::Mat static mask;
	cv::calcHist(&imgA, 1, channels, mask, imgHist, 1, histSize, ranges);

	return cv::compareHist(imgHist, resizedTemplate.m_hist, cv::HISTCMP_CORREL);
}
*/

cv::Rect CTemplateMatching::auto_size_match(const cv::Mat &srcImg, const CAutoSizedTemplates &autoSizedTemplates)
{
	// 匹配结果列表
	std::list<CMatchResult> resList;
	for(CAutoSizedTemplates::const_iterator i = autoSizedTemplates.cbegin(); i != autoSizedTemplates.cend(); i++)
	{
		cv::Mat matchResMat(srcImg.dims, srcImg.size, srcImg.type());

		// 模板匹配
		cv::matchTemplate(srcImg, i -> m_templateImage, matchResMat, cv::TM_SQDIFF);

		/*
		// 对匹配结果矩阵归一化
		cv::normalize(matchResMat, matchResMat, 0, 1, cv::NORM_MINMAX, -1);
		*/

		// 寻找最佳匹配矩形
		double minValue, maxValue;
		cv::Point minPoint, maxPoint;
		cv::minMaxLoc(matchResMat, &minValue, &maxValue, &minPoint, &maxPoint);
		cv::Rect resRect(minPoint.x, minPoint.y, i -> m_templateImage.cols, i -> m_templateImage.rows);

		// 匹配图片与原模板的直方图比较，得出两张图的相关性
		double correlation = i -> get_correlation(srcImg(resRect));

		// 匹配最佳的结果加入列表中
		resList.emplace_back(std::move(resRect), correlation);
	}

	auto most = resList.cbegin();	// 最相关的矩形
	for(auto i = resList.cbegin(); i != resList.cend(); i++)
	{
		if(i -> m_correlation > most -> m_correlation)
		{
			most = i;
		}
	}

	return most -> m_rect;
}