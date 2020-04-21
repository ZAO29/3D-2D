#include "stdafx.h"
#include "Debugging.h"
#include <thread>


void InternalError(const char * txt)
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(200));
	system("pause");
	//throw std::runtime_error(txt);
}