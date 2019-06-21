#include "UIButton.h"
#include "../Component.h"
#include "Renderer.h"
#include "../GameObject.h"
#include "../Material.h"
#include "../Transform.h"
#include "ButtonCollider.h"

#include "../Input.h"

LMNT_USING

CUIButton::CUIButton()
{
	m_eUIType = UT_BUTTON;
}

CUIButton::CUIButton(const CUIButton & component) :
	CUI(component)
{
}

CUIButton::~CUIButton()
{
}

bool CUIButton::Initialize()
{
	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("ButtonRenderer");

	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(UI_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();

	pMaterial->SetDiffuseTexture("Button", L"Start.png");
	pMaterial->SetSampler(SAMPLER_LINEAR);
	pMaterial->SetMaterial(Vector4::white);

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	CTransform*	pTransform = m_pGameObject->GetTransform();

	pTransform->SetPivot(0.5f, 0.5f, 0.f);
	pTransform->SetWorldScale(100.f, 50.f, 1.f);
	SAFE_RELEASE(pTransform);

	CButtonCollider* pCollider = m_pGameObject->AddComponent<CButtonCollider>("ButtonCollider");
	pCollider->SetScale(110.0f, 55.0f);
	pCollider->SetPivot(0.5f, 0.5f);
	SAFE_RELEASE(pCollider);


	return true;
}

void CUIButton::OnMouseEnter(const Vector2 & vPosition)
{
	CButtonCollider* pCollider = m_pGameObject->GetComponentFromType<CButtonCollider>(CT_BUTTON_COLLIDER);

#ifdef _DEBUG
	pCollider->SetColor(Vector4::red);
#endif
	SAFE_RELEASE(pCollider);

	m_vDiffuse = Vector4::white;
}

void CUIButton::OnMouseStay(const Vector2 & vPosition, float fTime)
{
	CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);

	if (GETKEYDOWN(VK_LBUTTON))
	{
		m_vDiffuse = Vector4::red;
		pRenderer->SetDiffuse(m_vDiffuse);
	}

	if (m_vDiffuse != Vector4::red)
	{
		m_vDiffuse = Vector4::white * 2.0f;
		pRenderer->SetDiffuse(m_vDiffuse);
	}

	SAFE_RELEASE(pRenderer);
}

void CUIButton::OnMouseExit(const Vector2 & vPosition)
{
	m_vDiffuse = Vector4::white;

	CButtonCollider* pCollider = m_pGameObject->GetComponentFromType<CButtonCollider>(CT_BUTTON_COLLIDER);
#ifdef _DEBUG
	pCollider->SetColor(Vector4::red);
#endif
	SAFE_RELEASE(pCollider);

	CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetDiffuse(Vector4::white);
	SAFE_RELEASE(pRenderer);
}
