#include "StageManager00.h"
#include "Transform.h"
#include "GameObject.h"
#include "FallBlock.h"
#include "../Client.h"
#include "Collider.h"
#include "Timer.h"
#include "TimerManager.h"
#include "Input.h"
#include "Player.h"
#include "Material.h"
#include "Bird.h"
#include "Scene.h"
#include "Layer.h"
#include "CameraMove.h"
#include "../GameData.h"
#include "Device.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "../Scene/IntroScene.h"
#include "TitleSceneManager.h"

CStageManager00::CStageManager00():
	m_bBeginEvent(false),
	m_fBeginEventTimer(0.0f),
	m_bInitialized(false),
	m_fSlowTimer(1.0f),
	m_eEventState(ES_NORMAL),
	m_pGranny(NULL),
	m_pBird(NULL),
	m_bTutorialClimb(false),
	m_bTutorialDash(false),
	m_eSignState(SS_HIDE),
	m_eSignReserveState(SS_HIDE),
	m_bCameraEvent(false),
	m_bTextFadeOut(false),
	m_fEndEventTimer(0.0f)
{
	m_bDialogueEvent = false;
	m_iDialogueProgress = -1;
}


CStageManager00::~CStageManager00()
{
	SAFE_RELEASE(m_pTutorialSign);
	SAFE_RELEASE(m_pEventText);	
}

void CStageManager00::SetBird(CGameObject * pBird)
{
	m_pBird = pBird;
}

void CStageManager00::InitializeDialogue()
{
	m_pDialogue->OpenDialogue(PD_LEFT, PC_MADELINE, "Madeline Normal");
	m_pDialogue->CloseDialogue();
}

void CStageManager00::SetDialogue(CDialogue * pDialogue)
{
	m_pDialogue = pDialogue;
}

void CStageManager00::AddFallBlockBridge(CGameObject * pBlock)
{
	CFallBlock* pFallBlock = pBlock->GetComponentFromType<CFallBlock>(CT_FALL_BLOCK);
	pFallBlock->SetCameraShake(false);
	pFallBlock->SetColliderFixed(true);
	SAFE_RELEASE(pFallBlock);
	m_vecFallBlockBridge.push_back(pBlock);
}

void CStageManager00::SetPlayer(CGameObject * pPlayer)
{
	m_pPlayer = pPlayer;
}

void CStageManager00::SetGranny(CGameObject * pGranny)
{
	m_pGranny = pGranny;
}

void CStageManager00::SetFallBlock(CGameObject * pBlock)
{
	m_pFallBlock = pBlock;
}

void CStageManager00::DashPlayer()
{
	CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);


	if (pPlayer->m_iDashCount < pPlayer->m_iDashCountMax)
	{
		++pPlayer->m_iDashCount;
		pPlayer->m_fDashTimer = 0;
		pPlayer->m_bOnDash = true;
		pPlayer->m_eState = AS_DASH;
		pPlayer->m_bJump = false;
		pPlayer->m_bJumpAuto = false;
		pPlayer->m_bAffactGravity = false;
		pPlayer->m_bMoveAuto = false;
		pPlayer->m_bOnClimb = false;
		pPlayer->SetHairColor();

		//L
		if (GETASYNCKEY(VK_LEFT) && !GETASYNCKEY(VK_UP) && !GETASYNCKEY(VK_RIGHT) && !GETASYNCKEY(VK_DOWN))
		{
			pPlayer->m_vDashDirection = Vector3(-1.0f, 0.0f, 0.0f).Normalize();
		}
		//T
		else if (!GETASYNCKEY(VK_LEFT) && GETASYNCKEY(VK_UP) && !GETASYNCKEY(VK_RIGHT) && !GETASYNCKEY(VK_DOWN))
		{
			pPlayer->m_vDashDirection = Vector3(0.0f, -1.0f, 0.0f).Normalize();
		}
		//R
		else if (!GETASYNCKEY(VK_LEFT) && !GETASYNCKEY(VK_UP) && GETASYNCKEY(VK_RIGHT) && !GETASYNCKEY(VK_DOWN))
		{
			pPlayer->m_vDashDirection = Vector3(1.0f, 0.0f, 0.0f).Normalize();
		}
		//B
		else if (!GETASYNCKEY(VK_LEFT) && !GETASYNCKEY(VK_UP) && !GETASYNCKEY(VK_RIGHT) && GETASYNCKEY(VK_DOWN))
		{
			pPlayer->m_vDashDirection = Vector3(0.0f, 1.0f, 0.0f).Normalize();
		}
		/////////////////////////////대각선///////////////////////////////////////
		else if (GETASYNCKEY(VK_LEFT) && GETASYNCKEY(VK_UP) && !GETASYNCKEY(VK_RIGHT) && !GETASYNCKEY(VK_DOWN))
		{
			pPlayer->m_vDashDirection = Vector3(-1.0f, -1.0f, 0.0f).Normalize();
		}
		//RT
		else if (!GETASYNCKEY(VK_LEFT) && GETASYNCKEY(VK_UP) && GETASYNCKEY(VK_RIGHT) && !GETASYNCKEY(VK_DOWN))
		{
			pPlayer->m_vDashDirection = Vector3(1.0f, -1.0f, 0.0f).Normalize();
		}
		//LB
		else if (GETASYNCKEY(VK_LEFT) && !GETASYNCKEY(VK_UP) && !GETASYNCKEY(VK_RIGHT) && GETASYNCKEY(VK_DOWN))
		{
			pPlayer->m_vDashDirection = Vector3(-1.0f, 1.0f, 0.0f).Normalize();
		}
		//RB
		else if (!GETASYNCKEY(VK_LEFT) && !GETASYNCKEY(VK_UP) && GETASYNCKEY(VK_RIGHT) && GETASYNCKEY(VK_DOWN))
		{
			pPlayer->m_vDashDirection = Vector3(1.0f, 1.0f, 0.0f).Normalize();
		}
		else
		{
			pPlayer->m_vDashDirection = m_pTransform->right();
		}
	}

	SAFE_RELEASE(pPlayer);
}

bool CStageManager00::Initialize()
{

	m_pTutorialSign = CGameObject::CreateObject("TutorialSign", m_pLayer);
	CTransform* pTransform = m_pTutorialSign->GetTransform();
	pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
	pTransform->SetPivot(0.5f, 1.0f, 0.0f);
	pTransform->SetWorldScale(192.0f, 136.0f, 1.0f);
	pTransform->SetWorldRotation(0.0f, PI / 2.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = m_pTutorialSign->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial* pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	pRenderer->SetImageInfo("ui\\tutorial\\climb00");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);








	//Fade
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Fade", m_pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(5000.0f, 5000.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Black", L"Celeste\\Black.png");
		pMaterial->SetSampler(SAMPLER_POINT);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_pFader = pGameObject;
		SAFE_RELEASE(pGameObject);
	}


	//Prologue / Epilogue Text

	m_pEventText = CGameObject::CreateObject("Event Text", m_pLayer);
	CTransform* pEventTextTransform = m_pTutorialSign->GetTransform();
	pEventTextTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
	pEventTextTransform->SetPivot(0.0f, 0.0f, 0.0f);
	pEventTextTransform->SetWorldScale(192.0f, 136.0f, 1.0f);
	SAFE_RELEASE(pEventTextTransform);

	CText* pText = m_pEventText->AddComponent<CText>("Event Text");
	//pText->SetFont(L"나눔고딕OTF ExtraBold", 50);
	pText->SetRenderArea(0, 0, 1000, 200);
	pText->SetText(L"그래, 매들린. 넌 할 수 있어...");
	pText->SetRelativeCamera(true);
	pText->AlphaBlend(true);
	pText->SetOpacity(0.0f);
	SAFE_RELEASE(pText);

	return true;
}

int CStageManager00::Update(float fTime)
{

	if (!m_bInitialized)
	{
		CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
		pPlayer->SetMaxDash(0);
		SAFE_RELEASE(pPlayer);

		m_bInitialized = true;
	}



	CTimer* pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainTimer");
	float fFixedDeltaTime = pTimer->GetFixedDeltaTime();

	CTransform* pPlayerTransform = m_pPlayer->GetTransform();
	Vector3 vPlayerPosition = pPlayerTransform->GetWorldPosition();
	SAFE_RELEASE(pPlayerTransform);


	//FIRST_EVENT
	if (!m_bBeginEvent)
	{
		GET_SINGLE(CInput)->DisableInput();


		CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
		Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
		SAFE_RELEASE(pCameraTransform);

		CTransform* pFaderTransform = m_pFader->GetTransform();
		pFaderTransform->SetWorldPosition(vCameraPosition - Vector3(1000, 1000, 0.0f));
		SAFE_RELEASE(pFaderTransform);

		CTransform* pTextTransform = m_pEventText->GetTransform();
		pTextTransform->SetWorldPosition(1000, 360, 0.0f);
		SAFE_RELEASE(pTextTransform);


		m_fBeginEventTimer += fTime;
		float fProgress = m_fBeginEventTimer / 1.0f;
		if (m_fBeginEventTimer <= 1.0f)
		{

			float localProgress = m_fBeginEventTimer / 0.5f;

			if (localProgress <= 1.0f)
			{
				CText* pText = m_pEventText->GetComponentFromType<CText>(CT_TEXT);
				pText->SetRenderArea(-500, -100, 500, 100);
				pText->SetText(L"이제 겨우 시작일 뿐이잖아, 매들린.");
				pText->AlphaBlend(true);
				pText->SetOpacity(localProgress);
				SAFE_RELEASE(pText);
			}
		}
		else if (m_fBeginEventTimer >= 2.0f && m_fBeginEventTimer <= 3.0f)
		{
			float localProgress = (m_fBeginEventTimer - 2.0f) / 1.0f;

			if (localProgress <= 1.0f)
			{
				CText* pText = m_pEventText->GetComponentFromType<CText>(CT_TEXT);
				pText->SetOpacity(1.0f - localProgress);
				SAFE_RELEASE(pText);
			}
		}
		else if (m_fBeginEventTimer >= 4.0f && m_fBeginEventTimer <= 5.0f)
		{
			float localProgress = (m_fBeginEventTimer - 4.0f) / 1.0f;

			if (localProgress <= 1.0f)
			{
				CText* pText = m_pEventText->GetComponentFromType<CText>(CT_TEXT);
				pText->SetRenderArea(-500, -100, 500, 100);
				pText->SetText(L"뭘 그렇게 긴장하는 거야?");
				pText->AlphaBlend(true);
				pText->SetOpacity(localProgress);
				SAFE_RELEASE(pText);
			}
		}
		else if (m_fBeginEventTimer >= 6.0f && m_fBeginEventTimer <= 7.0f)
		{
			float localProgress = (m_fBeginEventTimer - 6.0f) / 1.0f;

			if (localProgress <= 1.0f)
			{
				CText* pText = m_pEventText->GetComponentFromType<CText>(CT_TEXT);
				pText->SetOpacity(1.0f - localProgress);
				SAFE_RELEASE(pText);
			}
		}
		else if (m_fBeginEventTimer >= 8.00f && m_fBeginEventTimer <= 9.0f)
		{
			float localProgress = (m_fBeginEventTimer - 8.0f) / 1.0f;

			if (localProgress <= 1.0f)
			{
				CText* pText = m_pEventText->GetComponentFromType<CText>(CT_TEXT);
				pText->SetRenderArea(-500, -100, 500, 100);
				pText->SetText(L"어서 심호흡 해.");
				pText->AlphaBlend(true);
				pText->SetOpacity(localProgress);
				SAFE_RELEASE(pText);
			}
		}
		else if (m_fBeginEventTimer >= 10.0f && m_fBeginEventTimer <= 11.0f)
		{
			float localProgress = (m_fBeginEventTimer - 10.0f) / 1.0f;

			if (localProgress <= 1.0f)
			{
				CText* pText = m_pEventText->GetComponentFromType<CText>(CT_TEXT);
				pText->SetOpacity(1.0f - localProgress);
				SAFE_RELEASE(pText);
			}
		}
		else if (m_fBeginEventTimer >= 12.0f)
		{
			GET_SINGLE(CGameData)->OpenScene(Vector3(1376.0f, 512.0f, 0.0f), m_pScene, m_pLayer);
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->Enable(false);
			SAFE_RELEASE(pRenderer);
			GET_SINGLE(CSoundManager)->Play("mus_lvl0_intro_loop");
			m_bBeginEvent = true;
			GET_SINGLE(CInput)->EnableInput();
		}
	}

	//Fall Block
	Vector3 vFallRegionMin(2304.0f,256.0f, 0.0f);
	Vector3 vFallRegionMax(2400.0f, 512.0f, 0.0f);

	if (vPlayerPosition.x >= vFallRegionMin.x && vPlayerPosition.y >= vFallRegionMin.y &&
		vPlayerPosition.x <= vFallRegionMax.x && vPlayerPosition.y <= vFallRegionMax.y)
	{
		CFallBlock* pFallBlock = m_pFallBlock->GetComponentFromType<CFallBlock>(CT_FALL_BLOCK);
		if (pFallBlock->GetFallBlockState() == FBS_STANDBY)
		{
			pFallBlock->SetFallBlockState(FBS_SHAKE);
		}
		SAFE_RELEASE(pFallBlock);
	}

	int iSize = m_vecFallBlockBridge.size();

	for (int i = 0; i < iSize; ++i)
	{
		CTransform* pBridgeTransform = m_vecFallBlockBridge[i]->GetTransform();
		CCollider* pBridgeCollider = pBridgeTransform->GetGameObject()->GetComponentFromType<CCollider>(CT_COLLIDER);
		Vector3 vBridgePosition = pBridgeTransform->GetWorldPosition();
		Vector3 vBridgeSize = pBridgeCollider->GetScale();
		SAFE_RELEASE(pBridgeCollider);
		SAFE_RELEASE(pBridgeTransform);

		if (vPlayerPosition.x >= vBridgePosition.x && vPlayerPosition.x <= vBridgePosition.x + vBridgeSize.x)
		{
			CFallBlock* pFallBlock = m_vecFallBlockBridge[i]->GetComponentFromType<CFallBlock>(CT_FALL_BLOCK);
			if (pFallBlock->GetFallBlockState() == FBS_STANDBY)
			{
				pFallBlock->SetFallBlockState(FBS_SHAKE);
			}
			SAFE_RELEASE(pFallBlock);
		}
	}


	if (vPlayerPosition.x >= 7680.0f && vPlayerPosition.x <= 7937.0f && m_eEventState == ES_NORMAL)
	{
		m_eEventState = ES_EVENT00;
		GET_SINGLE(CInput)->DisableInput(); 
	}

	if (m_eEventState == ES_EVENT00)
	{
		m_fSlowTimer -= fFixedDeltaTime;


		pTimer->SetTimeScale(m_fSlowTimer);
		if (m_fSlowTimer <= 0)
		{
			pTimer->SetTimeScale(0.0f);
			m_eEventState = ES_EVENT01;

			CTransform* pBirdTransform = m_pBird->GetTransform();
			Vector3 vBirdPosition = pBirdTransform->GetWorldPosition();
			SAFE_RELEASE(pBirdTransform);
			CTransform* pTutorialSignTransform = m_pTutorialSign->GetTransform();
			pTutorialSignTransform->SetWorldPosition(vBirdPosition - Vector3(0.0f, 64.0f, 0.0f));
			SAFE_RELEASE(pTutorialSignTransform);
			m_eSignState = SS_DASH;
			m_eSignReserveState = SS_DASH;
			m_fTutorialSignRotationTimer = 0.0f;
			m_bOnTutorial = true;
			m_bExitTutorial = false;
			CRenderer* pSignRenderer = m_pTutorialSign->GetComponentFromType<CRenderer>(CT_RENDERER);
			pSignRenderer->SetImageInfo("ui\\tutorial\\dash");
			SAFE_RELEASE(pSignRenderer);

			CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
			pPlayer->SetMaxDash(1);
			SAFE_RELEASE(pPlayer);
		}
	}

	if (m_eEventState == ES_EVENT01)
	{
		if (GETASYNCKEYDOWN('X'))
		{
			m_bExitTutorial = true;
			m_eSignState = SS_HIDE;
			m_eSignReserveState = SS_HIDE;

			DashPlayer();
			m_eEventState = ES_EVENT_END;
			pTimer->SetTimeScale(1.0f);
			GET_SINGLE(CInput)->EnableInput();
		}
	}
	
	if (m_eEventState == ES_EVENT_END)
	{
		CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);

		if (vPlayerPosition.x >= 8064.0f && pPlayer->m_bOnGround)
		{
			CGameObject* pCameraObject = m_pScene->GetMainCameraObject();
			CCameraMove* pCameraMove = pCameraObject->GetComponentFromType<CCameraMove>(CT_CAMERA_MOVE);
			pCameraMove->Unlock();
			SAFE_RELEASE(pCameraObject);
			GET_SINGLE(CInput)->DisableInput();
			pPlayer->SetEvent(true);
			m_bCameraEvent = true;

			CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
			{
				pAnimator->GetAnimationController()->TransitionState("Stand");
			}
			SAFE_RELEASE(pAnimator);
		}
		SAFE_RELEASE(pPlayer);
	}
	if (m_bCameraEvent)
	{
		m_fCameraEventTimer += fFixedDeltaTime;

		if (m_fCameraEventTimer > 2.0f)
		{
			if (m_fCameraEventTimer <= 8.0f)
			{
				CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
				pCameraTransform->SetWorldPosition(pCameraTransform->GetWorldPosition() - Vector3(0.0f, 300.0f * fFixedDeltaTime, 0.0f));
				SAFE_RELEASE(pCameraTransform);
			}		

			if (m_fCameraEventTimer > 8.0f)
			{
				CTransform* pTextTransform = m_pEventText->GetTransform();
				pTextTransform->SetWorldPosition(DEVICE_RESOLUTION.iWidth / 2.0f, DEVICE_RESOLUTION.iHeight / 2.0f, 0.0f);
				SAFE_RELEASE(pTextTransform);

				CText* pText = m_pEventText->GetComponentFromType<CText>(CT_TEXT);
				pText->SetText(L"그래, 매들린. 넌 할 수 있어...");
				float fOpacity = pText->GetOpacity();
				if (fOpacity < 1.0f)
				{
					pText->SetOpacity(fOpacity + fFixedDeltaTime * 0.5f);
				}
				else
				{
					m_bTextFadeOut = true;
					m_bCameraEvent = false;
					m_fCameraEventTimer = 0.0f;
				}
				SAFE_RELEASE(pText);
			}
		}	
	}

	if (m_bTextFadeOut && m_eEventState == ES_EVENT_END)
	{
		m_fEndEventTimer += fFixedDeltaTime;

		float fProgress = m_fEndEventTimer / 2.0f;
		if (m_fEndEventTimer <= 2.0f)
		{
			CText* pText = m_pEventText->GetComponentFromType<CText>(CT_TEXT);
			pText->SetOpacity(1.0f - fProgress);
			SAFE_RELEASE(pText);
		}
		else if(m_fEndEventTimer >= 2.0f && m_fEndEventTimer <= 3.0f)
		{
			float fTempProgress = (m_fEndEventTimer - 2.0f) / 1.0f;
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->Enable(true);
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, fTempProgress));
			SAFE_RELEASE(pRenderer);
		}
		else if (m_fEndEventTimer >= 3.0f)
		{
			GET_SINGLE(CSoundManager)->Stop(SOUND_BGM);
			GET_SINGLE(CSoundManager)->Stop(SOUND_ENV);
			GET_SINGLE(CSoundManager)->Stop(SOUND_SFX);

			m_bTextFadeOut = false;
			GET_SINGLE(CSceneManager)->ChangeScene(SCC_TO_PREVIOUS);
			CIntroScene* pIntroScene = dynamic_cast<CIntroScene*>(GET_SINGLE(CSceneManager)->GetNextScene());
			CLayer* pIntroSceneUILayer = pIntroScene->FindLayer("UI");
			CGameObject* pManager = pIntroSceneUILayer->FindGameObjectWithTag("Main Menu Manager");
			CTitleSceneManager* pManagerComponent = pManager->GetComponentFromType<CTitleSceneManager>(CT_TITLE_SCENE_MANAGER);
			pManagerComponent->SetChapter0End(m_pScene);
			SAFE_RELEASE(pManagerComponent);
			SAFE_RELEASE(pManager);
			SAFE_RELEASE(pIntroSceneUILayer);

		}
	}

	//TUTORIAL CLIMB 00
	CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
	Vector3 vTutorialMin00 = Vector3(3360.0f, 512.0f, 0.0f);
	Vector3 vTutorialMax00 = Vector3(3488.0f, 544.0f, 0.0f);
	if (vPlayerPosition.x >= vTutorialMin00.x && vPlayerPosition.x <= vTutorialMax00.x &&
		vPlayerPosition.y >= vTutorialMin00.y && vPlayerPosition.y <= vTutorialMax00.y && !m_bTutorialClimb)
	{
		CTransform* pBirdTransform = m_pBird->GetTransform();
		Vector3 vBirdPosition = pBirdTransform->GetWorldPosition();
		SAFE_RELEASE(pBirdTransform);

		CTransform* pTutorialSignTransform = m_pTutorialSign->GetTransform();
		pTutorialSignTransform->SetWorldPosition(vBirdPosition - Vector3(0.0f, 64.0f, 0.0f));
		SAFE_RELEASE(pTutorialSignTransform);
		m_eSignState = SS_CLIMB0;
		m_eSignReserveState = SS_CLIMB0;
		m_fTutorialSignRotationTimer = 0.0f;
		m_bTutorialClimb = true;
		m_bOnTutorial = true;
		m_bExitTutorial = false;
		CRenderer* pSignRenderer = m_pTutorialSign->GetComponentFromType<CRenderer>(CT_RENDERER);
		pSignRenderer->SetImageInfo("ui\\tutorial\\climb00");
		SAFE_RELEASE(pSignRenderer);
	}

	if (m_bOnTutorial)
	{
		if (m_eSignState == SS_CLIMB1  && vPlayerPosition.y <= 350.0f)
		{
			m_bExitTutorial = true;
			m_eSignState = SS_HIDE;
			m_eSignReserveState = SS_HIDE;
		}

		if (m_eSignState == SS_CLIMB0 && pPlayer->m_bOnClimb)
		{
			CTransform* pTutorialSignTransform = m_pTutorialSign->GetTransform();
			float fY = pTutorialSignTransform->GetWorldRotation().y;
			SAFE_RELEASE(pTutorialSignTransform);
			if (fY == 0.0f)
			{
				m_eSignState = SS_HIDE;
				m_eSignReserveState = SS_CLIMB1;
				m_fTutorialSignRotationTimer = 0.0f;
			}
			else if (fY <= PI / 2.0f + 0.001f && fY >= PI / 2.0f - 0.001f)
			{
				m_eSignState = SS_CLIMB1;
				m_eSignReserveState = SS_CLIMB1;
				m_fTutorialSignRotationTimer = 0.0f;
			}
		}

		if (m_eSignState == SS_CLIMB1 && !pPlayer->m_bOnClimb)
		{
			CTransform* pTutorialSignTransform = m_pTutorialSign->GetTransform();
			float fY = pTutorialSignTransform->GetWorldRotation().y;
			SAFE_RELEASE(pTutorialSignTransform);
			if (fY == 0.0f)
			{
				m_eSignState = SS_HIDE;
				m_eSignReserveState = SS_CLIMB0;
				m_fTutorialSignRotationTimer = 0.0f;
			}
			else if (fY <= PI / 2.0f + 0.001f && fY >= PI / 2.0f - 0.001f)
			{
				m_eSignState = SS_CLIMB1;
				m_eSignReserveState = SS_CLIMB0;
				m_fTutorialSignRotationTimer = 0.0f;
			}
		}
		SAFE_RELEASE(pPlayer);

		if (m_eSignState == SS_CLIMB0)
		{
			m_fTutorialSignRotationTimer += fFixedDeltaTime;
			float fRotateProgress = m_fTutorialSignRotationTimer / 0.1f;
			CTransform* pTutorialSignTransform = m_pTutorialSign->GetTransform();
			pTutorialSignTransform->SetPivot(0.5f, 1.0f, 0.0f);
			pTutorialSignTransform->SetWorldRotation(0.0f, Lerp(PI / 2.0f, 0.0f, fRotateProgress), 0.0f);

			if (fRotateProgress >= 1.0f)
			{
				pTutorialSignTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				m_eSignState = m_eSignReserveState;
			}
			SAFE_RELEASE(pTutorialSignTransform);
		}
		if (m_eSignState == SS_CLIMB1)
		{
			m_fTutorialSignRotationTimer += fFixedDeltaTime;
			float fRotateProgress = m_fTutorialSignRotationTimer / 0.1f;
			CTransform* pTutorialSignTransform = m_pTutorialSign->GetTransform();
			pTutorialSignTransform->SetPivot(0.5f, 1.0f, 0.0f);
			pTutorialSignTransform->SetWorldRotation(0.0f, Lerp(PI / 2.0f, 0.0f, fRotateProgress), 0.0f);

			if (fRotateProgress >= 1.0f)
			{
				pTutorialSignTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				m_eSignState = m_eSignReserveState;
			}
			SAFE_RELEASE(pTutorialSignTransform);
		}
		if (m_eSignState == SS_DASH)
		{
			m_fTutorialSignRotationTimer += fFixedDeltaTime;
			float fRotateProgress = m_fTutorialSignRotationTimer / 0.1f;
			CTransform* pTutorialSignTransform = m_pTutorialSign->GetTransform();
			pTutorialSignTransform->SetPivot(0.5f, 1.0f, 0.0f);
			pTutorialSignTransform->SetWorldRotation(0.0f, Lerp(PI / 2.0f, 0.0f, fRotateProgress), 0.0f);

			if (fRotateProgress >= 1.0f)
			{
				pTutorialSignTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				m_eSignState = m_eSignReserveState;
			}
			SAFE_RELEASE(pTutorialSignTransform);
		}
		if (m_eSignState == SS_HIDE)
		{
			m_fTutorialSignRotationTimer += fFixedDeltaTime;
			float fRotateProgress = m_fTutorialSignRotationTimer / 0.1f;
			CTransform* pTutorialSignTransform = m_pTutorialSign->GetTransform();
			pTutorialSignTransform->SetPivot(0.5f, 1.0f, 0.0f);
			pTutorialSignTransform->SetWorldRotation(0.0f, Lerp(0.0f, PI / 2.0f, fRotateProgress), 0.0f);

			if (fRotateProgress >= 1.0f)
			{
				pTutorialSignTransform->SetWorldRotation(0.0f, PI / 2.0f, 0.0f);
				m_eSignState = m_eSignReserveState;
				m_fTutorialSignRotationTimer = 0.0f;

				if (m_bExitTutorial)
				{
					m_bOnTutorial = false;
					CBird* pBird = m_pBird->GetComponentFromType<CBird>(CT_BIRD);
					pBird->FlyAway();
					SAFE_RELEASE(pBird);
				}

				CRenderer* pSignRenderer = m_pTutorialSign->GetComponentFromType<CRenderer>(CT_RENDERER);
				if (m_eSignState == SS_DASH)
				{
					pSignRenderer->SetImageInfo("ui\\tutorial\\dash");
				}
				else if (m_eSignState == SS_CLIMB0)
				{
					pSignRenderer->SetImageInfo("ui\\tutorial\\climb00");
				}
				else if (m_eSignState == SS_CLIMB1)
				{
					pSignRenderer->SetImageInfo("ui\\tutorial\\climb01");
				}
				SAFE_RELEASE(pSignRenderer);

			}
			SAFE_RELEASE(pTutorialSignTransform);
		}
	}


	if (vPlayerPosition.x >= 5216.0f && vPlayerPosition.y >= 32.0f && !m_bDialogueEvent && m_iDialogueProgress == -1)
	{
		GET_SINGLE(CInput)->DisableInput();
		CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
		if (pPlayer->m_bOnGround)
		{
			pPlayer->SetEvent(true);

			CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
			{
				pAnimator->GetAnimationController()->TransitionState("Stand");
			}
			SAFE_RELEASE(pAnimator);

			m_iDialogueProgress = 0;
			m_fDialogueTimer = 0.0f;
			m_pDialogue->OpenDialogue(PD_LEFT, PC_MADELINE, "Madeline Normal");
			m_pDialogue->SetAnimation("Madeline Normal Talk");
			m_pDialogue->SetString(L"저기... 실례합니다, 할머니.     ");
			m_pDialogue->SetReserveAnimation("Madeline Normal");

		}
		SAFE_RELEASE(pPlayer);
	}

	if (m_iDialogueProgress == 0)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_iDialogueProgress = 1;
			m_pDialogue->CloseDialogue();
		}
	}

	if (m_iDialogueProgress == 1)
	{
		m_fDialogueTimer += fTime;
		if (m_fDialogueTimer >= 1.5f)
		{
			CTransform* pGrannyTransform = m_pGranny->GetTransform();
			pGrannyTransform->SetWorldRotation(0.0f, PI, 0.0f);

			CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
			{
				pAnimator->GetAnimationController()->TransitionState("Run Slow");
			}

			pPlayerTransform = m_pPlayer->GetTransform();
			if (m_fDialogueTimer <= 4.0f)
			{
				pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(32.0f * 4.0f, 0.0f, 0.0f) * fTime);
			}
			else
			{
				if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
				{
					pAnimator->GetAnimationController()->TransitionState("Stand");
				}

				m_fDialogueTimer = 0.0f;
				m_iDialogueProgress = 2;
			}

			SAFE_RELEASE(pGrannyTransform);
			SAFE_RELEASE(pPlayerTransform);
			SAFE_RELEASE(pAnimator);
		}
	}

	if (m_iDialogueProgress == 2)
	{
		m_pDialogue->OpenDialogue(PD_LEFT, PC_MADELINE, "Madeline Normal");
		m_pDialogue->SetAnimation("Madeline Normal Talk");
		m_pDialogue->SetString(L"입구에 있는 표지판이 망가져서 그러는데...\n여기가 산 정상으로 가는 길이 맞나요??     ");
		m_pDialogue->SetReserveAnimation("Madeline Normal"); 
		m_iDialogueProgress = 3;
	}

	if (m_iDialogueProgress == 3)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_iDialogueProgress = 4;
			m_pDialogue->CloseDialogue();
		}
	}

	if (m_iDialogueProgress == 4)
	{
		if (m_pDialogue->GetState() == DS_NONE)
		{
			m_pDialogue->OpenDialogue(PD_RIGHT, PC_GRANNY, "Granny Normal");
			m_pDialogue->SetAnimation("Granny Normal Talk");
			m_pDialogue->SetString(L"그래... 산 입구라면 얼마 남지 않았단다.\n저기 보이는 다리만 건너면 돼.     ");
			m_pDialogue->SetReserveAnimation("Granny Normal");
			m_iDialogueProgress = 5;
		}
	}

	if (m_iDialogueProgress == 5)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_iDialogueProgress = 6;
			m_pDialogue->CloseDialogue();
		}
	}
	
	if (m_iDialogueProgress == 6)
	{
		CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

		if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
		{
			pAnimator->GetAnimationController()->TransitionState("Run Slow");
		}

		pPlayerTransform = m_pPlayer->GetTransform();
		pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(32.0f * 4.0f, 0.0f, 0.0f) * fTime);

		if (pPlayerTransform->GetWorldPosition().x >= 5750.0f)
		{
			if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
			{
				pAnimator->GetAnimationController()->TransitionState("Stand");
			}
			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = 7;
		}
		SAFE_RELEASE(pPlayerTransform);
		SAFE_RELEASE(pAnimator);
	}


	if (m_iDialogueProgress == 7)
	{
		m_fDialogueTimer += fTime;

		if (m_fDialogueTimer >= 0.5f)
		{
			pPlayerTransform = m_pPlayer->GetTransform();
			pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
			SAFE_RELEASE(pPlayerTransform);
		}

		if (m_fDialogueTimer >= 0.75f)
		{
			CTransform* pGrannyTransform = m_pGranny->GetTransform();
			pGrannyTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = 8;
			SAFE_RELEASE(pGrannyTransform);
		}
	}


	if (m_iDialogueProgress == 8)
	{
		m_fDialogueTimer += fTime;

		if (m_fDialogueTimer >= 1.0f)
		{
			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = 9;
		}
	}

	if (m_iDialogueProgress == 9)
	{
		m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
		m_pDialogue->SetAnimation("Madeline Normal Talk");
		m_pDialogue->SetString(L"그나저나, 사람을 불러서 입구 쪽 길을\n정비해야 할 것 같아요.     ");
		m_pDialogue->SetReserveAnimation("Madeline Normal");
		m_iDialogueProgress = 10;
	}
	if (m_iDialogueProgress == 10)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->SetAnimation("Madeline Sad Talk");
			m_pDialogue->SetString(L"길이 무너지는 바람에 거의 죽을 뻔 했다구요.     ");
			m_pDialogue->SetReserveAnimation("Madeline Normal");
			m_iDialogueProgress = 11;
		}
	}
	if (m_iDialogueProgress == 11)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->CloseDialogue();

			CAnimator* pGrannyAnimator = m_pGranny->GetComponentFromType<CAnimator>(CT_ANIMATOR);
			if (pGrannyAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "OldLady Laugh")
			{
				pGrannyAnimator->GetAnimationController()->TransitionState("OldLady Laugh");
			}
			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = 12;
			SAFE_RELEASE(pGrannyAnimator);
		}
	}

	if (m_iDialogueProgress == 12)
	{
		m_fDialogueTimer += fTime;

		if (m_fDialogueTimer >= 1.2f)
		{
			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = 13;
		}
	}	
	
	if (m_iDialogueProgress == 13)
	{
		m_pDialogue->OpenDialogue(PD_LEFT, PC_GRANNY, "Granny Laugh");
		m_pDialogue->SetAnimation("Granny Laugh");
		m_pDialogue->SetString(L"겨우 \"입구\"에서부터 죽을 뻔 했으면,\n하늘 산은 너에게 조금 버거울지도 모르겠구나.     ");
		m_pDialogue->SetReserveAnimation("Granny Laugh");

		CAnimator* pGrannyAnimator = m_pGranny->GetComponentFromType<CAnimator>(CT_ANIMATOR);
		if (pGrannyAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "OldLady Air Quotes")
		{
			pGrannyAnimator->GetAnimationController()->TransitionState("OldLady Air Quotes");
		}
		m_fDialogueTimer = 0.0f;
		m_iDialogueProgress = 14;
		SAFE_RELEASE(pGrannyAnimator);
	}

	if (m_iDialogueProgress == 14)
	{
		m_fDialogueTimer += fTime;

		if (m_fDialogueTimer >= 1.92f)
		{
			CAnimator* pGrannyAnimator = m_pGranny->GetComponentFromType<CAnimator>(CT_ANIMATOR);
			if (pGrannyAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "OldLady Point Cane")
			{
				pGrannyAnimator->GetAnimationController()->TransitionState("OldLady Point Cane");
			}
			SAFE_RELEASE(pGrannyAnimator);

			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = 15;
		}
	}

	if (m_iDialogueProgress == 15)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->CloseDialogue();

			CAnimator* pGrannyAnimator = m_pGranny->GetComponentFromType<CAnimator>(CT_ANIMATOR);
			if (pGrannyAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "OldLady Laugh")
			{
				pGrannyAnimator->GetAnimationController()->TransitionState("OldLady Laugh");
			}
			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = 16;
			SAFE_RELEASE(pGrannyAnimator);
		}
	}
	
	if (m_iDialogueProgress == 16)
	{
		if (m_pDialogue->GetState() == DS_NONE)
		{
			m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Upset");
			m_pDialogue->SetAnimation("Madeline Upset Talk");
			m_pDialogue->SetString(L"...     ");
			m_pDialogue->SetReserveAnimation("Madeline Angry");
			m_iDialogueProgress = 17;
			m_fDialogueTimer = 0.0f;
		}
	}
	if (m_iDialogueProgress == 17)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{			
			m_pDialogue->SetAnimation("Madeline Angry Talk");
			m_pDialogue->SetString(L"뭐, 당신 같은 쭈구렁 할머니도\n입구를 통과했을 테니,\n저도 계속 괜찮겠죠, 뭐.     ");
			m_pDialogue->SetReserveAnimation("Madeline Angry");
			m_iDialogueProgress = 18;
			m_fDialogueTimer = 0.0f;
		}
	}

	if (m_iDialogueProgress == 18)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->CloseDialogue();
			CAnimator* pGrannyAnimator = m_pGranny->GetComponentFromType<CAnimator>(CT_ANIMATOR);
			if (pGrannyAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "OldLady Idle")
			{
				pGrannyAnimator->GetAnimationController()->TransitionState("OldLady Idle");
			}
			SAFE_RELEASE(pGrannyAnimator);
			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = 19;
		}
	}
	if (m_iDialogueProgress == 19)
	{
		if (m_pDialogue->GetState() == DS_NONE)
		{
			m_pDialogue->OpenDialogue(PD_LEFT, PC_GRANNY, "Granny Normal");
			m_pDialogue->SetAnimation("Granny Normal Talk");
			m_pDialogue->SetString(L"하하하... 그래 그래.\n너 좋을 대로 하려무나.     ");
			m_pDialogue->SetReserveAnimation("Granny Normal");
			m_iDialogueProgress = 20;
			m_fDialogueTimer = 0.0f;
		}
	}
	if (m_iDialogueProgress == 20)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->SetAnimation("Granny Normal Talk");
			m_pDialogue->SetString(L"하지만 이거 하난 명심하렴.\n하늘 산은 조금 유별난 곳이란다.     ");
			m_pDialogue->SetReserveAnimation("Granny Creep A");
			m_iDialogueProgress = 21;
			m_fDialogueTimer = 0.0f;
		}
	}
	if (m_iDialogueProgress == 21)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->SetAnimation("Granny Creep A Talk");
			m_pDialogue->SetString(L"뭐.. 아마 두 눈으로 직접 보게 될 테니.\n긴 말은 하지 않으마.     ");
			m_pDialogue->SetReserveAnimation("Granny Creep B");
			m_iDialogueProgress = 22;
			m_fDialogueTimer = 0.0f;
		}
	}

	if (m_iDialogueProgress == 22)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->SetAnimation("Granny Creep B Talk");
			m_pDialogue->SetString(L"아니... 어쩌면,\n아직 아가가 보기엔 이를지도 모르겠는걸.     ");
			m_pDialogue->SetReserveAnimation("Granny Creep B");
			m_iDialogueProgress = 23;
			m_fDialogueTimer = 0.0f;
		}
	}

	if (m_iDialogueProgress == 23)
	{
		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->CloseDialogue();
			m_iDialogueProgress = 24;
			m_fDialogueTimer = 0.0f;
		}
	}
	if (m_iDialogueProgress == 24)
	{
		if (m_pDialogue->GetState() == DS_NONE)
		{
			m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Upset");
			m_pDialogue->SetAnimation("Madeline Upset Talk");
			m_pDialogue->SetString(L"흥!\n시덥잖은 소리 마세요, 부인.     ");
			m_pDialogue->SetReserveAnimation("Madeline Upset");
			m_iDialogueProgress = 25;
			m_fDialogueTimer = 0.0f;
		}
	}
	if (m_iDialogueProgress == 25)
	{
		CTransform* pTr = m_pPlayer->GetTransform();
		Vector3 vTempPlayerPosition = pTr->GetWorldPosition();

		if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
		{
			m_pDialogue->CloseDialogue();
			CAnimator* pGrannyAnimator = m_pGranny->GetComponentFromType<CAnimator>(CT_ANIMATOR);
			if (pGrannyAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "OldLady Laugh")
			{
				pGrannyAnimator->GetAnimationController()->TransitionState("OldLady Laugh");
			}
			m_fDialogueTimer = 0.0f;
			m_iDialogueProgress = -1;
			m_bDialogueEvent = true;
			CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
			pPlayer->ResetCurrentPosition();

			pPlayer->SetEvent(false);
			pPlayer->SetAwake(true);

			CTransform* pBirdTransform = m_pBird->GetTransform();
			pBirdTransform->SetWorldPosition(8064.0f, 128.0f, 0.0f);
			SAFE_RELEASE(pBirdTransform);

			GET_SINGLE(CInput)->EnableInput();
			SAFE_RELEASE(pPlayer);
			SAFE_RELEASE(pGrannyAnimator);
		}


		SAFE_RELEASE(pTr);
	}

	return 0;
}
