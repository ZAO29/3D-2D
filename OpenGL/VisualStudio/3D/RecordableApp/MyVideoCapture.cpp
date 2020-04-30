#include "stdafx.h"
#include "MyVideoCapture.h"
#include <ZGL/Debugging.h>

#include <opencv2/imgproc/imgproc.hpp>

MyVideoCapture::MyVideoCapture()
{
}


MyVideoCapture::~MyVideoCapture()
{
}

void MyVideoCapture::Init(int width, int height, std::string namefile)
{
	m_capture.Init(width, height);
	double fps = 30;
	//int fcc = cv::VideoWriter::fourcc('X','2','6','4');
	int fcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
	//int fcc = cv::VideoWriter::fourcc('X', 'V', 'I', 'D');
	bool isColor = false;
	m_writer = cv::VideoWriter(namefile,fcc, fps, cv::Size(width, height),isColor);
	m_writer.open(namefile, fcc, fps, cv::Size(width, height),isColor);
}

void MyVideoCapture::BindForWriting()
{
	m_capture.BindForWriting();
}

void MyVideoCapture::BindForReading()
{
}

void MyVideoCapture::RenderQuad()
{
	m_capture.RenderQuad();
}

void MyVideoCapture::Snapshot()
{
	cv::Mat m = m_capture.Snapshot();

	
	//std::string realname = "D://OUTPUT//DEBUG//test_" + std::to_string(m_count) + "." + "png";
	
	cv::cvtColor(m, m, cv::COLOR_BGRA2GRAY, 1);
	m.convertTo(m, CV_8U);
	/*bool succeed = cv::imwrite(realname.c_str(), m);
	m_count++;
	if (!succeed)
	{
		INTERNALERROR("impossible to write image");
	}*/

	m_writer.write(m);
	m_count++;

}

void MyVideoCapture::End()
{
	m_writer.release();
}
