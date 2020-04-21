#include "Bezier.h"

template<class Vec>
BezierCurve<Vec>::~BezierCurve()
{
}


template<class Vec>
inline BoundingBox<Vec> BezierCurve<Vec>::getBoundingBox() const
{
	if (m_ctrlPts.size() == 0)
		throw std::runtime_error("impossible to compute bounding box of a void BezierCurve");
	Vec v_min = m_ctrlPts[0], v_max = m_ctrlPts[0];
	int dim = v_min.length();
	for (auto& ctrlPt : m_ctrlPts)
	{
		for (int i = 0; i < dim; i++)
		{
			if (v_min[i] > ctrlPt[i])
				v_min[i] = ctrlPt[i];
			if (v_max[i] < ctrlPt[i])
				v_max[i] = ctrlPt[i];
		}
	}

	return BoundingBox<Vec>(v_min, v_max);
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
BezierSurface<Vec>::BezierSurface(int n, int m):m_ctrlPts(n)
{
	for (auto& v : m_ctrlPts)
	{
		v = BezierCurve<Vec>(m);
	}
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
inline BezierSurface<Vec> BezierSurface<Vec>::transpose()
{
	int n = m_ctrlPts.size();
	int m = m_ctrlPts[0].size();

	BezierSurface trans(m, n);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			trans.setCtrlPt(i, j, getCtrlPt(j, i));
		}
	}

	return trans;
}



template<class Vec>
template<typename Precision>
inline BezierSurface<Vec> BezierSurface<Vec>::derivateV()
{
	BezierSurface<Vec> bezSurfaceTrans = transpose();
	
	BezierSurface<Vec> BezierSurfaceDerTrans = bezSurfaceTrans.derivateU<Precision>();

	return BezierSurfaceDerTrans.transpose();

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


template<class Vec>
inline std::vector<std::vector<Vec>> PieceWiseBezierSurface<Vec>::Sample(int i, int j, int nbPtu, int nbPtv)
{
	return m_grid[i][j].Sample(nbPtu, nbPtv);
}


template<class Vec>
template<typename Precision>
inline Vec PieceWiseBezierSurface<Vec>::Eval(Precision u, Precision v)
{
	Precision umod(m_grid[0].size());
	u = MyModulo<Precision>(u,umod);
	while (u < 0)
		u += umod;

	int i = (int)std::floor(u);
	
	Precision uReal = u - Precision(i);

	Precision vmod(m_grid.size());
	v = MyModulo<Precision>(v, vmod);
	while (v < 0)
		v += vmod;
	int j = (int)std::floor(v);
	
	Precision vReal = v - Precision(j);

	return m_grid[j][i].Eval(uReal, vReal);
}

template<class Vec>
template<typename Precision>
inline PieceWiseBezierSurface<Vec> PieceWiseBezierSurface<Vec>::derivateU()
{
	unsigned int n = getN();
	unsigned int m = getM();
	PieceWiseBezierSurface derivateU(n,m);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			derivateU.m_grid[i][j] = m_grid[i][j].derivateU<Precision>();
		}
	}

	return derivateU;
}

template<class Vec>
template<typename Precision>
inline PieceWiseBezierSurface<Vec> PieceWiseBezierSurface<Vec>::derivateV()
{
	unsigned int n = getN();
	unsigned int m = getM();
	PieceWiseBezierSurface derivateV(n, m);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			derivateV.m_grid[i][j] = m_grid[i][j].derivateV<Precision>();
		}
	}

	return derivateV;
}

template<class Vec>
PieceWiseBezierSurface<Vec>::PieceWiseBezierSurface(int n, int m):m_grid(n)
{
	for (auto &v : m_grid)
	{
		v.resize(m);
	}
}

template<class Vec>
inline BoundingBox<Vec> PieceWiseBezierCurve<Vec>::getBoundingBox() const
{
	if (m_list.size() == 0)
		throw std::runtime_error("bounding box of empty pwbezcurve cannot be computed");

	BoundingBox<Vec> bb = m_list[0].getBoundingBox();

	for (auto& bezcurve : m_list)
	{
		bb.Union(bezcurve.getBoundingBox());
	}

	return bb;
}









