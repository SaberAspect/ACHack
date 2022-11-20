#pragma once
#include "glDraw.h"
#include "offsets.h"

const int VIRTUAL_SCREEN_WIDTH = 800;
const int GAME_UNIT_MAGIC = 400;

const float PLAYER_HEIGHT = 5.25f;
const float PLAYER_WIDTH = 2.0f;
const float EYE_HEIGHT = 4.5f;

const float PLAYER_ASPECT_RATIO = PLAYER_HEIGHT / PLAYER_WIDTH;

class ESP
{
public:
	float* matrix = (float*)(0x501ae8);
	int* gameMode = (int*)0x50f49c;

	int viewport[4];

	void DrawESPBox(entPlayer* p, entPlayer* e, Vector3);
	void Draw();
};

