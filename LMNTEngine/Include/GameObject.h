#pragma once
#include "Ref.h"

LMNT_BEGIN

class LMNT_DLL CGameObject :
	public CRef
{
private:
	friend class CScene;

private:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();
	
public:
	static CGameObject* CreateObject(const string& strTag, class CLayer* pLayer = NULL);

private:
	unsigned int m_iID;
	class CScene*		m_pScene;
	class CLayer*		m_pLayer;
	class CTransform*	m_pTransform;
	list<class CComponent*> m_ComponentList;
	list<class CCollider*> m_ColliderList;
	list<class CComponent*> m_FindComponentList;

#pragma region Prototype

private:
	static unordered_map<class CScene*, unordered_map<string, CGameObject*>>	m_mapPrototype;

public:
	static CGameObject* CreatePrototype(const string& strTag, class CScene* pScene);
	static CGameObject* CreateClone(const string& strTag, class CScene* pScene,
		class CLayer* pLayer = NULL);
	static bool ChangePrototypeScene(const string& strTag, class CScene* pCurScene,
		class CScene* pChangeScene);
	static bool ErasePrototype(const string& strTag, class CScene* pScene);
	static bool ErasePrototypeList(class CScene* pScene);

private:
	static unordered_map<string, CGameObject*>* FindPrototypeList(class CScene* pScene);
	static CGameObject* FindPrototype(const string& strTag, class CScene* pScene);


#pragma endregion
public:
	CScene* GetScene() const;
	CLayer* GetLayer() const;
	void SetScene(class CScene* pScene);
	void SetLayer(class CLayer* pLayer);
	void ChangeLayer(class CLayer* pTargetLayer);
	class CTransform* GetTransform() const;
	bool Save(FILE* pFile);
	bool Load(FILE* pFile);
	

public:
	bool Initialize();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime);
	int OnCollisionStay(class CCollider* pThis, class CCollider* pTarget, float fTime);
	int OnCollisionExit(class CCollider* pThis, class CCollider* pTarget, float fTime);
	int OnMouseEnter(const Vector2& vPosition);
	int OnMouseStay(const Vector2& vPosition, float fTime);
	int OnMouseExit(const Vector2& vPosition);
	void Render(float fTime);
	CGameObject* Clone()	const;

public:
	bool CheckComponentFromType(COMPONENT_TYPE eType);
	CComponent* AddComponent(class CComponent* pComponent);
	const list<class CCollider*>* GetColliderList() const;
	const list<CComponent*>* GetComponentsFromTag(const string& strTag);
	list<CComponent*>* GetComponentsFromType(int eType);

	template <typename T>
	T* GetComponentFromTag(const string& strTag)
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

		for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddRef();
				return *iter;
			}
		}

		return NULL;
	}

	template <typename T>
	T* GetComponentFromType(int eType)
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

		for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetComponentType() == eType)
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}

		return NULL;
	}


	template <typename T>
	T* AddComponent(const string& strTag)
	{
		T*	pCom = new T;

		pCom->SetTag(strTag);
		pCom->SetScene(m_pScene);
		pCom->SetLayer(m_pLayer);
		pCom->SetGameObject(this);
		pCom->SetTransform(m_pTransform);

		if (!pCom->Initialize())
		{
			SAFE_RELEASE(pCom);
			return NULL;
		}

		return (T*)AddComponent((CComponent*)pCom);
	}
};

LMNT_END