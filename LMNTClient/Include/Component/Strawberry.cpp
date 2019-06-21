#include "Strawberry.h"
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
#include "SoundManager.h"
#include "Player.h"


CStrawberry::CStrawberry():
	m_bInitialized(false),
	m_fSafeTimer(0.0f),
	m_fTimer(0.0f),
	m_fParticleTimer(0.0f),
	m_eStrawberryType(BT_NORMAL),
	m_eStrawberryState(SS_IDLE),
	m_pPlayer(NULL),
	m_fDistance(100.0f),
	m_bSafe(false)


{
}


CStrawberry::~CStrawberry()
{
}

void CStrawberry::StartTimer()
{
	m_bSafe = true;
	m_fSafeTimer = 0.0f;
}

void CStrawberry::SetStrawberryType(BERRY_TYPE eType)
{
	m_eStrawberryType = eType;
	m_bInitialized = false;
}

bool CStrawberry::Initialize()
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
	pAnimator->SetFixedScaleFactor(Vector3(4.0f, 4.0f, 1.0f));
	CAnimationState* pState = pController->CreateAnimationState("Strawberry Idle");
	pState->AddClip("Strawberry Idle", AO_LOOP, 0.5f, "Gameplay", L"Celeste\\Gameplay.png");
	pController->SetEntryState("Strawberry Idle");
	pState->GetClip()->AddCustomInfo("collectables\\strawberry\\normal00");
	pState->GetClip()->AddCustomInfo("collectables\\strawberry\\normal01");
	pState->GetClip()->AddCustomInfo("collectables\\strawberry\\normal02");
	pState->GetClip()->AddCustomInfo("collectables\\strawberry\\normal03");
	pState->GetClip()->AddCustomInfo("collectables\\strawberry\\normal04");
	pState->GetClip()->AddCustomInfo("collectables\\strawberry\\normal05");
	pState->GetClip()->AddCustomInfo("collectables\\strawberry\\normal06");

	//pController->CreateAnimationState("Wing Strawberry Idle");
	//pState->AddClip("Wing Strawberry Idle", AO_LOOP, 0.5f, "Gameplay", L"Celeste\\Gameplay.png");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings00");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings01");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings02");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings03");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings04");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings05");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings06");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings07");
	//pState->GetClip()->AddCustomInfo("collectables\\strawberry\\wings08");
	//
	//pController->CreateAnimationState("Golden Strawberry Idle");
	//pState->AddClip("Golden Strawberry Idle", AO_LOOP, 0.5f, "Gameplay", L"Celeste\\Gameplay.png");
	//pState->GetClip()->AddCustomInfo("goldberry\\idle00");
	//pState->GetClip()->AddCustomInfo("goldberry\\idle01");
	//pState->GetClip()->AddCustomInfo("goldberry\\idle02");
	//pState->GetClip()->AddCustomInfo("goldberry\\idle03");
	//pState->GetClip()->AddCustomInfo("goldberry\\idle04");
	//pState->GetClip()->AddCustomInfo("goldberry\\idle05");
	//
	//pController->CreateAnimationState("Golden Wing Strawberry Idle");
	//pState->AddClip("Golden Wing Strawberry Idle", AO_LOOP, 0.5f, "Gameplay", L"Celeste\\Gameplay.png");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings00");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings01");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings02");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings03");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings04");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings05");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings06");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings07");
	//pState->GetClip()->AddCustomInfo("collectables\\goldberry\\wings08");
														  
	m_pAnimator = pAnimator;
	SAFE_RELEASE(pAnimator);
	SAFE_RELEASE(pRenderer);





	CRectCollider2D* pCollider = m_pGameObject->AddComponent<CRectCollider2D>("Collider");
	Vector3 vScale(40.0f, 40.0f, 1.0f);
	pCollider->SetScale(vScale);
	pCollider->SetPivot(0.5f, 0.5f, 0.0f);
	SAFE_RELEASE(pCollider);


	return true;
}

int CStrawberry::Update(float fTime)
{
	if (!m_bInitialized)
	{
		m_vOriginPosition = m_pTransform->GetWorldPosition();
		switch (m_eStrawberryType)
		{
		case BT_NORMAL:
			m_pAnimator->GetAnimationController()->TransitionState("Strawberry Idle");
			break;
		case BT_WING:
			m_pAnimator->GetAnimationController()->TransitionState("Wing Strawberry Idle");
			break;
		case BT_GOLDEN:
			m_pAnimator->GetAnimationController()->TransitionState("Golden Strawberry Idle");
			break;
		case BT_GOLDEN_WING:
			m_pAnimator->GetAnimationController()->TransitionState("Golden Wing Strawberry Idle");
			break;
		case BT_GHOST_NORMAL:
			m_pAnimator->GetAnimationController()->TransitionState("Strawberry Idle");
			break;
		case BT_GHOST_WING:
			m_pAnimator->GetAnimationController()->TransitionState("Strawberry Idle");
			break;
		case BT_GHOST_GOLDEN_NORMAL:
			m_pAnimator->GetAnimationController()->TransitionState("Strawberry Idle");
			break;
		case BT_GHOST_GOLDEN_WING:
			m_pAnimator->GetAnimationController()->TransitionState("Strawberry Idle");
			break;
		default:
			m_pAnimator->GetAnimationController()->TransitionState("Strawberry Idle");
			break;
		}
		m_bInitialized = true;
	}



	if (m_eStrawberryState == SS_IDLE)
	{
		m_fParticleTimer += fTime;

		if (m_fParticleTimer >= 0.5f)
		{
			m_fParticleTimer -= 0.5f;

			Vector3 position = m_pTransform->GetWorldPosition();
			Vector3 targetPosition = position + Vector3::RandomDirection() * 75.0f;
			Vector4 color = Vector4(1.0f, RandomRange(150.0f, 255.0f) / 255.0f, RandomRange(150.0f, 255.0f) / 255.0f, 1.0f);

			CGameObject* pParticle = GET_SINGLE(CGameData)->CreateParticle(m_pLayer, position, targetPosition, color, true, true, 1.0f, 3.0f, 20.0f);
			SAFE_RELEASE(pParticle);
		}
		m_pTransform->SetWorldPosition(m_vOriginPosition + Vector3(0.0f, sinf(2.0f * PI * m_fTimer) * 5.0f, 0.0f));
	}


	if (m_eStrawberryState == SS_FOLLOW)
	{
		if (m_pPlayer)
		{
			CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
			bool bOnGround = pPlayer->OnGround();

			bool bSpawnded = pPlayer->GetSpawned();
			SAFE_RELEASE(pPlayer);

			if (bSpawnded)
			{
				if (!m_bSafe && bOnGround)
				{
					StartTimer();
				}

				if (m_bSafe && !bOnGround)
				{
					m_bSafe = false;
					m_fSafeTimer = 0.0f;
				}

				Vector3 position = m_pTransform->GetWorldPosition();
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				Vector3 playerPosition = pPlayerTransform->GetWorldPosition();
				SAFE_RELEASE(pPlayerTransform);

				if ((playerPosition - position).Length() >= m_fDistance)
				{
					//Vector3 targetPosition = (playerPosition - position).Normalize() * 200.0f * fTime;
					Vector3 movePosition = XMVectorLerp(position, playerPosition, 0.75f * fTime);
					m_pTransform->SetWorldPosition(movePosition);
				}
			}
			else
			{
				Vector3 position = m_pTransform->GetWorldPosition();
				if ((m_vOriginPosition - position).Length() >= 10.0f)
				{
					//Vector3 targetPosition = (playerPosition - position).Normalize() * 200.0f * fTime;
					Vector3 movePosition = XMVectorLerp(position, m_vOriginPosition, 5.0f * fTime);
					m_pTransform->SetWorldPosition(movePosition);
				}
				else
				{
					m_pTransform->SetWorldPosition(m_vOriginPosition);
					m_eStrawberryState = SS_IDLE;
				}
			}
		}
	}


	if (m_pPlayer)
	{
		CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
		bool bAreaChanged = pPlayer->GetAreaChanged();
		SAFE_RELEASE(pPlayer);

		if (m_eStrawberryState == SS_FOLLOW && bAreaChanged)
		{			
			m_eStrawberryState = SS_GET;
		}
	}

	if (m_bSafe)
	{
		m_fSafeTimer += fTime;

		if (m_fSafeTimer >= 0.5f)
		{
			m_eStrawberryState = SS_GET;
		}
	}



	if (m_eStrawberryState == SS_GET)
	{
		m_pGameObject->Destroy(true);

		GET_SINGLE(CSoundManager)->Play("game_gen_strawberry_red_get_5000");
		for (int i = 0; i < 15; ++i)
		{
			Vector3 position = m_pTransform->GetWorldPosition();
			Vector3 targetPosition = position + Vector3::RandomDirection() * RandomRange(70.0f, 75.0f);
			Vector4 color = Vector4(1.0f, RandomRange(150.0f, 255.0f) / 255.0f, RandomRange(150.0f, 255.0f) / 255.0f, 1.0f);

			CGameObject* pParticle = GET_SINGLE(CGameData)->CreateParticle(m_pLayer, position, targetPosition, color, true, true, 1.0f, 3.0f, RandomRange(100.0f, 120.0f));
			SAFE_RELEASE(pParticle);
		}
	}


	return 0;
}

int CStrawberry::OnCollisionEnter(CCollider * pThis, CCollider * pTarget, float fTime)
{
	CGameObject* pTargetObject = pTarget->GetGameObject();

	if (pTargetObject->GetTag() == "Madeline" && m_eStrawberryState == SS_IDLE)
	{
		m_pPlayer = pTargetObject;
		m_eStrawberryState = SS_FOLLOW;

		GET_SINGLE(CSoundManager)->Play("game_gen_strawberry_touch");
		GET_SINGLE(CSoundManager)->Play("game_gen_touchswitch_any");
	}

	SAFE_RELEASE(pTargetObject);
	return 0;
}
