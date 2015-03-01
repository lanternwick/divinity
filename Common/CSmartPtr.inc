#include "CSmartPtr.h"



template<class t>
CSmartPtr<t>::CSmartPtr()
{
	m_refCounter = 0;
}

template<class t>
CSmartPtr<t>::CSmartPtr(t* p)
{
	m_refCounter = 0;
	if(m_refCounter)
		m_refCounter->Release();
	if(!p) return;
	m_refCounter = new CSmartPtr<t>::CSmartRefCounter<t>(p);
	m_refCounter->AddRef();
}

template<class t>
CSmartPtr<t>::CSmartPtr(CSmartPtr<t>& copy)
{
	m_refCounter = copy.m_refCounter;	
	if(!copy.Null())
		copy.m_refCounter->AddRef();
}

template<class t>
CSmartPtr<t>::~CSmartPtr()
{
	Release();
}

template<class t>
t* CSmartPtr<t>::operator ->()
{
	return m_refCounter->m_p;
}

template<class t>
const t* CSmartPtr<t>::operator ->() const
{
	return m_refCounter->m_p;
}

template<class t>
void CSmartPtr<t>::Release()
{
	if((!Null()) && (m_refCounter->Release()==0))
		delete m_refCounter;
	m_refCounter = 0;
}

template<class t>
void CSmartPtr<t>::Attach(t* p)
{
	Release();
	m_refCounter = new CSmartPtr<t>::CSmartRefCounter<t>(p);
	m_refCounter->AddRef();
}

template<class t>
CSmartPtr<t>& CSmartPtr<t>::operator =(CSmartPtr<t>& rhs)
{
	if(!rhs.Null())
		rhs.m_refCounter->AddRef();
	Release();
	m_refCounter = rhs.m_refCounter;
	return *this;
}

template<class t>
bool CSmartPtr<t>::Null() const
{
	return m_refCounter == 0;
}

template<class t>
long CSmartPtr<t>::RefCount() const
{
	return m_refCounter->m_refCount;
}

template<class t>
bool operator==(CSmartPtr<t>& lhs, CSmartPtr<t>& rhs)
{
	return lhs.m_refCounter == rhs.m_refCounter;
}

template<class t>
bool operator!=(CSmartPtr<t>& lhs, CSmartPtr<t>& rhs)
{
	return lhs.m_refCounter != rhs.m_refCounter;
}
