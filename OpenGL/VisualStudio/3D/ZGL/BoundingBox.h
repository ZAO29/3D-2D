#pragma once
#include <glm.hpp>
template<typename Vec>
class BoundingBox
{
public:
	BoundingBox(Vec box_min, Vec box_max) :m_boxMin(box_min), m_boxMax(box_max) {}
	BoundingBox() {}
	~BoundingBox() {}

	Vec getMinBox() { return m_boxMin; }
	Vec getMaxBox() { return m_boxMax; }

	void Union(BoundingBox<Vec> bounding)
	{
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

private:
	Vec m_boxMin;
	Vec m_boxMax;

};



