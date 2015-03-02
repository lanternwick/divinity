#ifndef GROUPDOT_H
#define GROUPDOT_H

#include "..\DivUI\UserInterface.h"
#include "..\DivUI\UIBaseWindow.h"
#include "..\Common\b_spline.h"
#include <list>

class GroupDot : public UIBaseWindow
{
public:
	GroupDot();
	virtual ~GroupDot();

	void Hide();
	void Show();
	void ToggleVis();

	virtual bool Mouse_ButtonUp(int button);
	
	void SetParent(GroupDot* parent);
	void SetFinalX(float x);
	void SetFinalY(float y);
	void SetFinalZ(float z);
	void SetRadius(float r);
	void Name(const std::string name);
	void TextSize(float s);
	const std::string Name() const;
	const float TextSize() const;

	void AttachGroup(GroupDot* gd);
	VECTOR pos();
	RESULT Think();
	RESULT Render();

	RESULT Create(IDevice* device, IGraphObjFactory* factory, UserInterface* ui);
private:
	bool m_moving;
	bool m_hiding;
	b_spline m_path;
	GroupDot* m_par;
	std::list<GroupDot*> m_groups;
	//std::list<LauncherDot*> m_launchers;
	VECTOR m_fin;
	float m_r;
	unsigned long m_color_fin;
	float m_t;
	UserInterface* m_ui;
	std::string m_name;
	float m_text_size;
};

#endif
