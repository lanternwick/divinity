#ifndef _CSMARTPTR_H
#define _CSMARTPTR_H

#ifdef _EXPORTS
#define SMART_PTR_API __declspec(dllexport)
#else
#define SMART_PTR_API __declspec(dllimport)
#endif

#include "define_exports.h"

template<class t>
class SMART_PTR_API CSmartPtr
{
public:
	CSmartPtr();
	CSmartPtr(t* p);
	CSmartPtr(CSmartPtr& copy);
	~CSmartPtr();

	void Attach(t* p);
	void Release();

	friend bool operator==(CSmartPtr<t>& lhs, CSmartPtr<t>& rhs);
	friend bool operator!=(CSmartPtr<t>& lhs, CSmartPtr<t>& rhs);
	CSmartPtr<t>& operator = (CSmartPtr<t>& rhs);
	t* operator->();
	const t* operator->() const;
	bool Null() const;
	long RefCount() const;
protected:
	template<class t>
	class CSmartRefCounter
	{
	public:
		CSmartRefCounter(t* p)
		{
			if(!p) return;
			m_p = p;
			m_refCount = 0;
		}
		~CSmartRefCounter()
		{
			if(m_refCount)
				throw "Remaining References!";
			delete m_p;
		}
		long AddRef()
		{
			return ++m_refCount;
		}

		long Release()
		{
			if(!m_refCount) throw "Ref Count == 0";
			return --m_refCount;
		}
		t* m_p;
		long m_refCount;
	};

	CSmartRefCounter<t>* m_refCounter;	
};

#include "CSmartPtr.inc"

#endif