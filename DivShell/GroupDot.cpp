#include "GroupDot.h"

#include <cstdlib>

float rnd(float a, float b)
{
	float r = float(rand())/float(RAND_MAX);
	return (b-a)*r+a;
}

GroupDot::GroupDot()
{
	m_par = NULL;
	m_visible = false;
	m_color_fin = 0xff0000ff;
	m_moving = false;
	m_hiding = false;
	m_ui = NULL;
}

GroupDot::~GroupDot()
{
	/*for(std::list<GroupDot*>::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
	{
		delete (*i);
	}*/
	/*for(std::list<LauncherDot*>::iterator j = m_launchers.begin(); j != m_launchers.end(); ++j)
	{
		delete (*j);
	}*/
	if(m_window)
	{
		delete m_window;
		m_window = NULL;
	}
	m_par = NULL;
	m_ui = NULL;
}

void GroupDot::Hide()
{
	VECTOR vc, vf, vn;
	vf.x = (float)m_par->m_pos.x;
	vf.y = (float)m_par->m_pos.y;
	vf.z = 0.5f;
	vc = m_pos;
	m_path.erase();
	m_path.add_point(vc);
	for(int i = 0; i < 2; ++i)
	{
		vn.x = rnd(vc.x, vf.x/float(4-i));
		vn.y = rnd(vc.y, vf.y/float(4-i));
		vn.z = rnd(0.0f, 1.0f);
		m_path.add_point(vn);
		vc = vn;

	}
	m_path.add_point_ref(&m_par->m_pos);
	//m_path.add_point(vf);
	m_moving = true;
	m_hiding = true;
	//x(m_par->m_pos.x);
	//y(m_par->m_pos.y);
	//visible(false);
	m_t = Core::GetTime();

}

void GroupDot::Show()
{
	VECTOR vc, vf, vn;
	vc.x = (float)m_par->m_pos.x;
	vc.y = (float)m_par->m_pos.y;
	vc.z = 0.5f;
	vf = m_fin;
	m_path.erase();
	m_path.add_point(vc);
	for(int i = 0; i < 2; ++i)
	{
		vn.x = rnd(vc.x, vf.x/float(4-i));
		vn.y = rnd(vc.y, vf.y/float(4-i));
		vn.z = rnd(0.0f, 1.0f);
		m_path.add_point(vn);
		vc = vn;

	}
	m_path.add_point(vf);
	m_moving = true;
	//x(m_fin.x);
	//y(m_fin.y);
	visible(true);
	
	m_t = Core::GetTime();
}

void GroupDot::ToggleVis()
{
	if(m_visible)
	{
		for(std::list<GroupDot*>::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
		{
			(*i)->Hide();
		}
		Hide();
	}
	else
	{
		Show();
	}
}

RESULT GroupDot::Think()
{
	VECTOR v;
	float t = (Core::GetTime()-m_t);
	if(m_moving)
	{
		v = m_path.at_t(t*1.0f);

		cx(m_r*(1.0f/(0.5f+v.z)));
		cy(m_r*(1.0f/(0.5f+v.z)));

		x(v.x-cx()/2);
		y(v.y-cy()/2);
		m_color = ColorInterpolate(m_color_fin, 0xff000000, v.z);
		if(t>=1.0f)
		{
			if(m_hiding)
			{
				visible(false);
				m_hiding = false;
			}
			m_moving = false;
		}
		//return OK;
	}
	UIBaseWindow::Think();
	return OK;

}

RESULT GroupDot::Render()
{
	if(!m_visible)
	{
		return OK;
	}
	UIBaseWindow::Render();
	if(!m_ui)
	{
		return OK;
	}
	RECT r;
	float s = m_text_size;
	int n = m_name.length()-1;
	r.left = m_pos.x+(m_size.cx/2)-((n*11*s)/2);
	r.right = m_pos.x+(m_size.cx/2)+((n*11*s)/2);
	r.top = m_pos.y+(m_size.cy/2)-((15*s)/2);
	r.bottom = m_pos.y+(m_size.cy/2)+((15*s)/2);

	m_ui->Fonts().DrawText(moniker("default"), m_name.c_str(), r, NULL, s, 0xff7777ff);
	return OK;
}

bool GroupDot::Mouse_ButtonUp(int button)
{
	if(button == 0)
	{
		for(std::list<GroupDot*>::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
		{
			(*i)->ToggleVis();
		}
		/*for(std::list<LauncherDot*>::iterator j = m_launchers.begin(); j != m_launchers.end(); ++j)
		{
			(*j)->ToggleVis();
		}*/
	}
	if(button == 1)
	{

	}
	return false;
}

void GroupDot::SetParent(GroupDot* parent)
{
	m_par = parent;
}

void GroupDot::SetFinalX(float x)
{
	m_fin.x = x;
}

void GroupDot::SetFinalY(float y)
{
	m_fin.y = y;
}

void GroupDot::SetFinalZ(float z)
{
	m_fin.z = z;
}

void GroupDot::SetRadius(float r)
{
	m_r = r;
}

void GroupDot::Name(const std::string name)
{
	m_name = name;
}

const std::string GroupDot::Name() const
{
	return m_name;
}

VECTOR GroupDot::pos()
{
	return m_fin;
}

RESULT GroupDot::Create(IDevice* device, IGraphObjFactory* factory, UserInterface* ui)
{
	for(std::list<GroupDot*>::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
	{
		(*i)->Create(device, factory, ui);
		//(*i)->m_visible = false;
	}
	if(m_par)
	{
		UIBaseWindow::Create(device, factory, m_par->m_fin.x-m_r, m_par->m_fin.y-m_r, m_r*2, 
			m_r*2, m_color_fin, moniker("sphere"), NULL);
	}
	else
	{
		UIBaseWindow::Create(device, factory, m_fin.x-m_r, m_fin.y-m_r, m_r*2, 
			m_r*2, m_color_fin, moniker("sphere"), NULL);
		cx(m_r*device->width()*(1.0f/(0.5f+m_fin.z)));
		cy(m_r*device->width()*(1.0f/(0.5f+m_fin.z)));

		x(m_fin.x*device->width()-cx()/2);
		y(m_fin.y*device->width()-cy()/2);
		m_color = ColorInterpolate(m_color_fin, 0xff000000, m_fin.z);
		Update();
	}
	
	m_fin.x *= device->width();
	m_fin.y *= device->width();
	m_r *= device->width();
	/*for(std::list<LauncherDot*>::iterator j = m_launchers.begin(); j != m_launchers.end(); ++j)
	{
		(*j)->Create(device, factory);
		(*i)->m_visible = false;
	}*/
	m_ui = ui;
	return OK;

}

void GroupDot::AttachGroup(GroupDot* gd)
{
	m_groups.push_back(gd);
}

void GroupDot::TextSize(float s)
{
	m_text_size = s;
}

const float GroupDot::TextSize() const
{
	return m_text_size;
}