#ifndef _SINGLETON_H
#define _SINGLETON_H

template<class t> class singleton
{
public:
	singleton<t>(){}
	singleton<t>(t instance){destroy();_instance=instance;}
	virtual ~singleton<t>(){}

	static t create()
	{
		static t i;
		if(!_instance) _instance = i;
		return _instance;
	}
	static void destroy()
	{
		if(_instance)
		{
			_instance = NULL;
		}
	}
	static t& instance()
	{
		if(!_instance)
			create();
		return _instance;
	}
protected:
	static t _instance;
};

template<class t>
t singleton<t>::_instance = NULL;

#endif
