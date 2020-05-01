#include "stdafx.h"
#include "RosaceShaderApp.h"

#include <ZGL/imgui/imgui.h>
#include "opencv2/imgproc/imgproc.hpp"

#define SHADER_TIME "u_time"
#define SHADER_SCALE "u_scale"
#define SHADER_NB_PT "u_nbPt"

RosaceShaderApp::RosaceShaderApp()
{
}


RosaceShaderApp::~RosaceShaderApp()
{
}

bool RosaceShaderApp::Init()
{
	//m_bfullScreen = true;
	//m_width = 1920;
	//m_height = 1080;
	m_name = "RosaceShader";
	m_bfullScreen = false;
	m_width = 1280;
	m_height = 720;


	RecordableApp::Init();


	MapUniform uniformMap;

	uniformMap[SHADER_TIME]= UniformVar(eZGLtypeUniform::ZGL_FVEC1);
	uniformMap[SHADER_SCALE] = UniformVar(eZGLtypeUniform::ZGL_FVEC2);
	uniformMap[SHADER_NB_PT] = UniformVar(eZGLtypeUniform::ZGL_IVEC1);

	m_RosaceShader.Init("Rosace", false, uniformMap);
	m_compoShader.Init("CompositionShader", false, MapUniform());

	glm::vec2 scale = glm::vec2(float(m_width) / float(m_height), 1.);
	if (m_width > m_height)
	{
		scale = glm::vec2(1.,float(m_height) / float(m_width));
	}

	m_RosaceShader.Enable();
	m_RosaceShader.updateUniform(SHADER_SCALE, &scale);
    m_RosaceShader.updateUniform(SHADER_NB_PT, &m_nbPt);

	{
		std::vector<glm::vec2> quad_vertex_buffer_data = {
			glm::vec2(-1.0f , -1.0f),
			glm::vec2(1.0f , -1.0f),
			glm::vec2(-1.0f ,  1.0f),
			glm::vec2(-1.0f ,  1.0f),
			glm::vec2(1.0f , -1.0f),
			glm::vec2(1.0f ,  1.0f)
		};
		
		
		
		
		// pos
		ZGLStride stride1;
		stride1.m_offset = sizeof(glm::vec2),
			stride1.m_type = GL_FLOAT;
		stride1.m_size = 2;
		
		
		ZGLVAODrawableParam paramDrawable;

		paramDrawable.m_stride = sizeof(glm::vec2);
		paramDrawable.m_nbVertex = quad_vertex_buffer_data.size();
		paramDrawable.m_pVertices = (void *)&quad_vertex_buffer_data[0];
		paramDrawable.m_strides = { stride1};

		m_quad.Init(paramDrawable);
	}

	m_capture.Init(m_width, m_height);

	TexParam texparameter;
	texparameter.m_width = m_width;
	texparameter.m_height = m_height;
	texparameter.m_channel = GL_RED;
	texparameter.m_type = GL_UNSIGNED_BYTE;
	m_texR.Init(texparameter);
	m_texG.Init(texparameter);
	m_texB.Init(texparameter);

	return true;
}



int makeImpair(int a)
{
	return a + (a % 2) + 1;
}


void RosaceShaderApp::OpenGLRender()
{
	setTargetRender();
	m_capture.BindForWriting();
	glClearColor(0.,0.,0.,1.);
	m_RosaceShader.Enable();
	float t = m_speed*(m_cumulTime-m_reinitTime)+1;
	m_RosaceShader.updateUniform(SHADER_TIME, &t);
	m_quad.Render(GL_TRIANGLES);
	
	
	cv::Mat im = m_capture.Snapshot();
	cv::cvtColor(im, im, cv::COLOR_BGRA2GRAY, 1);
	im.convertTo(im, CV_8U);
	cv::Mat out1, out2, out3;
	//im = im / 255.;
	int kernel_impair = makeImpair(m_kernelSize);
	int kernel2 = makeImpair(m_kernelSize*m_kernelMult.x);
	int kernel3 = makeImpair(m_kernelSize*m_kernelMult.y);
	cv::GaussianBlur(im, out1, cv::Size(kernel_impair, kernel_impair), 0, 0);
	cv::GaussianBlur(im, out2, cv::Size(kernel2,kernel2), 0, 0);
	cv::GaussianBlur(im, out3, cv::Size(kernel3,kernel3), 0, 0);

	if (m_equalize)
	{
		cv::equalizeHist(out1, out1);
		cv::equalizeHist(out2, out2);
		cv::equalizeHist(out3, out3);

		//out1 = out1 / 255;
		//out2 = out2 / 255;
		//out3 = out3 / 255;
	}
	m_texR.update(out1.ptr<float>());
	m_texG.update(out2.ptr<float>());
	m_texB.update(out2.ptr<float>());

	setTargetRender();
	m_compoShader.Enable();
	m_texR.Bind(GL_TEXTURE0);
	m_texG.Bind(GL_TEXTURE1);
	m_texB.Bind(GL_TEXTURE2);
	m_quad.Render(GL_TRIANGLES);

	
	

}



void RosaceShaderApp::ImguiDraw()
{
	RecordableApp::ImguiDraw();
	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);



	if (ImGui::SliderInt("nb pt ", &m_nbPt, 100, 1000))
	{
		m_RosaceShader.Enable();
		m_RosaceShader.updateUniform(SHADER_NB_PT, &m_nbPt);
	}
	if (ImGui::SliderInt("kernel Size", &m_kernelSize, 0, 200));
	
	bool breinit = false;
	if (ImGui::Button("reinit"))
	{
		m_reinitTime = m_cumulTime;
	}

	ImGui::SliderFloat("speed", &m_speed, 0.1, 5);
	ImGui::SliderInt2("kernel mult",&m_kernelMult[0], 1, 10);
	


	ImGui::End();
}
