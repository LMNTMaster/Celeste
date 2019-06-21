#pragma once

#include "Macro.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Flag.h"


namespace std
{
	template<>
	struct hash<LMNT::_tagVector2>
	{
		size_t operator()(const LMNT::_tagVector2	& key) const
		{
			return hash_value<float>(key.x + key.y);
		}
	};


	template<>
	struct hash<LMNT::_tagVector3>
	{
		size_t operator()(const LMNT::_tagVector3	& key) const
		{
			return hash_value<float>(key.x + key.y + key.z);
		}
	};

}

LMNT_BEGIN



typedef struct LMNT_DLL _tagAtlasData
{
	char pDirectory[512] = {};
	unsigned short wX = 0;
	unsigned short wY = 0;
	unsigned short wHeight = 0;
	unsigned short wWidth = 0;
	int iAtlasSizeX = 0;
	int iAtlasSizeY = 0;
}ATLASDATA, *PATLASDATA;

//Shader
typedef struct LMNT_DLL _tagVertexColor
{
	Vector3 vertexPosition;
	Vector4 vertexColor;

	_tagVertexColor()
	{}

	//Copy Constructor
	_tagVertexColor(const _tagVertexColor& vtx)
	{}

	//Init
	_tagVertexColor(float x, float y, float z, float r, float g, float b, float a) :
		vertexPosition(x, y, z),
		vertexColor(r, g, b, a)		
	{}
	//

}VERTEXCOLOR, *PVERTEXCOLOR;

// Vertex Tex
typedef struct LMNT_DLL _tagVertexTex
{
	Vector3	vPos;
	Vector2	vUV;

	_tagVertexTex()
	{
	}

	_tagVertexTex(const _tagVertexTex& vtx) :
		vPos(vtx.vPos),
		vUV(vtx.vUV)
	{
	}

	_tagVertexTex(float x, float y, float z, float u, float v) :
		vPos(x, y, z),
		vUV(u, v)
	{
	}
}VERTEXTEX, *PVERTEXTEX;


// 상수 버퍼 사용시 주의사항 : 반드시 메모리 사이즈를 16바이트 기준으로
// 맞춰서 사용해야 한다. 예를 들어 24바이트 일 경우 반드시 8바이트를
// 더 선언해서 맞춰주어야 한다.
typedef struct LMNT_DLL _tagTransformConstantBuffer
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProjection;
	Matrix	matWV;
	Matrix	matWVP;
	Matrix	matVP;
	Matrix   matWP;
	Vector3 vPivot;
	float		fEmpty1;
	Vector3 vLength;
	float		fEmpty2;
	
}TRANSFORMCBUFFER, *PTRANSFORMCBUFFER;

typedef struct LMNT_DLL _tagColliderConstantBuffer
{
	int bUI;
	Vector3 vEmpty;
	Vector4 vColor;
}COLLIDERCBUFFER, *PCOLLIDERCBUFFER;

typedef struct LMNT_DLL _tagMaterial
{
	Vector4		vDiffuse;
}MATERIAL, *PMATERIAL;

// Animation 2D Constant Buffer
typedef struct LMNT_DLL _tagAnimation2DCBuffer
{
	int		iType;
	int		iFrameX;
	int		iFrameY;
	int		iStartX;
	int		iStartY;
	int		iLengthX;
	int		iLengthY;
	int		iMaxX;
	int		iMaxY;
	Vector3	vEmpty;
}ANIMATION2DCBUFFER, *PANIMATION2DCBUFFER;

// Custom Animation Info
typedef struct LMNT_DLL _tagCustomAnimationInfo
{
	int		iStartX;
	int		iStartY;
	int		iSizeX;
	int		iSizeY;
	int		iMaxSizeX;
	int		iMaxSizeY;
}CUSTOMANIMATIONINFO, *PCUSTOMANIMATIONINFO;


// Renderer Constant Buffer
typedef struct LMNT_DLL _tagRendererCBuffer
{
	int		iTransfer;
	int		iSize;
	void*	pData;
}RENDERERCBUFFER, *PRENDERERCBUFFER;












LMNT_END