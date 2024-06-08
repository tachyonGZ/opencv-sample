#pragma once
#include "stdafx.h"

class CRotatedRect
{
public:
	/**
		@brief ��ͼƬ�ϻ��Ƴ�����
		@param image ������ͼƬ�ϻ��ƾ���
		@param container �������ε�����
		@param scalar ��ɫ
		@param thinkness ������ϸ
		@param lineType �������Ʒ�ʽ
	**/
	template<typename Container>
	static void draw_on_image(cv::Mat &image, Container &container, cv::Scalar &&scalar, int thinkness, cv::LineTypes lineType = cv::LineTypes::LINE_8)
	{
		for(const CRotatedRect &iter : container)
		{
			std::array<cv::Point2f, 4> boxPoints;
			iter.m_rotatedRect.points(boxPoints.data());
			for(size_t j = 0; j < boxPoints.max_size(); j++)
			{
				cv::line(image, boxPoints[j], boxPoints[(j + 1) % 4], scalar, thinkness, lineType);
			}
		}
	}

	// ������С��Ӿ��ε���Ϣ
	static const cv::Mat draw_min_area_rect_info(cv::Mat &&image, const std::list<CRotatedRect> &rotatedRectVec, cv::Scalar &&scalar)
	{
		std::stringstream sstream(std::ios_base::in | std::ios_base::out | std::ios_base::ate);
		for(const CRotatedRect &iter : rotatedRectVec)
		{
			
			cv::Point2f pos = iter.m_rotatedRect.center;
			sstream.str("area: ");
			sstream <<  iter.m_rotatedRect.size.width * iter.m_rotatedRect.size.height;
			cv::putText(image, sstream.str(), pos, cv::FONT_HERSHEY_SIMPLEX, 1.0f, scalar);

			pos.y += 20.0f;
			sstream.str("aspect ratio: ");
			sstream << iter.m_rotatedRect.size.height * iter.m_rotatedRect.size.width;
			cv::putText(image, sstream.str(), pos, cv::FONT_HERSHEY_SIMPLEX, 1.0f, scalar);
		}
		return image;
	}

	// ��ӡ��С��Ӿ��ε���Ϣ
	static const cv::Mat &print_min_area_rect_info(cv::Mat &&image, const std::list<CRotatedRect> &rotatedRectVec, cv::Scalar &&scalar)
	{
		std::stringstream sstream(std::ios_base::in | std::ios_base::out | std::ios_base::ate);
		size_t i = 0;
		for(const CRotatedRect &iter : rotatedRectVec)
		{
			cv::putText(image, std::to_string(i), iter.m_rotatedRect.center, cv::FONT_HERSHEY_SIMPLEX, 0.75f, scalar);
			sstream << "id: " << i
					<< "\narea: " << iter.m_rotatedRect.size.width * iter.m_rotatedRect.size.height
					<< "\naspect ratio: " << iter.m_rotatedRect.size.height / iter.m_rotatedRect.size.width << '\n';
			i++;
		}

		std::cout << sstream.str();
		return image;
	}

	/*
		@brief ���ɸѡ
		@param container ����ΪCRotatedRect���͵�����
		@param check ɸѡ���������
			����һ��double���͵�ֵ�������
			������ֵΪtrueʱ����ʾ���ͨ��ɸѡ�����Ա���
	*/
	template<typename Container, typename Check>
	static void select_area(Container &container, const Check &check)
	{
		using iterator = Container::iterator;
		for(iterator iter = container.begin(); iter != container.end(); )
		{
			if(!check(iter->m_rotatedRect.size.height * iter->m_rotatedRect.size.width))
			{
				iter = container.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	/*
		@brief ���ɸѡ
		@param container ����ΪCRotatedRect���͵�����
		@param check ɸѡ���������
			����һ��double���͵�ֵ�������
			������ֵΪtrueʱ����ʾ���ͨ��ɸѡ�����Ա���
	*/
	template<typename Container, typename Check>
	static void select_aspect_ratio(Container &container, Check &check)
	{
		using iterator = Container::iterator;
		for(iterator iter = container.begin(); iter != container.end(); )
		{
			if(!check(iter->m_rotatedRect.size.height / iter->m_rotatedRect.size.width))
			{
				iter = container.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
public:
	CRotatedRect(const cv::RotatedRect &rotatedRect)
		:	m_rotatedRect(rotatedRect)
	{
		;
	}

	CRotatedRect(cv::RotatedRect &&rotatedRect)
		:	m_rotatedRect(rotatedRect)
	{
		;
	}

	CRotatedRect &operator=(cv::RotatedRect &&rotatedRect)
	{
		m_rotatedRect = rotatedRect;
		return *this;
	}

	CRotatedRect &draw_on_image(const cv::Mat &image, cv::Scalar &&scalar, int thinkness = 1, cv::LineTypes lineType = cv::LineTypes::LINE_8)
	{
		std::array<cv::Point2f, 4> boxPoints;
		m_rotatedRect.points(boxPoints.data());
		for(size_t j = 0; j < boxPoints.max_size(); j++)
		{
			cv::line(image, boxPoints[j], boxPoints[(j + 1) % 4], scalar, thinkness, lineType);
		}

		return *this;
	}

	CRotatedRect &write_text_on_image(const cv::Mat &image, const std::string &text, cv::HersheyFonts fontFace, int fontScale, cv::Scalar &&scalar, int thinkness = 1)
	{
		std::array<cv::Point2f, 4> boxPoints;
		m_rotatedRect.points(boxPoints.data());
		cv::putText(image, text, boxPoints[1], fontFace, fontScale, scalar, thinkness);

		return *this;
	}

	inline cv::Point2f get_center() const
	{
		return m_rotatedRect.center;
	}
public:
	cv::RotatedRect m_rotatedRect;
};