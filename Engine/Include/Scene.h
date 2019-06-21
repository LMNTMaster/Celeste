#pragma once
#include "Ref.h"

LMNT_BEGIN

class LMNT_DLL CScene :
	public CRef
{
private:
	friend class CSceneManager;

public:
	CScene();
	virtual ~CScene();


protected:
	//list<class CSceneComponent*>							m_SceneComponentList;
	vector<class CLayer*>										m_vecLayer;
	vector<class CGameObject*>								m_vecGameObject;
	unsigned int m_iIDCounter;
	unordered_map<string, class CGameObject*>		m_mapCamera;
	class CGameObject*											m_pMainCamera;
	list<class CCollider*> m_ColliderList;
	list<class CButtonCollider*> m_ButtonColliderList;
	bool m_bInitialized;

	bool* m_pCollisionMatrix;

public:

	void AddGameObject(class CGameObject * pGameObject);
	void RemoveGameObject(class CGameObject* pGameObject);
	void AddCollisionMatrix();
	void SetCollisionMatrix(class CLayer* pLayer1, class CLayer* pLayer2, bool bCollision);
	bool GetLayerCollision(CLayer * pLayer1, CLayer* pLayer2);
	CLayer* CreateLayer(const string& strTag, int iZOrder = 0);
	CLayer* FindLayer(unsigned int iIndex) const ;
	CLayer* FindLayer(const string& strTag);
	size_t GetLayerCount() const;
	bool ChangeLayerZOrder(const string& strTag, int iZOrder);
	class CGameObject* CreateCamera(const string& strKey, const Vector3& vPos, const Vector3& vRot = Vector3::Zero);
	class CGameObject* FindCameraObject(const string& strKey);
	class CCamera* FindCamera(const string& strKey);
	class CTransform* FindCameraTransform(const string& strKey);
	class CGameObject* GetMainCameraObject();
	class CCamera* GetMainCamera();
	class CTransform* GetMainCameraTransform();

	const list<class CCollider*>* GetColliderList() const;
	const list<class CButtonCollider*>* GetButtonColliderList() const;

	void RemoveCollider(class CCollider* pCollider);
	void RemoveButtonCollider(class CButtonCollider* pCollider);

	void AddCollider(class CCollider* pCollider);
	void AddButtonCollider(class CButtonCollider* pCollider);

	bool Save(FILE* pFile);
	bool Load(FILE* pFile);
private:
	static bool LayerSort(class CLayer* p1, class CLayer* p2);

public:
	virtual bool Initialize();
	int Input(float fTime);
	virtual void Start();
	int Update(float fTime);
	int LateUpdate(float fTime);
	int Collision(float fTime);
	void Render(float fTime);



public:
	template<typename T>
	T* AddSceneComponent(const string& strTag)
	{
		T* pComponent = new T;

		pComponent->SetScene(this);
		
		if (!pComponent->Initialize())
		{
			SAFE_RELEASE(pComponent);
			return NULL;
		}

		pComponent->AddRef();

		m_SceneComponentList.push_back(pComponent);

		return pComponent;
	}

};

LMNT_END