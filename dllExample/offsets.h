#pragma once
#include "Vector3.h"

typedef int int32_t;

class entPlayer
{
public:
	int32_t vTable; //0x0000
	Vector3 vHead; //0x0004
	Vector3 vVel1; //0x0010
	Vector3 unknown; //0x001C
	Vector3 vVel2; //0x0028
	Vector3 vFoot; //0x0034
	Vector3 vAngles; //0x0040
	char pad_004C[172]; //0x004C
	int32_t iHealth; //0x00F8
	int32_t iArmor; //0x00FC
	char pad_0100[220]; //0x0100
	int32_t N000000AB; //0x01DC
	char pad_01E0[4]; //0x01E0
	int32_t ITeamId; //0x01E4
	char pad_01E8[20]; //0x01E8
	int32_t iFrags; //0x01FC
	char pad_0200[4]; //0x0200
	int32_t iDeaths; //0x0204
	char pad_0208[4]; //0x0208
	int32_t N000000B7; //0x020C
	char pad_0210[20]; //0x0210
	bool bShooting; //0x0224
	char sName[16]; //0x0225
	char pad_0235[247]; //0x0235
	int32_t iTeamNum; //0x032C
	char pad_0330[68]; //0x0330
	class Weapon* currWeapon; //0x0374
}; //Size: 0x0378
static_assert(sizeof(entPlayer) == 0x378);


class Weapon
{
public:
	char pad_0000[4]; //0x0000
	int32_t weaponId; //0x0004
	class player* WeaponHolder; //0x0008
	class wName* weaponName; //0x000C
	class Reserve* currReserve; //0x0010
	class Ammo* currAmmo; //0x0014
	class Delay* currShotDelay; //0x0018
	char pad_001C[40]; //0x001C
}; //Size: 0x0044
static_assert(sizeof(Weapon) == 0x44);

class Reserve
{
public:
	int32_t Amount; //0x0000
}; //Size: 0x0004
static_assert(sizeof(Reserve) == 0x4);

class Ammo
{
public:
	int32_t Amount; //0x0000
}; //Size: 0x0004
static_assert(sizeof(Ammo) == 0x4);

class Delay
{
public:
	int32_t Amount; //0x0000
	char pad_0004[120]; //0x0004
}; //Size: 0x007C
static_assert(sizeof(Delay) == 0x7C);

class player
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(player) == 0x44);

class wName
{
public:
	char name[16]; //0x0000
}; //Size: 0x0010
static_assert(sizeof(wName) == 0x10);

class EntityList
{
public:
	char pad_0000[4]; //0x0000
	class entPlayer* EntArray[32]; //0x0004
}; //Size: 0x0084
static_assert(sizeof(EntityList) == 0x84);

class numPlayers
{
public:
	int32_t players; //0x0000
}; //Size: 0x0004
static_assert(sizeof(numPlayers) == 0x4);

class currGameMode
{
public:
	int32_t mode; //0x0000
}; //Size: 0x0004
static_assert(sizeof(currGameMode) == 0x4);

//Trace result struct def
struct traceresult_s
{
	Vector3 end;
	bool collided;
};