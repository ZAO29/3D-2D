#include "stdafx.h"
#include "Bezier.h"


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

template<>
float MyModulo<float>(float a, float b)
{
	return std::fmod(a, b);
}



PieceWiseBezierSurface<glm::vec3> InitPWBezSurf(PieceWiseBezierCurve<glm::vec2> track, std::vector<PieceWiseBezierCurve<glm::vec2>> sections)
{
	int n = track.size();
	int m = sections[0].size();
	if ((sections.size()-1) != n)
		INTERNALERROR("invalid data");

	glm::vec3 z(0., 0., 1.);

	for (auto &section : sections)
	{
		if (section.size() != m)
		{
			INTERNALERROR("invalid data");
		}

		for (int i = 0; i < m; i++)
		{
			BezierCurve<glm::vec2> sectionC = section.getPiece(i);
			if (sectionC.size() != 4)
				INTERNALERROR("invalid data size different of 4");
		}
	}

	PieceWiseBezierSurface<glm::vec3> pwBezSurf(n, m);
	BezierSurface<glm::vec3> bezSurf(4, 4);
	for (int i = 0; i < n; i++)
	{
		BezierCurve<glm::vec2>  portionTrack2D = track.getPiece(i);
		BezierCurve<glm::vec2> derivate = portionTrack2D.Derivate<float>();
		glm::vec2 derivate0 = derivate.Eval<float>(0) / 3.f;
		glm::vec2 derivate1 = derivate.Eval<float>(1) / 3.f;

		glm::vec2 p0 = portionTrack2D.Eval<float>(0);
		glm::vec2 p1 = portionTrack2D.Eval<float>(1);

		glm::vec3 normal0(derivate0.y, -derivate0.x, 0.);
		glm::vec3 normal1(derivate1.y, -derivate1.x, 0.);
		normal0 = glm::normalize(normal0);
		normal1 = glm::normalize(normal1);

		glm::vec3 derivate03d(derivate0, 0.);
		glm::vec3 derivate13d(derivate1, 0.);

		glm::vec3 p03d(p0, 0.);
		glm::vec3 p13d(p1, 0.);

		PieceWiseBezierCurve<glm::vec2> section0 = sections[i];
		PieceWiseBezierCurve<glm::vec2> section1 = sections[i+1];


		for (int j = 0; j < m; j++)
		{
			BezierCurve<glm::vec2> bcurve0 = section0.getPiece(j);
			BezierCurve<glm::vec2> bcurve1 = section1.getPiece(j);
			

			std::vector<std::vector<glm::vec3> > ctrlPt(4);
			for (auto& pt : ctrlPt)
			{
				pt.resize(4);
			}

			for (int ii = 0; ii < 4; ii++)
			{
				glm::vec2 pt = bcurve0.getCtrlPt(ii);
				ctrlPt[0][ii]= pt.x*z + pt.y*normal0 + p03d;
				ctrlPt[1][ii] = ctrlPt[0][ii] + derivate03d;
				pt = bcurve1.getCtrlPt(ii);
				ctrlPt[3][ii] = pt.x*z + pt.y*normal1 + p13d;
				ctrlPt[2][ii] = ctrlPt[3][ii] - derivate13d;
			}

			bezSurf.setCtrlPt(ctrlPt);



			pwBezSurf.setBezSurface(i, j, bezSurf);

		}
	}

	return pwBezSurf;
}
