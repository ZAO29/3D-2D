#pragma once

#include "Constant.h"
#include "glm.hpp"
#include <ZGL/BoundingBox.h>
#include "ZGL/Debugging.h"


#include <vector>
template <class Vec>
class BezierCurve
{
public:
	BezierCurve(){}
	~BezierCurve();

	BezierCurve(int n) :m_ctrlPts(n) {}

	void setCtrlPt(std::vector<Vec> ctrlPts) { m_ctrlPts = ctrlPts; }
	std::vector<Vec> getCtrlPt() { return m_ctrlPts; }

	Vec getCtrlPt(int i) { return m_ctrlPts[i]; }
	void setCtrlPt(int i, Vec v) { m_ctrlPts[i] = v; }

	unsigned int size() { return m_ctrlPts.size(); }

	template<typename Precision>
	Vec Eval(Precision t);

	template<typename Precision>
	std::vector<Vec> Sample(int nbPt);



	BoundingBox<Vec> getBoundingBox() const;

	void Translate(Vec const & t)
	{
		for (auto& v : m_ctrlPts)
			{
				v += t;
			}
	}

	void Scale(Vec const & s)
	{
		for (auto& v : m_ctrlPts)
		{
			v /= s;
		}
	}


	template<typename Mat>
	void Mult(Mat const &m) {
		for (auto& v : m_ctrlPts)
		{
			v = m * v;
		}
	}



	template<typename Precision>
	BezierCurve<Vec> Derivate();

private:
	std::vector<Vec> m_ctrlPts;


};

template <typename Precision>
Precision MyBeta(Precision i, Precision j); // { throw std::runtime_error("beta precision not implemented"); return Precision(); };

template <class Precision>
Precision BinomialCoeff(int i, int m);

template <class Precision>
Precision MyModulo(Precision a, Precision b);


template <class Precision>
Precision evalBernStein(Precision t, int i, int m);

template <class Vec>
class BezierSurface
{
public:
	BezierSurface(){}
	~BezierSurface() {}

	BezierSurface(int n, int m);


	void setCtrlPt(std::vector<std::vector<Vec> > ctrlPts);

	void setCtrlPt(std::vector<BezierCurve<Vec>> v);

	Vec getCtrlPt(int i, int j) { return m_ctrlPts[i].getCtrlPt(j);}

	void setCtrlPt(int i, int j, Vec v) { m_ctrlPts[i].setCtrlPt(j,v); }

	template<typename Precision>
	Vec Eval(Precision u, Precision v);

	template<typename Precision>
	BezierSurface<Vec> derivateU();

	template<typename Precision>
	BezierSurface<Vec> derivateV();


	BezierSurface<Vec> transpose();

	template<typename Precision>
	BezierCurve<Vec> Eval(Precision u);

	template<typename Precision>
	std::vector< std::vector<Vec> > Sample(int nbPtu, int nbPtv);

private:
	std::vector<BezierCurve<Vec> > m_ctrlPts;
};

template < class Vec>
class PieceWiseBezierSurface
{
public:
	PieceWiseBezierSurface() {};
	PieceWiseBezierSurface(int n, int m);

	unsigned int getN() { return m_grid.size(); }
	unsigned int getM() { return m_grid[0].size(); }

	template<typename Precision>
	Vec Eval(Precision u, Precision v);

	void setBezSurface(int i, int j, BezierSurface<Vec> bezSurf) { m_grid[i][j] = bezSurf; }

	BezierSurface<Vec> getBezSurface(int i, int j) { return m_grid[i][j]; }

	template<typename Precision>
	PieceWiseBezierSurface<Vec> derivateU();

	template<typename Precision>
	PieceWiseBezierSurface<Vec> derivateV();

	std::vector<std::vector<Vec> > Sample(int i, int j, int nbPtu, int nbPtv);

private:
	std::vector<std::vector<BezierSurface<Vec>>> m_grid;
};



template < class Vec>
class PieceWiseBezierCurve
{
public:
	PieceWiseBezierCurve() {};

	void Init(std::vector<BezierCurve<Vec>> list) { m_list = list; }

	unsigned int size() { return m_list.size(); }

	std::vector<BezierCurve<Vec>> getPieces() {
		return m_list;
	}

	BezierCurve<Vec> getPiece(int i) { return m_list[i]; }

	unsigned int getN() { return m_list.size(); }

	BoundingBox<Vec> getBoundingBox() const;
	
	void Translate(Vec const &v) { for (auto& bezcurve : m_list) { bezcurve.Translate(v); } }

	template<typename Mat>
	void Mult(Mat const &m) { for (auto& bezcurve : m_list) { bezcurve.Mult<Mat>(m); } }

	void Scale(Vec const &v) { for (auto& bezcurve : m_list) { bezcurve.Scale(v); } }


private:
	std::vector<BezierCurve<Vec>> m_list;
};




#include "Bezier.hpp"

PieceWiseBezierSurface<glm::vec3> InitPWBezSurf(PieceWiseBezierCurve<glm::vec2> track, std::vector<PieceWiseBezierCurve<glm::vec2>> sections);




