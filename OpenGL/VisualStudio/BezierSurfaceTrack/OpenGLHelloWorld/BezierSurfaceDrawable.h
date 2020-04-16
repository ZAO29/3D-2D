#pragma once
#include "glm.hpp"
#include "Bezier.h"
#include "ZGLDrawable.h"

class BezierSurfaceDrawable
{
public:
	BezierSurfaceDrawable();
	~BezierSurfaceDrawable();

	void Init(std::vector<std::vector<glm::vec3>> ctrlPts, int nbPtu, int nbPtv);

	void draw(int method);

private:
	ZGLVAODrawable m_drawable;
	BezierSurface<glm::vec3> m_bezierSurface;
};

