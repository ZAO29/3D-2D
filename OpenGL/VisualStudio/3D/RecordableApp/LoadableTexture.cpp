#include "stdafx.h"
#include "LoadableTexture.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <ZGL/Debugging.h>
#include <ZGL/GLGLEW.h>

LoadableTexture::LoadableTexture()
{
}


LoadableTexture::~LoadableTexture()
{
}

bool LoadableTexture::Load(std::string file)
{
	TexParam param;
	cv::Mat imageCV = cv::imread(file, cv::IMREAD_UNCHANGED);
	
	if (imageCV.rows == 0
		|| imageCV.cols == 0)
	{
		INTERNALERROR(" number of rows or number of columns is 0");
	}
	else
	{
		param.m_width = imageCV.cols;
		param.m_height = imageCV.rows;
	}
	
	int nbChannel = imageCV.channels();

	switch (nbChannel)
	{
		case 0:
			INTERNALERROR(" O channel detected");
			break;
		case 1:
			param.m_channel = GL_RED;
			break;
		case 2:
			param.m_channel = GL_RG;
			break;
		case 3:
			param.m_channel = GL_RGB;
			cv::cvtColor(imageCV, imageCV, cv::COLOR_RGB2BGR);
			break;
		case 4:
			param.m_channel = GL_RGBA;
			break;
		default:
			INTERNALERROR(" unhandle nub channel");
	}

	int type = imageCV.type()%8;
	
	//param.m_unpackAlignement = (imageCV.step & 3) ? 1 : 4;
	param.m_unpackAlignement = 1;


	void* data = nullptr;

	switch (type)
	{
		case CV_8U:
			param.m_type = GL_UNSIGNED_BYTE;
			data = imageCV.ptr<GLubyte>();
			break;
		case CV_8S:
			param.m_type = GL_BYTE;
			data = imageCV.ptr<GLbyte>();
			break;
		case CV_16F:
			param.m_type = GL_SHORT;
			data = imageCV.ptr<GLshort>();
			break;
		case CV_32F:
			param.m_type = GL_FLOAT;
			data = imageCV.ptr<GLfloat>();
			break;
		case CV_64F:
			param.m_type = GL_DOUBLE;
			data = imageCV.ptr<GLdouble>();
			break;
		default:
			INTERNALERROR(" unhandle type of precision");

	}

	
	Init(param, data);

	

	//glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return true;


}
