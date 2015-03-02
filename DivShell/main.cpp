#define WIN32_MEAN_AND_LEAN

#include "..\DivCore\DivCore.h"


#include <windows.h>
#include <shlwapi.h>
#include <vector>
#include <direct.h>
#include <crtdbg.h>


#include "..\Common\IMod.h"
#include "..\DivGraph\DivGraph.h"
#include "..\DivGraph\IDevice.h"
#include "..\DivUI\DivUI.h"
#include "GroupDot.h"
#include "..\DivUI\KeyConstants.h"
#include "..\DivUI\UICursor.h"
#include "..\DivUI\UITitlebar.h"

/*inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}*/

#include "SYSWindow.h"
#include "..\Common\gwen.h"

LRESULT __stdcall MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg)
	{
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);

}

typedef IMod* GetModObject();
typedef void DestroyModObject();

void CreateGD(GroupDot* gd, g_block<char>& b, UserInterface* ui)
{
	std::string name("group");
	GroupDot* n = NULL;
	float x, y, z, r, ts;
	std::string na;
	std::stringstream ss;
	std::stringstream(b.attribute(std::string("x"))) >> x;
	std::stringstream(b.attribute(std::string("y"))) >> y;
	std::stringstream(b.attribute(std::string("z"))) >> z;
	std::stringstream(b.attribute(std::string("radius"))) >> r;
	std::stringstream(b.attribute(std::string("name"))) >> na;
	std::stringstream(b.attribute(std::string("text_size"))) >> ts;
	
	gd->SetFinalX(x);
	gd->SetFinalY(y);
	gd->SetFinalZ(z);
	gd->SetRadius(r);
	gd->Name(na);
	gd->TextSize(ts);
	ui->Windows().AttachWindow(gd);

	for(int i = 0; i < b.num_blocks_of_type(name); ++i)
	{
		n = new GroupDot;
		
		n->SetParent(gd);
		gd->AttachGroup(n);
		CreateGD(n, b.block(name, i), ui);
	}
	// create launchers
}

int RunShell(HWND hWnd)
{
	IDevice* dev = DivGraph_GetDevice();
	IGraphObjFactory* factory = DivGraph_GetFactory();
	if(!dev)
	{
		return 1;
	}
	std::stringstream ss;
	file_g_input<char> fi(std::string("shell.cfg"));
	g_block<char> root;
	g_parser<char> p;
	try
	{
		p.parse(fi);
		root = p.root();
	}
	catch(g_exception e)
	{
		Core::WriteLog(e.format_e().c_str(), 0);
		return 1;
	}
	int cx, cy, windowed;
	try
	{
		g_block<char> win_block = root.block(std::string("window"));
		if(win_block.attribute_exists(std::string("windowed")))
		{
			windowed = 1;
		}
		else
		{
			windowed = 0;
		}
		ss << win_block.attribute(std::string("cx"));
		ss >> cx;
		ss.str(std::string());
		ss << win_block.attribute(std::string("cy"));
		ss >> cy;
	}
	catch(g_exception e)
	{
		Core::WriteLog(e.format_e().c_str(), 0);
		return 0;
	}
	RESULT r = dev->Create(hWnd, windowed, cx, cy);
	if(RFAILED(r))
	{
		return 1;
	}
	factory->SetDevice(dev);
	UserInterface* ui = NULL;
	ui = new UserInterface;
	
	//singleton<UserInterface*> sui(m_ui);

	ui->Input().CreateDevice(hWnd); //Initialize(hWnd, m_device, m_factory);
	DivUI_InitEx(true, ui);
	ui->Fonts().SetDevice(dev);
	ui->Fonts().SetFactory(factory);
	ui->Fonts().LoadFont("fonts\\verdana.font", moniker("default"));
	ITexture* tex;
	factory->LoadTexture("textures\\sphere.texture", tex, "sphere");

	GroupDot* gd = new GroupDot;
	g_block<char> group_b;
	try
	{
		group_b = root.block(std::string("group"));
	}
	catch(g_exception e)
	{
		Core::WriteLog(e.format_e().c_str(), 0);
		return 1;
	}
	CreateGD(gd, group_b, ui);
	gd->Create(dev, factory, ui);
	gd->visible(true);
	UICursor* cursor = NULL;
	cursor = new UICursor;
	cursor->Create(dev, factory);
	dev->SetRenderState(RS_ALPHABLENDENABLE, TRUE);
	ui->Windows().AttachWindow(cursor);
	UITitlebar* win = NULL;
	win = new UITitlebar;
	UIBaseWindow* base = NULL;
	base = new UIBaseWindow;
	base->Create(dev, factory, .07, .07, .04, .04, 0xffffffff, moniker(""), NULL);
	base->visible(true);
	win->Create(dev, factory, 20, 0xffff77ff, moniker(""), base);
	
	ui->Windows().AttachWindow(win);
	MSG msg;
	while(true)
	{
		if(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(ui->Input().GetKey(DIV_KEY_ESCAPE))
		{
			break;
		}
		Core::BeginFrame();
		dev->Clear(0xff000000);
		dev->BeginScene();
		ui->Think();
		ui->Render();
		dev->EndScene();
		dev->Flip();
		Core::EndFrame();
		

	}
	tex->Release();
	delete tex;
	delete base;
	//delete gd;
	delete ui;
	dev->Cleanup();
	return 1;
}

INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdLine, INT)
{
	Core::Init();
	Core::Function_Enter("WinMain()");

	IMod* mod;

	HMODULE module = NULL; //LoadLibrary("..\\Times Of War\\TimesOfWar.dll");
	
	HMODULE dev_mod = LoadLibrary("D3DDev.dll");
	if(!dev_mod) return 1;
	DivGraph_Init(dev_mod);
	SYSWindow* MainWnd = new SYSWindow;
	MainWnd->Create(100, 100, 640, 480, "Div");
	ShowWindow(MainWnd->hWnd(), SW_SHOWNORMAL);
	UpdateWindow(MainWnd->hWnd());
	if(!module)
	{
		int res = RunShell(MainWnd->hWnd());
		if(res)
		{
			MainWnd->Cleanup();
			delete MainWnd;
			OutputDebugString(Core::ReadLog(-1));
			Core::Destroy();
			FreeLibrary(dev_mod);
			return res;
		}
	}
	char path[256];
	GetModuleFileName(module, path, 255);
	char* iter = path + 255;
	while(*iter != '\\')
		--iter;
	iter[1] = 0;
	GetModObject* Game = (GetModObject*)GetProcAddress(module, "Game");
	DestroyModObject* DestroyGame = (DestroyModObject*)GetProcAddress(module, "DestroyGame");
	_chdir(path);
	if(!Game) return 1;
	mod = Game();


	mod->Set_hWnd(MainWnd->hWnd());


	int ret;
		
	//try
	{
		mod->Init(DivGraph_GetDevice(), cmdLine);
		ret = mod->Run(DivGraph_GetDevice(), cmdLine);	
		mod->Cleanup();
	}
	/*catch(...)
	{
		char buff[4096];
		sprintf(buff, "An exception has occured in module:\n%s\n", engine->ECore()->GetFunctionStack());
		OutputDebugString(buff);
		MessageBox(MainWnd->hWnd(), buff, "Exception", MB_OK);
	}*/
	
	
	// kill the window
	//try
	{
		//delete mod;
		MainWnd->Cleanup();
		delete MainWnd;
		DestroyGame();
		OutputDebugString("Dumping memory leaks from DivShell.exe\n");
		if(!_CrtDumpMemoryLeaks())
		{
			OutputDebugString("No Leaks Found in DivShell.exe\n");
		}
		Core::Function_Exit();
	}
	//catch(...)
	{
	//	OutputDebugString("Error in Cleanup!\n");
	}
	Core::Destroy();
	FreeLibrary(dev_mod);
	FreeLibrary(module);

	return ret;
}