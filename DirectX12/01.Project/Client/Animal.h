#pragma once

//#define MAX_WARLIKE_COUNT 20
//#define MAX_PASSIVE_COUNT 40
//#define MAX_EVASION_COUNT 40

#define WARLIKE_RESPAWN_TIME 60.f
#define PASSIVE_RESPAWN_TIME 50.f
#define EVASION_RESPAWN_TIME 40.f

#define WARLIKE_SPAWN_RADIUS 2000.f
#define PASSIVE_SPAWN_RADIUS 2000.f
#define EVASION_SPAWN_RADIUS 8000.f

enum ANIMAL_TYPE
{
	A_BEAR,
	A_BOAR,
	A_DEER,
	A_WOLF,
	A_END
};

enum BEHAVIOR_TYPE
{
	B_WARLIKE,
	B_PASSIVE,
	B_EVASION,
	B_END
};

struct tAnimalStatus
{
	float fHp;
	float fStamina;

	float fSpeed;
	float fDamage;

	float fBehaviorTime;

	BEHAVIOR_TYPE eType;
	ANIMAL_TYPE eKind;

	UINT uiAnimalId;
};