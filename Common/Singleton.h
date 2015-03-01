#ifndef _SINGLETON_H
#define _SINGLETON_H

template<class t> class Singleton
{
public:
	Singleton<t>(){}
	~Singleton<t>(){}

	static t* Create()
	{
		if(!_instance) _instance = new t;
		return _instance;
	}
	static void Destroy()
	{
		if(_instance)
		{
			delete _instance;
			_instance = NULL;
		}
	}
	static t* Instance()
	{
		return _instance;
	}
protected:
	static t* _instance;
};

template<class t>
t* Singleton<t>::_instance = NULL;

#endif
