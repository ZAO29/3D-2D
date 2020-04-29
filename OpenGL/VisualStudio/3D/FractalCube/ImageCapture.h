#pragma once
#include <string>
#include <ZGL/FBO.h>
#include <opencv2/core.hpp>

class ImageCapture
{
public:
	ImageCapture();
	~ImageCapture();


	void Init(int width, int height);

	void BindForWriting();

	void Snapshot(std::string namefile,std::string ext);

private :
	FBO m_FBO;
	cv::Mat m_im;
	float* m_data;
	unsigned int m_count = 0;

};

