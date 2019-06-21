#include "ResourcesManager.h"
#include "Mesh.h"
#include "Core.h"
#include "Sampler.h"
#include "Texture.h"
#include "PathManager.h"

LMNT_USING

DEFINITION_SINGLE(CResourcesManager)

CResourcesManager::CResourcesManager()
{
}


CResourcesManager::~CResourcesManager()
{
	Safe_Release_Map(m_mapMesh);
	Safe_Release_Map(m_mapTexture);
	Safe_Release_Map(m_mapSampler);


	unordered_map<string, PATLASDATA>::iterator iter;
	unordered_map<string, PATLASDATA>::iterator iterEnd = m_mapAtlasData.end();

	for (iter = m_mapAtlasData.begin(); iter != iterEnd; ++iter)
	{
		delete iter->second;
	}
	m_mapAtlasData.clear();

}

void CResourcesManager::InsertAtlasData(PATLASDATA pData)
{
	m_mapAtlasData.insert(make_pair(pData->pDirectory, pData));
}

PATLASDATA CResourcesManager::GetAtlasData(const string & strKey)
{
	unordered_map<string, PATLASDATA>::iterator iter = m_mapAtlasData.find(strKey);

	if (iter == m_mapAtlasData.end())
	{
		return NULL;
	}

	return iter->second;
}

bool CResourcesManager::Initialize()
{
	//»ê
	//LoadMesh(ENGINE_MESH_MOUNTAIN_TEX, L"Celeste\\model.txt");
	//LoadMesh(ENGINE_MESH_CUBE_TEX, L"Celeste\\cube.txt");



	//Cube
	{
		VERTEXCOLOR tTriangleColor[24] =
		{
			VERTEXCOLOR(-1.0f, 1.0f, -1.0f ,	1.0f, 1.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f),
			VERTEXCOLOR(1.0f, 1.0f, 1.0f ,		0.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f),

			VERTEXCOLOR(-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, -1.0f, -1.0f ,	1.0f, 0.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f),

			VERTEXCOLOR(-1.0f, -1.0f, 1.0f 	,	1.0f, 1.0f, 1.0f, 1.0f),
			VERTEXCOLOR(-1.0f, -1.0f, -1.0f	,	1.0f, 0.0f, 0.0f, 1.0f),
			VERTEXCOLOR(-1.0f, 1.0f, -1.0f 	,	0.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f),

			VERTEXCOLOR(1.0f, -1.0f, 1.0f	,	0.0f, 1.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, -1.0f, -1.0f	,	1.0f, 0.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, 1.0f, -1.0f	,	1.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(1.0f, 1.0f, 1.0f		,	0.0f, 0.0f, 0.0f, 1.0f),

			VERTEXCOLOR(-1.0f, -1.0f, -1.0f	,	1.0f, 1.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, -1.0f, -1.0f	,	1.0f, 0.0f, 0.0f, 1.0f),
			VERTEXCOLOR(1.0f, 1.0f, -1.0f	,	0.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(-1.0f, 1.0f, -1.0f	,	0.0f, 0.0f, 1.0f, 1.0f),

			VERTEXCOLOR(-1.0f, -1.0f, 1.0f	,	0.0f, 1.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, -1.0f, 1.0f 	,	1.0f, 0.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, 1.0f, 1.0f  	,	1.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(-1.0f, 1.0f, 1.0f 	,	0.0f, 0.0f, 0.0f, 1.0f),
		};

		UINT	iTriangleIdx[12][3] = {
			{ 3,1,0 }, { 2,1,3 },
			{ 6,4,5 }, { 7,4,6 },
			{ 11,9,8 }, { 10,9,11 },
			{ 14,12,13 }, { 15,12,14 },
			{ 19,17,16 }, { 18,17,19 },
			{ 22,20,21 }, { 23,20,22 }
		};

		//D3D11_USAGE_DEFAULT
		CMesh*	pMesh = CreateMesh(ENGINE_MESH_CUBE_COLOR, 8, sizeof(VERTEXCOLOR), D3D11_USAGE_DYNAMIC,
			tTriangleColor, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 36, 4, D3D11_USAGE_DYNAMIC, DXGI_FORMAT_R32_UINT,
			iTriangleIdx);


		SAFE_RELEASE(pMesh);
	}

	//MountainWall
	{
		VERTEXTEX tVerteces[19] =
		{
			VERTEXTEX(-1.80564f, 6.18966f, 0.98476f, 0.0953534f, 0.548108f),
			VERTEXTEX(-2.42836f, 5.90744f, -0.184345f,0.0276982f, 0.522371f),
			VERTEXTEX(-2.20775f, 6.25889f, 0.476503f,0.0352947f, 0.501302f),
			VERTEXTEX(-2.21263f, 6.76605f, -0.159511f,0.121907f, 0.566479f),
			VERTEXTEX(-2.14591f, 6.92345f, 0.429657f,0.128862f, 0.576873f),
			VERTEXTEX(-2.1178f,   6.75201f, -0.315788f,0.0613216f, 0.565475f),
			VERTEXTEX(-2.28191f, 6.04393f, -0.884858f,0.123894f, 0.581105f),
			VERTEXTEX(- 2.18238f, 6.61573f, - 0.530936f,0.112168f, 0.521419f),
			VERTEXTEX(-2.16072f, 7.15944f, 0.320631f,0.0976017f, 0.5873f),
			VERTEXTEX(-2.47809f, 5.64409f, -1.18069f, 0.0934468f, 0.594841f),
			VERTEXTEX(-2.03973f, 7.12108f, 0.262369f,0.0819441f, 0.599286f),
			VERTEXTEX(-2.14283f, 4.95443f, 0.801203f,0.0493662f, 0.598544f),
			VERTEXTEX(-1.69225f, 5.72257f, 1.15911f,   0.00552693f, 0.553402f),
			VERTEXTEX(-2.73896f, 4.67786f, -0.021654f,0.0237651f, 0.599758f),
			VERTEXTEX(-2.64779f, 4.8614f,  0.194453f,   0.00374077f, 0.575318f),
			VERTEXTEX(- 2.38794f, 4.70099f,  0.546858f,0.0556155f, 0.496337f),
			VERTEXTEX(-1.83189f, 5.37516f, 1.1877f,0.0862365f, 0.490749f),
			VERTEXTEX(-2.65834f, 4.81947f, -0.598065f,0.10214f, 0.49912f),
			VERTEXTEX(-2.54319f, 5.01443f, -0.997008f,0.0139472f, 0.526086f)																				
		};

		UINT	iIdx[19][3] = 
		{
			{2,14,15},
			{2,4,8},
			{1,14,2},
			{2,8,10},
			{2,0,4},
			{3,5,1},
			{5,7,1},
			{6,17,7},
			{3,2,10},
			{9,18,6},
			{11,2,15},
			{11,16,0},
			{16,12,0},
			{17,13,1},
			{1,13,14},
			{1,7,17},
			{6,18,17},
			{3,1,2},
			{11,0,2}
		};

		//D3D11_USAGE_DEFAULT
		CMesh*	pMesh = CreateMesh(ENGINE_MESH_MOUNTAIN_WALL_TEX, 19, sizeof(VERTEXTEX), D3D11_USAGE_DYNAMIC,
			tVerteces, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 57, 4, D3D11_USAGE_DYNAMIC, DXGI_FORMAT_R32_UINT,
			iIdx);


		SAFE_RELEASE(pMesh);
	}



	//Triangle
	{
		VERTEXCOLOR tTriangleColor[3] =
		{
			VERTEXCOLOR(0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
			VERTEXCOLOR(0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(-0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f)
		};

		UINT	iTriangleIdx[3] = { 0, 1, 2 };

		//D3D11_USAGE_DEFAULT
		CMesh*	pMesh = CreateMesh(ENGINE_MESH_TRIANGLE_COLOR, 3, sizeof(VERTEXCOLOR), D3D11_USAGE_DYNAMIC,
			tTriangleColor, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 3, 4, D3D11_USAGE_DYNAMIC, DXGI_FORMAT_R32_UINT,
			iTriangleIdx);


		SAFE_RELEASE(pMesh);
	}

	//Rectangle
	{
		VERTEXCOLOR tRectangle[4] =
		{
			VERTEXCOLOR(-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
			VERTEXCOLOR(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(-0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f),
			VERTEXCOLOR(0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f)
		};

		//UINT	iRectangleIdx[2][3] = {{0, 2, 1}, { 1, 2, 3} };

		CMesh*	pMesh = CreateMesh(ENGINE_MESH_RECTANGLE_COLOR, 4, sizeof(VERTEXCOLOR), D3D11_USAGE_DYNAMIC,
			tRectangle, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		SAFE_RELEASE(pMesh);
	}

	//Circle
	{
		const int iSegment = 10;
		float fRadius = 1;
		float iAngularDistance = (float)360 / (float)iSegment;

		VERTEXCOLOR pVertices[iSegment + 1] = {};
		
		pVertices[0] = VERTEXCOLOR(0, 0, 0, 0, 0, 0, 1);
		for (int i = 1; i < iSegment + 1; ++i)
		{
			Vector2 dir = Vector2::AngleToDirection((i -1) * iAngularDistance * D2R) * fRadius;
			pVertices[i] = VERTEXCOLOR(dir.x, dir.y, 0, 0, 0, 0, 0);
		}


		UINT	pIndices[iSegment][3] = {};
		for (int i = 0; i < iSegment ; ++i)
		{
			pIndices[i][0] = 0;
			pIndices[i][1] = i + 2;
			pIndices[i][2] = i + 1;
		}

		pIndices[iSegment - 1][0] = 0;
		pIndices[iSegment - 1][1] = 1;
		pIndices[iSegment - 1][2] = iSegment;


		CMesh*	pMesh = CreateMesh(ENGINE_MESH_CIRCLE_COLOR, iSegment + 1, sizeof(VERTEXCOLOR), D3D11_USAGE_DYNAMIC,
			pVertices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 3 * iSegment, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
			pIndices);


		SAFE_RELEASE(pMesh);
	}

	//Orthography Triangle
	{
		VERTEXCOLOR tTriangleColor[3] =
		{
			VERTEXCOLOR(0.5f, 0.875f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
			VERTEXCOLOR(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f)
		};

		UINT	iTriangleIdx[3] = { 0, 1, 2 };

		//D3D11_USAGE_DEFAULT
		CMesh*	pMesh = CreateMesh(ENGINE_MESH_ORTHO_TRIANGLE_COLOR, 3, sizeof(VERTEXCOLOR), D3D11_USAGE_DYNAMIC,
			tTriangleColor, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 3, 4, D3D11_USAGE_DYNAMIC, DXGI_FORMAT_R32_UINT,
			iTriangleIdx);

		SAFE_RELEASE(pMesh);
	}

	//Orthography Rectangle
	{
		VERTEXCOLOR tRectangle[4] =
		{
			VERTEXCOLOR(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
			VERTEXCOLOR(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
			VERTEXCOLOR(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f),
			VERTEXCOLOR(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f)
		};
		
		UINT	iRectangleIdx[2][3] = {{ 0, 1, 2 },{ 1, 3, 2 } };

		CMesh*	pMesh = CreateMesh(ENGINE_MESH_ORTHO_RECTANGLE_COLOR, 4, sizeof(VERTEXCOLOR), D3D11_USAGE_DYNAMIC,
			tRectangle, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
			iRectangleIdx);


		SAFE_RELEASE(pMesh);
	}

	

	//Orthography Circle
	{
		const int iSegment = 10;
		float iAngularDistance = (float)360 / (float)iSegment;

		VERTEXCOLOR pVertices[iSegment + 1] = {};

		pVertices[0] = VERTEXCOLOR(0, 0, 0, 0, 0, 0, 1);
		for (int i = 1; i < iSegment + 1; ++i)
		{
			Vector2 dir = Vector2::AngleToDirection((i - 1) * iAngularDistance);
			pVertices[i] = VERTEXCOLOR(dir.x, dir.y, 0, 0, 0, 0, 0);
		}


		UINT	pIndices[iSegment][3] = {};
		for (int i = 0; i < iSegment; ++i)
		{
			pIndices[i][0] = 0;
			pIndices[i][1] = i + 1;
			pIndices[i][2] = i + 2;
		}

		pIndices[iSegment - 1][0] = 0;
		pIndices[iSegment - 1][1] = iSegment;
		pIndices[iSegment - 1][2] = 1;


		CMesh*	pMesh = CreateMesh(ENGINE_MESH_ORTHO_CIRCLE_COLOR, iSegment + 1, sizeof(VERTEXCOLOR), D3D11_USAGE_DYNAMIC,
			pVertices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 3 * iSegment, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
			pIndices);


		SAFE_RELEASE(pMesh);
	}

	//Debug Rectangle Frame
	{
		VERTEXCOLOR	tFrameRectColor[5] =
		{
			VERTEXCOLOR(0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f),
			VERTEXCOLOR(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f),
			VERTEXCOLOR(1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f),
			VERTEXCOLOR(0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f),
			VERTEXCOLOR(0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f)
		};

		CMesh* pMesh = CreateMesh(ENGINE_MESH_FRAME_RECT_COLOR,	5, sizeof(VERTEXCOLOR),
			D3D11_USAGE_DEFAULT,	tFrameRectColor, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		SAFE_RELEASE(pMesh);
	}


	//Debug Circle Frame
	{
		const int iSegment = 18;
		float iAngularDistance = (float)360 / (float)iSegment;

		VERTEXCOLOR pVertices[iSegment + 1] = {};


		for (int i = 0; i < iSegment ; ++i)
		{
			Vector2 dir = Vector2::AngleToDirection(i  * iAngularDistance);
			pVertices[i] = VERTEXCOLOR(dir.x, dir.y, 0, 0, 0, 1, 1);
		}

		pVertices[iSegment] = pVertices[0];


		CMesh* pMesh = CreateMesh(ENGINE_MESH_FRAME_CIRCLE_COLOR, iSegment + 1 , sizeof(VERTEXCOLOR),
			D3D11_USAGE_DEFAULT, pVertices, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		SAFE_RELEASE(pMesh);
	}




	{

		VERTEXTEX	tPerspectiveRectTex[4] =
		{
			VERTEXTEX(0.f, 0.f, 0.f, 0.f, 1.f),
			VERTEXTEX(1.f, 0.f, 0.f, 1.f, 1.f),
			VERTEXTEX(0.f, 1.f, 0.f, 0.f, 0.f),
			VERTEXTEX(1.f, 1.f, 0.f, 1.f, 0.f)
		};


		UINT	iRectIdx[2][3] = { { 0, 3, 1 },{ 0, 2, 3 } };

		CMesh* pMesh = CreateMesh(ENGINE_MESH_RECTANGLE_TEX,
			4, sizeof(VERTEXTEX), D3D11_USAGE_DEFAULT,
			tPerspectiveRectTex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			6, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
			iRectIdx);

		SAFE_RELEASE(pMesh);
	}


	{

		VERTEXTEX	tOrthoRectTex[4] =
		{
			VERTEXTEX(0.f, 0.f, 0.f, 0.f, 0.f),
			VERTEXTEX(1.f, 0.f, 0.f, 1.f, 0.f),
			VERTEXTEX(0.f, 1.f, 0.f, 0.f, 1.f),
			VERTEXTEX(1.f, 1.f, 0.f, 1.f, 1.f)
		};


		UINT	iRectIdx[2][3] = { { 0, 1, 3 }, {0, 3, 2 } };

		CMesh* pMesh = CreateMesh(ENGINE_MESH_ORTHORECT_TEX,
			4, sizeof(VERTEXTEX), D3D11_USAGE_DEFAULT,
			tOrthoRectTex, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			6, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
			iRectIdx);

		SAFE_RELEASE(pMesh);
	}

	//Grid
	{		
		VERTEXCOLOR	tFrameRectColor[5] =
		{
			VERTEXCOLOR(0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f),
			VERTEXCOLOR(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f),
			VERTEXCOLOR(1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f),
			VERTEXCOLOR(0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f),
			VERTEXCOLOR(0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f)
		};

		CMesh* pMesh = CreateMesh(ENGINE_MESH_FRAME_RECT_COLOR, 5, sizeof(VERTEXCOLOR),
			D3D11_USAGE_DEFAULT, tFrameRectColor, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		SAFE_RELEASE(pMesh);
	}


	CSampler*	pSampler = CreateSampler(SAMPLER_LINEAR);
	SAFE_RELEASE(pSampler);

	pSampler = CreateSampler(SAMPLER_POINT, D3D11_FILTER_MIN_MAG_MIP_POINT);
	SAFE_RELEASE(pSampler);

	pSampler = CreateSampler(SAMPLER_POINT_LOOP,
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP);
	SAFE_RELEASE(pSampler);


	return true;
}


CMesh * CResourcesManager::CreateMesh(
	const string & strKey, UINT iVtxCount, UINT iVtxSize,
	D3D11_USAGE eVtxUsage, void * pVtxData, D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType,
	UINT iIdxCount, UINT iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eIdxFmt, void * pIdxData)
{
	CMesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;

	pMesh = new CMesh;

	if (!pMesh->CreateMesh(strKey, iVtxCount, iVtxSize, eVtxUsage,
		pVtxData, ePrimitiveType, iIdxCount, iIdxSize, eIdxUsage,
		eIdxFmt, pIdxData))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}
void CResourcesManager::LoadMesh(const string & strKey, const wchar_t * pFileName, const string & strPathKey)
{
	wstring	strPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);
	strPath += pFileName;
	
	char strFullPath[512] = {};
	WideCharToMultiByte(CP_ACP, 0, strPath.c_str(), -1, strFullPath, lstrlenW(strPath.c_str()), 0, 0);


	vector<Vector3> VerticesPosition;
	vector<Vector2> VerticesUV;
	vector<UINT> VerticesIndex1;
	vector<UINT> VerticesIndex2;
	vector<UINT> VerticesIndex3;
	vector<UINT>TextureIndices1;
	vector<UINT>TextureIndices2;
	vector<UINT>TextureIndices3;

	int iVertexCount = 0;

	FILE* pFile = NULL;

	vector<VERTEXTEX> Vertices;

	fopen_s(&pFile, strFullPath, "rt");

	if(!pFile)
	{
		return;
	}
	else
	{

		char pLine[512] = {};
		fgets(pLine, 512, pFile);
		iVertexCount = stoi(pLine);

		int iLineCounter = 1;
		while (true)
		{
			iLineCounter++;
			fgets(pLine, 512, pFile);
			int iLength = strlen(pLine);
			if (iLength == 0)
			{
				break;
			}
			if (iLineCounter == 31787)
			{
				break;
			}
			VERTEXTEX tempVertex;
			char* pContext = NULL;
			char* tempValue = strtok_s(&pLine[0], " ", &pContext);
			tempVertex.vPos.x = stof(tempValue);
			tempValue = strtok_s(NULL, " ", &pContext);
			tempVertex.vPos.y = stof(tempValue);
			tempValue = strtok_s(NULL, " ", &pContext);
			tempVertex.vPos.z = stof(tempValue);
			tempValue = strtok_s(NULL, " ", &pContext);
			tempVertex.vUV.x = stof(tempValue);
			tempValue = strtok_s(NULL, " ", &pContext);
			tempVertex.vUV.y = stof(tempValue);

			Vertices.push_back(tempVertex);

			continue;
			//if (pLine[0] == 'v' && pLine[1] != 't' && pLine[1] != 'n')
			//{
			//	//fscanf()
			//	Vector3 position;
			//	char* pContext = NULL;
			//	char* tempValue = strtok_s(&pLine[1], " ", &pContext);
			//	position.x = stof(tempValue);
			//	tempValue = strtok_s(NULL, " ", &pContext);
			//	position.y = stof(tempValue);
			//	tempValue = strtok_s(NULL, " ", &pContext);
			//	position.z = stof(tempValue);
			//
			//	VerticesPosition.push_back(position);
			//}
			//else if (pLine[0] == 'v' && pLine[1] == 't' && pLine[1] != 'n')
			//{
			//	Vector2 uv;
			//	char* pContext = NULL;
			//	char* tempValue = strtok_s(&pLine[2], " ", &pContext);
			//	uv.x = stof(tempValue);
			//	tempValue = strtok_s(NULL, " ", &pContext);
			//	uv.y = stof(tempValue);
			//	VerticesUV.push_back(uv);
			//}
			//else if (pLine[0] == 'f')
			//{
			//	UINT VertexIndex[3];
			//	UINT TextureIndex[3];
			//	UINT NormalIndex[3];
			//	
			//	char* pContext = NULL;
			//	char* tempValue = strtok_s(&pLine[2], "/", &pContext);
			//	VertexIndex[0] = stoi(tempValue) - 1;// Vertex.x
			//	tempValue = strtok_s(NULL, "/", &pContext);
			//	TextureIndex[0] = stoi(tempValue) - 1;
			//	tempValue = strtok_s(NULL, " ", &pContext);
			//	NormalIndex[0] = stoi(tempValue) - 1;
			//	
			//	//2
			//	tempValue = strtok_s(NULL, "/", &pContext);
			//	VertexIndex[1] = stoi(tempValue) - 1;// Vertex.x
			//	tempValue = strtok_s(NULL, "/", &pContext);
			//	TextureIndex[1] = stoi(tempValue) - 1;
			//	tempValue = strtok_s(NULL, " ", &pContext);
			//	NormalIndex[1] = stoi(tempValue) - 1;
			//	//3
			//	tempValue = strtok_s(NULL, "/", &pContext);
			//	VertexIndex[2] = stoi(tempValue) - 1;// Vertex.x
			//	tempValue = strtok_s(NULL, "/", &pContext);
			//	TextureIndex[2] = stoi(tempValue) - 1;
			//	tempValue = strtok_s(NULL, " ", &pContext);
			//	NormalIndex[2] = stoi(tempValue) - 1;
			//	
			//	VerticesIndex1.push_back(VertexIndex[0]);
			//	VerticesIndex2.push_back(VertexIndex[1]);
			//	VerticesIndex3.push_back(VertexIndex[2]);
			//	TextureIndices1.push_back(TextureIndex[0]);
			//	TextureIndices2.push_back(TextureIndex[1]);
			//	TextureIndices3.push_back(TextureIndex[2]);
			//	//NormalIndices.push_back(NormalIndex);
			//}
			//else
			//{
			//	continue;
			//}
		}//While
		fclose(pFile);
	}//else


	//vector<VERTEXTEX> Vertices;
	//vector<UINT> Indices;	
	//
	//for (int i = 0; i < VerticesIndex1.size(); ++i)
	//{
	//	VERTEXTEX temp[3] = { 
	//		{
	//		VerticesPosition[VerticesIndex1[i]].x,
	//		VerticesPosition[VerticesIndex1[i]].y,
	//		VerticesPosition[VerticesIndex1[i]].z,
	//		VerticesUV[TextureIndices1[i]].x,
	//		VerticesUV[TextureIndices1[i]].y 
	//		},
	//		{
	//			VerticesPosition[VerticesIndex2[i]].x,
	//			VerticesPosition[VerticesIndex2[i]].y,
	//			VerticesPosition[VerticesIndex2[i]].z,
	//			VerticesUV[TextureIndices2[i]].x,
	//			VerticesUV[TextureIndices2[i]].y
	//		} ,
	//		{
	//				VerticesPosition[VerticesIndex3[i]].x,
	//				VerticesPosition[VerticesIndex3[i]].y,
	//				VerticesPosition[VerticesIndex3[i]].z,
	//				VerticesUV[TextureIndices3[i]].x,
	//				VerticesUV[TextureIndices3[i]].y 
	//		}
	//	};
	//
	//	Vertices.push_back(temp[0]);
	//	Vertices.push_back(temp[1]);
	//	Vertices.push_back(temp[2]);		
	//}

	vector<UINT> Indices;	
	Indices.resize(Vertices.size());
	for (int i = 0; i < Indices.size(); ++i)
	{
		Indices[i] = i;
	}
	
	//for (int i = 0; i < VerticesPosition.size(); ++i)
	//{
	//	VERTEXTEX temp = { VerticesPosition[i].x, VerticesPosition[i].y, VerticesPosition[i].z,VerticesUV[i].x, VerticesUV[i].y };
	//	Vertices.push_back(temp);
	//}
	
	CMesh*	pMesh = CreateMesh(strKey, Vertices.size(), sizeof(VERTEXTEX), D3D11_USAGE_DYNAMIC,
		&Vertices[0], D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, Indices.size(), 4, D3D11_USAGE_DYNAMIC, DXGI_FORMAT_R32_UINT,
		&Indices[0]);
	
	
		SAFE_RELEASE(pMesh);
	
}
CMesh * CResourcesManager::FindMesh(const string & strKey)
{
	unordered_map<string, class CMesh*>::iterator	iter = m_mapMesh.find(strKey);

	if (iter == m_mapMesh.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CTexture * CResourcesManager::LoadTexture(const string & strKey, const wchar_t * pFileName, const string & strPathKey)
{
	CTexture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(strKey, pFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture * CResourcesManager::LoadTexture(const string & strKey, const vector<wstring>& vecFileName, const string & strPathKey)
{
	CTexture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(strKey, vecFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture * CResourcesManager::LoadTextureFromFullPath(const string & strKey, const wchar_t * pFullPath)
{
	CTexture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	if (!pTexture->LoadTextureFromFullPath(strKey, pFullPath))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture * CResourcesManager::LoadTextureFromFullPath(const string & strKey, const vector<wstring>& vecFullPath)
{
	CTexture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	if (!pTexture->LoadTextureFromFullPath(strKey, vecFullPath))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture * CResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, class CTexture*>::iterator	iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CSampler * CResourcesManager::CreateSampler(const string & strKey, D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE eU, D3D11_TEXTURE_ADDRESS_MODE eV, D3D11_TEXTURE_ADDRESS_MODE eW, float fMipLODBias, UINT iMaxAnisotropy, D3D11_COMPARISON_FUNC eFunc, const Vector4 & vBorderColor, float fMinLOD, float fMaxLOD)
{
	CSampler*	pSampler = FindSampler(strKey);

	if (pSampler)
		return pSampler;

	pSampler = new CSampler;

	if (!pSampler->CreateSampler(strKey, eFilter, eU, eV, eW, fMipLODBias, iMaxAnisotropy,
		eFunc, vBorderColor, fMinLOD, fMaxLOD))
	{
		SAFE_RELEASE(pSampler);
		return NULL;
	}

	pSampler->AddRef();

	m_mapSampler.insert(make_pair(strKey, pSampler));

	return pSampler;
}

CSampler * CResourcesManager::FindSampler(const string & strKey)
{
	unordered_map<string, class CSampler*>::iterator	iter = m_mapSampler.find(strKey);

	if (iter == m_mapSampler.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
