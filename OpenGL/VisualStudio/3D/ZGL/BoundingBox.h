#pragma once
#include <glm.hpp>
template<typename Vec>
class BoundingBox
{
public:
	BoundingBox(Vec box_min, Vec box_max) :m_boxMin(box_min), m_boxMax(box_max),m_bEmpty(false) {}
	BoundingBox() {}
	~BoundingBox() {}

	Vec getMinBox() { return m_boxMin; }
	Vec getMaxBox() { return m_boxMax; }

	bool m_bEmpty = true;

	void Union(BoundingBox<Vec> bounding)
	{
		if (m_bEmpty)
		{
			m_boxMin = bounding.m_boxMin;
			m_boxMax = bounding.m_boxMax;
			m_bEmpty = bounding.m_bEmpty;
		}


		const int dim = m_boxMin.length();
		for (int i = 0; i < dim; i++)
		{
			if (m_boxMin[i] > bounding.m_boxMin[i])
			{
				m_boxMin[i] = bounding.m_boxMin[i];
			}

			if (m_boxMax[i] < bounding.m_boxMax[i])
			{
				m_boxMax[i] = bounding.m_boxMax[i];
			}
		}
	}

	template<typename Precision>
	Vec getCenter() { return (m_boxMax + m_boxMin) / Precision(2.); }

	Vec getSize() { return (m_boxMax - m_boxMin); }

private:
	Vec m_boxMin;
	Vec m_boxMax;

};



