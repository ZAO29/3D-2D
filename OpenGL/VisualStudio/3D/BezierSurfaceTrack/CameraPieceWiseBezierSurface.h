#pragma once
#include <ZGL/Camera.h>
#include "Bezier.h"

class CameraPieceWiseBezierSurface : public Camera
{
public:
	CameraPieceWiseBezierSurface(GLFWwindow* pwin, PieceWiseBezierSurface<glm::vec3> surf);
	~CameraPieceWiseBezierSurface();

	virtual void Update(float elapsedTime) override;

	virtual glm::mat4 getView() override;


private:

	glm::vec2 m_parameter = glm::vec2(0.5, 0.);

	PieceWiseBezierSurface<glm::vec3> m_surface;
	PieceWiseBezierSurface<glm::vec3> m_surfaceDerU;
	PieceWiseBezierSurface<glm::vec3> m_surfaceDerV;

};

