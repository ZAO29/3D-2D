template<class Vec>
BezierCurve<Vec>::~BezierCurve()
{
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








