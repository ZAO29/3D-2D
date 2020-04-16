#pragma once

#include "Constant.h"




#include <vector>
template <class Vec>
class BezierCurve
{
public:
	BezierCurve(){}
	~BezierCurve();


	void setCtrlPt(std::vector<Vec> ctrlPts) { m_ctrlPts = ctrlPts; }

	template<typename Precision>
	Vec Eval(Precision t);

	template<typename Precision>
	std::vector<Vec> Sample(int nbPt);

	std::vector<Vec> m_ctrlPts;

	
};

template <typename Precision>
Precision MyBeta(Precision i, Precision j); // { throw std::runtime_error("beta precision not implemented"); return Precision(); };

template <class Precision>
Precision BinomialCoeff(int i, int m);


template <class Precision>
Precision evalBernStein(Precision t, int i, int m);

template <class Vec>
class BezierSurface
{
public:
	BezierSurface(){}
	~BezierSurface() {}

	void setCtrlPt(std::vector<std::vector<Vec> > ctrlPts);


	template<typename Precision>
	Vec Eval(Precision u, Precision v);

	template<typename Precision>
	BezierCurve<Vec> Eval(Precision u);

	template<typename Precision>
	std::vector< std::vector<Vec> > Sample(int nbPtu, int nbPtv);

	std::vector<BezierCurve<Vec> > m_ctrlPts;
};



#include "Bezier.hpp"


