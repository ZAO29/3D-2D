#include "stdafx.h"
#include "BezierSurfaceDrawable.h"
#include "Constant.h"

#include "GL/glew.h"

BezierSurfaceDrawable::BezierSurfaceDrawable()
{
}


BezierSurfaceDrawable::~BezierSurfaceDrawable()
{
}

void BezierSurfaceDrawable::Init(std::vector<std::vector<glm::vec3>> ctrlPts, int nbPtu, int nbPtv)
{
	m_bezierSurface.setCtrlPt(ctrlPts);

	std::vector<std::vector<glm::vec3>> samples;
	samples = m_bezierSurface.Sample<float>(nbPtu, nbPtv);

	int nc = samples.size();
	int nl = samples[0].size();

	std::vector<glm::vec3> triangleStripSamples;

	bool left2Right = true;
	for (int i = 0; i < (nl - 1); i++)
	{

		
		if (left2Right)
		{
			for (int j = 0; j < nc; j++)
			{
				triangleStripSamples.push_back(samples[j][i]);
				triangleStripSamples.push_back(samples[j][i+1]);
			}
		}
		else
		{
			for (int j = nc-1; j >= 0; j--)
			{
				triangleStripSamples.push_back(samples[j][i]);
				triangleStripSamples.push_back(samples[j][i+1]);
			}
		}
		left2Right = !left2Right;
	}



	int i = 0;
	std::vector<VertexData> vertices;
	vertices.resize(triangleStripSamples.size());


	glm::vec3 colors[3];
	colors[0] = glm::vec3(1., 0., 0.);
	colors[1] = glm::vec3(0., 1., 0.);
	colors[2] = glm::vec3(0., 0., 1.);

	for (auto & vert : vertices)
	{
		vert = VertexData(triangleStripSamples[i], colors[i%3], 1.0);
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

void BezierSurfaceDrawable::draw(int method)
{
	m_drawable.Render(method);
}
