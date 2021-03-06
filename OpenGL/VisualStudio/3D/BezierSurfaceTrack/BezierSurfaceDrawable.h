#pragma once
#include "glm.hpp"
#include "Bezier.h"
#include <ZGL/ZGLDrawable.h>

class BezierSurfaceDrawable
{
public:
	BezierSurfaceDrawable();
	~BezierSurfaceDrawable();

	BezierSurfaceDrawable(const BezierSurfaceDrawable & a) {
		INTERNALERROR("not implemented");
	} 

	void Init(std::vector<std::vector<glm::vec3>> ctrlPts, int nbPtu, int nbPtv, glm::ivec2 offsetUV=glm::ivec2(0.));

	void Init(BezierSurface<glm::vec3> bezierSurface, int nbPtu, int nbPtv, glm::ivec2 offsetUV = glm::ivec2(0.));

	void draw(int method);

private:
	ZGLVAODrawable m_drawable;
	BezierSurface<glm::vec3> m_bezierSurface;
};

