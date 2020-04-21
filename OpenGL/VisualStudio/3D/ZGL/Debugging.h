#pragma once

#include <iostream>


#define INTERNALERROR(txt){ std::cout<<"INTERNAL ERROR FILE : "<<__FILE__<< " line "<<__LINE__<<std::endl;std::cout<<"INTERNAL ERROR "<<__FUNCTION__<<" : "<<#txt<<std::endl;InternalError(#txt);}


void InternalError(const char * txt);


