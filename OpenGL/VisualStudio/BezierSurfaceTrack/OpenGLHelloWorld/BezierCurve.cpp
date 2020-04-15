#include "stdafx.h"
#include "BezierCurve.h"


template<>
double MyBeta<double>(double i, double j)
{
	return std::beta(i, j);
}


template<>
float MyBeta<float>(float i, float j)
{
	return std::betaf(i, j);
}
