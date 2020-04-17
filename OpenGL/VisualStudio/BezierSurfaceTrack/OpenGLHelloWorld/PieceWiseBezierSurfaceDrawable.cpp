#include "stdafx.h"
#include "PieceWiseBezierSurfaceDrawable.h"


PieceWiseBezierSurfaceDrawable::PieceWiseBezierSurfaceDrawable()
{
}


PieceWiseBezierSurfaceDrawable::~PieceWiseBezierSurfaceDrawable()
{
}

void PieceWiseBezierSurfaceDrawable::Init(PieceWiseBezierSurface<glm::vec3> pwbezSurf, int nbPtu, int nbPtv)
{
	m_pwbezSurf = pwbezSurf;

	int n = m_pwbezSurf.getN();
	int m = m_pwbezSurf.getM();

	m_drawablesGrid.resize(n);

	for (auto& drawables : m_drawablesGrid)
	{
		drawables.resize(m);
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			m_drawablesGrid[i][j].Init(m_pwbezSurf.getBezSurface(i, j), nbPtu, nbPtv);
		}
	}
}

void PieceWiseBezierSurfaceDrawable::Draw(int method)
{
	for (auto& drawables : m_drawablesGrid)
	{
		
		for (auto& drawable : drawables)
		{
			drawable.draw(method);
		}

	}
}
