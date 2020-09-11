/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.h
 * Author: nicolas
 *
 * Created on September 20, 2019, 9:53 AM
 */

#ifndef CAMERA_H
#define CAMERA_H




#define GLM_FORCE_RADIANS

// GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


#include <GLFW/glfw3.h>

#include <iostream>

#define PI 3.141592653f

#define DEG2RAD (2*PI/360.0f)
#define RAD2DEG 1.0f/DEG2RAD



enum eCameraType
{
	TRACKBALLCAMERA,
	FREECAMERA,
	BEZIERCAMERA
};

struct confProjPerspective
{
    float m_degVerticalFOV = 45; // degree
    float m_ratio = 4.0f/ 3.0f; // width / height
    float m_nearPlane = 0.1f;
    float m_farPlane = 10000.0f;
    
};


struct confProjOrtho
{
    float m_left = -100.0f; // degree
    float m_right = 100.0f; // width / height
    float m_bottom = -100.0f;
    float m_top = 100.0f;
    float m_nearPlane = 1.0f;
    float m_farPlane = 1000.0f;
    
};
    






float getPitchRad(glm::vec3 const & dir);
float getHeadingRad(glm::vec3 const & dir);
float getPitchDeg(glm::vec3 const &  dir);
float getHeadingDeg(glm::vec3 const &  dir);

void setPitchAndHeadingRad(glm::vec3 & dir, float  H, float  P);
void setPitchAndHeadingDeg(glm::vec3 & dir, float const Hdeg, float const Pdeg);

   


class Camera {
public:
    
    enum eProjectionType
    {
        PERSPECTIVE,
        ORTHO
    };
    
    
    Camera(GLFWwindow * pWindow):m_pwindow(pWindow){};
    
    Camera(const Camera& orig){std::cout<<__FUNCTION__<<" NOT IMPLEMENTED"<<std::endl;}
    
    virtual ~Camera();
    
    void Init(glm::vec3 a_eye, glm::vec3 a_direction, glm::vec3 a_up, float speed){m_eye = a_eye;
        m_direction = a_direction;
        m_up = a_up;
		m_speed = speed;

    }
    
	virtual void Update(float elapsedTime);

	void ImguiDraw();
    
	

    virtual glm::mat4 getView() =0;
    


	glm::mat4 getProjectionAndView() { return getProjectionView()*getView(); }
    
    float getScaleEye(){return glm::length(m_eye);}
    
    
    void setScaleEye(float scale){m_eye = glm::normalize(m_eye)*scale;}
    
    
    void controlDeltaPitch();


    
    glm::vec3 getEyePos(){return m_eye;}
    void  setEyePos(glm::vec3 const eyepos){ m_eye = eyepos;}
    
    
    glm::vec3 getDirection() const { return m_direction;}
    void  setDirection(glm::vec3 const direction){ m_direction = direction;}
    
    glm::vec3 getRight() const { return m_right;}
    void  setRight(glm::vec3 const right){ m_right = right;}
    
    glm::vec3 getUp() const { return m_up;}
    void  setUp(glm::vec3 const up){ m_up = up;}
    
    
    //void setProjectionConfig(confProj a_proj){m_proj = a_proj;}
    
    glm::mat4 getProjectionView();
    
    bool m_angleCtrl = true;
    
    eProjectionType m_projectionType = PERSPECTIVE;
    confProjPerspective m_projPerspective;
    confProjOrtho m_projOrtho;
	float m_speed = 0.01f;
    
protected :
    
   
    glm::vec3  m_eye = glm::vec3(0,20,-20); // degree
    glm::vec3  m_direction = glm::vec3(0,-20,20); // width / height
    glm::vec3  m_right = glm::vec3(1.,0.,0.);
    glm::vec3  m_up = glm::vec3(0.,10.,0.);


public:
	glm::vec3 m_eyeInit = glm::vec3(0.416801,0.642099,0.519630);
	glm::vec3 m_dirInit = glm::vec3(0.450489,0.693998,0.561629);
    
    

	
    
    

    
    
    
    
    glm::vec2 m_mousePos;
    
    float m_deltaDegP = 0.f;
    float m_deltaDegH = 0.f;
	float m_deltaDegR = 0.f;
    
    
    
	GLFWwindow * m_pwindow = nullptr;
};

#endif /* CAMERA_H */

