#pragma once

#include "stdafx.h"

class CConnections
{
public:
	CConnections(const cv::Mat &mat)
		:	m_mat(mat.clone())
	{
		m_num = cv::connectedComponentsWithStats(
			mat,
			m_labelsMat,
			m_statsMat,
			m_centroidsMat
		);

		m_pLabelMap = std::make_unique<std::vector<bool>>(m_num, true);
		m_pLabelMap -> at(0) = false;
	}

	template<typename F>
	CConnections &area_select(F check)
	{
		for (int i = 1; i < m_num; i++)
		{
			if (!check(m_statsMat.at<int>(i, cv::CC_STAT_AREA)))
			{
				m_pLabelMap -> at(i) = false;
			}
		}
		return *this;
	}

	inline cv::Mat convert_to_image() const
	{
		// 初始化颜色表
		std::vector<cv::Vec3b> colorMap(m_num, cv::Vec3b(0, 0, 0));

		// 为每组Label分配颜色
		for (int i = 1; i < m_num; i++)
		{
			if (m_pLabelMap -> at(i))
			{
				colorMap[i] = cv::Vec3b(std::rand() % 256, std::rand() % 256, std::rand() % 256);
			}
		}

		// 填色
		cv::Mat resMat(m_mat.size(), CV_8UC3, cv::Scalar(0, 0, 0));
		for (int i = 0; i < m_mat.rows; i++)
		{
			for (int j = 0; j < m_mat.cols; j++)
			{
				if (m_pLabelMap->at(m_labelsMat.at<int>(i, j)))
				{
					resMat.at<cv::Vec3b>(i, j) = colorMap.at(m_labelsMat.at<int>(i, j));
				}
			}
		}
		return resMat;
	}

	inline cv::Mat threshold()
	{
		cv::Mat resMat(m_mat.size(), CV_8UC1, cv::Scalar(0));
		for (int i = 0; i < m_mat.rows; i++)
		{
			for (int j = 0; j < m_mat.cols; j++)
			{
				if (m_pLabelMap->at(m_labelsMat.at<int>(i, j)))
				{
					resMat.at<uchar>(i, j) = 255;
				}
			}
		}

		return resMat;
	}
public:
	int m_num = 0;

	// 掩码器 默认值为true
	std::unique_ptr<std::vector<bool>> m_pLabelMap;

	const cv::Mat m_mat;
	cv::Mat m_labelsMat, m_statsMat, m_centroidsMat;
};