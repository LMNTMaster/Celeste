#pragma once
#include "../Component.h"

LMNT_BEGIN

class LMNT_DLL CTile :
	public CComponent
{

private:
	friend class CGameObject;

private:
	CTile();
	~CTile();

private:
	void SetRenderer();
	int m_iOffsetX;
	int m_iOffsetY;

public:
	string m_strTileSetName;
	unsigned int m_iTileIndex;
	int m_iTileCount;

public:
	int GetTilesetCount() const;
	const string& GetTilesetName() const ;
	int GetTileIndex() const;
	int SetTileSetName(const string& strName);
	void SetTileIndex(unsigned int iIndex);

public:
	bool Initialize() override;

public:
	bool Save(FILE* pFile);
	bool Load(FILE* pFile);


};

LMNT_END

