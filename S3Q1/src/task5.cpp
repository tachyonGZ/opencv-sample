#include "../inc/stdafx.h"

class CRedrawProcess final
{
private:
	std::list<desuwa::CTemplate> m_hammerTemplateList;
	std::list<desuwa::CTemplate> m_swordTemplateList;
public:
	CRedrawProcess()
	{
		std::cout << "正在读取Hammer模板集...\n";
		for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator("D:\\Download\\IFR\\Task\\S3_Q3\\模板\\Hammer\\"))
		{
			m_hammerTemplateList.emplace_back(cv::imread(i.path().string(), cv::IMREAD_GRAYSCALE));
		}

		std::cout << "正在读取Sword模板集...\n";
		for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator("D:\\Download\\IFR\\Task\\S3_Q3\\模板\\Sword\\"))
		{
			m_swordTemplateList.emplace_back(cv::imread(i.path().string(), cv::IMREAD_GRAYSCALE));
		}
	}

	void redraw(cv::Mat &frameImg)
	{
		// 获取R通道
		std::vector<cv::Mat> matVec(3);
		cv::split(frameImg, matVec);
		cv::Mat &srcMatRef = matVec[2];

		// 阈值处理，使图像成为二值图
		cv::threshold(srcMatRef, srcMatRef, 150, 255, cv::THRESH_BINARY);

		// 膨胀
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
		cv::dilate(srcMatRef, srcMatRef, element);

		// 闭运算孔洞填充
		element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
		cv::morphologyEx(srcMatRef, srcMatRef, cv::MORPH_CLOSE, element);

		// 筛选最小外接矩形
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
				// 令宽大于高
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
					
					// 筛选最小外接矩形
					desuwa::CContours contour(perspectMatWithBorder);
					std::vector<desuwa::CRotatedRect> rotatedRectVec;
					contour.get_min_area_rect(std::back_inserter(rotatedRectVec));

					// 通过仿射变换获得中心点在原图上的位置
					std::array<cv::Point2f, 1> srcVec{rotatedRectVec[0].get_center()};
					std::array<cv::Point2f, 1> dstVec;

					cv::Mat invertTransform;	// 仿射变换的逆
					cv::invert(transform, invertTransform, cv::DECOMP_LU);

					cv::perspectiveTransform(srcVec, dstVec, std::move(invertTransform));

					// 画点
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
	cv::VideoCapture capture("D:\\Download\\IFR\\Task\\S3_Q3\\3低曝光红.mp4");
	if(!capture.isOpened())
	{
		throw std::exception("can not open video");
	}
	int totalFrame = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_COUNT));
	int fps = static_cast<int>(capture.get(cv::CAP_PROP_FPS)); 
	std::cout << "视频总帧数：" << totalFrame << "\n视频帧率：" << fps << std::endl;

	// 每帧所占的毫秒数
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
		//std::cout << "绘制耗时:" << duration.count() << "毫秒\n";

		cv::waitKey(mspf - (duration.count() % mspf));
	}
}