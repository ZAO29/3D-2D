#pragma once



#include <opencv2/highgui/highgui.hpp>

#include "ImageCapture.h"

class MyVideoCapture
{
public:
	MyVideoCapture();
	~MyVideoCapture();

	void Init(int width, int height, std::string namefile);
	void BindForWriting();

	void Snapshot();

	void End();

private:
		cv::VideoWriter m_writer;
		ImageCapture m_capture;
		int m_count = 0;
};

