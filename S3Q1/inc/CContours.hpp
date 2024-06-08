#pragma once
#include "stdafx.h"
#include "CRotatedRect.hpp"

class CContours
{
public:
	/**
		@param img 阈值图
	**/
	CContours(const cv::Mat &img)
	{
		cv::findContours(img, m_contoursVec, m_hierarchyVec, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
		//m_pSelectMap = std::make_unique<std::vector<bool>>(m_contoursVec.size(), true);
	}

	/**
		@brief 获取边界的最小外接矩形
		@param iter CRotatedRect元素类型的容器的迭代器
	**/
	template<typename OutputIt>
	const CContours &get_min_area_rect(OutputIt iter) const
	{
		for(auto &i : m_contoursVec)
		{
			iter = cv::minAreaRect(cv::Mat(i));
		}
		return *this;
	}
public:
	std::vector<std::vector<cv::Point>> m_contoursVec;
	std::vector<cv::Vec4i> m_hierarchyVec;
};