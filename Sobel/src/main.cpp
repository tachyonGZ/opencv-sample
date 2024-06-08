#include <opencv2/opencv.hpp>
#include <memory>
#include <cstdint>

struct CSobelKernel
{
public:
	cv::Mat xKernel, yKernel;
public:
	CSobelKernel()
		: xKernel(3, 3, CV_8SC1), yKernel(3, 3, CV_8SC1)
	{
		;
	}
};

std::unique_ptr<CSobelKernel> make_Sobel_kernel()
{
	std::unique_ptr<CSobelKernel> pKernel = std::make_unique<CSobelKernel>();

	std::array<schar, 9> xKernelArray{
		-1, -2, -1,
		 0,  0,  0,
		 1,  2,  1
	};

	std::copy(xKernelArray.begin(), xKernelArray.end(), pKernel->xKernel.begin<schar>());

	std::array<schar, 9> yKernelArray{
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};
	
	std::copy(yKernelArray.begin(), yKernelArray.end(), pKernel->yKernel.begin<schar>());

	#ifdef _DEBUG
	
	std::cout << "xKernel: ";

	for(cv::MatIterator_<schar> i = pKernel->xKernel.begin<schar>(); i != pKernel->xKernel.end<schar>(); i++)
	{
		std::cout << static_cast<std::int32_t>(*i) << ", ";
	}

	std::cout << std::endl;

	std::cout << "yKernel: ";

	for (cv::MatIterator_<schar> i = pKernel->yKernel.begin<schar>(); i != pKernel->yKernel.end<schar>(); i++)
	{
		std::cout << static_cast<std::int32_t>(*i) << ", ";
	}

	std::cout << std::endl;
	
	#endif

	return pKernel;

}

void sobel(const cv::Mat &src, cv::Mat &dst)
{
	std::unique_ptr<CSobelKernel> p_sobel = make_Sobel_kernel();


	cv::Mat imgSobelX(src.rows, src.cols, src.type());
	cv::filter2D(src, imgSobelX, -1, p_sobel->xKernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
	cv::imshow("image", imgSobelX);
	cv::waitKey(0);

	cv::Mat imgSobelY(src.rows, src.cols, src.type());
	cv::filter2D(src, imgSobelY, -1, p_sobel->yKernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
	cv::imshow("image", imgSobelY);
	cv::waitKey(0);

	imgSobelX.copyTo(dst);

	cv::MatIterator_<uchar> inputIter = dst.begin<uchar>();

	for (int r = 0; r < dst.rows; r++)
	{
		uchar* p_in = dst.ptr<uchar>(r);
		uchar* p_out = imgSobelY.ptr<uchar>(r);
		for (int c = 0; c < dst.cols * dst.channels(); c++)
		{
			p_in[c] += p_out[c];
		}
	}

	/*
	for (cv::MatIterator_<uchar> i = imgSobelY.begin<uchar>(); i != imgSobelY.end<uchar>(); i++)
	{
		//(*inputIter) += (*i) * (*i);
		//(*inputIter) += (*i);
	}
	*/
}

int main(int argc, char** argv)
{
	std::string imgFileFullName("D:\\MyFile\\");
	std::inserter(imgFileFullName, imgFileFullName.end());

	//imgFileName += "texas.jpg";
	cv::Mat img = cv::imread(imgFileFullName.c_str(), cv::IMREAD_GRAYSCALE);

	cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
	cv::imshow("image", img);
	cv::waitKey(0);

	cv::Mat imgSobel;

	sobel(img, imgSobel);

	cv::imshow("image", imgSobel);
	cv::waitKey(0);

	cv::destroyWindow("image");
	return 0;
}