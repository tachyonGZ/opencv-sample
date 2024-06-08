#pragma once
#include "stdafx.h"
#include "CAutoSizedTemplates.hpp"

/**
	@brief ģ��ƥ���㷨
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
		double m_correlation; //����������
	};

private:
	/**
		@brief ͨ���Ҷ�ֱ��ͼ�������Ƴ̶�
	**/
	/*
	double static get_correlation(const cv::Mat &imgA, const CResizedTemplate &resizedTemplate);
	*/
public:
	/**
		@brief ����Ӧģ��ƥ��
	**/
	cv::Rect static auto_size_match(const cv::Mat &srcImg, const CAutoSizedTemplates &autoSizedTemplates);
};