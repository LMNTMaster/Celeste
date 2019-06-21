#pragma once
#include "Ref.h"

LMNT_BEGIN

class LMNT_DLL CLayer :
	public CRef
{
private:
	friend class CScene;

private:
	CLayer();
	~CLayer();


private:
	list<class CGameObject*>	m_GameObjectList;
	class CScene*		m_pScene;
	int	m_iZOrder;

private:
	void SetScene(class CScene* pScene);
	void SetZOrder(int iZOrder);

public:
	int GetZOrder()	const;
	void RemoveGameObject(class CGameObject* pGameObject);
	void AddGameObject(class CGameObject* pGameObject);
	const list<class CGameObject*>* GetAllGameObjects() const;
	CGameObject* FindGameObjectWithTag(const string& strTag);

	bool Save(FILE* pFile);
	bool Load(FILE* pFile);
public:
	bool Initialize();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	int Collision(float fTime);
	void Render(float fTime);
};

LMNT_END