#ifndef B_SPLINE_H
#define B_SPLINE_H

#include "VECTOR.h"
#include <utility>

typedef VECTOR pt;

class b_spline
{
public:
	b_spline(){}
	~b_spline(){}

	pt at_t(float t, int k=3)
	{
		m_k = k;
		pt p;
		p.x = 0.0f;
		p.y = 0.0f;
		p.z = 0.0f;
		//t*=m_points.size()-1;
		//for(int i = 0; i < m_points.size(); ++i)
		//{
		//	p.x += m_points[i].x * blend(i, m_k, t*(m_points.size() - m_k + 2));
		//	p.y += m_points[i].y * blend(i, m_k, t*(m_points.size() - m_k + 2));
		//	p.z += m_points[i].z * blend(i, m_k, t*(m_points.size() - m_k + 2));

		//}
		if(t > 1.0f)
		{
			if(m_points.back().second)
				return *m_points.back().second;
			return m_points.back().first;
		}
		int i = t*(m_points.size()-1);
		float fr_t = ((m_points.size()-1)*t)-i;
		pt* p1, * p2;
		if(m_points[i].second)
			p1 = m_points[i].second;
		else
			p1 = &m_points[i].first;
		if(m_points[i+1].second)
			p2 = m_points[i+1].second;
		else
			p2 = &m_points[i+1].first;
		p.x = p1->x + fr_t * (p2->x - p1->x);
		p.y = p1->y + fr_t * (p2->y - p1->y);
		p.z = p1->z + fr_t * (p2->z - p1->z);
		return p;
	}
	
	void add_point(const pt& p)
	{
		std::pair<pt, pt*> pa;
		pa.second = NULL;
		pa.first = p;
		m_points.push_back(pa);
	}
	void add_point_ref(pt* p)
	{
		std::pair<pt, pt*> pa;
		pa.second = p;
		m_points.push_back(pa);	
	}
	void erase()
	{
		m_points.clear();
	}
private:
	std::vector<std::pair<pt,pt*> > m_points;
	int m_k;

	float knot(int i)
	{
		if(i < m_k)
		{
			return 0.0f;
		}
		if(i <= m_points.size()-1)
		{
			return static_cast<float>(i - m_k + 1);
		}
		return static_cast<float>((m_points.size()-1) - m_k + 2);
	}
	float blend(int i, int k, float t)
	{
		float n, d, a, b;
		if(m_k == 1)
		{
			if((knot(i) <= t) && (t < knot(i+1)))
			{
				return 1.0f;
			}
			else if((fabs(t - m_points.size() - k + 2) < 0.05f) && (knot(i) <= t)
				&& (t <= knot(i+1))) 
			{
				return  1.0f;
			}
			else
			{
				return 0.0f;
			}
		}

		d = knot(i + k - 1) - knot(i);
		if(fabs(d) < 0.05f)
		{
			a = 0.0f;
		}
		else
		{
			n = (t - knot(i)) * blend(i, k-1, t);
			a = n/d;
		}

		d = knot(i + k) - knot(i + 1);
		if(fabs(d) < 0.05f)
		{
			b = 0.0f;
		}
		else
		{
			n = ((knot(i+k))-t) * blend(i+1, k-1, t);
			b = n/d;
		}
		return a+b;
	}
};

#endif