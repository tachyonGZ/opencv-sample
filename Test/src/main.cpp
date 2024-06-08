#include <opencv2/opencv.hpp>
#include <iostream>

class Sharpen
{
public:
	static cv::Mat sharpen(const cv::Mat& imageMat)
	{
		const int nChannels = imageMat.channels();
		cv::Mat resultMat(imageMat.size(), imageMat.type());
		for (int j = 1; j < imageMat.rows - 1; j++)
		{
			const uchar *previous = imageMat.ptr<uchar>(j - 1);
			const uchar	*current = imageMat.ptr<uchar>(j);
			const uchar	*next = imageMat.ptr<uchar>(j + 1);

			uchar *pOutput = resultMat.ptr<uchar>(j);

			for (int i = nChannels; i < nChannels * (imageMat.cols - 1); i++)
			{
				pOutput++;

				*pOutput = cv::saturate_cast<uchar>(
					current[i] * 5 -
					current[i - 1] -
					current[i + 1] -
					previous[i] - 
					next[i]
				);
			}
		}

		resultMat.row(0).setTo(cv::Scalar(0));
		resultMat.col(0).setTo(cv::Scalar(0));
		resultMat.row(resultMat.rows - 1).setTo(cv::Scalar(0));
		resultMat.col(resultMat.cols - 1).setTo(cv::Scalar(0));

		return resultMat;
	}
};

int main(int argc, char **argv)
{
	cv::Mat srcMat = cv::imread(".\\test.jpg", cv::IMREAD_GRAYSCALE);
	if (srcMat.empty())
	{
		std::cerr << "不能打开文件！\n";
		return EXIT_FAILURE;
	}

	cv::namedWindow("Input", cv::WINDOW_AUTOSIZE);
	cv::imshow("Input", srcMat);

	cv::Mat dst0Mat = Sharpen::sharpen(srcMat);
	cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
	cv::imshow("Output", dst0Mat);

	cv::waitKey();


	cv::Mat kernelMat = (
		cv::Mat_<char>(3, 3) << 
		0, -1, 0,
		-1, 5, -1,
		0, -1, 0
	);

	
	return EXIT_SUCCESS;
}