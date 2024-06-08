#include "../inc/stdafx.h"

class CRedrawProcess final
{
private:
	std::list<desuwa::CTemplate> m_hammerTemplateList;
	std::list<desuwa::CTemplate> m_swordTemplateList;
public:
	CRedrawProcess()
	{
		std::cout << "���ڶ�ȡHammerģ�弯...\n";
		for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator("D:\\Download\\IFR\\Task\\S3_Q3\\ģ��\\Hammer\\"))
		{
			m_hammerTemplateList.emplace_back(cv::imread(i.path().string(), cv::IMREAD_GRAYSCALE));
		}

		std::cout << "���ڶ�ȡSwordģ�弯...\n";
		for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator("D:\\Download\\IFR\\Task\\S3_Q3\\ģ��\\Sword\\"))
		{
			m_swordTemplateList.emplace_back(cv::imread(i.path().string(), cv::IMREAD_GRAYSCALE));
		}
	}

	void redraw(cv::Mat &frameImg)
	{
		// ��ȡRͨ��
		std::vector<cv::Mat> matVec(3);
		cv::split(frameImg, matVec);
		cv::Mat &srcMatRef = matVec[2];

		// ��ֵ����ʹͼ���Ϊ��ֵͼ
		cv::threshold(srcMatRef, srcMatRef, 150, 255, cv::THRESH_BINARY);

		// ����
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
		cv::dilate(srcMatRef, srcMatRef, element);

		// ������׶����
		element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
		cv::morphologyEx(srcMatRef, srcMatRef, cv::MORPH_CLOSE, element);

		// ɸѡ��С��Ӿ���
		desuwa::CContours contour(srcMatRef);
		std::list<desuwa::CRotatedRect> listRect;
		contour.get_min_area_rect(std::back_inserter(listRect));

		desuwa::CRotatedRect::select_area(
			listRect,
			[](const double area){
				return area > 16500.0f;
			}
		);
	
		for(desuwa::CRotatedRect &rotatedRect : listRect)
		{
			std::array<cv::Point2f, 4> srcRect;
			rotatedRect.m_rotatedRect.points(srcRect.data());
			float y = rotatedRect.m_rotatedRect.size.height;
			float x = rotatedRect.m_rotatedRect.size.width;

			if(y > x)
			{
				// �����ڸ�
				std::swap(x ,y);
				cv::Point2f temp = srcRect[0];
				srcRect[0] = srcRect[1];
				srcRect[1] = srcRect[2];
				srcRect[2] = srcRect[3];
				srcRect[3] = std::move(temp);
			}

			std::array<cv::Point2f, 4> dstRect;
			dstRect[0] = cv::Point2f(0, 0);
			dstRect[1] = cv::Point2f(0, y);
			dstRect[2] = cv::Point2f(x, y);
			dstRect[3] = cv::Point2f(x, 0);

			cv::Mat transform = cv::getPerspectiveTransform(srcRect.data(), dstRect.data(), cv::DecompTypes::DECOMP_LU);
			cv::Mat perspectMat;
			cv::warpPerspective(srcMatRef, perspectMat, transform, cv::Size2f(x, y), cv::INTER_NEAREST);
			
			double hammerCorrelation = desuwa::CTemplate::get_correlation(m_hammerTemplateList.cbegin(), m_hammerTemplateList.cend(), perspectMat);
			double swordCorrelation = desuwa::CTemplate::get_correlation(m_swordTemplateList.cbegin(), m_swordTemplateList.cend(), perspectMat);
			if(hammerCorrelation > 0.6f || swordCorrelation > 0.6f)
			{

				if(hammerCorrelation > swordCorrelation)
				{
					rotatedRect.draw_on_image(frameImg, cv::Scalar(0, 255, 0), 2);
					rotatedRect.write_text_on_image(frameImg, "inactive", cv::HersheyFonts::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 0), 2);
					cv::Mat perspectMatWithBorder(perspectMat.rows + 1, perspectMat.cols + 1, perspectMat.type());
					cv::copyMakeBorder(perspectMat, perspectMatWithBorder, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar(0));
					
					//desuwa::CImageBorder::make_border(perspectMat, 1, 1, 1, 1, cv::Scalar(0, 0, 0));
					//cv::Mat perspectMatWithBorder = perspectMat;
					cv::floodFill(perspectMatWithBorder, cv::Point(0, 0), cv::Scalar(255));
					cv::bitwise_not(perspectMatWithBorder, perspectMatWithBorder);
					
					// ɸѡ��С��Ӿ���
					desuwa::CContours contour(perspectMatWithBorder);
					std::vector<desuwa::CRotatedRect> rotatedRectVec;
					contour.get_min_area_rect(std::back_inserter(rotatedRectVec));

					// ͨ������任������ĵ���ԭͼ�ϵ�λ��
					std::array<cv::Point2f, 1> srcVec{rotatedRectVec[0].get_center()};
					std::array<cv::Point2f, 1> dstVec;

					cv::Mat invertTransform;	// ����任����
					cv::invert(transform, invertTransform, cv::DECOMP_LU);

					cv::perspectiveTransform(srcVec, dstVec, std::move(invertTransform));

					// ����
					cv::circle(frameImg, dstVec[0], 8, cv::Scalar(0, 255, 0), -1);
				}
				else
				{
					rotatedRect.draw_on_image(frameImg, cv::Scalar(0, 255, 255), 2);
					rotatedRect.write_text_on_image(frameImg, "active", cv::HersheyFonts::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 255), 2);
				}
			}

		}
	}
};



void task5()
{
	cv::VideoCapture capture("D:\\Download\\IFR\\Task\\S3_Q3\\3���ع��.mp4");
	if(!capture.isOpened())
	{
		throw std::exception("can not open video");
	}
	int totalFrame = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_COUNT));
	int fps = static_cast<int>(capture.get(cv::CAP_PROP_FPS)); 
	std::cout << "��Ƶ��֡����" << totalFrame << "\n��Ƶ֡�ʣ�" << fps << std::endl;

	// ÿ֡��ռ�ĺ�����
	int mspf = 1000 / fps;

	cv::Mat frameImg;
	cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
	CRedrawProcess redrawProc;

	for(int i = 0; i < totalFrame; i++)
	{
		capture >> frameImg;
		std::chrono::steady_clock::time_point startTimePoint = std::chrono::high_resolution_clock::now();
		redrawProc.redraw(frameImg);
		cv::imshow("Video", frameImg);
		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTimePoint);
		//std::cout << "���ƺ�ʱ:" << duration.count() << "����\n";

		cv::waitKey(mspf - (duration.count() % mspf));
	}
}