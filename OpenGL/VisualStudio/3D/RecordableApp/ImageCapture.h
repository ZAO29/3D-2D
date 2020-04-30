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

	void BindForReading();

	void RenderQuad() { m_FBO.RenderQuad(); }

	void Snapshot(std::string namefile,std::string ext);
	cv::Mat Snapshot();


private :
	FBO m_FBO;
	 
	float* m_data;
	unsigned int m_count = 0;

};

