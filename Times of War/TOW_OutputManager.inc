

template<class t>
TOW_OutputManager<t>::~TOW_OutputManager()
{
	std::list<t*>::iterator i;
	t* temp;
	while(!objects.empty())
	{
		i = objects.begin();
		temp = (*i);
		objects.remove((*i));
	}
}

template<class t>
t* TOW_OutputManager<t>::InsertObject(t* oo)
{
	objects.push_back(oo);
	return oo;
}

