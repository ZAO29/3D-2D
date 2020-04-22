#pragma once
#include "Bezier.h"
#include "ZGL/ZGLDrawable.h"

template<typename Vec>
class BezierCurveDrawable
{
public:
	BezierCurveDrawable() {};
	~BezierCurveDrawable();
	template <typename Precision>
	void Init(std::vector<Vec> ctrlPts, int nbPt);


	BoundingBox<Vec> getBoundingBox() { return m_bezierCurve.getBoundingBox(); }

	void draw(int method);


private :
	BezierCurve<Vec> m_bezierCurve;
	ZGLVAODrawable m_drawable;

};




template<typename Vec>
class PieceWiseBezierCurveDrawable
{
public:
	PieceWiseBezierCurveDrawable() {};
	~PieceWiseBezierCurveDrawable() {};
	template <typename Precision>
	void Init(std::vector<BezierCurve<Vec>> list, int nbPt);

	void draw(int method);

	template<typename Precision>
	BoundingBox<Vec> getBoundingBox();


private:
	std::vector<BezierCurveDrawable<Vec>> m_bezCurveDrawables;

};

#include "BezierCurveDrawable.hpp"
