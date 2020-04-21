#include "stdafx.h"
#include "PWBezierCurveParser.h"
#include <iostream>
#include "tinyXML/tinyxml2.h"
#include "float.h"
#include <string>


using namespace tinyxml2;

PWBezierCurveParser::PWBezierCurveParser()
{
}


PWBezierCurveParser::~PWBezierCurveParser()
{
}

PieceWiseBezierCurve<glm::vec2> PWBezierCurveParser::Parse(std::string filename, std::string path)
{
	std::vector<glm::vec2> ctrlPt = ParseCtrlPt(filename, path);

	int nbPiece = (ctrlPt.size() - 1) / 3;

	std::vector<BezierCurve<glm::vec2>> listBezCurve(nbPiece);
	int i = 0;
	auto it = ctrlPt.begin();
	for (auto& bezcurve : listBezCurve)
	{
		bezcurve.setCtrlPt(std::vector<glm::vec2>(it, it + 4));
		it += 3;
	}

	PieceWiseBezierCurve<glm::vec2> PWBezCurve;
	PWBezCurve.Init(listBezCurve);

	auto bb = PWBezCurve.getBoundingBox();

	auto center = bb.getCenter<float>();

	PWBezCurve.Translate(-center);
	return PWBezCurve;

}

std::vector<glm::vec2> PWBezierCurveParser::ParseCtrlPt(std::string filename, std::string path)
{
	

	XMLDocument doc;
	XMLError status = doc.LoadFile(filename.c_str());
	if (status != XML_SUCCESS)
		INTERNALERROR("FAIL");


	tinyxml2::XMLElement* root = doc.FirstChildElement("svg");
	if (root == nullptr)
	{
		INTERNALERROR("fail");
	}
	tinyxml2::XMLElement* gv = root->FirstChildElement("g");
	if (gv == nullptr)
	{
		INTERNALERROR("fail");
	}
	tinyxml2::XMLElement* pathList = gv->FirstChildElement("path");

	std::string pathId(path);
	bool  find = false;
	while (pathList != nullptr && !find)
	{
		std::string currentPathId(pathList->Attribute("id"));
		std::cout << "attribute id : " << currentPathId << std::endl;
		if (pathId == currentPathId)
		{
			find = true;
			break;
		}
		pathList = pathList->NextSiblingElement("path");
	}

	if (!find)
		INTERNALERROR("not found");

	std::string bezierCurveDescription(pathList->Attribute("d"));

	std::cout << bezierCurveDescription << std::endl;

	std::vector<std::string> delimiters = { "C","c" };
	std::vector<bool> findDelimiters(delimiters.size());
	int i = 0, idFound = -1;
	bool found, alreadyFound = false;
	for (auto& delimiter : delimiters)
	{
		found = (bezierCurveDescription.find(delimiter) != std::string::npos);

		if (found)
		{
			idFound = i;
			if (alreadyFound)
			{
				INTERNALERROR("multiple delimitor found ");
			}

			findDelimiters[i] = true;
			alreadyFound = true;
		}
		i++;
	}

	if (!alreadyFound)
	{
		INTERNALERROR(" C/C delimitor not found");
	}

	bool isAbsolute = (idFound == 0);

	std::string tokenLeft = bezierCurveDescription.substr(0, bezierCurveDescription.find(delimiters[idFound]));
	std::string tokenRight = bezierCurveDescription.substr(bezierCurveDescription.find(delimiters[idFound]) + 1, bezierCurveDescription.size());
	std::cout << tokenLeft << std::endl;
	std::cout << tokenRight << std::endl;

	auto f = tokenLeft.find("M");
	if (f == std::string::npos)
	{
		f = tokenLeft.find("m");
		if (f == std::string::npos)
		{
			INTERNALERROR("impossible to find M or m");
		}
	}
	tokenLeft = tokenLeft.substr(f + 1, tokenLeft.size());

	std::vector<glm::vec2> listCtrlPt;

	listCtrlPt.push_back(getCtrlPt(tokenLeft));

	while (tokenRight.size() != 0)
	{

		glm::vec2 v = getCtrlPt(tokenRight);
		if (!isAbsolute)
			v += listCtrlPt[listCtrlPt.size()-1];

		listCtrlPt.push_back(v);
	}
	for (auto& v : listCtrlPt)
	{
		std::cout << v.x << " | " << v.y << std::endl;
	}
	std::cout << "yolo" << std::endl;
return listCtrlPt;
	
}

glm::vec2 PWBezierCurveParser::getCtrlPt(std::string & description)
{
	glm::vec2 v;

	int id = description.find(',');
	std::string currentLeft = description.substr(0, id);
	description = description.substr(id + 1, description.size());
	v.x = std::stof(currentLeft);
	size_t sz;
	v.y = std::stof(description, &sz);
	description = description.substr(sz);

	return v;


}
