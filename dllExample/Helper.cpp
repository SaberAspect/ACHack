#include "Helper.h"
#include <Windows.h>
#include <math.h>
#include <iostream>
#include "Vector3.h"

#define PI 3.1415826835

namespace Helpers {

    entPlayer* GetLocalPlayer()
    {
        return (entPlayer*)*(uintptr_t*)0x50f4f4;
    }
    numPlayers* GetNumPlayers()
    {
        return (numPlayers*)(uintptr_t*)0x510d98;
    }
    EntityList* GetEntityList()
    {
        return (EntityList*)*(uintptr_t*)0x50f4f8;
    }
    currGameMode* GetCurrGameMode()
    {
        return (currGameMode*)(uintptr_t*)0x50f49c;
    }
    bool isEntAlive(entPlayer* ent)
    {
        if (ent == nullptr || ent->iHealth < 0)
            return false;
        else
            return true;
    }
    bool isOnTeam(entPlayer* src, entPlayer* dst)
    {
        if (src->iTeamNum == dst->iTeamNum)
            return true;
        else
            return false;
    }
    entPlayer* getCloseestPlayer(entPlayer* origin)
    {
        double closestDistance = 99999999;
        int index = -1;
        numPlayers* playersInGame = GetNumPlayers();
        EntityList* entList = GetEntityList();
        entPlayer* closest;
        //Loop throught ent array
        if (playersInGame == nullptr || entList == nullptr || playersInGame->players == 0)
        {
            std::cout << "NO PLAYERS IN GAME, PLEASE JOIN A GAME\n";
            return NULL;
        }
        for (int i = 0; i < playersInGame->players; ++i)
        {

            closest = entList->EntArray[i];
            //is dead
            if (isEntAlive(closest) && isEntAlive(origin) && isValidEntity(closest))
                //If it is a team game mode we make sure that the current closest enemy is not a teamate
                if (isTeamGamemode()) {
                    if (!isOnTeam(closest, origin)) {
                        //if the current "closest" is larger than the next entity in the index, equal the closest to the current index
                        //Get distance funciton
                        if (Vector3::DistanceVec3(origin->vHead, closest->vHead) < closestDistance && isVisable(origin, closest))
                        {
                            closestDistance = Vector3::DistanceVec3(origin->vHead, closest->vHead);
                            index = i;
                        }
                    }
                }
            //If it is not a team game mode, than we just calculate for each player in the array
                else
                {
                    if (Vector3::DistanceVec3(origin->vHead, closest->vHead) < closestDistance && isVisable(origin, closest))
                    {
                        closestDistance = Vector3::DistanceVec3(origin->vHead, closest->vHead);
                        index = i;
                    }
                }
        }
        if (index >= 0) {
            if (closestDistance > 999) {
                std::cout << entList->EntArray[index]->sName << " is too far away\n";
                return NULL;
            }
            return entList->EntArray[index];
        }
    }
    bool isVisable(entPlayer* src, entPlayer* dst)
    {
        //Traceline function addr
        uintptr_t traceLine = 0x48a310;
        traceresult_s traceresult;
        //Setting collided to false
        traceresult.collided = false;
        //Our head position
        Vector3 from = src->vHead;
        //The ent we want to check
        Vector3 to = dst->vHead;

        __asm
        {
            push 0; pushing 0 onto the stack
            push 0; pushing 0 onto the stack
            push src; pushing our player ptr onto the stack
            push to.z; pushing coords
            push to.y
            push to.x
            push from.z; pushing coords
            push from.y
            push from.x
            lea eax, [traceresult]; loading the traceresult into the eax register
            call traceLine; calling the traceline function
            add esp, 36; cleaning up the stack
        }
        //we return weather or not we colided
        return !traceresult.collided;
    }
    bool isValidEntity(entPlayer* ent)
    {
        if (ent)
        {
            if (ent->vTable == 0x4E4A98 || ent->vTable == 0x4E4AC0)
            {
                return true;
            }
        }
        return false;
    }
    bool isTeamGamemode()
    {
        currGameMode* currGame = GetCurrGameMode();
        int gameMode = currGame->mode;

        return (gameMode == 0 || gameMode == 4 || gameMode == 5 || gameMode == 7 || gameMode == 11 || gameMode == 13 || gameMode == 14 || gameMode == 16 || gameMode == 17 || gameMode == 20 || gameMode == 21);
    }
    bool isKeyPressed(unsigned int key)
    {
        return GetAsyncKeyState(key) & 0x8000;

    }
    void Patch(BYTE* dst, BYTE* src, unsigned int size)
    {
        DWORD oldprotect;
        VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

        memcpy(dst, src, size);
        VirtualProtect(dst, size, oldprotect, &oldprotect);
    }
    void NOP(BYTE* dst, unsigned int size)
    {
        DWORD oldProc;
        VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProc);
        memset(dst, 0x90, size);
        VirtualProtect(dst, size, oldProc, &oldProc);

    }
    bool WorldToScreen(Vector3 pos, Vector3& screen, float matrix[16], int windowWidth, int windowHeight)
    {
        //Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
        Vector4 clipCoords;
        clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
        clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
        clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
        clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

        if (clipCoords.w < 0.1f)
            return false;

        //Perspective division, dividing by clip.W = Normalized Device Coordinates
        Vector3 NDC;
        NDC.x = clipCoords.x / clipCoords.w;
        NDC.y = clipCoords.y / clipCoords.w;
        NDC.z = clipCoords.z / clipCoords.w;

        //Transform to window coordinates
        screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
        screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
        return true;
    }
    Vector3 CalcAngles(Vector3 src, Vector3 dst)
    {
        Vector3 angles;
        Vector3 origin;

        origin = dst - src;
        float originLength = sqrt(pow(origin.x, 2) + pow(origin.y, 2) + pow(origin.z, 2));

        angles.x = -atan2(origin.x, origin.y) * (180 / PI) + 180; //0 to 360
        angles.y = asin(origin.z / originLength) * (180 / PI); //90  to -90
        angles.z = 0;

        return angles;

    }
    void aimAt(entPlayer* src, bool smooth, int smoothAmount)
    {
        entPlayer* closestEnt = getCloseestPlayer(src);
        if (isValidEntity(closestEnt))
        {
            Vector3 rotationAngles = CalcAngles(src->vHead, closestEnt->vHead);
            if (isVisable(src, closestEnt))
            {
                if (smooth)
                {
                    Vector3 angleDifference = rotationAngles - src->vAngles;
                    src->vAngles.x += angleDifference.x / smoothAmount;
                    src->vAngles.x += angleDifference.y / smoothAmount;
                }
                else
                {
                    Vector3 buff = rotationAngles;

                    rotationAngles.Normalize();

                    Vector3 newAngles = buff - rotationAngles;

                    src->vAngles.x = newAngles.x;
                    src->vAngles.y = newAngles.y;
                }
            }
        }
    }
    bool DisplayToggleHack(GL::Font font, int x, int y, const char* hack, bool state)
    {
        if (state) {
            font.Print(x, y, rgb::green, "%s", hack);

        }
        else {
            font.Print(x, y, rgb::red, "%s", hack);
        }
        return state;
    }

}