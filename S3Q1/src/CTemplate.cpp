#include "../inc/CTemplate.hpp"

CTemplate::CTemplate(cv::Mat &&tempate)
	: m_templateImage(tempate)
{
	const int static channels[] = { 0 };
	const int static histSize[] = { 256 };
	const float static pRanges[] = { 0.0f, 255.0f };
	const float static *ranges[] = { pRanges };
	cv::Mat static mask;
	cv::calcHist(&tempate, 1, channels, mask, m_hist, 1, histSize, ranges);
}

double CTemplate::get_correlation(cv::Mat &&image) const
{
	if(m_templateImage.type() != image.type() || m_templateImage.dims != image.dims)
	{
		throw std::exception("type of template and type of image not same");
	}

	if(m_templateImage.size() != image.size())
	{
		// 如果图像与模板的大小不相同就放缩图像
		cv::resize(image, image, m_templateImage.size());
	}

	cv::Mat imgHist;
	const int static channels[] = { 0 };
	const int static histSize[] = { 256 };
	const float static pRanges[] = { 0.0f, 255.0f };
	const float static *ranges[] = { pRanges };
	cv::Mat static mask;
	cv::calcHist(&image, 1, channels, mask, imgHist, 1, histSize, ranges);

	return cv::compareHist(imgHist, m_hist, cv::HISTCMP_CORREL);
}