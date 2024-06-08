#pragma once

#include "stdafx.h"

class CTemplate final
{
public:
	/**
		@param templateImage ģ��ĻҶ�ͼ
	**/
	CTemplate(cv::Mat &&templateImage);

	/**
		@brief ��ȡģ����һ��ͼƬ�������
		@param image ���Ƚϵ�ͼƬ��ӦΪ��ͨ���Ҷ�ͼ
		@return [0,1] ֮��ĸ�������Խ��������ƶ�Խ��
	**/
	double get_correlation(cv::Mat &&image) const;

	/**
		@brief ��ȡģ�弯����һ��ͼƬ�������
		@param first ģ���������׵�����
		@param last ģ��������β�������
		@param image ���Ƚϵ�ͼƬ��ӦΪ��ͨ���Ҷ�ͼ
		@return [0,1] ֮��ĸ�������Խ�������ģ�弯�ϵ����ƶ�Խ��
	**/
	template<typename InputIt>
	double static get_correlation(InputIt first, InputIt last, const cv::Mat &image, int method = cv::TM_CCOEFF_NORMED)
	{
		double maxCorrelation = 0.0f;
		cv::Mat resizedImage = image.clone();
		for(InputIt iter = first; iter != last; iter++)
		{
			// ������Сʹ�ߴ���ͬ
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