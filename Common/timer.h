#ifndef PROFILER_TIMER_H
#define PROFILER_TIMER_H

#include <windows.h>
#ifdef _MT

	#include "..\DivCore\mutex.h"

#endif // _MT
namespace profiler
{
	template<typename timer_type>
	class timer
	{
		public:
			typedef timer_type::time_type time_type;
			void begin()
			{
#ifdef _MT // _MT
				lock.lock();
#endif
				m_s = m_t.get_t();
#ifdef _MT
				lock.unlock();
#endif // _MT
			}

			void end()
			{
#ifdef _MT
				lock.lock();
#endif // _MT
				m_e = m_t.get_t();
#ifdef _MT
				lock.unlock();
#endif // _MT
			}

			time_type time()
			{
#ifdef _MT
				lock.lock();
#endif // _MT
				time_type tt = m_t.convert_to_seconds(m_s, m_e);
#ifdef _MT
				lock.unlock();
#endif // _MT
				return tt;
			}
		private:
			timer_type m_t;
			time_type m_s;
			time_type m_e;
#ifdef _MT
			mutex lock;
#endif // _MT
	};

	template<typename t>
	class def_timer_type
	{
	public:
		typedef t time_type;
		def_timer_type()
		{
			LARGE_INTEGER ti; 
			QueryPerformanceFrequency(&ti);
			m_frequency = ti.QuadPart;
		}
		time_type get_t()
		{
			LARGE_INTEGER ti;
			QueryPerformanceCounter(&ti);
			return (time_type)ti.QuadPart / m_frequency;
		}

		time_type convert_to_seconds(time_type start, time_type end)
		{
			return end - start;
		}
	private:
		__int64 m_frequency;
	};


};

#endif