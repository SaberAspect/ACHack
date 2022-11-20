#include "ESP.h"
#include "Helper.h"
#include <iostream>
#include <string>

void ESP::DrawESPBox(entPlayer* p, entPlayer* e, Vector3 screen)
{
	const GLubyte* color = nullptr;
	if (Helpers::isTeamGamemode() && Helpers::isOnTeam(p, e))
	{
		color = rgb::green;
	}
	else color = rgb::red;

	float dist = p->vHead.DistanceVec3(p->vHead, e->vHead);
	float scale = (GAME_UNIT_MAGIC / dist) * (viewport[2] / VIRTUAL_SCREEN_WIDTH);
	float x = screen.x - scale;
	float y = screen.y - scale * PLAYER_ASPECT_RATIO;
	float width = scale * 2;
	float height = scale * PLAYER_ASPECT_RATIO * 2;

	GL::DrawOutline(x, y, width, height, 2.0f, color);
}
void ESP::Draw()
{
	EntityList* list = Helpers::GetEntityList();
	entPlayer* local = Helpers::GetLocalPlayer();
	numPlayers* numPlayers = Helpers::GetNumPlayers();

	glGetIntegerv(GL_VIEWPORT, viewport);

	for (int i = 0; i < numPlayers->players; i++)
	{
		//Validation checking
		if (Helpers::isValidEntity(list->EntArray[i]))
		{
			entPlayer* e = list->EntArray[i];
			//Making sure that we are not rendering for entites that dont exist or that is not in range
			if (Helpers::isEntAlive(e) && e->vHead.DistanceVec3(e->vHead, local->vHead) < 100) {
				Vector3 center = e->vHead;
				center.z = center.z - EYE_HEIGHT + PLAYER_HEIGHT / 2;

				Vector3 screenCoords;
				if (Helpers::WorldToScreen(center, screenCoords, matrix, viewport[2], viewport[3]))
				{
					DrawESPBox(local, e, screenCoords);
				}
			}
		}
	}
}