#pragma once
#include "stdafx.h"

/**
	@brief ³¬¾ØÕó
**/
class CSuperMat
{
public:
	CSuperMat(const cv::Mat &mat, const cv::Mat &mask)
		:	m_mat(mat.clone()), m_mask(mask.clone())
	{
		;
	}
	CSuperMat(const CSuperMat &other)
	{
		this -> m_mat = other.m_mat.clone();
		this -> m_mask = other.m_mask.clone();
	}
	CSuperMat &operator=(const CSuperMat &other)
	{
		this -> m_mat = other.m_mat.clone();
		this -> m_mask = other.m_mask.clone();
	}
	CSuperMat(CSuperMat &&other)
	{
		this -> m_mat = other.m_mat;
		this -> m_mask = other.m_mask;
	}
	CSuperMat &operator=(const CSuperMat &other)
	{
		this -> m_mat = other.m_mat;
		this -> m_mask = other.m_mask;
	}
public:
	cv::Mat m_mat;
	cv::Mat m_mask;
};