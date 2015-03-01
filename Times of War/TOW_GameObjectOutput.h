#ifndef TOW_GAMEOBJECTOUTPUT_H
#define TOW_GAMEOBJECTOUTPUT_H

#pragma warning (disable : 4786)

#include "TOW_GameOutput.h"
#include "TOW_GameObject.h"
#include "..\DivGraph\DivGraph.h"
#include "IHeightMap.h"
#include "..\DivGraph\IMesh.h"

class TOW_GameObjectOutput : public TOW_GameOutput
{
public:
	TOW_GameObjectOutput();
	TOW_GameObjectOutput(TOW_GameObject* go);

	void SetGameObject(TOW_GameObject* go);
	TOW_GameObject* GetGameObject();

	RESULT Render();
	static void SetHeightMap(IHeightMap* heightMap);

private:
	TOW_GameObject* m_go;
	static IHeightMap* m_hmp;
};

#endif