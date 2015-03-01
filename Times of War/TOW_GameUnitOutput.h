#ifndef TOW_GAMEUNITOUTPUT_H
#define TOW_GAMEUNITOUTPUT_H

#include "TOW_GameUnit.h"
#include "TOW_GameOutput.h"
#include "..\DivGraph\DivGraph.h"
#include "..\Terrain\IHeightMap.h"

class TOW_GameUnitOutput : public TOW_GameOutput
{
public:
	TOW_GameUnitOutput();
	TOW_GameUnitOutput(TOW_GameUnit* gu);

	void SetGameUnit(TOW_GameUnit* gu);
	TOW_GameUnit* GetGameUnit();

	RESULT Render();
	static void SetHeightMap(IHeightMap* heightMap);

private:
	TOW_GameUnit* m_gu;
	static IHeightMap* m_hmp;
};

#endif