#include "stdafx.h"
#include "RosaceShaderApp.h"

#include <ZGL/imgui/imgui.h>
#include "opencv2/imgproc/imgproc.hpp"

#define SHADER_TIME "u_time"
#define SHADER_SCALE "u_scale"
#define SHADER_NB_PT "u_nbPt"
#define SHADER_DIR "u_dir"
#define SHADER_RESOLUTION "u_resolution"
#define SHADER_POWMULT "u_powmult"

RosaceShaderApp::RosaceShaderApp()
{
}


RosaceShaderApp::~RosaceShaderApp()
{
}

bool RosaceShaderApp::Init()
{
	m_bfullScreen = true;
	m_width = 1920;
	m_height = 1080;
	m_name = "RosaceShader";
	//m_bfullScreen = false;
	//m_width = 1280;
	//m_height = 720;


	RecordableApp::Init();


	MapUniform uniformMap;

	uniformMap[SHADER_TIME]= UniformVar(eZGLtypeUniform::ZGL_FVEC1);
	uniformMap[SHADER_SCALE] = UniformVar(eZGLtypeUniform::ZGL_FVEC2);
	uniformMap[SHADER_NB_PT] = UniformVar(eZGLtypeUniform::ZGL_IVEC1);

	m_RosaceShader.Init("Rosace",uniformMap);
	m_compoShader.Init("CompositionShader");

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


	//CPU BLurring
	TexParam texparameter;
	texparameter.m_width = m_width;
	texparameter.m_height = m_height;
	texparameter.m_channel = GL_RED;
	texparameter.m_type = GL_UNSIGNED_BYTE;
	
	for (int i = 0; i < 3; i++)
	{
		m_texRGB[i].Init(texparameter);
	}

	



	//GPU Blurring
	MapUniform blurUniform;
	blurUniform[SHADER_DIR] = UniformVar(eZGLtypeUniform::ZGL_FVEC2);
	blurUniform[SHADER_RESOLUTION] = UniformVar(eZGLtypeUniform::ZGL_FVEC2);
	blurUniform[SHADER_POWMULT] = UniformVar(eZGLtypeUniform::ZGL_FVEC2);
	m_blurShader.Init("Blurr", blurUniform);
	m_blurShader.Enable();
	glm::vec2 res(m_height, m_width);
	m_blurShader.updateUniform(SHADER_RESOLUTION, &res);
	m_blurShader.updateUniform(SHADER_POWMULT, &m_powMult);
	for (int i = 0; i < 2; i++)
	{
		m_pingpongFBO[i].Init(m_width, m_height);
	}
	for (int i = 0; i < 3; i++)
	{
		m_RGBFBO[i].Init(m_width, m_height);
	}


	return true;
}



int makeImpair(int a)
{
	return a + (a % 2) + 1;
}


void RosaceShaderApp::OpenGLRender()
{
	if (m_blurTexnik == eBLURRING::CPU)
		m_capture.BindForWriting();
	else
		m_pingpongFBO[0].BindForWriting();

	glClearColor(0.,0.,0.,1.);
	m_RosaceShader.Enable();
	float t = m_speed*(m_cumulTime-m_reinitTime)+1+m_timeOffset;
	m_RosaceShader.updateUniform(SHADER_TIME, &t);
	m_quad.Render(GL_TRIANGLES);
	
	if (m_blurTexnik == eBLURRING::CPU)
	{
		CPUBlurr();
	}
	else
	{
		GPUBlurr();
	}


	setTargetRender();
	m_compoShader.Enable();
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

	ImGui::SliderInt("offset", &m_timeOffset, 0, 200);

	bool CPUblur = (m_blurTexnik == eBLURRING::CPU);
	ImGui::Checkbox("equalize", &m_equalize);
	ImGui::SliderFloat("speed", &m_speed, 0.1, 5);
	if (ImGui::Checkbox("CPU blur",&CPUblur))
	{
		if (CPUblur)
		{
			m_blurTexnik = eBLURRING::CPU;
		}
		else
		{
			m_blurTexnik = eBLURRING::GPU;
		}
	}
	if (!CPUblur)
	{
		ImGui::SliderInt3("GPU Blur nb iter", &m_nbBlurFilter[0], 0, 20);
		bool bpow=ImGui::SliderFloat("GPU power", &m_powMult[0], 0., 2.);
		bool bmult=ImGui::SliderFloat("GPU mult", &m_powMult[1], 1., 1.2);

		if (bpow || bmult)
		{
			m_blurShader.Enable();
			m_blurShader.updateUniform(SHADER_POWMULT, &m_powMult[0]);
		}
	}
	else
	{
		
		ImGui::SliderInt2("kernel mult", &m_kernelMult[0], 1, 10);
		
	}

	
	


	ImGui::End();
}

void RosaceShaderApp::CPUBlurr()
{
	cv::Mat im = m_capture.Snapshot();
	cv::cvtColor(im, im, cv::COLOR_BGRA2GRAY, 1);
	im.convertTo(im, CV_8U);
	cv::Mat out1, out2, out3;
	//im = im / 255.;
	int kernel_impair = makeImpair(m_kernelSize);
	int kernel2 = makeImpair(m_kernelSize*m_kernelMult.x);
	int kernel3 = makeImpair(m_kernelSize*m_kernelMult.y);
	cv::GaussianBlur(im, out1, cv::Size(kernel_impair, kernel_impair), 0, 0);
	cv::GaussianBlur(im, out2, cv::Size(kernel2, kernel2), 0, 0);
	cv::GaussianBlur(im, out3, cv::Size(kernel3, kernel3), 0, 0);

	if (m_equalize)
	{
		cv::equalizeHist(out1, out1);
		cv::equalizeHist(out2, out2);
		cv::equalizeHist(out3, out3);

	}
	m_texRGB[0].update(out1.ptr<float>());
	m_texRGB[1].update(out2.ptr<float>());
	m_texRGB[2].update(out3.ptr<float>());

	for (int idRGB = 0; idRGB < 3; idRGB++)
	{
		m_texRGB[idRGB].Bind(GL_TEXTURE0+idRGB);
	}
}

void RosaceShaderApp::GPUBlurr()
{
	bool id = 0;
	bool idnext;
	glm::vec2 dir(0, 1);
	glm::vec2 zeroDir(0);
	m_blurShader.Enable();

	for (int idRGB = 0; idRGB < 3; idRGB++)
	{
		for (int k = 0; k < m_nbBlurFilter[idRGB]; k++)
		{
			for (int i = 0; i < 2; i++)
			{
				idnext = !id;
				m_pingpongFBO[id].BindForReading(GL_TEXTURE0);
				m_pingpongFBO[idnext].BindForWriting();
				m_blurShader.updateUniform(SHADER_DIR, &dir);
				dir = glm::vec2(dir.y, -dir.x);
				m_quad.Render(GL_TRIANGLES);
				id = idnext;
			}
		}
		m_pingpongFBO[id].BindForReading(GL_TEXTURE0);
		m_RGBFBO[idRGB].BindForWriting();
		m_blurShader.updateUniform(SHADER_DIR, &zeroDir);
		m_quad.Render(GL_TRIANGLES);

		if (m_equalize)
		{
			cv::Mat im = m_RGBFBO[idRGB].Snapshot();
			cv::cvtColor(im, im, cv::COLOR_BGRA2GRAY, 1);
			im.convertTo(im, CV_8U);
			cv::equalizeHist(im, im);

			m_texRGB[idRGB].update(im.ptr<float>());
			
		}
	}
	
	for (int i = 0; i < 3; i++)
	{
		if (m_equalize)
		{
			m_texRGB[i].Bind(GL_TEXTURE0 + i);
		}
		else
		{
			m_RGBFBO[i].BindForReading(GL_TEXTURE0+i);
		}
		

	}

}
