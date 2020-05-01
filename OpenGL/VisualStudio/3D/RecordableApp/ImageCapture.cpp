#include "stdafx.h"
#include "ImageCapture.h"


#include "GL/glew.h"



#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <ZGL/Debugging.h>



#include <opencv2/core/utils/logger.hpp>


;

ImageCapture::ImageCapture()
{
}


ImageCapture::~ImageCapture()
{
}

void ImageCapture::Init(int width, int height)
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);
	m_FBO.Init(width, height, 1);
	m_data = new float[width*height * 4];
}

void ImageCapture::BindForWriting()
{
	m_FBO.BindForWriting();
}

void ImageCapture::BindForReading()
{
	m_FBO.BindForReading(GL_TEXTURE0, 0);
}

void ImageCapture::Snapshot(std::string namefile, std::string ext)
{
	cv::Mat mat = Snapshot();
	std::string realname = namefile + "_" + std::to_string(m_count) + "." + ext;
	bool succeed = cv::imwrite(realname.c_str(), mat);
	m_count++;
	if (!succeed)
	{
		INTERNALERROR("impossible to write image");
	}
}

cv::Mat ImageCapture::Snapshot()
{

	TexParam param = m_FBO.getTexParam();
	if (param.m_type != GL_FLOAT || param.m_channel != GL_RGBA)
	{
		INTERNALERROR("unhandle type of texture");
	}

	
	m_FBO.getTexData(m_data);
	cv::Mat mat(param.m_height, param.m_width, CV_32FC4, m_data);
	mat = 255 * mat;
	cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA, 4);
	cv::flip(mat, mat, 1);

	return mat;
	
}
