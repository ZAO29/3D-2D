#include "Bezier.h"

template<class Vec>
BezierCurve<Vec>::~BezierCurve()
{
}


template<class Vec>
template<typename Precision>
inline BezierCurve<Vec> BezierCurve<Vec>::Derivate()
{
	unsigned int n = m_ctrlPts.size() - 1;
	std::vector<Vec> derivateCtrlPts(n);
	int i = 0;
	for (auto& derivateCtrlPt : derivateCtrlPts)
	{
		derivateCtrlPt = Precision(n) * (m_ctrlPts[i + 1] - m_ctrlPts[i]);
		i++;
	}

	BezierCurve<Vec> bez;
	bez.setCtrlPt(derivateCtrlPts);

	return bez;
}

template<class Vec>
template<typename Precision>
inline std::vector<Vec> BezierCurve<Vec>::Sample(int nbPt)
{
	Precision t(0.);
	Precision step = Precision(1.) / Precision(nbPt - 1);

	std::vector<Vec> samples;
	samples.resize(nbPt);

	

	for (auto& val : samples)
	{
		val = Eval<Precision>(t);
		t += step;
	}

	return samples;
}



template<class Precision>
Precision BinomialCoeff(int i, int m)
{
	return Precision(1) / (Precision(m + 1)*MyBeta<Precision>(Precision(m - i + 1), Precision(i + 1)));
}

template<class Precision>
Precision evalBernStein(Precision t, int i, int m)
{
	return BinomialCoeff<Precision>(i, m)*std::pow(t, Precision(i))*std::pow(Precision(1.) - t, Precision(m - i));
}

template<class Vec>
template<typename Precision>
inline Vec BezierCurve<Vec>::Eval(Precision t)
{
	Vec val = Vec(0);
	int i = 0;
	int m = m_ctrlPts.size() - 1;

	for (auto & ctrlPt : m_ctrlPts)
	{
		val += evalBernStein<Precision>(t, i, m) *ctrlPt;
		i++;
	}

	return val;
}


template<class Vec>
inline void BezierSurface<Vec>::setCtrlPt(std::vector<std::vector<Vec> > ctrlPts)
{
	m_ctrlPts.resize(ctrlPts.size());
	unsigned int i = 0;
	for (auto& BezierCurve : m_ctrlPts)
	{
		BezierCurve.setCtrlPt(ctrlPts[i]);
		i++;
	}
}


template<class Vec>
inline void BezierSurface<Vec>::setCtrlPt(std::vector<BezierCurve<Vec>> v)
{
	m_ctrlPts.resize(v.size());
	unsigned int i = 0;
	for (auto& BezierCurve : m_ctrlPts)
	{
		BezierCurve.setCtrlPt(v[i].getCtrlPt());
		i++;
	}
}


template<class Vec>
template<typename Precision>
inline BezierSurface<Vec> BezierSurface<Vec>::derivateU()
{
	std::vector<BezierCurve<Vec>> ctrlPtGrid(m_ctrlPts.size());
	int i = 0;
	for (auto& ctrlPts : ctrlPtGrid)
	{
		ctrlPts = m_ctrlPts[i].Derivate<Precision>();
		i++;
	}

	BezierSurface derivateUsurf;
	derivateUsurf.setCtrlPt(ctrlPtGrid);

	return derivateUsurf;

}

template<class Vec>
template<typename Precision>
inline Vec BezierSurface<Vec>::Eval(Precision u, Precision v)
{
	BezierCurve<Vec> bezCurve = Eval(u);
	return bezCurve.Eval(v);
}

template<class Vec>
template<typename Precision>
inline BezierCurve<Vec> BezierSurface<Vec>::Eval(Precision u)
{
	BezierCurve<Vec> bezCurve;
	std::vector<Vec> bezCurveCtrlPt;
	bezCurveCtrlPt.resize(m_ctrlPts.size());

	int i = 0;
	for (auto& ctrlPt : m_ctrlPts)
	{
		bezCurveCtrlPt[i] = ctrlPt.Eval(u);
		i++;
	}

	bezCurve.setCtrlPt(bezCurveCtrlPt);

	return bezCurve;
}

template<class Vec>
template<typename Precision>
inline std::vector<std::vector<Vec>> BezierSurface<Vec>::Sample(int nbPtu, int nbPtv)
{
	std::vector< std::vector<Vec> > samples;

	samples.resize(nbPtu);

	Precision u(0.);
	Precision step = Precision(1.) / Precision(nbPtu - 1);

	for (auto& sample : samples)
	{
		BezierCurve<Vec> bez = Eval(u);
		sample = bez.Sample<Precision>(nbPtv);
		
		u += step;
	}

	return samples;
}







