#include "Component.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"

LMNT_USING


CComponent::CComponent():
	m_pScene(NULL),
	m_pTransform(NULL),
	m_pLayer(NULL),
	m_pGameObject(NULL)
{
}

CComponent::CComponent(const CComponent & component)
{
	*this = component;
}

CComponent::~CComponent()
{
}

CScene * CComponent::GetScene() const
{
	if (m_pScene)
		m_pScene->AddRef();
	return m_pScene;
}

CLayer * CComponent::GetLayer() const
{
	if (m_pLayer)
		m_pLayer->AddRef();
	return m_pLayer;
}

CGameObject * CComponent::GetGameObject() const
{
	if (m_pGameObject)
		m_pGameObject->AddRef();
	return m_pGameObject;
}

CTransform * CComponent::GetTransform() const
{
	if (m_pTransform)
		m_pTransform->AddRef();

	return m_pTransform;
}

void CComponent::SetScene(CScene * pScene)
{
	m_pScene = pScene;
}

void CComponent::SetLayer(CLayer * pLayer)
{
	m_pLayer = pLayer;	
}

void CComponent::SetGameObject(CGameObject * pGameObject)
{
	m_pGameObject = pGameObject;
}

void CComponent::SetTransform(CTransform * pTransform)
{
	m_pTransform = pTransform;
}

int CComponent::GetComponentType() const
{
	return m_eComponentType;
}

int CComponent::Input(float fTime)
{
	return 0;
}

int CComponent::Update(float fTime)
{
	return 0;
}

int CComponent::LateUpdate(float fTime)
{
	return 0;
}

int CComponent::OnCollisionEnter(CCollider* pThis, CCollider* pTarget, float fTime)
{
	return 0;
}

int CComponent::OnCollisionStay(CCollider* pThis, CCollider* pTarget, float fTime)
{
	return 0;
}

int CComponent::OnCollisionExit(CCollider* pThis, CCollider* pTarget, float fTime)
{
	return 0;
}

void CComponent::OnMouseEnter(const Vector2& vPosition)
{
}

void CComponent::OnMouseStay(const Vector2& vPosition, float fTime)
{
}

void CComponent::OnMouseExit(const Vector2& vPosition)
{
}

void CComponent::Render(float fTime)
{
}

CComponent * CComponent::Clone() const
{
	return nullptr;
}

bool CComponent::Save(FILE * pFile)
{
	return true;
}

bool CComponent::Load(FILE * pFile)
{
	return true;
}
