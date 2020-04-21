#pragma once
#include "Bezier.h"



class PWBezierCurveParser
{
public:
	PWBezierCurveParser();
	~PWBezierCurveParser();
 
	static PieceWiseBezierCurve<glm::vec2> Parse(std::string filename, std::string path);
	static std::vector<glm::vec2> ParseCtrlPt(std::string filename, std::string path);
	static glm::vec2 getCtrlPt(std::string & description);
};

