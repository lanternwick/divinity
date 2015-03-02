#ifndef PROFILER_PROFILER_H
#define PROFILER_PROFILER_H

#pragma warning(disable:4786)

#include <vector>
#include <algorithm>
#include "timer.h"
namespace profiler
{
	template<typename prof_timer_type>
	class profiler
	{
	public:
		profiler(std::ostream& o)
			:m_output(o)
		{
			m_life.begin();
		}

		~profiler()
		{
#ifdef _MT
			lock.lock();
#endif // _MT
			m_life.end();
			prof_timer_type::time_type total_time = m_life.time();
			m_output << "Total Time: " << total_time << std::endl;
			for(std::vector<profile_section>::iterator i = m_sections.begin(); i != m_sections.end(); ++i)
			{
				m_output << (*i).name << '\t' << (*i).entries << '\t' << (*i).total_time << '\t' << ((*i).total_time / total_time)*100.0f << '%' << std::endl;
			}
#ifdef _MT
			lock.unlock();
#endif // _MT
		}

		void create_profile_section(moniker& name)
		{
#ifdef _MT
			lock.lock();
#endif // _MT
			profile_section p;
			p.name = name;
			std::vector<profile_section>::iterator ip = std::find(m_sections.begin(), m_sections.end(), p);
			if(ip == m_sections.end())
			{
				p.total_time = 0;
				p.entries = 0;
				m_sections.push_back(p);
			}
#ifdef _MT
			lock.unlock();
#endif // _MT
		}

		void delete_profile_section(moniker& name)
		{
#ifdef _MT
			lock.lock();
#endif // _MT
			profile_section p;
			p.name = name;
			std::vector<profile_section>::iterator ip = std::find(m_sections.begin(), m_sections.end(), p);
			if(ip != m_sections.end())
			{
				(*ip) = m_sections.back();
				m_sections.pop_back();
			}
#ifdef _MT
			lock.unlock();
#endif // _MT
		}

		void enter_profile_section(moniker& name)
		{
#ifdef _MT
			lock.lock();
#endif // _MT
			profile_section p;
			p.name = name;
			std::vector<profile_section>::iterator ip = std::find(m_sections.begin(), m_sections.end(), p);
			if(ip != m_sections.end())
			{
				++((*ip).entries);
				(*ip).t.begin();
			}
#ifdef _MT
			lock.unlock();
#endif // _MT
		}

		void leave_profile_section(moniker& name)
		{
#ifdef _MT
			lock.lock();
#endif // _MT
			profile_section p;
			p.name = name;
			std::vector<profile_section>::iterator ip = std::find(m_sections.begin(), m_sections.end(), p);
			if(ip != m_sections.end())
			{
				(*ip).t.end();
				(*ip).total_time += (*ip).t.time();
			}
#ifdef _MT
			lock.unlock();
#endif // _MT
		}
	private:
		struct profile_section
		{
			moniker name;
			prof_timer_type t;
			float total_time;
			unsigned long entries;
			bool operator ==(const profile_section& rhs)
			{
				return rhs.name() == name();
			}
		};
		prof_timer_type m_life;
		std::vector<profile_section> m_sections;
		std::ostream& m_output;
#ifdef _MT
		mutex lock;
#endif // _MT
	};

	typedef profiler<timer<def_timer_type<float> > > sprofiler;
};

#endif