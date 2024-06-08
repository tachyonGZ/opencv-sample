#pragma once
#include "stdafx.h"
#include "CAutoSizedTemplates.hpp"

/**
	@brief 模板匹配算法
**/
class CTemplateMatching
{
private:
	class CMatchResult
	{
	public:
		CMatchResult(cv::Rect &&rect, double correlation)
			:	m_rect(rect), m_correlation(correlation)
		{
			;
		}
	public:
		cv::Rect m_rect;
		double m_correlation; //结果的相关性
	};

private:
	/**
		@brief 通过灰度直方图计算相似程度
	**/
	/*
	double static get_correlation(const cv::Mat &imgA, const CResizedTemplate &resizedTemplate);
	*/
public:
	/**
		@brief 自适应模板匹配
	**/
	cv::Rect static auto_size_match(const cv::Mat &srcImg, const CAutoSizedTemplates &autoSizedTemplates);
};