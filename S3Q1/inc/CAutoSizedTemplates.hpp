#pragma once

#include "stdafx.h"
#include "CTemplate.hpp"

/**
	@brief ×ÔÊÊÓ¦Ä£°å
**/
class CAutoSizedTemplates final
{
public:
	using const_iterator = std::list<CTemplate>::const_iterator;
	using iterator = std::list<CTemplate>::iterator;
	const int m_nPiece = 0;
	std::list<CTemplate> resizedTemplatesList;

public:
	CAutoSizedTemplates(const cv::Mat &oriTemplate, const int nPiece);

	inline const_iterator cbegin() const
	{
		return resizedTemplatesList.cbegin();
	}

	inline const_iterator cend() const
	{
		return resizedTemplatesList.cend();
	}
};