#include "Player.h"
#include "Transform.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "AnimationState.h"
#include "Input.h"
#include "Layer.h"
#include "Component/Text.h"
#include "Component/Animator.h"
#include "AnimationController.h"
#include "AnimationState.h"
#include "AnimationClip2D.h" 
#include "SceneManager.h"
#include "Collider.h"
#include "../GameData.h"
#include "Component/Renderer.h"
#include "Dummy.h"
#include "SoundManager.h"
#include "Crumble.h"

const float fPlayerSize = 50.0f;
const float fTileSize = 32.0f;

CPlayer::CPlayer() :
	m_bInitialized(false),
	m_fDebugTimer(0),
	m_bJump(false),
	m_bJumpAuto(false),
	m_eState(AS_IDLE),
	m_bAffactGravity(false),
	m_fMoveRightPressTimer(0),
	m_fMoveLeftPressTimer(0),
	m_bPushable(false),
	m_bOnClimb(false),
	m_bOnSlide(false),
	m_bMoveAuto(false),
	m_bHeadColliding(false),
	m_eClimbDirection(DIR_CENTER),
	m_bDeath(false),
	m_bSpawned(true),
	m_bAwake(false),
	m_bEvent(false),
	m_bBoostable(false),
	m_pClimbWall(NULL),
	m_pArea(NULL),
	m_fAcceleration(2800.0f),
	m_bAutoMoveScale(1.0f),
	m_fGravityThreshold(-650.0f),
	m_bInteraction(false),
	m_iDashCountMax(0)
{
	m_eComponentType = (int)CT_PLAYER;
	SetTag("Player");
	
}

CPlayer::CPlayer(const CPlayer & component) :
	CComponent(component)
{
	*this = component;
}


CPlayer::~CPlayer()
{
	int i = 0;
}

PAREA CPlayer::GetCurrentArea() const
{
	return m_pArea;
}

bool CPlayer::GetAreaChanged() const
{
	return m_bAreaChanged;
}

bool CPlayer::GetSpawned() const
{
	return m_bSpawned;
}

void CPlayer::SetBoost(bool bBoost)
{
	m_bBoostable = bBoost;
}

void CPlayer::ResetPotentialForce()
{
	m_vPotential = Vector3::Zero;
}

void CPlayer::SetPotentialForce(const Vector3 & fForce)
{
	m_vPotential = fForce;
}

void CPlayer::SetBeginPosition(const Vector3 & vPosition)
{
	m_vBeginPosition = vPosition;
}

void CPlayer::AddHair(CGameObject * pHair, bool bShadow)
{
	if (!bShadow)
		m_vecHair.push_back(pHair);
	else
		m_vecHairShadow.push_back(pHair);
}

void CPlayer::SetHairColor()
{
	if (m_iDashCount == 0)
	{
		int iSize = m_vecHair.size();
		for (int i = 0; i < iSize; ++i)
		{
			CRenderer* pRenderer = m_vecHair[i]->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(MADLINE_HAIR_COLOR_NORMAL);
			m_vHairColor = MADLINE_HAIR_COLOR_NORMAL;
			SAFE_RELEASE(pRenderer);
		}
	}
	else if(m_iDashCount == 1)
	{
		int iSize = m_vecHair.size();
		for (int i = 0; i < iSize; ++i)
		{
			CRenderer* pRenderer = m_vecHair[i]->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(MADLINE_HAIR_COLOR_SINGLE_DASH);
			m_vHairColor = MADLINE_HAIR_COLOR_SINGLE_DASH;
			SAFE_RELEASE(pRenderer);
		}
	}
	else
	{
		int iSize = m_vecHair.size();
		for (int i = 0; i < iSize; ++i)
		{
			CRenderer* pRenderer = m_vecHair[i]->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(MADLINE_HAIR_COLOR_DOUBLE_DASH);
			m_vHairColor = MADLINE_HAIR_COLOR_DOUBLE_DASH;
			SAFE_RELEASE(pRenderer);
		}
	}
}

void CPlayer::AddForce(float fForce)
{
	m_fForce += fForce;
}

void CPlayer::SetScaling(const Vector3 & vBeginScale)
{
	CAnimator* pAnimator = m_pGameObject->GetComponentFromType<CAnimator>(CT_ANIMATOR);
	pAnimator->SetLocalScaleFactor(vBeginScale);
	SAFE_RELEASE(pAnimator);
	m_bOnScaling = true;
	m_vBeginScale = vBeginScale;
}

void CPlayer::UpdateScaling(float fTimeScale, float fDeltaTime)
{
	CAnimator* pAnimator = m_pGameObject->GetComponentFromType<CAnimator>(CT_ANIMATOR);
	Vector3 vCurrentScale = pAnimator->GetLocalScaleFactor();
	Vector3 vDistanceToDefault = vCurrentScale - Vector3::One;
	if (vDistanceToDefault.x <= 0.1f && vDistanceToDefault.y <= 0.1f)
	{
		m_bOnScaling = false;
		m_vBeginScale = Vector3::One;
		pAnimator->SetLocalScaleFactor(Vector3::One);
		SAFE_RELEASE(pAnimator);
		return;
	}

	pAnimator->SetLocalScaleFactor(Vector3::Lerp(vCurrentScale, Vector3::One, fDeltaTime * fTimeScale));
	SAFE_RELEASE(pAnimator);
}



float CPlayer::DashAcceleration(float fElapseTime)
{
	return (4 * m_fDashDistance * (m_fDashDuration - 2 * fElapseTime)) / (m_fDashDuration*m_fDashDuration);
}

void CPlayer::CreateDummy(const Vector4 & vColor)
{
	CAnimator* pAnimator = m_pGameObject->GetComponentFromType<CAnimator>(CT_ANIMATOR);
	CAnimationClip2D* pClip = pAnimator->GetAnimationController()->GetCurrentState()->GetClip();
	unsigned int iFrame = pClip->GetCurrentFrameCount();
	string strAtlasKey = pClip->GetAtlasKey(iFrame);	
	SAFE_RELEASE(pAnimator);

	CLayer* pVFXLayer = m_pScene->FindLayer("VFX0");
	//Hair프레임, 위치 필요함.
	CGameObject* pDummy = CGameObject::CreateObject("Dummy", pVFXLayer);
	SAFE_RELEASE(pVFXLayer);

	CTransform* pDummyTransform = pDummy->GetTransform();// 복사
	pDummyTransform->SetWorldPosition(m_pTransform->GetWorldPosition());
	pDummyTransform->SetWorldScale(m_pTransform->GetWorldScale());
	pDummyTransform->SetWorldRotation(m_pTransform->GetWorldRotation());
	pDummyTransform->SetPivot(m_pTransform->GetPivot());
	SAFE_RELEASE(pDummyTransform);	
	
	CRenderer* pRenderer = pDummy->AddComponent<CRenderer>("Dummy");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(COLORED_TEXTURE_SHADER); // 쉐이더 별도 설정 필요함.
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CDummy* pDummyComponent = pDummy->AddComponent<CDummy>("Dummy");
	pDummyComponent->SetRenderer(pRenderer);
	SAFE_RELEASE(pDummyComponent);

	CMaterial* pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	pRenderer->SetImageInfo(strAtlasKey);
	SAFE_RELEASE(pMaterial);

	pRenderer->SetDiffuse(vColor);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pDummy);

}

bool CPlayer::OnGround() const
{
	return m_bOnGround;
}

bool CPlayer::OnDash() const
{
	return m_bOnDash;
}

bool CPlayer::GetInteraction() const
{
	return m_bInteraction;
}

void CPlayer::SetInteraction(bool bInteraction)
{
	m_bInteraction = bInteraction;
}


float CPlayer::JumpWeight(float fDeltaTime)
{
	m_fJumpPressTimer += fDeltaTime;

	if (m_fJumpPressTimer <= m_fJumpPressTimerMinThreshold)
	{
		return m_fJumpPressTimerMinThreshold / m_fJumpPressTimerMaxThreshold;
	}
	if (m_fJumpPressTimer >= m_fJumpPressTimerMaxThreshold)
	{
		return 1.0f;
	}

	return m_fJumpPressTimer / m_fJumpPressTimerMaxThreshold;
}

float CPlayer::MoveRightWeight()
{
	if (m_fMoveRightPressTimer <= m_fMoveAxisWeightMinThreshold)
	{
		return m_fMoveAxisWeightMinThreshold / m_fMoveAxisWeightMaxThreshold;
	}
	if (m_fMoveRightPressTimer >= m_fMoveAxisWeightMaxThreshold)
	{
		return 1.0f;
	}
	return m_fMoveRightPressTimer / m_fMoveAxisWeightMaxThreshold;
}

float CPlayer::MoveLeftWeight()
{
	if (m_fMoveLeftPressTimer <= m_fMoveAxisWeightMinThreshold)
	{
		return m_fMoveAxisWeightMinThreshold / m_fMoveAxisWeightMaxThreshold;
	}
	if (m_fMoveLeftPressTimer >= m_fMoveAxisWeightMaxThreshold)
	{
		return 1.0f;
	}
	return m_fMoveLeftPressTimer / m_fMoveAxisWeightMaxThreshold;
}

float CPlayer::MoveAcceleration(float fTime)
{
	float positiveDirection = MoveRightWeight();
	float negativeDirection = MoveLeftWeight();
	
	m_fMoveRightAcceleration += positiveDirection * m_fMoveMaxAcceleration * fTime;
	m_fMoveLeftAcceleration -= negativeDirection * m_fMoveMaxAcceleration * fTime;

	if (m_fMoveRightAcceleration >= m_fMoveMaxSpeed)
	{
		m_fMoveRightAcceleration = m_fMoveMaxSpeed;
	}
	if (m_fMoveLeftAcceleration <= -m_fMoveMaxSpeed)
	{
		m_fMoveLeftAcceleration = -m_fMoveMaxSpeed;
	}

	m_fMoveHorizontalAcceleration = m_fMoveHorizontalAccelerationOffset + m_fMoveRightAcceleration + m_fMoveLeftAcceleration;
	
	return m_fMoveHorizontalAcceleration;
}

void CPlayer::SetBottomCollider(CCollider * pCollider)
{
	m_pBodyBottom = pCollider;
}

void CPlayer::SetTopCollider(CCollider * pCollider)
{
	m_pBodyTop = pCollider;
}

void CPlayer::SetLeftCollider(CCollider * pCollider)
{
	m_pBodyLeft = pCollider;
}

void CPlayer::SetRightCollider(CCollider * pCollider)
{
	m_pBodyRight = pCollider;
}

void CPlayer::ResetJumpOptions()
{
	m_fJumpHeight = fTileSize * 3.5f;
	m_fJumpDuration = 0.65f; // 0.65
}

void CPlayer::RenderHair(bool bRender)
{
	int iSize = m_vecHair.size();
	for (int i = 0; i < iSize; ++i)
	{
		CRenderer* pRenderer = m_vecHair[i]->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->Enable(bRender);
		SAFE_RELEASE(pRenderer);
	}
	iSize = m_vecHairShadow.size();
	for (int i = 0; i < iSize; ++i)
	{
		CRenderer* pRenderer = m_vecHairShadow[i]->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->Enable(bRender);
		SAFE_RELEASE(pRenderer);
	}
}

void CPlayer::JumpForced(float fForce, bool bAutoMove, float fAutoMoveScale, bool bTrailingForce)
{
	m_fJumpTimer = 1.0f;
	m_bAffactGravity = false;
	m_bJump = true;
	m_bOnGround = false;
	m_fForce = 0.0f;
	
	//m_vPotential = Vector3::Zero;
	m_fTotalJumpForce = 0.0;
	m_fBeginJumpHeight = m_pTransform->GetWorldPosition().y;
	m_eState = AS_JUMP_FAST_UP;
	m_bOnSlide = false;
	m_bOnClimb = false;
	m_bPushable = false;
	m_bMoveAuto = bAutoMove;
	m_bAutoMoveScale = fAutoMoveScale;
	m_bJumpAuto = !bTrailingForce;
	m_pClimbWall = NULL;
	AddForce(fForce);
}

void CPlayer::SetMaxDash(int iCount)
{
	m_iDashCountMax = iCount;
}

void CPlayer::ReduceDash()
{
	m_iDashCount -= 1;

	if (m_iDashCount < 0)
	{
		m_iDashCount = 0;
	}
}

void CPlayer::SetAnimationState(PLAYER_ANIMATION_STATE eState)
{
	m_eState = eState;
}

void CPlayer::SetEvent(bool bEvent)
{
	m_bEvent = bEvent;
}

void CPlayer::Reset()
{
	RenderHair(true);

	m_bBoostable = false;
	m_bOnDash = false;
	//m_iDashCountMax = 0;
	m_iDashCount = 0;
	m_fDashDistance = 215;
	m_fDashDuration = 0.72f; // 0.72;
	m_fDashTimer = 0;
	m_fDashEffectTimer = 0;
	m_fDashEffectTimerMax = 0.06f;

	//m_pHair(NULL),
	m_bJump = false;
	m_bJumpAuto=false;
	m_eState=AS_IDLE;
	m_bAffactGravity=false;
	m_fMoveRightPressTimer=0;
	m_fMoveLeftPressTimer=0;
	m_bPushable=false;
	m_bOnClimb=false;
	m_bOnSlide=false;
	m_bMoveAuto=false;
	m_bHeadColliding=false;
	m_bDeath=false;
	m_bSpawned=true;
	m_bAwake = false;
	//캐릭터 50px 기준
	//최대 150
	//최소 25
	ResetJumpOptions();
	m_fJumpMinWeight = 0.2f;

	m_fJumpPressTimerMinThreshold = 0.0f;
	m_fJumpPressTimerMaxThreshold = 1.0f;
	m_bOnGround = false;
	m_bAffactGravity = false;
	//초당 이동 거리
	m_fMoveMaxAcceleration = fTileSize * 100.0f;
	m_fMoveHorizontalAccelerationOffset = 0;
	m_fMoveHorizontalAcceleration = 0;
	m_fMoveRightAcceleration = 0;
	m_fMoveLeftAcceleration = 0;
	m_fMoveMaxSpeed = fTileSize * 11.0f;
	m_fMoveAxisWeightMinThreshold = 0.0f;
	m_fMoveAxisWeightMaxThreshold = 0.1f;

	m_fClimbSpeed = 150.0f;
	m_fSlideSpeed = 100.0f;

	m_pTransform->SetWorldRotation(Vector3(0, 0, 0));
	m_pTransform->SetWorldPosition(m_vCheckPoint);
	

	SetHairColor();
}

void CPlayer::ResetCurrentPosition()
{
	RenderHair(true);

	m_bBoostable = false;
	m_bOnDash = false;
	//m_iDashCountMax = 0;
	m_iDashCount = 0;
	m_fDashDistance = 215;
	m_fDashDuration = 0.72f; // 0.72;
	m_fDashTimer = 0;
	m_fDashEffectTimer = 0;
	m_fDashEffectTimerMax = 0.06f;

	//m_pHair(NULL),
	m_bJump = false;
	m_bJumpAuto = false;
	m_eState = AS_IDLE;
	m_bAffactGravity = false;
	m_fMoveRightPressTimer = 0;
	m_fMoveLeftPressTimer = 0;
	m_bPushable = false;
	m_bOnClimb = false;
	m_bOnSlide = false;
	m_bMoveAuto = false;
	m_bHeadColliding = false;
	m_bDeath = false;
	m_bSpawned = true;
	m_bAwake = false;
	//캐릭터 50px 기준
	//최대 150
	//최소 25
	ResetJumpOptions();
	m_fJumpMinWeight = 0.2f;

	m_fJumpPressTimerMinThreshold = 0.0f;
	m_fJumpPressTimerMaxThreshold = 1.0f;
	m_bOnGround = false;
	m_bAffactGravity = false;
	//초당 이동 거리
	m_fMoveMaxAcceleration = fTileSize * 100.0f;
	m_fMoveHorizontalAccelerationOffset = 0;
	m_fMoveHorizontalAcceleration = 0;
	m_fMoveRightAcceleration = 0;
	m_fMoveLeftAcceleration = 0;
	m_fMoveMaxSpeed = fTileSize * 11.0f;
	m_fMoveAxisWeightMinThreshold = 0.0f;
	m_fMoveAxisWeightMaxThreshold = 0.1f;

	m_fClimbSpeed = 150.0f;
	m_fSlideSpeed = 100.0f;
	SetHairColor();
}

void CPlayer::ResetDashCount()
{
	m_iDashCount = 0;
	SetHairColor();

}

void CPlayer::ResetPhysics()
{
	m_fMoveRightPressTimer = 0;
	m_fMoveLeftPressTimer = 0;
	m_fMoveMaxAcceleration = fTileSize * 100.0f;
	m_fMoveHorizontalAccelerationOffset = 0;
	m_fMoveHorizontalAcceleration = 0;
	m_fMoveRightAcceleration = 0;
	m_fMoveLeftAcceleration = 0;
	m_fMoveMaxSpeed = fTileSize * 11.0f;//11
	m_fMoveAxisWeightMinThreshold = 0.0f;
	m_fMoveAxisWeightMaxThreshold = 0.1f;
}

void CPlayer::SetAwake(bool bAwake)
{
	m_bAwake = bAwake;
}

bool CPlayer::Initialize()
{	
	m_pArea = GET_SINGLE(CGameData)->GetAreaData(m_pTransform->GetWorldPosition());
	return true;
}

int CPlayer::Input(float fTime)
{
	return 0;
}

int CPlayer::Update(float fTime)
{
	if (!m_bInitialized)
	{
		Reset();
		m_bInitialized = true;
		m_pTransform->SetWorldPosition(m_vBeginPosition);
	}

	m_bAreaChanged = GET_SINGLE(CGameData)->UpdateAreaData();

	if (m_bAreaChanged)
	{
		m_pArea = GET_SINGLE(CGameData)->GetCurrentArea();
		m_vCheckPoint = GET_SINGLE(CGameData)->GetCheckPointPosition(m_pArea, m_pTransform->GetWorldPosition());
	}

	if (GETKEYDOWN(VK_F6))
	{
		Reset();
	}

	CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
	Vector3 CameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CText* pText = m_pDebugText->GetComponentFromType<CText>(CT_TEXT);
	CAnimator* pAnimator = m_pGameObject->GetComponentFromType<CAnimator>(CT_ANIMATOR);



	if (!m_bAwake && !m_bEvent)
	{
		if(m_bSpawned)
			m_pTransform->SetWorldPosition(m_vCheckPoint);


		if (GETKEY(VK_LEFT) || GETKEY(VK_RIGHT) || GETKEY(VK_UP) || GETKEY(VK_DOWN) || GETKEY('Z') || GETKEY('X') || GETKEY('C'))
		{
			m_bAwake = true;
		}
	}
	else if (m_bEvent)
	{

	}
	else
	{
		if (m_bSpawned)
		{
#pragma region Boost Buffer
			{
				if (!m_bBoostable && !m_bPrevBoostable && !m_bBoostBuff)
				{
					m_bAffactBoostable = false;
				}
				if (m_bBoostable)
				{
					m_bAffactBoostable = true;
				}
				//블럭에서 떨어졌을 떄 유예시간을 주도록 함.
				if (m_bBoostable && m_bBoostable != m_bPrevBoostable)
				{
					m_bBoostBuff = true;
					m_fBoostBuffTimer = 0.0f;
				}

				if (m_bBoostBuff)
				{
					m_fBoostBuffTimer += fTime;

					if (m_fBoostBuffTimer >= 0.25f)
					{
						m_bBoostBuff = false;

						if (m_bBoostable)
						{
							m_bAffactBoostable = true;
						}
						else
						{
							m_bAffactBoostable = false;
						}
					}
					else
					{
						m_bAffactBoostable = true;
					}
				}
			}
#pragma endregion


#pragma region Scaling
			{
				if (m_bOnScaling)
				{
					UpdateScaling(8.0f, fTime);
				}
			}
#pragma endregion

#pragma region Move

			if (GETKEYDOWN(VK_RIGHT) && !m_bOnDash && !m_bOnClimb)
			{
				if (m_bJump || m_bJumpAuto)
				{
					m_pTransform->SetWorldRotation(Vector3(0.0f, 0.0f, 0.0f));
				}
				else
				{
					m_eState = AS_RUN_FAST;
					m_pTransform->SetWorldRotation(Vector3(0.0f, 0.0f, 0.0f));
				}
			}
			if (GETKEY(VK_RIGHT) && !m_bOnClimb)
			{
				if (!m_bOnDash && m_pTransform->GetWorldRotation() == Vector3(0.0f, PI, 0.0f))
				{
					m_pTransform->SetWorldRotation(Vector3(0.0f, 0.0f, 0.0f));
				}

				m_fMoveRightPressTimer += fTime;
			}
			if (GETKEYDOWN(VK_LEFT) && !m_bOnDash && !m_bOnClimb)
			{
				if (m_bJump || m_bJumpAuto)
				{
					//점프중에는 방향만 전환함
					m_pTransform->SetWorldRotation(Vector3(0.0f, PI, 0.0f));
				}
				else
				{
					m_eState = AS_RUN_FAST;
					m_pTransform->SetWorldRotation(Vector3(0.0f, PI, 0.0f));
				}
			}
			if (GETKEY(VK_LEFT) && !m_bOnClimb)
			{
				if (!m_bOnDash && m_pTransform->GetWorldRotation() == Vector3(0.0f, 0.0f, 0.0f))
				{
					m_pTransform->SetWorldRotation(Vector3(0.0f, PI, 0.0f));
				}

				m_fMoveLeftPressTimer += fTime;
			}

			if (GETKEYUP(VK_RIGHT))
			{
				m_fMoveRightPressTimer = 0.0f;
				m_fMoveRightAcceleration = 0.0f;
				m_eState = AS_IDLE;
			}
			if (GETKEYUP(VK_LEFT))
			{
				m_fMoveLeftPressTimer = 0.0f;
				m_fMoveLeftAcceleration = 0.0f;
				m_eState = AS_IDLE;
			}

			if (!m_bOnDash)
			{
				float fAcceleration = MoveAcceleration(fTime) * fTime;
				Vector3 position = m_pTransform->GetWorldPosition();
				m_pTransform->SetWorldPosition(position + Vector3(fAcceleration, 0.0f, 0.0f));
			}
#pragma endregion

#pragma region Climb
			{
				bool bRight = m_pTransform->GetWorldRotation().y < PI * 0.5f;


				//잡고 있는 벽이 이동하는 벽일 경우, 이동 거리를 동기화 시킨다.
				if (m_pClimbWall)
				{
					if (m_pClimbWall->GetTag().find("Zip") == string::npos)
					{
						CTransform* pWallTransform = m_pClimbWall->GetTransform();
						Vector3 vWallDeltaPosition = pWallTransform->GetDeltaPosition();
						SAFE_RELEASE(pWallTransform);
						m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() + vWallDeltaPosition);
					}
				}


				m_pBodyLeft->Straighten();
				m_pBodyRight->Straighten();

				//벽에 닿아 있을 떄
				if (m_bPushable)
				{
					if (m_bOnGround)
					{
						if ((GETKEY(VK_LEFT) && !bRight) || (GETKEY(VK_RIGHT) && bRight))
						{
							m_eState = AS_PUSH;
						}
						else
						{
							m_eState = AS_IDLE;
						}
					}
					else
					{
						//Climb
						if (GETKEY('Z'))
						{
							m_bOnClimb = true;
						}
						else
						{
							m_bOnClimb = false;
						}

						//벽쪽으로 진행하면 Slide / Climb 가능
						if ((GETKEY(VK_LEFT) && !bRight) || (GETKEY(VK_RIGHT) && bRight))
						{
							//Sliding
							m_bOnSlide = true;
						}
						else
						{
							if (!m_bJump && !m_bJumpAuto)
							{
								m_bAffactGravity = true;
							}
							m_bOnSlide = false;
						}
					}
				}//if (m_bPushable)
				else if (!m_bPushable && m_bOnClimb && m_eClimbDirection == DIR_TOP)
				{
					m_bOnClimb = false;
					m_bOnSlide = false;
					JumpForced(500.0f);
				}
				else
				{
					m_bOnClimb = false;
					m_bOnSlide = false;
				}


				//Climb 할 수 있을 때, 
				if (m_bOnClimb)
				{
					m_fForce = 0.0f;
					m_eState = AS_CLIMB_WALL_SLIDE;
					//둘 중 한쪽의 방향으로만 진행 가능하도록
					if (GETKEY(VK_UP))
					{
						m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() - Vector3(0.0f, m_fClimbSpeed * fTime, 0.0f));
						m_eClimbDirection = DIR_TOP;
						m_eState = AS_CLIMB_UP;
					}
					else if (GETKEY(VK_DOWN))
					{
						m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() + Vector3(0.0f, m_fClimbSpeed * fTime, 0.0f));
						m_eState = AS_CLIMB_UP;
						m_eClimbDirection = DIR_BOTTOM;
					}
					else if ((GETKEY(VK_RIGHT) && !bRight) || (GETKEY(VK_LEFT) && bRight))
					{
						m_eState = AS_CLIMB_LOOK_BACK;
						m_eClimbDirection = DIR_CENTER;
					}
					else
					{
						m_eClimbDirection = DIR_CENTER;
					}

					if (m_eState == AS_CLIMB_LOOK_BACK)
					{
						if (GETKEYDOWN('C'))
						{
							if (bRight)
							{
								m_pTransform->SetWorldRotation(Vector3(0.0f, PI, 0.0f));

								if (!m_bAffactBoostable)
									JumpForced(750.0f);
							}
							else if (!bRight)
							{
								m_pTransform->SetWorldRotation(Vector3(0.0f, 0.0f, 0.0f));
								if (!m_bAffactBoostable)
									JumpForced(750.0f);
							}
						}
					}
				}
				else if (m_bOnSlide)
				{
					m_fForce = 0.0f;
					m_eState = AS_CLIMB_WALL_SLIDE;
					m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() + Vector3(0.0f, m_fSlideSpeed * fTime, 0.0f));
				}
			}
#pragma endregion

#pragma region Jump
			{
				{
					float fWeight = 0;

					if (GETKEYDOWN('C') && !m_bJump && m_bOnGround && !m_bJumpAuto)
					{
						//GET_SINGLE(CGameData)->CreateFootDust(m_pTransform->GetWorldPosition());
						m_fJumpTimer = 0.0f;
						m_bAffactGravity = false;
						m_bJump = true;
						m_bOnGround = false;
						m_fForce = 0.0f;
						m_fTotalJumpForce = 0.0;
						m_fBeginJumpHeight = m_pTransform->GetWorldPosition().y;

						AddForce(250.0f);

						if (m_bAffactBoostable)
						{
							AddForce(200.0f + m_vPotential.y);
						}
						else
						{
							m_bAutoMoveScale = 0.8f;
						}

						m_eState = AS_JUMP_FAST_UP;
					}
					else if (GETKEYDOWN('C') && !m_bJump && !m_bOnGround &&m_bAffactBoostable && !m_bJumpAuto)
					{
						m_fJumpTimer = 0.0f;
						m_bAffactGravity = false;
						m_bJump = true;
						m_bOnGround = false;
						m_fForce = 0.0f;
						m_fTotalJumpForce = 0.0;
						m_fBeginJumpHeight = m_pTransform->GetWorldPosition().y;

						AddForce(250.0f);
						AddForce(200.0f + m_vPotential.y);

						m_eState = AS_JUMP_FAST_UP;
					}


					if (GETKEY('C'))
					{
						m_fJumpTimer += fTime;
						fWeight = JumpWeight(fTime);

						if (m_fJumpTimer <= 0.175f && !m_bJumpAuto)
						{
							AddForce((m_fAcceleration * 1.5f * fTime));
						}
					}

					if (m_bJump)
					{
						m_fForce -= m_fAcceleration * fTime;
						m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() - Vector3(0.0f, m_fForce * fTime, 0.0f));

						if (m_bMoveAuto)
						{
							//같은 비용이지만 가독성 떨어져서 방향 받아서 함
							bool bRight = m_pTransform->GetWorldRotation().y < PI * 0.5f;
							if (bRight)
							{
								m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() + Vector3(m_fMoveMaxSpeed * m_bAutoMoveScale * fTime, 0.0f, 0.0f));
							}
							else
							{
								m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() - Vector3(m_fMoveMaxSpeed * m_bAutoMoveScale * fTime, 0.0f, 0.0f));
							}
						}

						if (m_bAffactBoostable)
						{
							bool bRight = m_pTransform->GetWorldRotation().y < PI * 0.5f;
							m_vPotential.x = fabsf(m_vPotential.x);
							float fHorizontalForce = m_vPotential.x / m_fMoveMaxSpeed;
							//위에서 한번 이동 시키고, 추가로 이동시킴
							float fAcceleration = MoveAcceleration(fTime) * fTime;
							m_vPotential.x -= fAcceleration;
							Vector3 position = m_pTransform->GetWorldPosition();
							if (bRight)
							{
								m_pTransform->SetWorldPosition(position + Vector3(fAcceleration + fHorizontalForce * m_fMoveMaxSpeed * fTime, 0.0f, 0.0f));
							}
							else
							{
								m_pTransform->SetWorldPosition(position - Vector3(-fAcceleration + fHorizontalForce * m_fMoveMaxSpeed * fTime, 0.0f, 0.0f));
							}
						}

						if (m_fForce <= 0.0f)
						{
							m_bAffactBoostable = false;
							m_bBoostable = false;
							m_bJump = false;
							m_bJumpAuto = false;
							m_bAffactGravity = true;
							m_eState = AS_JUMP_SLOW_DOWN;
						}
					}
				}
			}
#pragma endregion

#pragma region Duck

			if (GETKEYDOWN(VK_DOWN) && m_eState == AS_IDLE)
			{
				m_eState = AS_DUCK;
			}

			if (GETKEYUP(VK_DOWN))
			{
				m_eState = AS_IDLE;
			}
#pragma endregion

#pragma region Push

			if (m_bOnGround && m_bPushable)
			{
				if (GETKEY(VK_LEFT) || GETKEY(VK_RIGHT))
				{
					m_eState = AS_PUSH;
				}
				else
				{
					m_eState = AS_IDLE;
				}
			}

#pragma endregion

#pragma region Look Up

			if (GETKEYDOWN(VK_UP) && m_eState == AS_IDLE)
			{
				m_eState = AS_LOOK_UP;
			}
			if (GETKEYUP(VK_UP))
			{
				m_eState = AS_IDLE;
			}

#pragma endregion

#pragma region Dash

			if (GETKEYDOWN('X') && !m_bOnClimb && !m_bInteraction)
			{
				if (m_iDashCount < m_iDashCountMax)
				{
					GET_SINGLE(CGameData)->ShakeCamera(0.5f, 0.75f);
					//GET_SINGLE(CGameData)->CreateFootDust(m_pTransform->GetWorldPosition());
					//GET_SINGLE(CGameData)->CreateDistortionEffect(m_pTransform->GetWorldPosition(), 0.5f, 300.0f);
					++m_iDashCount;
					SetHairColor();
					m_fDashTimer = 0;
					m_bOnDash = true;
					m_eState = AS_DASH;
					m_bJump = false;
					m_bJumpAuto = false;
					m_bAffactGravity = false;
					m_bMoveAuto = false;
					m_bOnClimb = false;


					//L
					if (GETKEY(VK_LEFT) && !GETKEY(VK_UP) && !GETKEY(VK_RIGHT) && !GETKEY(VK_DOWN))
					{
						m_vDashDirection = Vector3(-1.0f, 0.0f, 0.0f).Normalize();
						m_pTransform->SetWorldRotation(0.0f, PI, 0.0f);
					}
					//T
					else if (!GETKEY(VK_LEFT) && GETKEY(VK_UP) && !GETKEY(VK_RIGHT) && !GETKEY(VK_DOWN))
					{
						m_vDashDirection = Vector3(0.0f, -1.0f, 0.0f).Normalize();
					}
					//R
					else if (!GETKEY(VK_LEFT) && !GETKEY(VK_UP) && GETKEY(VK_RIGHT) && !GETKEY(VK_DOWN))
					{
						m_vDashDirection = Vector3(1.0f, 0.0f, 0.0f).Normalize();
						m_pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					}
					//B
					else if (!GETKEY(VK_LEFT) && !GETKEY(VK_UP) && !GETKEY(VK_RIGHT) && GETKEY(VK_DOWN))
					{
						if (m_bOnGround)
							m_vDashDirection = Vector3::Zero;
						else
							m_vDashDirection = Vector3(0.0f, 1.0f, 0.0f).Normalize();
					}
					/////////////////////////////대각선///////////////////////////////////////
					else if (GETKEY(VK_LEFT) && GETKEY(VK_UP) && !GETKEY(VK_RIGHT) && !GETKEY(VK_DOWN))
					{
						m_vDashDirection = Vector3(-1.0f, -1.0f, 0.0f).Normalize();
						m_pTransform->SetWorldRotation(0.0f, PI, 0.0f);
					}
					//RT
					else if (!GETKEY(VK_LEFT) && GETKEY(VK_UP) && GETKEY(VK_RIGHT) && !GETKEY(VK_DOWN))
					{
						m_vDashDirection = Vector3(1.0f, -1.0f, 0.0f).Normalize();
						m_pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					}
					//LB
					else if (GETKEY(VK_LEFT) && !GETKEY(VK_UP) && !GETKEY(VK_RIGHT) && GETKEY(VK_DOWN))
					{
						m_vDashDirection = Vector3(-1.0f, 1.0f, 0.0f).Normalize();
						m_pTransform->SetWorldRotation(0.0f, PI, 0.0f);
					}
					//RB
					else if (!GETKEY(VK_LEFT) && !GETKEY(VK_UP) && GETKEY(VK_RIGHT) && GETKEY(VK_DOWN))
					{
						m_vDashDirection = Vector3(1.0f, 1.0f, 0.0f).Normalize();
						m_pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					}
					else
					{
						m_vDashDirection = m_pTransform->right();
					}
				}
			}


			if (m_bOnDash)
			{
				m_fDashTimer += fTime;
				if (m_fDashTimer >= 0.075f)
				{
					float fTimer = m_fDashTimer - 0.075f;

					float fAcceleration = DashAcceleration(fTimer);

					//m_vDashDirection = Vector3(m_vDashDirection.x, m_vDashDirection.y , m_vDashDirection.z);

					m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() + m_vDashDirection * fAcceleration * fTime);

					if (fTimer >= m_fDashDuration / 2.0f)
					{
						m_bOnDash = false;
						m_bAffactGravity = true;

						if (m_bOnGround)
						{
							m_iDashCount = 0;
							m_eState = AS_IDLE;
						}
						else
						{
							m_fForce = 0.0f;
							m_eState = AS_FALL;
						}
					}


					m_fDashEffectTimer += fTime;
					if (m_fDashEffectTimer >= m_fDashEffectTimerMax)
					{
						m_fDashEffectTimer = 0;
						CreateDummy(MADLINE_HAIR_COLOR_SINGLE_DASH);

						for (int i = 0; i < 5; ++i)
						{
							Vector3 position = m_pTransform->GetWorldPosition() + Vector3::RandomDirection() * 10.0f;
							Vector3 targetPosition = position + Vector3::RandomDirection() * 20.0f;

							CGameObject* pParticle = GET_SINGLE(CGameData)->CreateParticle(m_pLayer, position, targetPosition, m_vHairColor, true, true, 1.0f, 3.0f, 20.0f);
							SAFE_RELEASE(pParticle);
						}
					}
				}
			}

#pragma endregion

#pragma region Gravity

			if (m_bSpawned && m_bAffactGravity && !m_bOnDash && !m_bOnGround && !m_bOnClimb && !m_bOnSlide)
			{
				m_fForce -= m_fAcceleration * fTime;

				if (m_fForce <= m_fGravityThreshold)
					m_fForce = m_fGravityThreshold;

				m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() - Vector3(0.0f, m_fForce * fTime, 0.0f));

				//충돌 할때까지 계속 떨어진다
				if (m_fForce * fTime <= 0.5f && !m_bJumpTransitioned)
				{
					m_eState = AS_JUMP_FAST_DOWN;
				}
			}
#pragma endregion

		}


	}//if (!m_bAwake && !m_bEvent) else
	


	
#pragma region Transition

	if (m_bDeath)
	{
		m_bDeath = false;
		m_bSpawned = false;
		m_bAwake = false;
		m_bJump = false;
		m_bAffactGravity = false;

		GET_SINGLE(CSoundManager)->Play("char_mad_death");
		RenderHair(false);

		pAnimator->GetAnimationController()->TransitionState("Death");
		CLayer* pUI = m_pScene->FindLayer("UI");
		GET_SINGLE(CGameData)->CreateDeathEffect(m_pTransform->GetWorldPosition(), m_pScene, m_pLayer);
		GET_SINGLE(CGameData)->CloseScene(m_pTransform->GetWorldPosition(), m_pScene, pUI, AE_RESET);
		SAFE_RELEASE(pUI);
	}

	if(m_bSpawned && !m_bEvent)
	{
		switch (m_eState)
		{
		case AS_IDLE_CARRY:
			break;
		case AS_RUN_CARRY:
			break;
		case AS_JUMP_SLOW_CARRY:
			break;
		case AS_FALL_CARRY:
			break;
		case AS_PICK_UP:
			break;
		case AS_THROW:
			break;
		case AS_IDLE:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
			{
				pAnimator->GetAnimationController()->TransitionState("Stand");
			}
		}
		break;
		case AS_IDLE_A:
			break;
		case AS_IDLE_B:
			break;
		case AS_IDLE_C:
			break;
		case AS_LOOK_UP:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Look Up")
			{
				pAnimator->GetAnimationController()->TransitionState("Look Up");
			}
		}
		break;
		case AS_WALK:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Walk")
			{
				pAnimator->GetAnimationController()->TransitionState("Walk");
			}
		}
		break;
		case AS_PUSH:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Push")
			{
				pAnimator->GetAnimationController()->TransitionState("Push");
			}
		}

		break;
		case AS_RUN_SLOW:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
			{
				pAnimator->GetAnimationController()->TransitionState("Run Slow");
			}
		}
		break;
		case AS_RUN_FAST:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Fast")
			{
				pAnimator->GetAnimationController()->TransitionState("Run Fast");
			}
		}
		break;
		case AS_RUN_STRUMBLE:
			break;
		case AS_RUN_WIND:
			break;
		case AS_DASH:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Dash")
			{
				pAnimator->GetAnimationController()->TransitionState("Dash");
				GET_SINGLE(CSoundManager)->Play("char_mad_dash_pink_right"); 
			}
		}
		break;
		case AS_DREAM_DASH_START:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Dream Dash In")
			{
				pAnimator->GetAnimationController()->TransitionState("Dash");
			}
		}
		break;
		case AS_DREAM_DASH:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Dream Dash Loop")
			{
				pAnimator->GetAnimationController()->TransitionState("Dash");
			}
		}
		break;
		case AS_DREAM_DASH_EXIT:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Dream Dash Out")
			{
				pAnimator->GetAnimationController()->TransitionState("Dash");
			}
		}
		break;
		case AS_SLIDE:
			break;
		case AS_JUMP_SLOW_UP:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Jump Slow Up")
			{
				pAnimator->GetAnimationController()->TransitionState("Jump Slow Up");
			}
		}
		break;
		case AS_JUMP_SLOW_DOWN:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Jump Slow Down")
			{
				pAnimator->GetAnimationController()->TransitionState("Jump Slow Down");
			}
		}
		break;
		case AS_JUMP_FAST_UP:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Jump Fast Up")
			{
				SetScaling(Vector3(0.6f, 1.5f, 0.25f));
				pAnimator->GetAnimationController()->TransitionState("Jump Fast Up");
				GET_SINGLE(CSoundManager)->Play("char_mad_jump");
				GET_SINGLE(CSoundManager)->Play("char_mad_jump_assisted");
			}
		}
		break;
		case AS_JUMP_FAST_DOWN:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Jump Fast Down")
			{
				pAnimator->GetAnimationController()->TransitionState("Jump Fast Down");
			}
		}
		break;
		case AS_TIRED:
			break;
		case AS_TIRED_STILL:
			break;
		case AS_CLIMB_WALL_SLIDE:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Climb Wall Slide")
			{
				pAnimator->GetAnimationController()->TransitionState("Climb Wall Slide");
			}
		}
		break;
		case AS_CLIMB_LOOK_BACK_START:
			break;
		case AS_CLIMB_LOOK_BACK:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Climb Look Back")
			{
				pAnimator->GetAnimationController()->TransitionState("Climb Look Back");
			}
		}
			break;
		case AS_CLIMB_UP:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Climb Up")
			{
				pAnimator->GetAnimationController()->TransitionState("Climb Up");
			}
		}
		break;
		case AS_CLIMB_PUSH:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Climb Push")
			{
				pAnimator->GetAnimationController()->TransitionState("Climb Push");
			}
		}
		break;
		case AS_CLIMB_PULL:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Climb Pull")
			{
				pAnimator->GetAnimationController()->TransitionState("Climb Pull");
			}
		}
		break;
		case AS_DUCK:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Duck")
			{
				pAnimator->GetAnimationController()->TransitionState("Duck");
			}
		}
		break;
		case AS_FALL_POSE:
			break;
		case AS_EDGE:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Duck")
			{
				pAnimator->GetAnimationController()->TransitionState("Duck");
			}
		}
		break;
		case AS_EDGE_BACK:
			break;
		case AS_FAINT_START:
			break;
		case AS_FAINT:
			break;
		case AS_FLIP:
			break;
		case AS_FLIP_SKID:
			break;
		case AS_DANGLING:
			break;
		case AS_DEATH_SIDE:
			break;
		case AS_DEATH_UP:
			break;
		case AS_DEATH_DOWN:
			break;
		case AS_SWIM_IDLE:
			break;
		case AS_SWIM_UP:
			break;
		case AS_SWIM_DOWN:
			break;
		case AS_STARFLY_START:
			break;
		case AS_STARFLY:
			break;
		case AS_BUBBLE:
			break;
		case AS_FALL:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Fall")
			{
				pAnimator->GetAnimationController()->TransitionState("Fall");
			}
		}
		break;
		case AS_BIG_FALL:
			break;
		case AS_BIG_FALL_RECOVER:
			break;
		case AS_SLEEP:
			break;
		case AS_SLEEP_BAG_DOWN:
			break;
		case AS_WAKE_UP:
			break;
		case AS_WAKE_UP_HALF:
			break;
		case AS_SPIN:
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Spin")
			{
				pAnimator->GetAnimationController()->TransitionState("Spin");
			}
		}
		break;
		case AS_SHAKING:
			break;
		case AS_HUG:
			break;
		case AS_STAR_MORPH:
			break;
		case AS_STAR_MORPH_IDLE:
			break;
		case AS_WALK_CARRY_THEO:
			break;
		case AS_WALK_CARRY_THEO_COLLAPSE:
			break;
		case AS_TENTACLE_GRAB:
			break;
		case AS_TENTACLE_GRABBED:
			break;
		case AS_TENTACLE_PULL:
			break;
		case AS_TENTACLE_DANGLING:
			break;
		case AS_SIT_DOWN:
			break;
		case AS_LAUNCH:
			break;
		case AS_LAUNCH_RECOVER:
			break;
		default:
			break;
		}
	}

#pragma endregion



	GET_SINGLE(CGameData)->SetPrevArea();
	SAFE_RELEASE(pAnimator);
	SAFE_RELEASE(pText);
//
	return 0;
}

int CPlayer::LateUpdate(float fTime)
{
	return 0;
}

int CPlayer::OnCollisionEnter(CCollider* pThis, CCollider * pTarget, float fTime)
{
	CGameObject* pTargetObject = pTarget->GetGameObject();
	string strTargetTag = pTargetObject->GetTag();

	if (strTargetTag == "Refill")
	{
		return 0;
	}
	if (strTargetTag == "Strawberry")
	{
		return 0;
	}


	if (strTargetTag.find("Crumble") !=  string::npos)
	{
		if (pThis != m_pBodyTop)
		{
			CGameObject* pTargetObject = pTarget->GetGameObject();
			CCrumble* pCrumble = pTargetObject->GetComponentFromType<CCrumble>(CT_CRUMBLE);
			bool bEnable = pCrumble->GetGeneration();

			if (!bEnable)
			{
				SAFE_RELEASE(pCrumble);
				SAFE_RELEASE(pTargetObject);
			}
			else
			{
				pCrumble->SetActive();
			}
			SAFE_RELEASE(pCrumble);
			SAFE_RELEASE(pTargetObject);
			return 0;
		}
	}

	if (!m_bDeath && m_bSpawned && m_bAwake)
	{
		if (strTargetTag == "DeadZone")
		{
			m_bDeath = true;
		}
	}




	if ((pThis == m_pBodyLeft || pThis == m_pBodyRight ))
	{
		if (!m_bJump && !m_bJumpAuto)
		{
			m_bAffactGravity = true;
			m_fForce = 0.0f;
		}
	}

	if (strTargetTag == "Spring")
	{
		ResetDashCount();

		CTransform* pTargetTransform = pTarget->GetTransform();
		Vector3 vSpringLT = pTargetTransform->GetWorldPosition() - pTarget->GetScale() * pTarget->GetPivot();
		SAFE_RELEASE(pTargetTransform);

		Vector3 vPosition = m_pTransform->GetWorldPosition();
		m_pTransform->SetWorldPosition(vPosition.x, vSpringLT.y, vPosition.z);
		
		JumpForced(1050.0f, false);
		
		CAnimator* pAnimator = pTargetObject->GetComponentFromType<CAnimator>(CT_ANIMATOR);
		CAnimationController* pController = pAnimator->GetAnimationController();
		pController->TransitionState("Spring Active");
		SAFE_RELEASE(pAnimator);
	}


	SAFE_RELEASE(pTargetObject);
	return 0;
}

int CPlayer::OnCollisionStay(CCollider* pThis, CCollider * pTarget, float fTime)
{
	CGameObject* pTempTargetObject = pTarget->GetGameObject();
	string strTemp = pTempTargetObject->GetTag();
	SAFE_RELEASE(pTempTargetObject);
	
	if (strTemp == "Refill")
	{
		return 0;
	}

	if (strTemp == "Spring")
	{
		return 0;
	}

	if (strTemp.find("Crumble") != string::npos)
	{
		CGameObject* pTargetObject = pTarget->GetGameObject();
		CCrumble* pCrumble = pTargetObject->GetComponentFromType<CCrumble>(CT_CRUMBLE);
		bool bEnable = pCrumble->GetGeneration();
		SAFE_RELEASE(pCrumble);
		SAFE_RELEASE(pTargetObject);
	
		if (!bEnable)
		{
			if (pThis == m_pBodyBottom)
			{
				m_bOnGround = false;
				m_bAffactGravity = true;
			}
			if (pThis == m_pBodyLeft || pThis == m_pBodyRight)
			{
				m_bPushable = false;
				m_pClimbWall = NULL;
			}
			return 0;
		}
	}

	if (strTemp == "Strawberry")
	{
		return 0;
	}
	
	if (!m_bDeath && m_bSpawned && m_bAwake)
	{
		CGameObject* pTargetObject = pTarget->GetGameObject();
		CAnimator* pAnimator = m_pGameObject->GetComponentFromType<CAnimator>(CT_ANIMATOR);
		CTransform* pTargetTile = pTarget->GetTransform();
		Vector3 TargetPosition = pTargetTile->GetWorldPosition();
		Vector3 TargetScale = pTarget->GetScale();
		Vector3 TargetPivot = pTarget->GetPivot();
		Vector3 LT = TargetPosition - TargetScale * TargetPivot;
		Vector3 RB = LT + TargetScale;
		SAFE_RELEASE(pTargetTile);
		Vector3 position = m_pTransform->GetWorldPosition();
		Vector3 direction = m_pTransform->GetDeltaPosition().Normalize();
	
		string strTargetTag = pTargetObject->GetTag();
	
		Vector3 PlayerLT = position - pThis->GetScale() * pThis->GetPivot();
		Vector3 PlayerRB = PlayerLT + pThis->GetScale();
	
	
		Vector3 TopPosition;
	
		{
			bool bCollisionBottom = pTarget->CollisionCheck(m_pBodyBottom);
			bool bCollisionLeft = pTarget->CollisionCheck(m_pBodyLeft);
			bool bCollisionRight = pTarget->CollisionCheck(m_pBodyRight);
	
			if (bCollisionBottom && bCollisionLeft)
			{
				if (direction.y > 0.5f)
				{
					TopPosition = position - m_pBodyBottom->GetScale() * m_pBodyBottom->GetPivot();
					m_bOnGround = true;
				}
				else
				{
					m_bOnGround = false;
				}
			}
			if (bCollisionBottom && bCollisionRight)
			{
				if (direction.y > 0.5f)
				{
					TopPosition = position - m_pBodyBottom->GetScale() * m_pBodyBottom->GetPivot();
					m_bOnGround = true;
				}
				else
				{
					m_bOnGround = false;
				}
			}
		}
	
	
		if (pThis == m_pBodyBottom)
		{
			if (strTargetTag == "Spike")
			{
				//점프 중 가시와 부딪힐 경우 충돌하지 않음.
				if (!m_bJump && !m_bJumpAuto && !m_bOnDash)
				{
					m_bDeath = true;
				}
				else
				{
					//가시의 방향이 Up일 경우 통과. 아닐 경우사망			
					CRenderer* pTargetRenderer = pTargetObject->GetComponentFromType<CRenderer>(CT_RENDERER);
					string strAtlasKey = pTargetRenderer->GetAtlasKey();
					SAFE_RELEASE(pTargetRenderer);
					if (strAtlasKey.find("up") == string::npos)
					{
						m_bDeath = true;
					}
				}
			}
			else if (strTargetTag == "Jumpthru")
			{
				if (!m_bOnClimb)
				{
					if (!m_bJump && !m_bJumpAuto && !m_bOnDash)
					{
						pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->SetAudioPackage("WoodWalkWay");
						//캐릭터의 충돌체의 바닥이 블록의 위쪽을 넘겼을 때에만 처리
						//if (PlayerRB.y <= LT.y)
						//{
						if (m_pTransform->GetWorldPosition().y >= LT.y)
						{
							m_pTransform->SetWorldPosition(position.x, LT.y, position.z);
							m_bAffactGravity = false;
							m_bOnGround = true;
							m_bJump = false;
							m_bJumpAuto = false;
							m_bMoveAuto = false;
							m_bOnDash = false;
							m_bAffactGravity = true;
							m_bOnSlide = false;
							m_fForce = 0.0f;
							ResetDashCount();

							if (GETKEY(VK_LEFT) || GETKEY(VK_RIGHT))
							{
								m_eState = AS_RUN_FAST;
							}
							else
							{
								if (m_eState != AS_LOOK_UP && m_eState != AS_DUCK)
								{
									m_eState = AS_IDLE;
								}
							}
						}
					}
				}
			}		
			else
			{
				pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->SetAudioPackage(strTargetTag);

				//일반 타일일 때
				if (m_pTransform->GetWorldPosition().y >= LT.y && !m_bOnDash)
				{
					m_pTransform->SetWorldPosition(position.x, LT.y, position.z);
					m_bAffactGravity = false;
					m_bOnSlide = false;
					m_bOnGround = true;
					m_bJump = false;
					m_bJumpAuto = false;
					m_bMoveAuto = false;
					m_iDashCount = 0;
					m_bOnDash = false;
					m_bAffactGravity = true;
					m_fForce = 0.0f;
					ResetDashCount();

					if (GETKEY(VK_LEFT) || GETKEY(VK_RIGHT))
					{
						m_eState = AS_RUN_FAST;
					}
					else
					{
						if (m_eState != AS_LOOK_UP && m_eState != AS_DUCK)
						{
							m_eState = AS_IDLE;
						}
					}
				}
			}
		}
	
		if (pThis == m_pBodyTop)
		{
			if (strTargetTag != "Jumpthru")
			{
				//부딪힌 블럭이 가시일 때,
				if (strTargetTag == "Spike")
				{
					//가시의 방향이 Up일 경우 통과. 아닐 경우사망			
					CRenderer* pTargetRenderer = pTargetObject->GetComponentFromType<CRenderer>(CT_RENDERER);
					string strAtlasKey = pTargetRenderer->GetAtlasKey();
					SAFE_RELEASE(pTargetRenderer);
					if (strAtlasKey.find("up") == string::npos)
					{
						m_bDeath = true;
					}
				}
				else if (m_bOnGround)
				{
					m_bDeath = true;
				}
				else
				{
					m_pTransform->SetWorldPosition(position.x, RB.y + (position.y - PlayerLT.y), position.z);
					m_bHeadColliding = true;
					m_bOnDash = false;
					m_bAffactGravity = true;
					m_fForce = 0.0f;
				}
			}
		}
	
		if (pThis == m_pBodyLeft)
		{
			if (strTargetTag != "Jumpthru")
			{
				m_bPushable = false;
				m_pClimbWall = NULL;

				if (strTargetTag == "Spike")
				{
					//가시의 방향이 Up일 경우 통과. 아닐 경우사망			
					CRenderer* pTargetRenderer = pTargetObject->GetComponentFromType<CRenderer>(CT_RENDERER);
					string strAtlasKey = pTargetRenderer->GetAtlasKey();
					SAFE_RELEASE(pTargetRenderer);

					if (strAtlasKey.find("up") == string::npos)
					{
						m_bDeath = true;
					}
				}
				else if (strTargetTag == "Jumpthru")
				{
					//통과
				}
				else
				{
					m_pTransform->SetWorldPosition(position.x + (RB.x - PlayerLT.x), position.y, position.z);
					m_pTransform->SetWorldRotation(0.0f, PI, 0.0f);
					m_bPushable = true;
					m_pClimbWall = pTargetObject;
					m_bOnDash = false;
					m_bMoveAuto = false;

					if (m_bOnGround)
						m_eState = AS_PUSH;

				}
			}
		}
	
		if (pThis == m_pBodyRight)
		{
			if (strTargetTag != "Jumpthru")
			{
				m_bPushable = false;
				m_pClimbWall = NULL;
				if (strTargetTag == "Spike")
				{
					//가시의 방향이 Up일 경우 통과. 아닐 경우사망			
					CRenderer* pTargetRenderer = pTargetObject->GetComponentFromType<CRenderer>(CT_RENDERER);
					string strAtlasKey = pTargetRenderer->GetAtlasKey();
					SAFE_RELEASE(pTargetRenderer);
					if (strAtlasKey.find("up") == string::npos)
					{
						m_bDeath = true;
					}
				}
				else if (strTargetTag == "Jumpthru")
				{
					//통과
				}
				else
				{
					m_pTransform->SetWorldPosition(position.x + (LT.x - PlayerRB.x), position.y, position.z);
					m_pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					m_bPushable = true;
					m_pClimbWall = pTargetObject;
					m_bOnDash = false;
					m_bMoveAuto = false;

					if (m_bOnGround)
						m_eState = AS_PUSH;
				}
			}
		}
	
		SAFE_RELEASE(pAnimator);
		SAFE_RELEASE(pTargetObject);	

		m_bPrevBoostable = m_bBoostable;
	}
	return 0;
}

int CPlayer::OnCollisionExit(CCollider * pThis, CCollider * pTarget, float fTime)
{
	if (!pTarget)
		return 0;


	CGameObject* pTargetObject = pTarget->GetGameObject();


	if (pTargetObject->GetTag() == "Refill")
	{
		SAFE_RELEASE(pTargetObject);
		return 0;
	}

	if (pTargetObject->GetTag() == "Strawberry")
	{
		SAFE_RELEASE(pTargetObject);
		return 0;
	}

	if (pTargetObject->GetTag().find("Crumble") != string::npos)
	{
		size_t iCount = pThis->GetCollisionCount();

		if (pThis == m_pBodyBottom && !m_bJump && !m_bJumpAuto)
		{
			Vector3 position = m_pTransform->GetWorldPosition();
			m_bAffactGravity = true;
			m_bOnGround = false;
			m_fForce = 0.0f;
		}
		if (pThis == m_pBodyLeft && m_bPushable && iCount == 1)
		{
			m_bPushable = false;
			m_pClimbWall = NULL;
		}

		if (pThis == m_pBodyRight && m_bPushable && iCount == 1)
		{
			m_bPushable = false;
			m_pClimbWall = NULL;
		}
		SAFE_RELEASE(pTargetObject);
		return 0;
	}

	SAFE_RELEASE(pTargetObject);

	if (pThis == m_pBodyBottom && !m_bJump && !m_bJumpAuto)
	{
		Vector3 position = m_pTransform->GetWorldPosition();
		m_bAffactGravity = true;
		m_bOnGround = false;
		m_fForce = 0.0f;
	}

	size_t iCount = pThis->GetCollisionCount();

	if (pThis == m_pBodyLeft && m_bPushable && iCount == 1)
	{
		m_bPushable = false;
		m_pClimbWall = NULL;
	}

	if (pThis == m_pBodyRight && m_bPushable && iCount == 1)
	{
		m_bPushable = false;
		m_pClimbWall = NULL;
	}
	return 0;
}

void CPlayer::Render(float fTime)
{
}

CPlayer * CPlayer::Clone() const
{
	return new CPlayer(*this);
}


