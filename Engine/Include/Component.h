#pragma once
#include "Ref.h"

LMNT_BEGIN

class LMNT_DLL CComponent :
	public CRef
{

protected:
	CComponent();
	CComponent(const CComponent& component);
	virtual ~CComponent() = 0;

protected:
	class CScene*			m_pScene;
	class CLayer*				m_pLayer;
	class CGameObject*	m_pGameObject;
	class CTransform*		m_pTransform;

public:
	class CScene* GetScene()	const;
	class CLayer* GetLayer()	const;
	class CGameObject* GetGameObject()	const;
	class CTransform* GetTransform()	const;

public:
	void SetScene(class CScene* pScene);
	void SetLayer(class CLayer* pLayer);
	void SetGameObject(class CGameObject* pGameObject);
	void SetTransform(class CTransform* pTransform);

protected:
	int m_eComponentType;

public:
	int GetComponentType()	const;

public:
	virtual bool Initialize() = 0;
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget , float fTime);
	virtual int OnCollisionStay(class CCollider* pThis, class CCollider* pTarget, float fTime);
	virtual int OnCollisionExit(class CCollider* pThis, class CCollider* pTarget, float fTime);
	virtual void OnMouseEnter(const Vector2& vPosition);
	virtual void OnMouseStay(const Vector2& vPosition, float fTime);
	virtual void OnMouseExit(const Vector2& vPosition);

	virtual void Render(float fTime);
	virtual CComponent* Clone()	const;

	virtual bool Save(FILE* pFile);
	virtual bool Load(FILE* pFile);
};

LMNT_END