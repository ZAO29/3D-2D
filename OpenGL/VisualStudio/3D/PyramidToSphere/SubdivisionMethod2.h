#pragma once
#include <ZGL/ZGLDrawable.h>
#include <ZGL/Shaders.h>
#include <ZGL/Camera.h>

struct DataVertex
{
	glm::vec3 m_pos;
	int m_colorId;
};


class SubdivisionMethod2
{
public:
	SubdivisionMethod2();
	~SubdivisionMethod2();

	void Init();
	void Render(Camera* pCam);
	void ImguiDraw();

private:
	float m_levelMax = 9;
	std::vector<ZGLVAODrawable> m_levelPyramids;
	Shader m_shader;
	float m_subdiv = 0.0f;

};

