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

#include "BezierCurve.hpp"