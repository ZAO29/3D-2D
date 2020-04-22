#include "BezierCurveDrawable.h"

template<typename Vec>
BezierCurveDrawable<Vec>::~BezierCurveDrawable()
{
}


template<typename Vec>
void BezierCurveDrawable<Vec>::draw(int method)
{
	m_drawable.Render(method);
}



template<typename Vec>
template<typename Precision>
void BezierCurveDrawable<Vec>::Init(std::vector<Vec> ctrlPts, int nbPt)
{
	m_bezierCurve.setCtrlPt(ctrlPts);

	std::vector<Vec> vertices = m_bezierCurve.Sample<Precision>(nbPt);


	// pos
	ZGLStride stride1;
	stride1.m_offset = sizeof(Vec),
		stride1.m_type = GL_FLOAT;
	stride1.m_size = vertices[0].length();



	ZGLVAODrawableParam paramDrawable;

	paramDrawable.m_stride = sizeof(Vec);
	paramDrawable.m_nbVertex = vertices.size();
	paramDrawable.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_strides = { stride1 };

	m_drawable.Init(paramDrawable);
}


template<typename Vec>
template<typename Precision>
void PieceWiseBezierCurveDrawable<Vec>::Init(std::vector<BezierCurve<Vec>> list, int nbPt)
{
	m_bezCurveDrawables.resize(list.size());

	int i = 0;
	for (auto & bezCurveDrawable : m_bezCurveDrawables)
	{
		bezCurveDrawable.Init<Precision>(list[i].getCtrlPt(), nbPt);
		i++;
	}
}

template<typename Vec>
template<typename Precision>
inline BoundingBox<Vec> PieceWiseBezierCurveDrawable<Vec>::getBoundingBox()
{
	if (m_bezCurveDrawables.size() == 0)
		INTERNALERROR("bounding box of empty pwbezcurve cannot be computed");

	BoundingBox<Vec> bb = m_bezCurveDrawables[0].getBoundingBox();

	for (auto& bezcurve : m_bezCurveDrawables)
	{
		bb.Union(bezcurve.getBoundingBox());
	}

	return bb;
}


template<typename Vec>
void PieceWiseBezierCurveDrawable<Vec>::draw(int method)
{
	for (auto & d : m_bezCurveDrawables)
	{
		d.draw(method);
	}
}




