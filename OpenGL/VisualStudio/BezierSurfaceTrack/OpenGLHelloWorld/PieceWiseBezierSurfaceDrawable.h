#pragma once
#include "BezierSurfaceDrawable.h"

class PieceWiseBezierSurfaceDrawable
{
public:
	PieceWiseBezierSurfaceDrawable();
	~PieceWiseBezierSurfaceDrawable();

	void Init(PieceWiseBezierSurface<glm::vec3> pwbezSurf, int nbPtu, int nbPtv);

	void Draw(int method);

private:

	std::vector<std::vector<BezierSurfaceDrawable>> m_drawablesGrid;
	PieceWiseBezierSurface<glm::vec3> m_pwbezSurf;
};

