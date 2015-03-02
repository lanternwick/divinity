// UIDemo.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "UIDemo.h"
#include <math.h>
#include "..\system\system.h"
#include "..\ui\ui.h"
#include "..\effects\effects.h"
#include "..\geometry\geometry.h"
#include <stdio.h>

#include <utils.h>

#include <result.h>
#include "tmpmap.h"
#include <WorldObj.h>
#include <WorldActor.h>
#include <WorldStateMove.h>
#include <WorldStateAttack.h>
#include <worldgrass.h>

typedef HANDLE HTEXTURE;

int msgCount = 0;
int state = 0;
float facing = 0.0f;
VECTOR cameraPos(4.0f, 5.0f, 4.0f);
bool dirty = FALSE;

HFONT BaseFont;

VOID SetupControlWindow();

namespace UIDemoObjects
{
	HANDLE GettingStarted;
	HANDLE MainWindow = NULL;
	HANDLE StatusWindow;
	HANDLE titleLabel;
	HANDLE hpLabel;
	HANDLE speedLabel;
	HANDLE moveButton;
	HANDLE attackButton;
	TmpMap* map = NULL;
	HANDLE snowEmitter;
	HANDLE mouseEmitter;
	HANDLE fireEmitter;
	HANDLE selEmitter;
	HANDLE snowPSys;
	HANDLE firePSys;
	HANDLE mousePSys;
	HANDLE firePSys2;
	HANDLE selPSys;
	HANDLE TetMdl;
	std::vector<WRLDActor*> Objects;
	WRLDActor* SelObj = NULL;
	WRLDGrass* grass = NULL;

}

using UIDemoObjects::Objects;

namespace UIDemoTextures
{
	HTEXTURE TitleTexture;
	HTEXTURE EnvCText;
	HTEXTURE EnvOText;
	HTEXTURE ParchBG;
	HTEXTURE ScrollBarBG;
	HTEXTURE close;
	HTEXTURE closeOver;
	HTEXTURE scrollU, scrollD;
	HTEXTURE scrollThumb;
	HTEXTURE TerrainText;
	HTEXTURE TerrainBlend;
	HTEXTURE Snow;
	HTEXTURE CursorTexture;
	HTEXTURE MoveButtonTexture;
	HTEXTURE AttackButtonTexture;
}

namespace UIDemoMsgProcs
{
	VOID MainMenuWindowProc(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{


	}

	VOID MoveWindowProc(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		switch(msg)
		{
		case DM_CLICK:
			if(UIDemoObjects::SelObj)
				state = 1;
			return;
		}
		return;
	}

	VOID AttackWindowProc(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		switch(msg)
		{
		case DM_CLICK:
			if(UIDemoObjects::SelObj)
				state = 2;
			return;
		}
		return;
	}

	VOID GettingStartedProc(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		/*switch(msg)
		{
		case DM_MOUSEOVER:
			SetDisplayBoxText(UIDemoObjects::MainDisplayBox, "Getting Started:\nWelcome to Times\n of War!\nLearn more by \nclicking here!\n");
		}*/

	}





}

namespace UIDemoEffectProcs
{
	VOID Snowy(PARTICLE* particles, INT numParticles, VECTOR* basePos, INT* ret, DWORD param, HANDLE emitter)
	{
		static INT active = 0;
		float timeD = TimeDelta();
		DWORD ticks = GetTicks();
		active += rand() % 25;
		if(active > numParticles) active = numParticles;
		PARTICLE* p = particles;
		*ret = active;
		for(INT i = 0; i < active; ++i, ++p)
		{
			if(p->alive)
			{
				p->myPos += p->myVel * timeD;
				p->myVel.x = rnd() - rnd();
				p->myVel.z = rnd() - rnd();
				if(p->myPos.y < 0.5f)
				{
					p->myVel.x = p->myVel.y = p->myVel.z = 0.0f;
				}
				if(ticks >= (p->myBirth + p->myLifeTime))
				{
					p->alive = FALSE;
				}
				p->color = ColorInt(0x99ffffff, 0x00000000, float(ticks - p->myBirth) / float(p->myLifeTime));

			}
			else
			{
				p->myPos.y = 10.0f;
				p->myPos.x = (float)rnd() * 25.0f;
				p->myPos.z = (float)rnd() * 25.0f;
				p->color = 0x99ffffff;
				p->alive = TRUE;
				p->myLifeTime = rand() % 700;
				p->myBirth = ticks;
				p->myVel.x = 0.0f;
				p->myVel.z = 0.0f;
				p->myVel.y = -2.6f * rnd() - 0.3f;
				p->mySize = 0.3f;
			}
		}
		return;
	}

	VOID Fire(PARTICLE* particles, INT numParts, VECTOR* basePos, INT* ret, DWORD param, HANDLE emitter)
	{
	 	float timeD = TimeDelta();
		DWORD ticks = GetTicks();
		*ret += 2;
		if(*ret > numParts) *ret = numParts;
		PARTICLE* p = particles;
		for(INT i = 0; i < *ret; ++i, ++p)
		{
			if(p->alive)
			{
				p->myPos += p->myVel * (timeD);
				p->myVel.x = rnd() - rnd();
				p->myVel.z = rnd() - rnd();
				if(ticks >= (p->myBirth + p->myLifeTime))
					p->alive = FALSE;
				p->color = ColorInt(0xccff4400, 0x00ffffff, float(ticks - p->myBirth) / float(p->myLifeTime));
				p->mySize = FloatInt(p->StartSize, p->EndSize, float(ticks - p->myBirth) / float(p->myLifeTime));

			}
			else
			{
				//if(emitter == NULL)
				//	continue;
				if(!CanInitParticle(emitter))
				{
					p->color = 0;
					continue;
				}
				p->color = 0xccff4400;
				p->myPos = *basePos;
				//p->myPos.x += (rnd() - rnd())*5.0f*(float(active) / float(numParts));
				//p->myPos.z += (rnd() - rnd())*5.0f*(float(active) / float(numParts));
				p->myPos.y = 0.2f;
				p->myVel.x = 0.0f;
				p->myVel.z = 0.0f;
				p->myVel.y = 0.7f * rnd()+0.7f;
				p->StartSize = 0.7f + rnd() * 0.2f;
				p->EndSize = 0.1f + rnd() * 0.5f;
				p->mySize = 0.7f;
				p->myBirth = ticks;
				p->myLifeTime = DWORD(25*rnd()+25);
				p->alive = TRUE;
			}
			
		}
		return;
	}

	VOID Fall(PARTICLE* particles, INT numParts, VECTOR* basePos, INT* ret, DWORD param, HANDLE emitter)
	{
		float timeD = TimeDelta();
		DWORD ticks = GetTicks();
		*ret += 2;
		if(*ret > numParts) *ret = numParts;
		PARTICLE* p = particles;
		for(INT i = 0; i < *ret; ++i, ++p)
		{
			if(p->alive)
			{
				p->myPos += p->myVel * timeD;
				p->myVel.y += 0.1f * timeD;
				if(ticks >= (p->myBirth + p->myLifeTime))
					p->alive = FALSE;
				p->color = ColorInt(p->StartColor, p->EndColor, float(ticks - p->myBirth) / float(p->myLifeTime));
				p->mySize = FloatInt(p->StartSize, p->EndSize, float(ticks - p->myBirth) / float(p->myLifeTime));

			}
			else
			{
				p->color = DWORD(param);
				p->StartColor = DWORD(param);
				p->EndColor = 0;
				p->myPos = *basePos;
				p->myVel.x = rnd() * 1.2f - 0.6f;
				p->myVel.z = rnd() * 1.2f - 0.6f;
				p->myVel.y = rnd() / 10.0f;
				p->StartSize = 0.2f;
				p->EndSize = 0.1f;
				p->mySize = 0.2f;
				p->myBirth = ticks;
				p->myLifeTime = DWORD(30*rnd()+25);
				p->alive = TRUE;
			}
		}
		return;
	}

	VOID Mouse(PARTICLE* particles, INT numParts, VECTOR* basePos, INT* ret, DWORD param, HANDLE emitter)
	{
		static INT active = 0;
		float timeD = TimeDelta();
		DWORD ticks = GetTicks();
		POINT mp = GetMousePos();
		DELTAPOINT dp = GetMouseDelta();
		active += 2;
		if(active > numParts) active = numParts;
		PARTICLE* p = particles;
		*ret = active;
		for(INT i = 0; i < active; ++i, ++p)
		{
			if(p->alive)
			{
				p->myPos += p->myVel * timeD;
				p->myVel.y += 980.0f * timeD;
				if(p->myPos.y >= 600.0f)
				{
					p->myVel.y *= -0.5f;
					//p->myPos.y = 0.0f;
				}
				
			
				if(ticks >= (p->myBirth + p->myLifeTime))
					p->alive = FALSE;
				p->color = ColorInt(p->color, 0x0, float(ticks - p->myBirth) / float(p->myLifeTime));
				//p->mySize = FloatInt(p->StartSize, p->EndSize, float(ticks - p->myBirth) / float(p->myLifeTime));

			}
			else
			{
				p->color = DWORD(param);
				p->myPos.x = float(mp.x);
				p->myPos.y = float(mp.y);
				p->myVel.x = dp.x * 60.0f  + rnd() * 300.0f - 150.0f;
				p->myVel.y = (-1.0f*(980.0f * rnd()));
				p->myVel.x /= 3.0f;
				p->myVel.y /= 3.0f;
				p->StartSize = 0.7f + rnd() * 0.2f;
				p->EndSize = 0.1f + rnd() * 0.5f;
				p->mySize = 10.f;
				p->myBirth = ticks;
				p->myLifeTime = 100;
				p->alive = TRUE;
			}
		}
		return;
	}

	VOID Fountain(PARTICLE* particles, INT numParts, VECTOR* basePos, INT* ret, DWORD param, HANDLE emitter)
	{
		float timeD = TimeDelta();
		DWORD ticks = GetTicks();
		*ret = numParts;
		PARTICLE* p = particles;
		for(INT i = 0; i < numParts; ++i, ++p)
		{
			if(p->alive)
			{
				p->myPos += p->myVel * (timeD);
				//p->myVel.x += 0.00001f*(ticks - p->myBirth) * (p->myVel.x < 0.0f ? -1.0f:1.0f);
				//p->myVel.z += 0.00001f*(ticks - p->myBirth) * (p->myVel.z < 0.0f ? -1.0f:1.0f);
				if(ticks >= (p->myBirth + p->myLifeTime))
					p->alive = FALSE;
				p->color = ColorInt(param, 0x00, float(ticks - p->myBirth) / float(p->myLifeTime));
				//p->mySize = FloatInt(p->StartSize, p->EndSize, float(ticks - p->myBirth) / float(p->myLifeTime));

			}
			else
			{
				//if(emitter == NULL)
				//	continue;
				if(!CanInitParticle(emitter))
				{
					p->color = 0;
					continue;
				}
				p->color = param;
				p->myPos = *basePos;
				//p->myPos.x += (rnd() - rnd())*5.0f*(float(active) / float(numParts));
				//p->myPos.z += (rnd() - rnd())*5.0f*(float(active) / float(numParts));
				//p->myPos.y = 0.2f;
				p->myVel.x = rnd() - rnd() * 3.0f;
				p->myVel.z = rnd() - rnd() * 3.0f;
				p->myVel.y = 5.7f;
				p->StartSize = 0.2f;
				p->EndSize = 0.1f;
				p->mySize = 0.1f;
				p->myBirth = ticks;
				p->myLifeTime = 15;
				p->alive = TRUE;
			}
			
		}
		return;
	}

	VOID _3DClick(PARTICLE* particles, INT numParts, VECTOR* basePos, INT* ret, DWORD param, HANDLE emitter)
	{
		float timeD = TimeDelta();
		DWORD ticks = GetTicks();
		*ret += 25;
		if(*ret > numParts)
			*ret = numParts;
		PARTICLE* p = particles;
		for(INT i = 0; i < *ret; ++i, ++p)
		{
			if(p->alive)
			{
				p->myPos += p->myVel * (timeD);
				p->myVel.y -= 5.0f * timeD;
				if(p->myPos.y < 0.2f)
					p->myVel.y *= -1 * 0.5f;
				if(ticks >= (p->myBirth + p->myLifeTime))
					p->alive = FALSE;
				p->color = ColorInt(param, 0x00, float(ticks - p->myBirth) / float(p->myLifeTime));
				//p->mySize = FloatInt(p->StartSize, p->EndSize, float(ticks - p->myBirth) / float(p->myLifeTime));

			}
			else
			{
				//if(emitter == NULL)
				//	continue;
				if(!CanInitParticle(emitter))
				{
					p->color = 0;
					continue;
				}
				p->color = param;
				p->myPos = *basePos;
				p->myPos.y += 0.2f;
				//p->myPos.x += (rnd() - rnd())*5.0f*(float(active) / float(numParts));
				//p->myPos.z += (rnd() - rnd())*5.0f*(float(active) / float(numParts));
				//p->myPos.y = 0.2f;
				p->myVel.x = rnd() * 1.0f - 0.5f;
				p->myVel.z = rnd() * 1.0f - 0.5f;
				p->myVel.y = 0.5f * rnd() + 3.5f;
				p->StartSize = 0.2f;
				p->EndSize = 0.1f;
				p->mySize = 0.1f;
				p->myBirth = ticks;
				p->myLifeTime = 25 * rnd() + 25;
				p->alive = TRUE;
			}
			
		}
		return;
	}
}

VOID WindowProc(HANDLE h, HANDLE src, UINT Message, DWORD p1, DWORD p2, DWORD p3)
{
	switch(Message)
	{
	case DM_CLICK:
		MessageBox(0, "...", "...", MB_OK);
		break;
	}
	return;
}

VOID MouseProc(INT button, INT x, INT y, BOOL down)
{
	OutputLogString("Mouse Proc...\n");
	WRLDStateMove* mState;
	WRLDStateAttack* aState;
	VECTOR click;
	VECTOR s((float)x, (float)y, 0.1f);
	VECTOR e((float)x, (float)y, 0.999f);
	PLANE p;
	BOOL isp = FALSE;
	p.a = 0.0f;
	p.b = 1.0f;
	p.c = 0.0f;
	p.d = 0.2f;
	Unproject(&s, &s);
	Unproject(&e, &e);
	if(down)
	{
		for(INT i = 0; i < UIDemoObjects::Objects.size(); ++i)
		{
			if(LineIntersectSphere(&click, &UIDemoObjects::Objects[i]->GetPos(), 1.0f, &s, &e))
			{
				if((button == 1) && (state != 2))
				{
					LineIntersectPlane(&click, &p, &s, &e);
					DeleteObj(UIDemoObjects::selPSys);
					ConstructParticleSystem(UIDemoObjects::snowEmitter, 80, &Objects[i]->GetPos(), UIDemoEffectProcs::Fall, UIDemoTextures::Snow, NULL, 0xff00ff00, TRUE, &UIDemoObjects::selPSys);
					
					DeleteObj(UIDemoObjects::firePSys2);
					UIDemoObjects::SelObj = UIDemoObjects::Objects[i];
					SetupControlWindow();
				}
				if(button == 2)
				{
					if((UIDemoObjects::SelObj == Objects[i]) || (UIDemoObjects::SelObj == NULL))
					{
						UIVisible(UIDemoObjects::MainWindow, TRUE);
						SetupControlWindow();
						if(UIDemoObjects::SelObj == NULL)
							UIDemoObjects::SelObj = Objects[i];
					}
					else
					{
						if(UIDemoObjects::SelObj)
						{
							aState = new WRLDStateAttack;
							aState->SetActor(UIDemoObjects::SelObj);
							aState->myTarget = Objects[i];
							UIDemoObjects::SelObj->AddState(aState);
							state = 0;
						}
					}
				}
				if(state == 2)
				{
					if(UIDemoObjects::SelObj != Objects[i])
					{
						if(UIDemoObjects::SelObj)
						{
							aState = new WRLDStateAttack;
							aState->SetActor(UIDemoObjects::SelObj);
							aState->myTarget = Objects[i];
							UIDemoObjects::SelObj->AddState(aState);
							state = 0;
						}
					}
				}
				isp = TRUE;
			}
		}
		if(!isp)
		{
			if(LineIntersectPlane(&click, &p, &s, &e))
			{
				if((button == 2) || (state == 1))
				{
					state = 0;
					DeleteObj(UIDemoObjects::selEmitter);
					ConstructEmitter(&UIDemoObjects::selEmitter, TRUE);
					ConstructParticleSystem(UIDemoObjects::selEmitter, 100, &click, UIDemoEffectProcs::_3DClick, UIDemoTextures::Snow,
											NULL, 0xff00ff00, TRUE, &UIDemoObjects::firePSys2);
					SetObjTimeout(UIDemoObjects::selEmitter, 50);
					if(UIDemoObjects::SelObj != NULL)
					{
						mState = new WRLDStateMove;
						mState->SetActor(UIDemoObjects::SelObj);
						UIDemoObjects::SelObj->AddState(mState);
						UIDemoObjects::SelObj->SetDest(&click);
					}
				}
			}
		}
	}
	
	return;
}

VOID DoKeys()
{
	POINT mp = GetMousePos();
	VECTOR vel;
	float timeD = TimeDelta();
	OutputLogString("Doing Keys...\n");
	if(GetKey(VK_UP))
	{
		vel = VECTOR(float(cos(float(facing))), 0.0f, float(sin(float(facing))));
		cameraPos += vel * timeD * 7.0f;
		dirty = TRUE;
	}
	if(GetKey(VK_DOWN))
	{
		vel = VECTOR(float(cos(float(facing))), 0.0f, float(sin(float(facing))));
		cameraPos -= vel * timeD * 7.0f;
		dirty = TRUE;
	}
	if(GetKey(VK_LEFT))
	{
		vel = VECTOR(float(cos(float(facing+(3.14159f  / 2.0f)))), 0.0f, float(sin(float(facing+(3.14159f  / 2.0f)))));
		cameraPos += vel * timeD * 7.0f;
		dirty = TRUE;
	}
	if(GetKey(VK_RIGHT))
	{
		vel = VECTOR(float(cos(float(facing+(3.14159f  / 2.0f)))), 0.0f, float(sin(float(facing+(3.14159f  / 2.0f)))));
		cameraPos -= vel * timeD * 7.0f;
		dirty = TRUE;
	}
	//if(GetKey(VK_DELETE))
	if(mp.x < 5)
	{
		facing += 0.1f * timeD * 7.0f;
		if(facing < 0.0f) facing += 2.0f * 3.14159f;
		dirty = TRUE;
	}
	//if(GetKey(VK_END))
	if(mp.x > 635)
	{
		facing -= 0.1f * timeD * 7.0f;
		if(facing > (2.0f * 3.14159f)) facing -= 2.0f * 3.14159f;
		dirty = TRUE;
	}
	
}

VOID KeyProc(char key, BOOL down)
{
	/*VECTOR vel;
	
	switch(key)
	{
	case VK_UP:
		vel = VECTOR(float(cos(float(facing))), 0.0f, float(sin(float(facing))));
		cameraPos += vel * timeD * 7.0f;
		break;
	case VK_DOWN:
		vel = VECTOR(float(cos(float(facing))), 0.0f, float(sin(float(facing))));
		cameraPos -= vel * timeD * 7.0f;
		break;
	case VK_LEFT:
		vel = VECTOR(float(cos(float(facing+(3.14159f  / 2.0f)))), 0.0f, float(sin(float(facing+(3.14159f  / 2.0f)))));
		cameraPos += vel * timeD * 7.0f;
		break;
	case VK_RIGHT:
		vel = VECTOR(float(cos(float(facing+(3.14159f  / 2.0f)))), 0.0f, float(sin(float(facing+(3.14159f  / 2.0f)))));
		cameraPos -= vel * timeD * 7.0f;
		break;
	case VK_DELETE:
		facing += 0.1f * timeD * 7.0f;
		if(facing < 0.0f) facing += 2.0f * 3.14159f;
		break;
	case VK_END:
		facing -= 0.1f * timeD * 7.0f;
		if(facing > (2.0f * 3.14159f)) facing -= 2.0f * 3.14159f;
		break;
	}
	*/
	return;
}



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



RECT MakeRect(INT x, INT y, INT cx, INT cy)
{
	RECT t;
	t.left = x;
	t.right = x + cx;
	t.top = y;
	t.bottom = y + cy;
	return t;
}

VOID SetupControlWindow()
{
	char* buff = new char[128];
	if(!UIDemoObjects::SelObj)
		return;
	sprintf(buff, "Hit Points: %i", UIDemoObjects::SelObj->HP);
	SetLabelText(UIDemoObjects::hpLabel, buff);
	sprintf(buff, "Speed: %f", UIDemoObjects::SelObj->GetSpeed());
	SetLabelText(UIDemoObjects::speedLabel, buff);
	delete buff;
}

RESULT UIDEMO_API InitMod()
{
	RESULT r = OK;
	DWORD cx;
	GetSetting(SET_SIZEX, &cx);
	InitGeometry();
	CreateDesktop();
	Set("AllowText", TRUE);
	SetMouseProc(MouseProc);
	char* filepath = new char[256];
	char buff[512];
	char* buff2 = new char[256];
	GetPath(&filepath);
	strcpy(buff, filepath);
	OutputLogString(buff);
	strcat(buff, "uidemo\\media\\textures\\close.bmp");
	OutputLogString(buff);
	r = LoadTexture(buff, (VOID**)&UIDemoTextures::close);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\close2.bmp");
	r = LoadTexture(buff, (VOID**)&UIDemoTextures::closeOver);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\titlebar.bmp");
	r = LoadTexture(buff, (VOID**)&UIDemoTextures::TitleTexture);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\envelopec.bmp");
	r = LoadTexture(buff, (VOID**)&UIDemoTextures::EnvCText);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\envelopeo.bmp");
	r = LoadTextureEx(buff, 0xff000000, (VOID**)&UIDemoTextures::EnvOText);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\cursor.tga");
	r = LoadTextureEx(buff, 0xff000000, (VOID**)&UIDemoTextures::CursorTexture);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\move.tga");
	r = LoadTextureEx(buff, 0xff000000, (VOID**)&UIDemoTextures::MoveButtonTexture);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\attack.tga");
	r = LoadTextureEx(buff, 0xff000000, (VOID**)&UIDemoTextures::AttackButtonTexture);
	if(RFAILED(r))
		return r;
	CreateCursor(UIDemoTextures::CursorTexture);
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\parchment.bmp");
	r = LoadTexture(buff, &UIDemoTextures::ParchBG);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\scrollbar.bmp");
	r = LoadTexture(buff, &UIDemoTextures::ScrollBarBG);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\scrolldnbttn.bmp");
	r = LoadTexture(buff, &UIDemoTextures::scrollD);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\scrollupbttn.bmp");
	r = LoadTexture(buff, &UIDemoTextures::scrollU);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\scrollthumb.bmp");
	r = LoadTexture(buff, &UIDemoTextures::scrollThumb);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\terr.tga");
	r = LoadTexture(buff, &UIDemoTextures::TerrainText);
	if(RFAILED(r))
		return r;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\textures\\particle.bmp");
	r = LoadTextureEx(buff, 0, &UIDemoTextures::Snow);
	if(RFAILED(r))
		return r;
	delete [] buff2;
	strcpy(buff, filepath);
	strcat(buff, "uidemo\\media\\models\\x.model");
	r = LoadModel(buff, &UIDemoObjects::TetMdl);
	if(RFAILED(r))
		return r;
	Objects.resize(10);
	for(int i = 0; i < 10; ++i)
	{
		Objects[i] = new WRLDActor;
		Objects[i]->SetMesh(UIDemoObjects::TetMdl);
		Objects[i]->SetPos(&VECTOR(rnd() * 25.0f, 0.0f, rnd() * 25.0f));
		Objects[i]->SetSpeed(2.0f);
		Objects[i]->HP = 100;
	}
	LoadFont("Arial", 12, 0, FALSE, FALSE, (HANDLE*)&BaseFont);
	
	r = CreateDWindow("Control Menu", UIDemoMsgProcs::MainMenuWindowProc, NULL, 50, 50, 150, 150, 0x99aaaaaa, 0xffffffff, 
					  UIDemoTextures::TitleTexture, NULL, UIDemoTextures::close, 
					  UIDemoTextures::closeOver, TRUE, _3D, TRUE, (HANDLE*)&UIDemoObjects::MainWindow);
	CreateLabel(52, 10, 100, 17, UIDemoObjects::MainWindow, "", 0xff000000, "Verdana", 12, 1, FALSE, FALSE, "None", &UIDemoObjects::hpLabel);
	CreateLabel(52, 27, 100, 17, UIDemoObjects::MainWindow, "", 0xff000000, "Verdana", 12, 1, FALSE, FALSE, "None", &UIDemoObjects::speedLabel);
	if(RFAILED(r))
		return r;
	//r = CreateButton("Getting\nStarted", UIDemoObjects::MainWindow, UIDemoMsgProcs::GettingStartedProc, 14, 50, 75, 40, 
	//	UIDemoTextures::TitleTexture, 0xffffffff, 0xff000000, "Verdana", 16, 1, FALSE, FALSE, _3D, &UIDemoObjects::GettingStarted);  
	if(RFAILED(r))
		return r;
	r = CreateDWindow("Status Bar", NULL, NULL, 0, 0, cx, 17, 0xffeeeeee, 0, NULL, UIDemoTextures::TitleTexture,
		NULL, NULL, FALSE, None, FALSE, &UIDemoObjects::StatusWindow);
	r = CreateDWindow("Move Button", UIDemoMsgProcs::MoveWindowProc, UIDemoObjects::MainWindow, 10, 10, 32, 32, 0xffffffff, 0, NULL, UIDemoTextures::MoveButtonTexture,
		NULL, NULL, FALSE, None, FALSE, &UIDemoObjects::moveButton);
	r = CreateDWindow("Attack Button", UIDemoMsgProcs::AttackWindowProc, UIDemoObjects::MainWindow, 10, 52, 32, 32, 0xffffffff, 0, NULL, UIDemoTextures::AttackButtonTexture,
		NULL, NULL, FALSE, None, FALSE, &UIDemoObjects::attackButton);
	UIVisible(UIDemoObjects::MainWindow, TRUE);
	UIVisible(UIDemoObjects::StatusWindow, TRUE);
	CreateLabel(10, 2, cx, 17, UIDemoObjects::StatusWindow, "Welcome to the Times of War GUI Demo!", 0xff000000, "Verdana", 14, 1, TRUE, FALSE, "TOWLabel", &UIDemoObjects::titleLabel);
	UIDemoObjects::map = new TmpMap;
	UIDemoObjects::map->Create(20, 20, 1.0f);
	//UIDemoObjects::grass = new WRLDGrass;
	//UIDemoObjects::grass->fac = &facing;
	//UIDemoObjects::grass->v = &cameraPos;
	//UIDemoObjects::grass->Create();
	r = ConstructEmitter(&UIDemoObjects::snowEmitter, TRUE);
	if(RFAILED(r))
		return r;
	r = ConstructEmitter(&UIDemoObjects::mouseEmitter, FALSE);
	if(RFAILED(r))
		return r;
	r = ConstructEmitter(&UIDemoObjects::fireEmitter, TRUE);
	if(RFAILED(r))
		return r;
	//SetObjTimeout(UIDemoObjects::fireEmitter, 0);
	r = ConstructParticleSystem(UIDemoObjects::snowEmitter, 2000, &VECTOR(5.0f, 0.0f, 5.0f), UIDemoEffectProcs::Snowy, UIDemoTextures::Snow, NULL, 0, TRUE, &UIDemoObjects::snowPSys);
	if(RFAILED(r))
		return r;

	r = ConstructParticleSystem(UIDemoObjects::fireEmitter, 150, &VECTOR(5.0f, 0.0f, 5.0f), UIDemoEffectProcs::Fire, UIDemoTextures::Snow, NULL, 0, TRUE, &UIDemoObjects::firePSys);
	if(RFAILED(r))
		return r;
	r = ConstructParticleSystem(UIDemoObjects::mouseEmitter, 550, &VECTOR(5.0f, 0.0f, 5.0f), UIDemoEffectProcs::Mouse, UIDemoTextures::Snow, NULL, 0xff0000ff, FALSE, &UIDemoObjects::mousePSys);
	if(RFAILED(r))
		return r;

	
	SetRenderState(RS_FOGCOLOR, 0xff000000);
	SetRenderState(RS_FOGENABLE, FALSE);
	SetRenderState(RS_FOGSTART, FtoDW(1.0f));
	SetRenderState(RS_FOGEND, FtoDW(20.0f));
	SetRenderState(RS_CULLMODE, CULL_NONE);
	SetRenderState(RS_AMBIENT, 0xffffffff);
	MatrixProjection(20.0f, 4 / 3.0f);
	SetProjection();
	MatrixLookAt(&VECTOR(1.0f, 1.0f, 1.0f), &VECTOR(5.0f, 0.0f, 5.0f));
	SetView();
	Set("Paused", FALSE);
	ShowCursor(FALSE);
	return OK;
}


RESULT UIDEMO_API RenderScene()
{
	static DWORD numPrimsLast;
	static DWORD LastTime = 0;
	DWORD start, end = 0;
	float fps;
	char* fpstext = NULL;
	DWORD cx, cy;
	GetSetting(SET_SIZEX, &cx);
	GetSetting(SET_SIZEY, &cy);
	fpstext = new char[128];
	RECT rect;
	rect.top = cy - 40;
	rect.left = cx - 100;
	rect.bottom = cy;
	rect.right = cx;
	
	start = GetTickCount();
	Clear(0);
	BeginScene();
		
		
	
		
		SelectObject(UIDemoTextures::TerrainText);
		//UIDemoObjects::map->Render(end);
		//numPrimsLast+=end; 
		RenderObject(UIDemoObjects::mousePSys);
		//RenderObject(UIDemoObjects::snowPSys);
		//RenderObject(UIDemoObjects::firePSys);

		//for(int i =0; i < Objects.size(); ++i)
		{
		//	Objects[i]->Render();
		}

		//RenderObject(UIDemoObjects::MainWindow);
		//RenderObject(UIDemoObjects::StatusWindow);
		if(state != 0)
			RenderObject(UIDemoObjects::mousePSys);
		DrawCursor();
		//SelectObject(UIDemoTextures::TerrainBlend);
		//SetRenderState(RS_SRCBLEND, BLEND_SRCALPHA);
		//SetRenderState(RS_DESTBLEND, BLEND_INVSRCCOLOR);
		//SetRenderState(RS_ALPHABLENDENABLE, TRUE);
		//end =0;
		//UIDemoObjects::map->Render(end);
		//SetRenderState(RS_SRCBLEND,  BLEND_SRCALPHA);
		//SetRenderState(RS_DESTBLEND, BLEND_ONE);
		//numPrimsLast+=end;
		
		fps = 1000.0f / (LastTime == 0 ? 1 : LastTime);
		sprintf(fpstext, "Divinity @ %f\n%i Primitives Drawn", fps, numPrimsLast);
		
		SelectFont(BaseFont);
		//Set("AllowText", TRUE);
		UIBeginDraw();
			DrawTextOut(fpstext, rect, 0, 0xffffffff);
		UIEndDraw();
		//Set("AllowText", FALSE);
	EndScene();
	
	Flip();
	end = GetTickCount();
	numPrimsLast = GetDrawnPrimitives();
	LastTime = end - start;
	delete fpstext;
	return OK;
}

RESULT UIDEMO_API UpdateScene()
{
	DoKeys();
	//MatrixLookAt(&cameraPos, &VECTOR(cameraPos.x + ((float)cos(float(facing)) * cameraPos.y), 0.0f, cameraPos.z + ((float)sin(float(facing)) * cameraPos.y)));
	SetView();
	UpdateObjects();
	for(int i =0; i < Objects.size(); ++i)
	{
		Objects[i]->Update();
		if(Objects[i]->HP <= 0)
		{
			if(UIDemoObjects::SelObj == Objects[i])
			{
				delete Objects[i];
				Objects[i] = new WRLDActor;
				UIDemoObjects::SelObj = Objects[i];
			}
			else
			{
				delete Objects[i];
				Objects[i] = new WRLDActor;
			}
			Objects[i]->SetMesh(UIDemoObjects::TetMdl);
			Objects[i]->SetPos(&VECTOR(rnd() * 25.0f, 0.0f, rnd() * 25.0f));
			Objects[i]->SetSpeed(2.0f);
			Objects[i]->HP = 100;
		}
	}
	SetupControlWindow();
	return OK;
}

RESULT UIDEMO_API DestroyMod()
{
	delete UIDemoObjects::map;
	for(int i =0; i < Objects.size(); ++i)
	{
		delete Objects[i];
	}
	return OK;
}

RESULT UIDEMO_API Splash(char* filename, SIZE* s)
{
	strcpy(filename, "media\\tow.bmp");
	s->cx = 441;
	s->cy = 108;
	return OK;
}
