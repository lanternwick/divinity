#ifndef _ADAPTERUITOWORLD_H
#define _ADAPTERUITOWORLD_H

class AdapterUItoWorld
{
	AdapterUItoWorld();
	~AdapterUItoWorld();

	RESULT Click(int x, int y, int button);
	RESULT MouseDown(int x, int y, int button);
	RESULT MouseUp(int x, int y, int button);

	RESULT AttachObject(HANDLE Obj);