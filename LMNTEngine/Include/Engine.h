#pragma once

#include <Windows.h>
#include <string>
#include <string.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <assert.h>
#include <crtdbg.h>
#include <conio.h>
#include <time.h>
#include <random>
#include <functional>
#include <algorithm>
#include <queue>
#include <process.h>

//Font
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace std;

#include "Types.h"


LMNT_BEGIN

// Path Key
#define	ROOT_PATH	"RootPath"
#define	SHADER_PATH	"ShaderPath"
#define	TEXTURE_PATH	"TexturePath"
#define DATA_PATH "DataPath"
#define PREFAB_PATH "PrefabPath"
#define SOUND_PATH "SoundPath"
#define BGM_PATH "SoundBGMPath"
#define SFX_PATH "SoundSFXPath"
#define UI_SFX_PATH "UISoundSFXPath"

// Shader Key
#define	STANDARD_COLOR_SHADER	"StandardColorShader"
#define	STANDARD_TEXTURE_SHADER		"StandardTexShader"
#define COLORED_TEXTURE_SHADER "ColoredTexShader"
#define	COLLIDER_SHADER			"ColliderShader"
#define UI_TEXTURE_SHADER		"UIShader"
#define UI_COLLIDER_SHADER		"UIColliderShader"
#define UI_GRID_SHADER				"UIGridShader"


// InputLayout Key
#define	POS_COLOR_LAYOUT		"PosColorLayout"
#define	POS_UV_LAYOUT			"PosUVLayout"

//Constant Buffer Key
#define TRANSFORM_CBUFFER "TransformConstantBuffer"
#define COLLIDER_CBUFFER		"ColliderConstantBuffer"
#define	MATERIAL_CBUFFER		"MaterialConstantBuffer"
#define	ANIMATION2D_CBUFFER		"Animation2DCBuffer"
#define	GRID_CBUFFER		"GridCBuffer"

// Sampler State Key
#define	SAMPLER_LINEAR			"SamplerLinear"
#define SAMPLER_POINT			"SamplerPoint"
#define SAMPLER_POINT_LOOP	"SamplerPointLoop"

// Default Engine Mesh Key
#define ENGINE_MESH_CUBE_COLOR				"EngineCubeColorMesh"
#define	ENGINE_MESH_TRIANGLE_COLOR		"EngineTriangleColorMesh"
#define	ENGINE_MESH_RECTANGLE_COLOR		"EngineRectColorMesh"
#define	ENGINE_MESH_CIRCLE_COLOR	"EngineStripRectColorMesh"

#define	ENGINE_MESH_ORTHO_TRIANGLE_COLOR		"EngineOrthoTriangleColorMesh"
#define	ENGINE_MESH_ORTHO_RECTANGLE_COLOR	"EngineOrthoRectColorMesh"
#define	ENGINE_MESH_ORTHO_CIRCLE_COLOR			"EngineOrthoStripRectColorMesh"

#define	ENGINE_MESH_ORTHORECT_TEX	"EngineOrthoRectTexMesh"
#define	ENGINE_MESH_RECTANGLE_TEX	"EngineRectTexMesh"
#define ENGINE_MESH_MOUNTAIN_WALL_TEX "EngineMountainWallTexMesh"
#define ENGINE_MESH_MOUNTAIN_TEX "EngineMountainTexMesh"
#define ENGINE_MESH_CUBE_TEX		"EngineCubeTexMesh"

//Debug Mesh
#define ENGINE_MESH_FRAME_RECT_COLOR		"EngineFrameRectColorMesh"
#define ENGINE_MESH_FRAME_CIRCLE_COLOR		"EngineFrameCircleColorMesh"


// Render State Key
#define	ALPHA_BLEND		"AlphaBlend"
#define	ALPHA_BLEND_DISABLE		"AlphaBlendDisable"
#define	DEPTH_DISABLE		"DepthDisable"
#define	DEPTH_ENABLE		"DepthEnable"
#define	CULLNONE				"CullNone"
#define	CULLBACK				"CullBack"
#define	WIREFRAME			"WireFrame"



//Math
#define PI		  3.14159265358979f
#define D2R	  0.01745329251994f
#define R2D	57.29577951308231f


//Extra Input
#define MOUSE_WHEEL_UP		300
#define MOUSE_WHEEL_DOWN	301


static float Lerp(float fBegin, float fEnd, float fRate)
{
	return (1 - fRate) * fBegin + fRate * fEnd;	
}
static float Slerp(float fBegin, float fEnd, float fRate)
{
	//??
}

static float RandomBetweenZeroToOne()
{
	int i = rand() % 10001;
	return (float)i * 0.0001f;
}

static float RandomRange(float fMin, float fMax)
{
	float fRange = fMax - fMin;
	float fValue = RandomBetweenZeroToOne() * fRange;
	fValue += fMin;

	return fValue;
}


template <typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator	iter;
	T::iterator	iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	p.clear();
}

template <typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator	iter;
	T::iterator	iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	p.clear();
}

template <typename T>
void Safe_Release_VecList(T& p)
{
	T::iterator	iter;
	T::iterator	iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}

	p.clear();
}

template <typename T>
void Safe_Release_Map(T& p)
{
	T::iterator	iter;
	T::iterator	iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}

	p.clear();
}

typedef struct _tagResolution
{
	unsigned int	iWidth;
	unsigned int	iHeight;

	_tagResolution() :
		iWidth(0),
		iHeight(0)
	{
	}

	_tagResolution(const _tagResolution& res)
	{
		*this = res;
	}

	_tagResolution(unsigned int iW, unsigned int iH) :
		iWidth(iW),
		iHeight(iH)
	{
	}
}RESOLUTION, *PRESOLUTION;


LMNT_END
