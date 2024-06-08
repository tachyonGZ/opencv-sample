#include "../inc/CAutoSizedTemplates.hpp"

CAutoSizedTemplates::CAutoSizedTemplates(const cv::Mat &oriTemplate, const int nPiece)
	: m_nPiece(nPiece)
{
	double ratio = 1.0f / nPiece;
	for(int i = 0; i < nPiece; i++)
	{
		// 创建缩放后的新模板
		cv::Mat resizeTemplateMat = oriTemplate.clone();
		cv::resize(
			resizeTemplateMat, 
			resizeTemplateMat, 
			cv::Size(
				oriTemplate.cols - static_cast<int>(i * ratio * oriTemplate.cols),
				oriTemplate.rows - static_cast<int>(i * ratio * oriTemplate.rows)
			)
		);
		resizedTemplatesList.emplace_back(std::move(resizeTemplateMat));
	}
}