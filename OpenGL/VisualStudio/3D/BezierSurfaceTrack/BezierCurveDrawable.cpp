#include "stdafx.h"
#include "BezierCurveDrawable.h"

#include "GL/glew.h"





BezierCurveDrawable::~BezierCurveDrawable()
{
}

void BezierCurveDrawable::Init(std::vector<glm::vec3> ctrlPts, int nbPt)
{
	m_bezierCurve.setCtrlPt(ctrlPts);

	std::vector<glm::vec3> samples = m_bezierCurve.Sample<float>(nbPt);


	glm::vec3 color(0., 1., 0.);
	float alpha = 1.0;

	int i = 0;
	std::vector<VertexData> vertices;
	vertices.resize(samples.size());

	for (auto & vert : vertices)
	{
		vert = VertexData(samples[i], color, alpha);
		i++;
	}


	// pos
	ZGLStride stride1;
	stride1.m_offset = sizeof(glm::vec3),
	stride1.m_type = GL_FLOAT;
	stride1.m_size = 3;

	
	// color
	ZGLStride stride2;
	stride2.m_offset = sizeof(glm::vec3),
	stride2.m_type = GL_FLOAT;
	stride2.m_size = 3;

	//alpha
	ZGLStride stride3;
	stride3.m_offset = sizeof(float),
		stride3.m_type = GL_FLOAT;
	stride3.m_size = 1;



	ZGLVAODrawableParam paramDrawable;

	paramDrawable.m_stride = sizeof(VertexData);
	paramDrawable.m_nbVertex = vertices.size();
	paramDrawable.m_pVertices = (void *)&vertices[0];
	paramDrawable.m_strides = { stride1, stride2, stride3 };

	m_drawable.Init(paramDrawable);
}

void BezierCurveDrawable::draw(int method)
{
	m_drawable.Render(method);
}


