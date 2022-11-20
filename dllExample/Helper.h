#pragma once
#include "offsets.h"
#include "glDraw.h"
#include "Drawing.h"
#include <Windows.h>

namespace Helpers {
	entPlayer* GetLocalPlayer();
	numPlayers* GetNumPlayers();
	EntityList* GetEntityList();
	bool isEntAlive(entPlayer* ent);
	bool isOnTeam(entPlayer* src, entPlayer* dst);
	bool isVisable(entPlayer* src, entPlayer* dst);
	entPlayer* getCloseestPlayer(entPlayer* origin);
	bool isValidEntity(entPlayer* ent);
	bool isTeamGamemode();
	bool isKeyPressed(unsigned int key);
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void NOP(BYTE* dst, unsigned int size);
	bool WorldToScreen(Vector3 pos, Vector3& screen, float matrix[16], int windowWidth, int windowHeight);
	Vector3 CalcAngles(Vector3 src, Vector3 dst);
	void aimAt(entPlayer* src, bool smooth, int smoothAmount);
	bool DisplayToggleHack(GL::Font font, int x, int y, const char* hack, bool state);

}

