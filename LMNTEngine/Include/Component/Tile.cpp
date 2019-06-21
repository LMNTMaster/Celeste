#include "Tile.h"
#include "../ResourcesManager.h"
#include "Renderer.h"
#include "../Component.h"
#include "../GameObject.h"

LMNT_USING

CTile::CTile():
	m_iOffsetX (0),
	m_iOffsetY(0),
	m_iTileIndex(0),
	m_strTileSetName("")
{
	m_eComponentType = CT_TILE;
	SetTag("Tile");
}


CTile::~CTile()
{
}

void CTile::SetRenderer()
{
	string key = "tilesets\\" + m_strTileSetName;
	ATLASDATA tData = *GET_SINGLE(CResourcesManager)->GetAtlasData(key);
	int x = tData.wWidth / 8;
	m_iOffsetX = m_iTileIndex % x;
	m_iOffsetY = m_iTileIndex / x;
	CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetImageInfo(tData.wX + m_iOffsetX * 8, tData.wY + m_iOffsetY * 8, 8, 8, tData.iAtlasSizeX, tData.iAtlasSizeY);
	SAFE_RELEASE(pRenderer);		
}

int CTile::GetTilesetCount() const
{
	return m_iTileCount;
}

const string & CTile::GetTilesetName() const
{
	return m_strTileSetName;
	// TODO: 여기에 반환 구문을 삽입합니다.
}

int CTile::GetTileIndex() const
{	
	return m_iTileIndex;
}

//return tileset index count
int CTile::SetTileSetName(const string & strName)
{
	m_strTileSetName = strName;

	string key = "tilesets\\" + m_strTileSetName;
	ATLASDATA tData = *GET_SINGLE(CResourcesManager)->GetAtlasData(key);

	int x = tData.wWidth / 8;
	int y = tData.wHeight / 8;

	//m_iOffsetX = 0;
	//m_iOffsetY = 0;

	CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetImageInfo(tData.wX + m_iOffsetX * 8, tData.wY + m_iOffsetY * 8, 8, 8, tData.iAtlasSizeX, tData.iAtlasSizeY);
	SAFE_RELEASE(pRenderer);

	m_iTileCount = x * y;
	return m_iTileCount;
}

void CTile::SetTileIndex(unsigned int iIndex)
{
	string key = "tilesets\\" + m_strTileSetName;
	ATLASDATA tData = *GET_SINGLE(CResourcesManager)->GetAtlasData(key);

	int x = tData.wWidth / 8;

	m_iOffsetX = iIndex % x;
	m_iOffsetY = iIndex / x;

	CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetImageInfo(tData.wX + m_iOffsetX * 8, tData.wY + m_iOffsetY * 8, 8, 8, tData.iAtlasSizeX, tData.iAtlasSizeY);
	SAFE_RELEASE(pRenderer);

	m_iTileIndex = iIndex;
}

bool CTile::Initialize()
{
	return true;
}

bool CTile::Save(FILE* pFile)
{
	int iLength = m_strTileSetName.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strTileSetName.c_str(), iLength, 1, pFile);
	fwrite(&m_iTileIndex, 4, 1, pFile);
	return true;
}

bool CTile::Load(FILE* pFile)
{
	char cBuffer[512] = {};
	int iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(cBuffer, iLength, 1, pFile);
	fread(&m_iTileIndex, 4, 1, pFile);

	m_strTileSetName = cBuffer;

	SetRenderer();
	return true;
}
