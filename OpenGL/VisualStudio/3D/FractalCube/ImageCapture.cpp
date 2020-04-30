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
	int width, height, alignment;
	//glEnable(GL_TEXTURE_2D);
	m_FBO.BindForReading(GL_TEXTURE0);
	glGetIntegerv(GL_PACK_ALIGNMENT, &alignment);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	// Fiddle with alignment to make sure you get properly aligned buffer/width.


	int mipmapLevel = 0;
	//glGetTexImage(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, GL_FLOAT, &imagedata[0]);
	glGetTexImage(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, GL_FLOAT, &m_data[0]);



	cv::Mat mat(height, width, CV_32FC4, m_data);
	mat = 255 * mat;
	cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA, 4);
	cv::flip(mat, mat, 1);

	return mat;
	
}
