#pragma once

#include "stdafx.h"

class CTemplate final
{
public:
	/**
		@param templateImage 模板的灰度图
	**/
	CTemplate(cv::Mat &&templateImage);

	/**
		@brief 获取模板与一张图片的相关性
		@param image 做比较的图片，应为单通道灰度图
		@return [0,1] 之间的浮点数，越大代表相似度越高
	**/
	double get_correlation(cv::Mat &&image) const;

	/**
		@brief 获取模板集合与一张图片的相关性
		@param first 模板容器的首迭代器
		@param last 模板容器的尾后迭代器
		@param image 做比较的图片，应为单通道灰度图
		@return [0,1] 之间的浮点数，越大代表与模板集合的相似度越高
	**/
	template<typename InputIt>
	double static get_correlation(InputIt first, InputIt last, const cv::Mat &image, int method = cv::TM_CCOEFF_NORMED)
	{
		double maxCorrelation = 0.0f;
		cv::Mat resizedImage = image.clone();
		for(InputIt iter = first; iter != last; iter++)
		{
			// 调整大小使尺寸相同
			//cv::Mat resizedImage;
			if(resizedImage.size() != iter -> m_templateImage.size())
			{
				cv::resize(resizedImage, resizedImage, iter -> m_templateImage.size());
			}

			cv::Mat resMat;
			cv::matchTemplate(resizedImage, iter->m_templateImage, resMat, method);
			double value;
			cv::minMaxLoc(resMat, nullptr, &value, nullptr, nullptr);
			if(value > maxCorrelation)
			{
				maxCorrelation = value;
			}
		}

		return maxCorrelation;
	}
public:
	cv::Mat m_templateImage;
	cv::Mat m_hist;
};