#pragma once
#include "glm.hpp"
#include "Bezier.h"
#include "ZGLDrawable.h"

class BezierCurveDrawable
{
public:
	BezierCurveDrawable() {};
	~BezierCurveDrawable();
	void Init(std::vector<glm::vec3> ctrlPts, int nbPt);

	void draw(int method);


private :
	BezierCurve<glm::vec3> m_bezierCurve;
	unsigned int m_nbPt;
	ZGLVAODrawable m_drawable;

};

