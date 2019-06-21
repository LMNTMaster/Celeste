#include "Refill.h"
#include "AnimationClip2D.h"
#include "AnimationState.h"
#include "AnimationController.h"
#include "Transition.h"
#include "Component/RectCollider2D.h"
#include "Layer.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "../GameData.h"
#include "Player.h"
#include "SoundManager.h"

CRefill::CRefill():
	m_eState(RS_IDLE),
	m_bInitialized(false),
	m_bDisable(false),
	m_pBaseAnimator(NULL),
	m_pEffectAnimator(NULL),
	m_pBaseRenderer(NULL),
	m_pEffectRenderer(NULL),
	m_fTimer(0.0f),
	m_fParticleTimer(0.0f),

	m_fLocalScale(0.0f),
	m_fStartAmplitude(10.0f),
	m_fFrequency(7.5f)
{
}


CRefill::~CRefill()
{
}

void CRefill::SetDisable()
{
	m_bDisable = true;
	m_fRegenerationTimer = 0.0f;
	m_pBaseRenderer->Enable(false);
	m_pEffectRenderer->Enable(false);
	m_eState = RS_DISABLE;
}

void CRefill::Regeneration()
{
	m_bDisable = false;
	m_fRegenerationTimer = 0.0f;

	m_eState = RS_REGENERATION;
	m_fScalingTimer = 0.0f;
	m_pBaseRenderer->Enable(true);
	m_pEffectRenderer->Enable(true);

	for (int i = 0; i < 30; ++i)
	{
		Vector3 position = m_pTransform->GetWorldPosition();
		Vector3 targetPosition = position + Vector3::RandomDirection() * RandomRange(50.0f, 60.0f);
		Vector4 color = Vector4(RandomRange(150.0f, 255.0f) / 255.0f, 1.0f, RandomRange(150.0f, 255.0f) / 255.0f, 1.0f);

		CGameObject* pParticle = GET_SINGLE(CGameData)->CreateParticle(m_pLayer, position, targetPosition, color, true, true, 1.0f, 3.0f, RandomRange(80.0f, 100.0f));
		SAFE_RELEASE(pParticle);
	}


	int iRandomAudioClip = rand() % 5;

	switch (iRandomAudioClip)
	{
	case 0:
		GET_SINGLE(CSoundManager)->Play("game_gen_seed_pulse_01");
		break;
	case 1:
		GET_SINGLE(CSoundManager)->Play("game_gen_seed_pulse_02");
		break;
	case 2:
		GET_SINGLE(CSoundManager)->Play("game_gen_seed_pulse_03");
		break;
	case 3:
		GET_SINGLE(CSoundManager)->Play("game_gen_seed_pulse_04");
		break;
	case 4:
		GET_SINGLE(CSoundManager)->Play("game_gen_seed_pulse_05");
		break;
	
	default:
		break;
	}
}

bool CRefill::Initialize()
{
	{
		CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial* pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_POINT);
		SAFE_RELEASE(pMaterial);

		CAnimator* pAnimator = m_pGameObject->AddComponent<CAnimator>("Animator");
		pAnimator->SetTargetRenderer(pRenderer);
		CAnimationController* pController = new CAnimationController;
		pAnimator->SetAnimationController(pController);
		pAnimator->SetSyncScale(true);
		pAnimator->SetFixedScaleFactor(Vector3(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f));
		CAnimationState* pState = pController->CreateAnimationState("Refill Idle");
		pState->AddClip("Refill Idle", AO_LOOP, 0.5f, "Gameplay", L"Celeste\\Gameplay.png");
		pController->SetEntryState("Refill Idle");
		pState->GetClip()->AddCustomInfo("objects\\refill\\idle00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\idle01");
		pState->GetClip()->AddCustomInfo("objects\\refill\\idle02");
		pState->GetClip()->AddCustomInfo("objects\\refill\\idle03");
		pState->GetClip()->AddCustomInfo("objects\\refill\\idle04");

		m_pBaseAnimator = pAnimator;
		m_pBaseRenderer = pRenderer;

		SAFE_RELEASE(pAnimator);
		SAFE_RELEASE(pRenderer);
	}

	{
		CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial* pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_POINT);
		SAFE_RELEASE(pMaterial);

		CAnimator* pAnimator = m_pGameObject->AddComponent<CAnimator>("Animator");
		pAnimator->SetTargetRenderer(pRenderer);
		CAnimationController* pController = new CAnimationController;
		pAnimator->SetAnimationController(pController);
		pAnimator->SetSyncScale(false);
		CAnimationState* pState = pController->CreateAnimationState("Refill Glow");
		pState->AddClip("Refill Glow", AO_LOOP, 3.0f, "Gameplay", L"Celeste\\Gameplay.png");
		pController->SetEntryState("Refill Glow");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash00");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash01");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash02");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash03");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash04");
		pState->GetClip()->AddCustomInfo("objects\\refill\\flash05");

		m_pEffectAnimator = pAnimator;		
		m_pEffectRenderer = pRenderer;
		SAFE_RELEASE(pAnimator);
		SAFE_RELEASE(pRenderer);
	}


	CRectCollider2D* pCollider = m_pGameObject->AddComponent<CRectCollider2D>("Collider");
	Vector3 vScale(40.0f, 40.0f, 1.0f);
	pCollider->SetScale(vScale);
	pCollider->SetPivot(0.5f, 0.5f, 0.0f);
	SAFE_RELEASE(pCollider);

	return true;
}

int CRefill::Update(float fTime)
{
	if (!m_bInitialized)
	{
		m_vOriginPosition = m_pTransform->GetWorldPosition();
	
		m_bInitialized = true;	
	}

	if (m_eState == RS_REGENERATION)
	{
		m_fScalingTimer += fTime;
		m_fLocalScale = 40.0f + sinf(PI * m_fFrequency * m_fScalingTimer) * m_fStartAmplitude * (1.0f - m_fScalingTimer);

		m_pTransform->SetWorldScale(m_fLocalScale, m_fLocalScale,1.0f);

		if (m_fScalingTimer >= 1.0f)
		{
			m_fScalingTimer = 0.0f;
			m_eState = RS_IDLE;
			m_pTransform->SetWorldScale(40.0f, 40.0f, 1.0f);
		}
	}
	
	if (m_bDisable)
	{
		m_fRegenerationTimer += fTime;

		if (m_fRegenerationTimer >= 4.0f)
		{
			Regeneration();
		}
	}
	else
	{
		m_fTimer += fTime * 0.25f;

		if (m_fTimer >= 1.0f)
		{
			m_fTimer -= 1.0f;
		}

		m_fParticleTimer += fTime;

		if (m_fParticleTimer >= 0.5f)
		{
			m_fParticleTimer -= 0.5f;

			Vector3 position = m_pTransform->GetWorldPosition();
			Vector3 targetPosition = position + Vector3::RandomDirection() * 75.0f;
			Vector4 color = Vector4(RandomRange(150.0f, 255.0f) / 255.0f, 1.0f, RandomRange(150.0f, 255.0f) / 255.0f, 1.0f);

			CGameObject* pParticle = GET_SINGLE(CGameData)->CreateParticle(m_pLayer, position, targetPosition, color, true, true, 1.0f, 3.0f, 20.0f);
			SAFE_RELEASE(pParticle);
		}


		m_pTransform->SetWorldPosition(m_vOriginPosition + Vector3(0.0f, sinf(2.0f * PI * m_fTimer) * 5.0f, 0.0f));

	}
	return 0;
}

int CRefill::OnCollisionEnter(CCollider * pThis, CCollider * pTarget, float fTime)
{
	CGameObject* pTargetObject = pTarget->GetGameObject();
	string strTargetTag = pTargetObject->GetTag();
	
	if (strTargetTag == "Madeline" && !m_bDisable)
	{
		CPlayer* pPlayer = pTargetObject->GetComponentFromType<CPlayer>(CT_PLAYER);
		pPlayer->ReduceDash();
		pPlayer->SetHairColor();
	
		GET_SINGLE(CSoundManager)->Play("game_gen_touchswitch_any"); 
		GET_SINGLE(CSoundManager)->Play("ui_game_increment_dashcount");

		SetDisable();
		SAFE_RELEASE(pPlayer);
	
		for (int i = 0; i < 15; ++i)
		{
			Vector3 position = m_pTransform->GetWorldPosition();
			Vector3 targetPosition = position + Vector3::RandomDirection() * RandomRange(70.0f, 75.0f);
			Vector4 color = Vector4(RandomRange(150.0f, 255.0f) / 255.0f, 1.0f, RandomRange(150.0f, 255.0f) / 255.0f, 1.0f);
	
			CGameObject* pParticle = GET_SINGLE(CGameData)->CreateParticle(m_pLayer, position, targetPosition, color, true, true, 1.0f, 3.0f, RandomRange(100.0f, 120.0f));
			SAFE_RELEASE(pParticle);
		}
	}
	
	SAFE_RELEASE(pTargetObject);
	return 0;
}
