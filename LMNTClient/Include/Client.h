
#pragma once

#include "Engine.h"
#include "GameObject.h"

LMNT_USING

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment(lib, "Engine64_Debug")
	#else
		#pragma comment(lib, "Engine64")
#endif // _DEBUG
#else
	#ifdef _DEBUG
		#pragma comment(lib, "Engine_Debug")
	#else
		#pragma comment(lib, "Engine")
	#endif // _DEBUG
#endif // 

#define CELESTE_PIXEL 4
#define CELESTE_BLOCK 32
#define MADLINE_HAIR_COLOR_NORMAL Vector4(172.0f/255.0f, 50.0f / 255.0f, 50.0f/ 255.0f, 1.0f)
#define MADLINE_HAIR_COLOR_FLYPOWER Vector4(242.0f/255.0f, 235.0f / 255.0f, 109.0f/ 255.0f, 1.0f)
#define MADLINE_HAIR_COLOR_SINGLE_DASH Vector4(68.0f/255.0f, 183.0f/255.0f, 1.0f, 1.0f)
#define MADLINE_HAIR_COLOR_DOUBLE_DASH Vector4(1.0f, 109.0f / 255.0f, 239.0f/ 255.0f, 1.0f)


enum MOVING_BLOCK_STATE
{
	MBS_IDLE,
	MBS_MOVE,
	MBS_ARRIVE,
	MBS_RETURN,
};

enum MOVING_BLOCK_TYPE
{
	MBT_TOUCH = 0x0001,
	MBT_DASH = 0x0002,
};

enum BERRY_TYPE
{
	BT_NORMAL,
	BT_WING,
	BT_GOLDEN,
	BT_GOLDEN_WING,
	BT_GHOST_NORMAL,
	BT_GHOST_WING,
	BT_GHOST_GOLDEN_NORMAL,
	BT_GHOST_GOLDEN_WING
};

enum CLIENT_COMPONENT_TYPE
{
	CT_FOLLOW_CAMERA = LMNT::CT_END,
	CT_PLAYER,
	CT_PARTICLEMOVE,
	CT_TITLE_SCENE_MANAGER,
	CT_PARTICLE_MANAGER,
	CT_HAIR,
	CT_CRUMBLE,
	CT_CAMERA_MOVE,
	CT_BIRD,
	CT_FALL_BLOCK
};

enum SCENE_AFTER_EFFECT
{
	AE_DESTROY,
	AE_STOP,
	AE_RESET,
	AE_LOAD
};


typedef struct _tagArea
{
	bool bCleared ;
	Vector4 vRange;
	vector<Vector3> vecCheckPoint;
	vector<CGameObject*> vecObjects;

	_tagArea() : bCleared(false)
	{
	}

}AREA, *PAREA;