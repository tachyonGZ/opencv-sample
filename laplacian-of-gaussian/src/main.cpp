#include <opencv2/opencv.hpp>
#include <cmath>

std::unique_ptr<cv::Mat> make_LOG_kernel(const int radius, const double sigma)
{
	const int kernelWidth = radius * 2 + 1;
	const int kernelHeight = kernelWidth;
	const int kernelSize = kernelWidth * kernelWidth;

	const double sigma_2 = std::pow(sigma, 2);
	const double sigma_6 = std::pow(sigma_2, 3);

	std::unique_ptr<cv::Mat> p_kernel = std::make_unique<cv::Mat>(cv::Size(kernelWidth, kernelHeight), CV_8SC1);

	//cv::MatIterator_<schar> iter = p_kernel->begin<schar>();

	std::vector<schar> kernelVec(kernelSize);

	std::vector<schar>::iterator iter = kernelVec.begin();
	for (int r = -radius; r < radius; r++)
	{
		for (int c = -radius; c < radius; c++)
		{
			double distance_2 = static_cast<double>(r * r + c * c);

			(*iter) = static_cast<schar>(
				((distance_2 - 2.0f * sigma_2) / sigma_6) * std::exp(-distance_2 / (2.0f * sigma_2))
			);

			iter++;
		}
	}

	std::copy(kernelVec.begin(), kernelVec.end(), p_kernel->begin<schar>());

	return p_kernel;
}

int main(int argc, char **argv)
{
	cv::Mat img = cv::imread("D:\\MyFile\\texas.jpg", cv::IMREAD_GRAYSCALE);

	cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
	cv::imshow("image", img);
	cv::waitKey(0);

	cv::Mat kernelLOG;
	std::unique_ptr<cv::Mat> p_kernelLOG = make_LOG_kernel(3, 1.0f);
	

	cv::Mat imgLOG(img.rows, img.cols, CV_8UC1);
	cv::filter2D(img, imgLOG, -1, *p_kernelLOG, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
	cv::imshow("image", imgLOG);
	cv::waitKey(0);

	cv::destroyWindow("image");
	return 0;
}