#include "StageManager01.h"
#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "Input.h"
#include "../Client.h"
#include "Component.h"
#include "TimerManager.h"
#include "Timer.h"
#include "../GameData.h"
#include "Layer.h"
#include "MovingBlock.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "SoundManager.h"
#include "Device.h"

#include "SceneManager.h"
#include "../Scene/IntroScene.h"
#include "TitleSceneManager.h"


CStageManager01::CStageManager01():
	m_bInitialized(false),
	m_pComplete(NULL),
	m_pPlayer(NULL),
	m_pTheo(NULL),
	m_pNPCLable(NULL),
	m_iTheoInterationCount(0),
	m_iTheoDialogueProgress(0),
	m_iSelfDialogueProgress(0),
	m_eLableState(LS_HIDE),
	m_fLableTimer(0.0f),
	m_bOnInteraction(false),
	m_fDialogueTimer(0.0f),
	m_fSoundFadingTimer(0.0f),
	m_eFadeState(SFS_NORMAL),
	m_pTheoEventArea(NULL),
	m_pMonologueEventArea(NULL)
{
}
CStageManager01::~CStageManager01()
{
}

void CStageManager01::SoundFadeOut()
{
	if (m_eFadeState != SFS_FADE_OUT && m_eFadeState != SFS_FADE)
	{
		m_eFadeState = SFS_FADE_OUT;
		m_fSoundFadingTimer = 0.0f;
	}
}

void CStageManager01::SoundFadeIn()
{
	if (m_eFadeState != SFS_FADE_IN && m_eFadeState != SFS_NORMAL)
	{
		m_eFadeState = SFS_FADE_IN;
		m_fSoundFadingTimer = 0.0f;
	}
}

void CStageManager01::CreateZipMover(const string & strFileName, const Vector3 & vBeginPosition, const Vector3 & bEndPosition, const Vector3 & vForce, int iBlockType)
{
	CLayer* pLayer = m_pScene->FindLayer("Object");
	CGameObject* pZipMover = GET_SINGLE(CGameData)->LoadPrefab(strFileName, m_pScene, pLayer);
	SAFE_RELEASE(pLayer);

	CMovingBlock* pMovingBlock = pZipMover->AddComponent<CMovingBlock>("MovingBlock");
	pMovingBlock->SetBeginPosition(vBeginPosition);
	pMovingBlock->SetEndPosition(bEndPosition);
	pMovingBlock->SetBlockType(iBlockType);
	pMovingBlock->SetForce(vForce);
	SAFE_RELEASE(pMovingBlock);

	CTransform* pTransform = pZipMover->GetTransform();
	pTransform->SetWorldPosition(vBeginPosition);
	SAFE_RELEASE(pTransform);
}

void CStageManager01::LockPlayer()
{	
	SoundFadeOut();
	m_bOnInteraction = true;
	CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
	pPlayer->SetInteraction(true);
	pPlayer->SetEvent(true);
	SAFE_RELEASE(pPlayer);
	GET_SINGLE(CInput)->DisableInput();
}

void CStageManager01::UnLockPlayer()
{
	SoundFadeIn();
	m_bOnInteraction = false;
	CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
	pPlayer->SetInteraction(false);
	pPlayer->SetEvent(false);
	pPlayer->ResetPhysics();
	SAFE_RELEASE(pPlayer);
	GET_SINGLE(CInput)->EnableInput();
}

void CStageManager01::SetMadelineCampfire(CGameObject * pTarget)
{
	m_pCampfire = pTarget;
}

void CStageManager01::SetDialogue(CDialogue * pDialogue)
{
	m_pDialogue = pDialogue;
}

void CStageManager01::InitializeDialogue()
{
	m_pDialogue->OpenDialogue(PD_LEFT, PC_MADELINE, "Madeline Normal");
	m_pDialogue->CloseDialogue();
}

void CStageManager01::SetPlayer(CGameObject * pPlayer)
{
	m_pPlayer = pPlayer;
}

bool CStageManager01::Initialize()
{
	GET_SINGLE(CInput)->EnableInput();

	CreateZipMover("Zipmover32", Vector3(3392.0f, -2400.0f, 0.0f), Vector3(3680.0f, -2400.0f, 0.0f), Vector3(500.0f,	0.0f,			0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover42", Vector3(4512.0f, -3552.0f, 0.0f), Vector3(4512.0f, -3872.0f, 0.0f), Vector3(0.0f,		250.0f,		0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover23", Vector3(5824.0f, -3296.0f, 0.0f), Vector3(6048.0f, -3296.0f, 0.0f), Vector3(500.0f,	0.0f,			0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover42", Vector3(8384.0f, -5120.0f, 0.0f), Vector3(8640.0f, -5120.0f, 0.0f), Vector3(500.0f,	0.0f,			0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover52", Vector3(8864.0f, -5152.0f, 0.0f), Vector3(8864.0f, -5344.0f, 0.0f), Vector3(0.0f,		250.0f,		0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover24", Vector3(7232.0f, -5984.0f, 0.0f), Vector3(7040.0f, -5984.0f, 0.0f), Vector3(-500.0f,	0.0f,			0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover27S", Vector3(7520.0f, -6432.0f, 0.0f), Vector3(7520.0f, -6080.0f, 0.0f), Vector3(0.0f,		250.0f,		0.0f), MBT_TOUCH);

	CreateZipMover("Zipmover22", Vector3(12128.0f, -6624.0f, 0.0f), Vector3(12128.0f, -6912.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover22", Vector3(12352.0f, -6592.0f, 0.0f), Vector3(12352.0f, -6880.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover22", Vector3(12576.0f, -6656.0f, 0.0f), Vector3(12576.0f, -6944.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	
	CreateZipMover("Zipmover24", Vector3(8864.0f, -7584.0f, 0.0f), Vector3(8864.0f, -7840.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover42", Vector3(12608.0f, -7552.0f, 0.0f), Vector3(12608.0f, -7744.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover62", Vector3(12960.0f, -7520.0f, 0.0f), Vector3(12960.0f, -7744.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover33", Vector3(11840.0f, -8192.0f, 0.0f), Vector3(11840.0f, -8384.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover23S", Vector3(11584.0f, -8480.0f, 0.0f), Vector3(11200.0f, -8480.0f, 0.0f), Vector3(-500.0f, 0.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover35S", Vector3(13248.0f, -8576.0f, 0.0f), Vector3(13792.0f, -8576.0f, 0.0f), Vector3(500.0f, 0.0f, 0.0f), MBT_TOUCH);
	
	//16!!!
	CreateZipMover("Zipmover34S", Vector3(14304.0f, -8992.0f, 0.0f), Vector3(14560.0f, -8992.0f, 0.0f), Vector3(500.0f, 0.0f, 0.0f), MBT_DASH);
	
	
	CreateZipMover("Zipmover42", Vector3(15648.0f, -8640.0f, 0.0f), Vector3(15904.0f, -8640.0f, 0.0f), Vector3(500.0f, 0.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover62", Vector3(13920.0f, -9376.0f, 0.0f), Vector3(13920.0f, -9568.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover33", Vector3(13536.0f, -10528.0f, 0.0f), Vector3(13728.0f, -10528.0f, 0.0f), Vector3(500.0f, 0.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover24S", Vector3(14944.0f, -10752.0f, 0.0f), Vector3(14944.0f, -11008.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover23S", Vector3(15360.0f, -11776.0f, 0.0f), Vector3(15584.0f, -11776.0f, 0.0f), Vector3(500.0f, 0.0f, 0.0f), MBT_TOUCH);
	//CreateZipMover("Zipmover34S", Vector3(16416.0f, -11872.0f, 0.0f), Vector3(16416.0f, -12128.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);
	CreateZipMover("Zipmover34", Vector3(16416.0f, -11872.0f, 0.0f), Vector3(16416.0f, -12128.0f, 0.0f), Vector3(0.0f, 250.0f, 0.0f), MBT_TOUCH);

	
	//ex
	CreateZipMover("Zipmover43", Vector3(12320.0f, -10848.0f, 0.0f), Vector3(12608.0f, -10848.0f, 0.0f), Vector3(500.0f, 0.0f, 0.0f), MBT_TOUCH);


	//Dialogue Exist; Press X
	{
		CLayer* pUILayer = m_pScene->FindLayer("BackUI");

		CGameObject* pLable = CGameObject::CreateObject("Lable", pUILayer);

		CTransform* pTransform = pLable->GetTransform();
		pTransform->SetWorldScale(0.0f, 0.0f, 1.0f);
		pTransform->SetWorldPosition(3040.0f, -4832.0f, 0.0f);
		pTransform->SetPivot(0.5f, 1.0f, 0.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pLable->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial* pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_POINT);

		pRenderer->SetImageInfo("ui\\keypress\\x");
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pUILayer);
		m_pNPCLable = pLable;
		SAFE_RELEASE(pLable);

	}


	//Complete Image
	{
		CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
		Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
		SAFE_RELEASE(pCameraTransform);

		CLayer* pUILayer = m_pScene->FindLayer("UI");
		CGameObject* pComplete = CGameObject::CreateObject("Complete", pUILayer);

		//Pivot이 0, 0일 떄 화면 과 크기가 맞다면 화면에 항상 들어옴 
		CTransform* pTransform = pComplete->GetTransform();
		pTransform->SetWorldScale(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight, 1.0f);
		pTransform->SetWorldPosition(vCameraPosition);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pComplete->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial* pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Chapter1_Complete", L"Celeste\\Chapter01.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.0f));

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pUILayer);
		m_pComplete = pComplete;
		SAFE_RELEASE(pComplete);

	}


	//Fade
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Fade", m_pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(0.0f, 50000.0f, 0.0f);
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


	GET_SINGLE(CSoundManager)->LoadSound(SOUND_BGM, "mus_epilogue_loop", true, "mus_epilogue_loop.wav", BGM_PATH);

	return true;
}

int CStageManager01::Update(float fTime)
{

	if (GETASYNCKEYDOWN(VK_F1))
	{
		CTransform* pTransform = m_pPlayer->GetTransform();
		pTransform->SetWorldPosition(16528.0f, -12480.0f, 0.0f);
		SAFE_RELEASE(pTransform);
	}



	if (!m_bInitialized)
	{
		CLayer* pNpcLayer = m_pScene->FindLayer("NPC");
		CGameObject* pTheo = pNpcLayer->FindGameObjectWithTag("Theo");
		CGameObject* pCampfire = pNpcLayer->FindGameObjectWithTag("M_Campfire");
		m_pCampfire = pCampfire;
		m_pTheo = pTheo;
		SAFE_RELEASE(pCampfire);
		SAFE_RELEASE(pTheo);
		SAFE_RELEASE(pNpcLayer);
		m_bInitialized = true;

		CTransform* pTheoTransform = m_pTheo->GetTransform();
		Vector3 vTheoPosition = pTheoTransform->GetWorldPosition();
		SAFE_RELEASE(pTheoTransform);
		CTransform* pCampfireTransform = m_pCampfire->GetTransform();
		Vector3 vCampfirePosition = pCampfireTransform->GetWorldPosition();
		SAFE_RELEASE(pCampfireTransform);


		m_pTheoEventArea = GET_SINGLE(CGameData)->GetAreaData(vTheoPosition);
		m_pMonologueEventArea = GET_SINGLE(CGameData)->GetAreaData(vCampfirePosition);

		CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
		pPlayer->SetMaxDash(5);
		SAFE_RELEASE(pPlayer);
			
	}



	CTransform* pTempPlayerTransform = m_pPlayer->GetTransform();
	Vector3 vPlayerPosition = pTempPlayerTransform->GetWorldPosition();
	Vector3 vPlayerRotation = pTempPlayerTransform->GetWorldRotation();
	SAFE_RELEASE(pTempPlayerTransform);


	PAREA pPlayerArea = GET_SINGLE(CGameData)->GetAreaData(vPlayerPosition);

#pragma region Theo Lable

	if (pPlayerArea == m_pTheoEventArea)
	{		
		CTransform* pTheo = m_pTheo->GetTransform();
		Vector3 vTheoPosition = pTheo->GetWorldPosition();
		SAFE_RELEASE(pTheo);

		CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);

		
		if ((vPlayerPosition - vTheoPosition).Length() <= 150.0f)
		{
			if (m_iTheoInterationCount <= 5 && !m_bOnInteraction)
			{
				if (m_eLableState == LS_HIDE)
				{
					pPlayer->SetInteraction(true);
					m_eLableState = LS_ON_GENERATION;
					m_fLableTimer = 0.0f;
				}

				if (GETKEYDOWN('X') && pPlayer->OnGround())
				{
					++m_iTheoInterationCount;
					pPlayer->SetInteraction(true);
					m_eLableState = LS_ON_HIDING;
					m_fLableTimer = 0.0f;
					m_bOnInteraction = true;

					CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
						pAnimator->GetAnimationController()->TransitionState("Stand");
					SAFE_RELEASE(pAnimator);
				}
			}
		}
		else
		{
			if (m_eLableState == LS_ON_STANDBY)
			{
				pPlayer->SetInteraction(false);
				m_eLableState = LS_ON_HIDING;
				m_fLableTimer = 0.0f;
			}
		}
		SAFE_RELEASE(pPlayer);

		if (m_eLableState == LS_HIDE)
		{
			CTransform* pTransform = m_pNPCLable->GetTransform();
			pTransform->SetWorldScale(0.0f, 0.0f, 0.0f);
			SAFE_RELEASE(pTransform);
		}

		if (m_eLableState == LS_ON_GENERATION)
		{
			Vector3 vScale(44.0f, 56.0f, 1.0f);
			m_fLableTimer += fTime;
			float fProgress = m_fLableTimer / 0.15f;

			if (fProgress >= 1.0f)
			{
				CTransform* pTransform = m_pNPCLable->GetTransform();
				pTransform->SetWorldScale(vScale);
				SAFE_RELEASE(pTransform);
				m_eLableState = LS_ON_STANDBY;
				m_fLableTimer = 0.0f;
			}
			else
			{
				vScale = vScale * sinf(fProgress * PI * 0.5f);
				CTransform* pTransform = m_pNPCLable->GetTransform();
				pTransform->SetWorldScale(vScale);
				SAFE_RELEASE(pTransform);
			}
		}

		if (m_eLableState == LS_ON_HIDING)
		{
			Vector3 vScale(44.0f, 56.0f, 1.0f);
			m_fLableTimer += fTime;
			float fProgress = m_fLableTimer / 0.15f;

			if (fProgress >= 1.0f) 
			{
				CTransform* pTransform = m_pNPCLable->GetTransform();
				pTransform->SetWorldScale(0.0f, 0.0f, 0.0f);
				SAFE_RELEASE(pTransform);
				m_eLableState = LS_HIDE;
				m_fLableTimer = 0.0f;
			}
			else
			{
				vScale = vScale * cosf(fProgress * PI * 0.5f);
				CTransform* pTransform = m_pNPCLable->GetTransform();
				pTransform->SetWorldScale(vScale);
				SAFE_RELEASE(pTransform);
			}
		}

		if (m_eLableState == LS_ON_STANDBY)
		{
			CTransform* pTransform = m_pNPCLable->GetTransform();
			pTransform->SetWorldScale(44.0f, 56.0f, 1.0f);
			SAFE_RELEASE(pTransform);
		}
	}
#pragma endregion


#pragma region Sound
	{
		if (m_eFadeState == SFS_NORMAL)
		{
		}

		if (m_eFadeState == SFS_FADE_IN)
		{
			m_fSoundFadingTimer += fTime;
			float fProgress = m_fSoundFadingTimer / 0.5f;

			float fBGMVolume = Lerp(0.25f, 1.0f, fProgress);
			GET_SINGLE(CSoundManager)->Volume(SOUND_BGM, fBGMVolume);
			GET_SINGLE(CSoundManager)->Volume(SOUND_ENV, fBGMVolume);
			if (fProgress >= 1.0f)
			{
				m_eFadeState = SFS_NORMAL;
			}
		}

		if (m_eFadeState == SFS_FADE_OUT)
		{
			m_fSoundFadingTimer += fTime;
			float fProgress = m_fSoundFadingTimer / 0.5f;

			float fBGMVolume = Lerp(1.0f, 0.25f, fProgress);
			GET_SINGLE(CSoundManager)->Volume(SOUND_BGM, fBGMVolume);
			GET_SINGLE(CSoundManager)->Volume(SOUND_ENV, fBGMVolume);
			if (fProgress >= 1.0f)
			{
				m_eFadeState = SFS_FADE;
			}
		}

		if (m_eFadeState == SFS_FADE)
		{
		}
	}
#pragma endregion

	if (pPlayerArea == m_pTheoEventArea)
	{
#pragma region Theo A

		{
			if (m_bOnInteraction && m_iTheoInterationCount == 1)
			{
				LockPlayer();

				CTransform* pTheo = m_pTheo->GetTransform();

				// Theo의 오른쪽에 Player 있음
				if (pTheo->GetWorldPosition().x - vPlayerPosition.x < 0)
				{
					pTheo->SetWorldRotation(0.0f, 0.0f, 0.0f);
					CTransform* pPlayerTransform = m_pPlayer->GetTransform();
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					SAFE_RELEASE(pPlayerTransform);
				}
				else
				{
					pTheo->SetWorldRotation(0.0f, PI, 0.0f);
					CTransform* pPlayerTransform = m_pPlayer->GetTransform();
					pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					SAFE_RELEASE(pPlayerTransform);
				}
				SAFE_RELEASE(pTheo);

				//Open Dialogue No Prevously
				if (m_iTheoDialogueProgress == 0)
				{
					m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
					m_pDialogue->SetAnimation("Theo Normal Talk");
					m_pDialogue->SetString(L"안녕! 여행자 친구!     ");
					m_pDialogue->SetReserveAnimation("Theo Normal");
					m_iTheoDialogueProgress = 1;

				}
				//Close Dialogue . No Continuosly
				if (m_iTheoDialogueProgress == 1)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 2;
						m_pDialogue->CloseDialogue();
					}
				}
				if (m_iTheoDialogueProgress == 2)
				{
					if (m_pDialogue->GetState() == DS_NONE)
					{
						m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
						m_pDialogue->SetBackground(PC_MADELINE);
						m_pDialogue->SetAnimation("Madeline Normal Talk");
						m_pDialogue->SetString(L"어... 안녕.     ");
						m_pDialogue->SetReserveAnimation("Madeline Normal");
						m_iTheoDialogueProgress = 3;
					}
				}
				if (m_iTheoDialogueProgress == 3)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 4;
						m_pDialogue->CloseDialogue();
					}
				}

				if (m_iTheoDialogueProgress == 4)
				{
					CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
					{
						pAnimator->GetAnimationController()->TransitionState("Run Slow");
					}

					CTransform* pPlayerTransform = m_pPlayer->GetTransform();

					Vector3 vTargetPosition(3232.0f, -4736.0f, 0.0f);
					Vector3 vTargetDirection = (vTargetPosition - pPlayerTransform->GetWorldPosition()).Normalize();

					//
					if (vTargetDirection.x < 0)
					{
						pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					}
					else
					{
						pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					}

					pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(32.0f * 4.0f * vTargetDirection.x, 0.0f, 0.0f) * fTime);

					if ((pPlayerTransform->GetWorldPosition() - vTargetPosition).Length() <= 4.0f)
					{
						pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
						if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
						{
							pAnimator->GetAnimationController()->TransitionState("Stand");
						}
						m_iTheoDialogueProgress = 5;
					}
					SAFE_RELEASE(pPlayerTransform);
					SAFE_RELEASE(pAnimator);
				}

				if (m_iTheoDialogueProgress == 5)
				{
					m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
					m_pDialogue->SetAnimation("Theo Normal Talk");
					m_pDialogue->SetString(L"오늘 진짜 등산하기 끝내주는\n날인거 같지 않아?     ");
					m_pDialogue->SetReserveAnimation("Theo Normal");
					m_iTheoDialogueProgress = 6;
				}

				if (m_iTheoDialogueProgress == 6)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 7;
						m_pDialogue->CloseDialogue();
					}
				}
				if (m_iTheoDialogueProgress == 7)
				{
					m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Distracted");
					m_pDialogue->SetAnimation("Madeline Distracted Talk");
					m_pDialogue->SetString(L"뭐...\n그런 것 같네.     ");
					m_pDialogue->SetReserveAnimation("Madeline Distracted");
					m_iTheoDialogueProgress = 8;
				}
				if (m_iTheoDialogueProgress == 8)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 9;
						m_pDialogue->CloseDialogue();
					}
				}
				if (m_iTheoDialogueProgress == 9)
				{
					m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
					m_pDialogue->SetAnimation("Theo Normal Talk");
					m_pDialogue->SetString(L"여기까지 오면서 몇 번이나\n고비가 있었는지 몰라!!     ");
					m_pDialogue->SetReserveAnimation("Theo Normal");
					m_iTheoDialogueProgress = 10;
				}
				if (m_iTheoDialogueProgress == 10)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_pDialogue->SetAnimation("Theo Excited Talk");
						m_pDialogue->SetString(L"아직도 이 곳이 현실 세계인지\n믿기 힘들 정도라니깐!     ");
						m_pDialogue->SetReserveAnimation("Theo Excited");
						m_iTheoDialogueProgress = 11;
					}
				}
				if (m_iTheoDialogueProgress == 11)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 12;
						m_pDialogue->CloseDialogue();
					}
				}
				if (m_iTheoDialogueProgress == 12)
				{
					m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Sad");
					m_pDialogue->SetAnimation("Madeline Sad Talk");
					m_pDialogue->SetString(L"그래, 나도 마찬가지야...\n정말 올라가기 쉽지 않은 것 같아.     ");
					m_pDialogue->SetReserveAnimation("Madeline Sad");
					m_iTheoDialogueProgress = 13;
				}
				if (m_iTheoDialogueProgress == 13)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_pDialogue->SetAnimation("Madeline Distracted Talk");
						m_pDialogue->SetString(L"어쩌면 그게...\n내가 바라던 거일 수도 있지만...     ");
						m_pDialogue->SetReserveAnimation("Madeline Distracted");
						m_iTheoDialogueProgress = 14;
					}
				}
				if (m_iTheoDialogueProgress == 14)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 15;
						m_pDialogue->CloseDialogue();
					}
				}
				if (m_iTheoDialogueProgress == 15)
				{
					m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Excited");
					m_pDialogue->SetAnimation("Theo Excited Talk");
					m_pDialogue->SetString(L"와, 너 진심으로 하는 소리야?\n난 그냥, 이런 황량한 곳에서 다른 사람을\n본 걸로 만족스러운데 말이야.     ");
					m_pDialogue->SetReserveAnimation("Theo Excited");
					m_iTheoDialogueProgress = 16;
				}
				if (m_iTheoDialogueProgress == 16)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_pDialogue->SetAnimation("Theo Normal Talk");
						m_pDialogue->SetString(L"아무튼, 내 이름은 테오야.\n\"상상도 못 할 정도로 엄청나게 먼 나라\"에서\n온 탐험가지!     ");
						m_pDialogue->SetReserveAnimation("Theo Normal");
						m_iTheoDialogueProgress = 17;
					}
				}
				if (m_iTheoDialogueProgress == 17)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 18;
						m_pDialogue->CloseDialogue();
					}
				}
				if (m_iTheoDialogueProgress == 18)
				{
					m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Sad");
					m_pDialogue->SetAnimation("Madeline Sad");
					m_pDialogue->SetString(L"...     ");
					m_pDialogue->SetReserveAnimation("Madeline Sad");
					m_iTheoDialogueProgress = 19;
				}
				if (m_iTheoDialogueProgress == 19)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 20;
						m_pDialogue->CloseDialogue();
					}
				}
				if (m_iTheoDialogueProgress == 20)
				{
					m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Thinking");
					m_pDialogue->SetAnimation("Theo Thinking Talk");
					m_pDialogue->SetString(L"음...? 넌 조금 과묵한 편이구나?     ");
					m_pDialogue->SetReserveAnimation("Theo Thinking");
					m_iTheoDialogueProgress = 21;
				}
				if (m_iTheoDialogueProgress == 21)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_pDialogue->SetAnimation("Theo Normal Talk");
						m_pDialogue->SetString(L"베일에 싸인 외톨이 늑대라... 그래 알겠어.\n그럼 내가 한번, 너의 어두웠던 과거에 대한\n상상의 나래를 펼쳐보실까.     ");
						m_pDialogue->SetReserveAnimation("Theo Normal");
						m_iTheoDialogueProgress = 22;
					}
				}
				if (m_iTheoDialogueProgress == 22)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iTheoDialogueProgress = 0;
						m_pDialogue->CloseDialogue();
						UnLockPlayer();
					}
				}

			}
		}
#pragma endregion

#pragma region Theo B

		if (m_bOnInteraction && m_iTheoInterationCount == 2)
		{
			LockPlayer();

			CTransform* pTheo = m_pTheo->GetTransform();

			// Theo의 오른쪽에 Player 있음
			if (pTheo->GetWorldPosition().x - vPlayerPosition.x < 0)
			{
				pTheo->SetWorldRotation(0.0f, 0.0f, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			else
			{
				pTheo->SetWorldRotation(0.0f, PI, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			SAFE_RELEASE(pTheo);

			if (m_iTheoDialogueProgress == 0)
			{
				CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

				if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
				{
					pAnimator->GetAnimationController()->TransitionState("Run Slow");
				}

				CTransform* pPlayerTransform = m_pPlayer->GetTransform();

				Vector3 vTargetPosition(3232.0f, -4736.0f, 0.0f);
				Vector3 vTargetDirection = (vTargetPosition - pPlayerTransform->GetWorldPosition()).Normalize();

				//
				if (vTargetDirection.x < 0)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				}
				else
				{
					pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				}

				pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(32.0f * 4.0f * vTargetDirection.x, 0.0f, 0.0f) * fTime);

				if ((pPlayerTransform->GetWorldPosition() - vTargetPosition).Length() <= 4.0f)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
					{
						pAnimator->GetAnimationController()->TransitionState("Stand");
					}
					m_iTheoDialogueProgress = 1;
				}
				SAFE_RELEASE(pPlayerTransform);
				SAFE_RELEASE(pAnimator);
			}
			if (m_iTheoDialogueProgress == 1)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Sad");
				m_pDialogue->SetAnimation("Madeline Sad Talk");
				m_pDialogue->SetString(L"아... 미안. 난 매들린이야.\n잠깐 다른 생각을 좀 하느라고 그랬어.     ");
				m_pDialogue->SetReserveAnimation("Madeline Sad");
				m_iTheoDialogueProgress = 2;
			}
			if (m_iTheoDialogueProgress == 2)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 3;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 3)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
				m_pDialogue->SetAnimation("Theo Normal Talk");
				m_pDialogue->SetString(L"뭐야, 그런 거였어?\n그렇다면야, 여기가 완전 적합한 장소지!     ");
				m_pDialogue->SetReserveAnimation("Theo Normal");
				m_iTheoDialogueProgress = 4;
			}
			if (m_iTheoDialogueProgress == 4)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Excited Talk");
					m_pDialogue->SetString(L"발가락이 얼어붙는 것만 뺀다면.\n여기만큼 조용히 생각에 잠기기\n좋은 곳도 없으니까!     ");
					m_pDialogue->SetReserveAnimation("Theo Excited");
					m_iTheoDialogueProgress = 5;
				}
			}
			if (m_iTheoDialogueProgress == 5)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 6;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 6)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Sad");
				m_pDialogue->SetAnimation("Madeline Sad Talk");
				m_pDialogue->SetString(L"그래. 네 말이 맞을지도...     ");
				m_pDialogue->SetReserveAnimation("Madeline Sad");
				m_iTheoDialogueProgress = 7;
			}
			if (m_iTheoDialogueProgress == 7)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_fDialogueTimer = 0.0f;
					m_pDialogue->CloseDialogue();
					m_iTheoDialogueProgress = 8;
				}
			}
			if (m_iTheoDialogueProgress == 8)
			{
				m_fDialogueTimer += fTime;

				if (m_fDialogueTimer >= 2.0f)
				{
					m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
					m_pDialogue->SetAnimation("Madeline Normal Talk");
					m_pDialogue->SetString(L"그래서... 네가 왔다고 하는 그\n\"엄청 먼 땅\"은 어딜 말하는 거야?     ");
					m_pDialogue->SetReserveAnimation("Madeline Normal");
					m_fDialogueTimer = 0.0f;
					m_iTheoDialogueProgress = 9;
				}
			}
			if (m_iTheoDialogueProgress == 9)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->CloseDialogue();
					m_iTheoDialogueProgress = 10;
				}
			}
			if (m_iTheoDialogueProgress == 10)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Excited");
				m_pDialogue->SetAnimation("Theo Excited Talk");
				m_pDialogue->SetString(L"생각보다 호기심이 많은 친구로군.\n난 말이야\n아주 아주 신비롭고 이국적인 왕국인...     ");
				m_pDialogue->SetReserveAnimation("Theo Excited");
				m_iTheoDialogueProgress = 11;
			}
			if (m_iTheoDialogueProgress == 11)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Excited Talk");
					m_pDialogue->SetString(L"시애틀에서 왔어.     ");
					m_pDialogue->SetReserveAnimation("Theo Excited");
					m_iTheoDialogueProgress = 12;
				}
			}
			if (m_iTheoDialogueProgress == 12)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->CloseDialogue();
					m_iTheoDialogueProgress = 13;
				}
			}
			if (m_iTheoDialogueProgress == 13)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
				m_pDialogue->SetAnimation("Madeline Normal Talk");
				m_pDialogue->SetString(L"음... 특별해 보이는 이름이네.     ");
				m_pDialogue->SetReserveAnimation("Madeline Normal");
				m_iTheoDialogueProgress = 14;
			}
			if (m_iTheoDialogueProgress == 14)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 0;
					m_pDialogue->CloseDialogue();
					UnLockPlayer();
				}
			}
		}

#pragma endregion


#pragma region Theo C

		if (m_bOnInteraction && m_iTheoInterationCount == 3)
		{
			LockPlayer();

			CTransform* pTheo = m_pTheo->GetTransform();

			// Theo의 오른쪽에 Player 있음
			if (pTheo->GetWorldPosition().x - vPlayerPosition.x < 0)
			{
				pTheo->SetWorldRotation(0.0f, 0.0f, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			else
			{
				pTheo->SetWorldRotation(0.0f, PI, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			SAFE_RELEASE(pTheo);

			if (m_iTheoDialogueProgress == 0)
			{
				CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

				if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
				{
					pAnimator->GetAnimationController()->TransitionState("Run Slow");
				}

				CTransform* pPlayerTransform = m_pPlayer->GetTransform();

				Vector3 vTargetPosition(3232.0f, -4736.0f, 0.0f);
				Vector3 vTargetDirection = (vTargetPosition - pPlayerTransform->GetWorldPosition()).Normalize();

				//
				if (vTargetDirection.x < 0)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				}
				else
				{
					pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				}

				pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(32.0f * 4.0f * vTargetDirection.x, 0.0f, 0.0f) * fTime);

				if ((pPlayerTransform->GetWorldPosition() - vTargetPosition).Length() <= 4.0f)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
					{
						pAnimator->GetAnimationController()->TransitionState("Stand");
					}
					m_iTheoDialogueProgress = 1;
				}
				SAFE_RELEASE(pPlayerTransform);
				SAFE_RELEASE(pAnimator);
			}

			if (m_iTheoDialogueProgress == 1)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Excited");
				m_pDialogue->SetAnimation("Theo Excited Talk");
				m_pDialogue->SetString(L"후아! 여기 완전 무법천지잖아!     ");
				m_pDialogue->SetReserveAnimation("Theo Excited");
				m_iTheoDialogueProgress = 2;
			}
			if (m_iTheoDialogueProgress == 2)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Normal Talk");
					m_pDialogue->SetString(L"아무래도 버려진 곳 같은데...\n왜 아깝게 도시를 통째로 버린 걸까?     ");
					m_pDialogue->SetReserveAnimation("Theo Normal");
					m_iTheoDialogueProgress = 3;
				}
			}
			if (m_iTheoDialogueProgress == 3)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 4;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 4)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
				m_pDialogue->SetAnimation("Madeline Normal Talk");
				m_pDialogue->SetString(L"옛날에 대형 기업 몇몇이 힘을 모아\n이 도시를 지었는데, 막상 여기에 살려는 사람이\n아무도 없었다고 그랬던 것 같아.     ");
				m_pDialogue->SetReserveAnimation("Madeline Normal");
				m_iTheoDialogueProgress = 5;
			}
			if (m_iTheoDialogueProgress == 5)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Madeline Distracted Talk");
					m_pDialogue->SetString(L"...사람들은 왜 이 도시를\n좋아하지 않았던 걸까...?     ");
					m_pDialogue->SetReserveAnimation("Madeline Distracted");
					m_iTheoDialogueProgress = 6;
				}
			}
			if (m_iTheoDialogueProgress == 6)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 7;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 7)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Excited");
				m_pDialogue->SetAnimation("Theo Excited Talk");
				m_pDialogue->SetString(L"내가 볼 땐,\n분명 정부의 음모가 있었던 게 분명해.\n내기해도 좋아!     ");
				m_pDialogue->SetReserveAnimation("Theo Excited");
				m_iTheoDialogueProgress = 8;
			}
			if (m_iTheoDialogueProgress == 8)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 9;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 9)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
				m_pDialogue->SetAnimation("Madeline Normal Talk");
				m_pDialogue->SetString(L"정말 아까워...\n이 도시를 건설한 게,\n아무 의미 없는 짓이었다니...     ");
				m_pDialogue->SetReserveAnimation("Madeline Normal");
				m_iTheoDialogueProgress = 10;
			}
			if (m_iTheoDialogueProgress == 10)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 11;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 11)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
				m_pDialogue->SetAnimation("Theo Normal Talk");
				m_pDialogue->SetString(L"뭐, 그래도 아무 의미가 없지는 않은 것 같은데.\n우리들이 이렇게\n잔해 위에서 뛰어놀 수 있으니깐.     ");
				m_pDialogue->SetReserveAnimation("Theo Normal");
				m_iTheoDialogueProgress = 12;
			}
			if (m_iTheoDialogueProgress == 12)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 0;
					m_pDialogue->CloseDialogue();
					UnLockPlayer();
				}
			}
		}


#pragma endregion

#pragma region Theo D

		if (m_bOnInteraction && m_iTheoInterationCount == 4)
		{
			LockPlayer();

			CTransform* pTheo = m_pTheo->GetTransform();

			// Theo의 오른쪽에 Player 있음
			if (pTheo->GetWorldPosition().x - vPlayerPosition.x < 0)
			{
				pTheo->SetWorldRotation(0.0f, 0.0f, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			else
			{
				pTheo->SetWorldRotation(0.0f, PI, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			SAFE_RELEASE(pTheo);

			if (m_iTheoDialogueProgress == 0)
			{
				CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

				if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
				{
					pAnimator->GetAnimationController()->TransitionState("Run Slow");
				}

				CTransform* pPlayerTransform = m_pPlayer->GetTransform();

				Vector3 vTargetPosition(3232.0f, -4736.0f, 0.0f);
				Vector3 vTargetDirection = (vTargetPosition - pPlayerTransform->GetWorldPosition()).Normalize();

				//
				if (vTargetDirection.x < 0)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				}
				else
				{
					pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				}

				pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(32.0f * 4.0f * vTargetDirection.x, 0.0f, 0.0f) * fTime);

				if ((pPlayerTransform->GetWorldPosition() - vTargetPosition).Length() <= 4.0f)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
					{
						pAnimator->GetAnimationController()->TransitionState("Stand");
					}
					m_iTheoDialogueProgress = 1;
				}
				SAFE_RELEASE(pPlayerTransform);
				SAFE_RELEASE(pAnimator);
			}


			if (m_iTheoDialogueProgress == 1)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
				m_pDialogue->SetAnimation("Madeline Normal Talk");
				m_pDialogue->SetString(L"...\n넌 탐험 때문에 여기까지 온거야?     ");
				m_pDialogue->SetReserveAnimation("Madeline Normal");
				m_iTheoDialogueProgress = 2;
			}
			if (m_iTheoDialogueProgress == 2)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 3;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 3)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
				m_pDialogue->SetAnimation("Theo Normal Talk");
				m_pDialogue->SetString(L"뭐? 아, 그래 맞아\n나는 버려진 장소를 탐험하는 취미를 가지고 있거든.     ");
				m_pDialogue->SetReserveAnimation("Theo Normal");
				m_iTheoDialogueProgress = 4;
			}
			if (m_iTheoDialogueProgress == 4)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Thinking Talk");
					m_pDialogue->SetString(L"그리고 그 장소들을 멋지게 찍어내는,\n신예 사진작가가 되고 싶기도 해서 말이야.     ");
					m_pDialogue->SetReserveAnimation("Theo Thinking");
					m_iTheoDialogueProgress = 5;
				}
			}
			if (m_iTheoDialogueProgress == 5)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 6;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 6)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
				m_pDialogue->SetAnimation("Madeline Normal Talk");
				m_pDialogue->SetString(L"아, 진짜?\n그건 좀 관심이 가는 취미인데!\n그러면 블로그 같은것도 있어?     ");
				m_pDialogue->SetReserveAnimation("Madeline Normal");
				m_iTheoDialogueProgress = 7;
			}
			if (m_iTheoDialogueProgress == 7)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 8;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 8)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Thinking");
				m_pDialogue->SetAnimation("Theo Thinking Talk");
				m_pDialogue->SetString(L"뭐어?? 블로그라고??\n저기, 매들린... 세상에 그런 촌뜨기가 어딨어.\n요즘에는 다들 인스타픽스를 사용한다구.     ");
				m_pDialogue->SetReserveAnimation("Theo Thinking");
				m_iTheoDialogueProgress = 9;
			}
			if (m_iTheoDialogueProgress == 9)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Thinking Talk");
					m_pDialogue->SetString(L"내 인스타픽스 아이디는\n\"언더스타 테오\"야.\n한번 검색해 볼래?!     ");
					m_pDialogue->SetReserveAnimation("Theo Thinking");
					m_iTheoDialogueProgress = 10;
				}
			}
			if (m_iTheoDialogueProgress == 10)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 0;
					m_pDialogue->CloseDialogue();
					UnLockPlayer();
				}
			}
		}
#pragma endregion

#pragma region Theo E

		if (m_bOnInteraction && m_iTheoInterationCount == 5)
		{
			LockPlayer();

			CTransform* pTheo = m_pTheo->GetTransform();

			// Theo의 오른쪽에 Player 있음
			if (pTheo->GetWorldPosition().x - vPlayerPosition.x < 0)
			{
				pTheo->SetWorldRotation(0.0f, 0.0f, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			else
			{
				pTheo->SetWorldRotation(0.0f, PI, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			SAFE_RELEASE(pTheo);

			if (m_iTheoDialogueProgress == 0)
			{
				CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

				if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
				{
					pAnimator->GetAnimationController()->TransitionState("Run Slow");
				}

				CTransform* pPlayerTransform = m_pPlayer->GetTransform();

				Vector3 vTargetPosition(3232.0f, -4736.0f, 0.0f);
				Vector3 vTargetDirection = (vTargetPosition - pPlayerTransform->GetWorldPosition()).Normalize();

				//
				if (vTargetDirection.x < 0)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				}
				else
				{
					pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				}

				pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(32.0f * 4.0f * vTargetDirection.x, 0.0f, 0.0f) * fTime);

				if ((pPlayerTransform->GetWorldPosition() - vTargetPosition).Length() <= 4.0f)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
					{
						pAnimator->GetAnimationController()->TransitionState("Stand");
					}
					m_iTheoDialogueProgress = 1;
				}
				SAFE_RELEASE(pPlayerTransform);
				SAFE_RELEASE(pAnimator);
			}


			if (m_iTheoDialogueProgress == 1)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
				m_pDialogue->SetAnimation("Theo Normal Talk");
				m_pDialogue->SetString(L"음... 여긴 지형이 꽤 복잡한데.\n어떻게 할래, 매들린?\n설마 여기서 포기할 생각은 아니지?     ");
				m_pDialogue->SetReserveAnimation("Theo Normal");
				m_iTheoDialogueProgress = 2;
			}

			if (m_iTheoDialogueProgress == 2)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 3;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 3)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
				m_pDialogue->SetAnimation("Madeline Normal Talk");
				m_pDialogue->SetString(L"그래. 절대로 포기 안 해.\n반드시 정상을 밟고 말겠어.     ");
				m_pDialogue->SetReserveAnimation("Madeline Normal");
				m_iTheoDialogueProgress = 4;
			}
			if (m_iTheoDialogueProgress == 4)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 5;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 5)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Excited");
				m_pDialogue->SetAnimation("Theo Excited Talk");
				m_pDialogue->SetString(L"이야! 결의가 가득한 걸!     ");
				m_pDialogue->SetReserveAnimation("Theo Excited");
				m_iTheoDialogueProgress = 6;
			}

			if (m_iTheoDialogueProgress == 6)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Normal Talk");
					m_pDialogue->SetString(L"그래. 멋져, 멋져.     ");
					m_pDialogue->SetReserveAnimation("Theo Normal");
					m_iTheoDialogueProgress = 7;
				}
			}
			if (m_iTheoDialogueProgress == 7)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 8;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 8)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Distracted");
				m_pDialogue->SetAnimation("Madeline Distracted Talk");
				m_pDialogue->SetString(L"... 부끄럽게스리, 멋지다고 할 것 까지야...     ");
				m_pDialogue->SetReserveAnimation("Madeline Distracted");
				m_iTheoDialogueProgress = 9;
			}
			if (m_iTheoDialogueProgress == 9)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Madeline Normal Talk");
					m_pDialogue->SetString(L"너도 분명 정상에 도착할 수 있을 거야.     ");
					m_pDialogue->SetReserveAnimation("Madeline Normal");
					m_iTheoDialogueProgress = 10;
				}
			}
			if (m_iTheoDialogueProgress == 10)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 11;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 11)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
				m_pDialogue->SetAnimation("Theo Normal Talk");
				m_pDialogue->SetString(L"음, 글쎄, 아마도?     ");
				m_pDialogue->SetReserveAnimation("Theo Normal");
				m_iTheoDialogueProgress = 12;
			}
			if (m_iTheoDialogueProgress == 12)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Thinking Talk");
					m_pDialogue->SetString(L"솔직히 말해서 나는,\n정상에 올라가는 건 관심 없거든.     ");
					m_pDialogue->SetReserveAnimation("Theo Thinking");
					m_iTheoDialogueProgress = 13;
				}
			}
			if (m_iTheoDialogueProgress == 13)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Normal Talk");
					m_pDialogue->SetString(L"하지만 도시 너머에 존재한다고 하는,\n\"전설 속의 개 쩌는 유적\"에는 엄청나게 관심이 있지!     ");
					m_pDialogue->SetReserveAnimation("Theo Normal");
					m_iTheoDialogueProgress = 14;
				}
			}
			if (m_iTheoDialogueProgress == 14)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Excited Talk");
					m_pDialogue->SetString(L"아마 19세기에 지어진 유적이라던데...\n음... 그럼 \"전설\" 까지는 아닌가?     ");
					m_pDialogue->SetReserveAnimation("Theo Excited");
					m_iTheoDialogueProgress = 15;
				}
			}
			if (m_iTheoDialogueProgress == 15)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Excited Talk");
					m_pDialogue->SetString(L"뭐 어쨌든, 길이 아무리 위험해도,\n나는 그 유적을 내 눈으로 꼭 보겠다, 이거야.     ");
					m_pDialogue->SetReserveAnimation("Theo Excited");
					m_iTheoDialogueProgress = 16;
				}
			}
			if (m_iTheoDialogueProgress == 16)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 0;
					m_pDialogue->CloseDialogue();
					UnLockPlayer();
				}
			}
		}
#pragma endregion

#pragma region Theo F

		if (m_bOnInteraction && m_iTheoInterationCount == 6)
		{
			LockPlayer();

			CTransform* pTheo = m_pTheo->GetTransform();

			// Theo의 오른쪽에 Player 있음
			if (pTheo->GetWorldPosition().x - vPlayerPosition.x < 0)
			{
				pTheo->SetWorldRotation(0.0f, 0.0f, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			else
			{
				pTheo->SetWorldRotation(0.0f, PI, 0.0f);
				CTransform* pPlayerTransform = m_pPlayer->GetTransform();
				pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				SAFE_RELEASE(pPlayerTransform);
			}
			SAFE_RELEASE(pTheo);

			if (m_iTheoDialogueProgress == 0)
			{
				CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);

				if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
				{
					pAnimator->GetAnimationController()->TransitionState("Run Slow");
				}

				CTransform* pPlayerTransform = m_pPlayer->GetTransform();

				Vector3 vTargetPosition(3232.0f, -4736.0f, 0.0f);
				Vector3 vTargetDirection = (vTargetPosition - pPlayerTransform->GetWorldPosition()).Normalize();

				//
				if (vTargetDirection.x < 0)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
				}
				else
				{
					pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				}

				pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(32.0f * 4.0f * vTargetDirection.x, 0.0f, 0.0f) * fTime);

				if ((pPlayerTransform->GetWorldPosition() - vTargetPosition).Length() <= 4.0f)
				{
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
					{
						pAnimator->GetAnimationController()->TransitionState("Stand");
					}
					m_iTheoDialogueProgress = 1;
				}
				SAFE_RELEASE(pPlayerTransform);
				SAFE_RELEASE(pAnimator);
			}


			if (m_iTheoDialogueProgress == 1)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo Normal");
				m_pDialogue->SetAnimation("Theo Normal Talk");
				m_pDialogue->SetString(L"저기, 매들린.\n아주 멍청한 사람이, 무책임한 짓을 하기 전에\n보통 뭐라고 말하는지 알아?     ");
				m_pDialogue->SetReserveAnimation("Theo Normal");
				m_iTheoDialogueProgress = 2;
			}
			if (m_iTheoDialogueProgress == 2)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 3;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 3)
			{
				m_pDialogue->OpenDialogue(PD_RIGHT, PC_MADELINE, "Madeline Normal");
				m_pDialogue->SetAnimation("Madeline Normal Talk");
				m_pDialogue->SetString(L"어... 음...\n\"과감해져라\" ?     ");
				m_pDialogue->SetReserveAnimation("Madeline Normal");
				m_iTheoDialogueProgress = 4;
			}
			if (m_iTheoDialogueProgress == 4)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_iTheoDialogueProgress = 5;
					m_pDialogue->CloseDialogue();
				}
			}
			if (m_iTheoDialogueProgress == 5)
			{
				m_pDialogue->OpenDialogue(PD_LEFT, PC_THEO, "Theo WTF");
				m_pDialogue->SetAnimation("Theo WTF Talk");
				m_pDialogue->SetString(L"아니, 틀렸어.     ");
				m_pDialogue->SetReserveAnimation("Theo WTF");
				m_iTheoDialogueProgress = 6;
			}
			if (m_iTheoDialogueProgress == 6)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					m_pDialogue->SetAnimation("Theo Excited Talk");
					m_pDialogue->SetString(L"그게 뭐냐면 말이야...     ");
					m_pDialogue->SetReserveAnimation("Theo Excited");
					m_iTheoDialogueProgress = 7;
				}
			}
			if (m_iTheoDialogueProgress == 7)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					CAnimator* pAnimator = m_pTheo->GetComponentFromType<CAnimator>(CT_ANIMATOR);
					pAnimator->GetAnimationController()->TransitionState("Theo Yolo");
					SAFE_RELEASE(pAnimator);

					m_pDialogue->SetAnimation("Theo Yolo Talk");
					m_pDialogue->SetString(L"인 생 한 바 아아아아아아 앙!!     ");
					m_pDialogue->SetReserveAnimation("Theo Yolo");
					m_iTheoDialogueProgress = 8;
				}
			}
			if (m_iTheoDialogueProgress == 8)
			{
				if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
				{
					CAnimator* pAnimator = m_pTheo->GetComponentFromType<CAnimator>(CT_ANIMATOR);
					pAnimator->GetAnimationController()->TransitionState("Theo Idle");
					SAFE_RELEASE(pAnimator);

					m_iTheoDialogueProgress = 0;
					m_iTheoInterationCount = 7;
					m_pDialogue->CloseDialogue();
					UnLockPlayer();
				}
			}
		}
#pragma endregion

		
	}





	if (pPlayerArea == m_pMonologueEventArea)
	{

#pragma region Monologue	
		{
			CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);

			Vector3 vTargetPosition(16528.0f, -12736.0f, 0.0f);
			Vector3 vDirection = (vTargetPosition - vPlayerPosition).Normalize();

			if (vPlayerPosition.y < -12704.0f && vPlayerPosition.x >= 16320.0f && pPlayer->OnGround() && m_iSelfDialogueProgress == 0)
			{
				LockPlayer();
				pPlayer->ResetPhysics();
				m_iSelfDialogueProgress = 1;
				CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
				if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
				{
					pAnimator->GetAnimationController()->TransitionState("Stand");
				}
				SAFE_RELEASE(pAnimator);
			}

			if (m_bOnInteraction)
			{
				if (m_iSelfDialogueProgress == 1)
				{
					m_fDialogueTimer += fTime;

					if (m_fDialogueTimer >= 0.5f)
					{
						//목적지가 오른쪽일 때
						if (vDirection.x > 0)
						{
							CTransform* pPlayerTransform = m_pPlayer->GetTransform();
							pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
							SAFE_RELEASE(pPlayerTransform);
						}
						else
						{
							CTransform* pPlayerTransform = m_pPlayer->GetTransform();
							pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
							SAFE_RELEASE(pPlayerTransform);
						}

						m_iSelfDialogueProgress = 2;
					}
				}

				//회전한 상태에서, 이동함.
				else if (m_iSelfDialogueProgress == 2)
				{
					CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
					{
						pAnimator->GetAnimationController()->TransitionState("Run Slow");
					}
					SAFE_RELEASE(pAnimator);

					CTransform* pPlayerTransform = m_pPlayer->GetTransform();
					pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(pPlayerTransform->right().x * 150 * fTime, 0.0f, 0.0f));
					SAFE_RELEASE(pPlayerTransform);

					if ((vTargetPosition - vPlayerPosition).Length() <= 5.0f)
					{
						CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
						if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
						{
							pAnimator->GetAnimationController()->TransitionState("Stand");
						}
						SAFE_RELEASE(pAnimator);

						m_iSelfDialogueProgress = 3;
						m_fDialogueTimer = 0.0f;
					}
				}
				else if (m_iSelfDialogueProgress == 3)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 0.25f)
					{
						m_fDialogueTimer = 0.0f;
						m_iSelfDialogueProgress = 4;
					}
				}

				else if (m_iSelfDialogueProgress == 4)
				{
					CTransform* pPlayerTransform = m_pPlayer->GetTransform();
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					SAFE_RELEASE(pPlayerTransform);

					m_iSelfDialogueProgress = 5;
				}

				else if (m_iSelfDialogueProgress == 5)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 0.25f)
					{
						m_fDialogueTimer = 0.0f;
						m_iSelfDialogueProgress = 6;
					}
				}

				else if (m_iSelfDialogueProgress == 6)
				{
					m_pDialogue->OpenDialogue(PD_LEFT, PC_MADELINE, "Madeline Distracted");
					m_pDialogue->SetAnimation("Madeline Distracted Talk");
					m_pDialogue->SetString(L"으아... 여기에서 한숨 돌리자...     ");
					m_pDialogue->SetReserveAnimation("Madeline Distracted");
					m_iSelfDialogueProgress = 7;
				}
				else if (m_iSelfDialogueProgress == 7)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iSelfDialogueProgress = 8;
						m_pDialogue->CloseDialogue();
					}
				}
				else if (m_iSelfDialogueProgress == 8)
				{
					Vector3 vTargetPosition2 = Vector3(16320.0f, -12768.0f, 0.0f);
					CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
					{
						pAnimator->GetAnimationController()->TransitionState("Run Slow");
					}
					SAFE_RELEASE(pAnimator);

					CTransform* pPlayerTransform = m_pPlayer->GetTransform();
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(pPlayerTransform->right().x * 150 * fTime, 0.0f, 0.0f));
					Vector3 TempPlayerPosition = pPlayerTransform->GetWorldPosition();

					SAFE_RELEASE(pPlayerTransform);

					if (vTargetPosition2.x >= TempPlayerPosition.x)
					{
						CTransform* pPlayerTransform = m_pPlayer->GetTransform();
						pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
						SAFE_RELEASE(pPlayerTransform);

						CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
						if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
						{
							pAnimator->GetAnimationController()->TransitionState("Stand");
						}
						SAFE_RELEASE(pAnimator);

						m_iSelfDialogueProgress = 9;
						m_fDialogueTimer = 0.0f;
					}
				}

				else if (m_iSelfDialogueProgress == 9)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 1.0f)
					{
						CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
						if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Duck")
						{
							pAnimator->GetAnimationController()->TransitionState("Duck");
						}
						SAFE_RELEASE(pAnimator);
						m_fDialogueTimer = 0.0f;
						m_iSelfDialogueProgress = 10;
					}
				}

				else if (m_iSelfDialogueProgress == 10)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 0.5f)
					{
						CAnimator* pAnimator = m_pCampfire->GetComponentFromType<CAnimator>(CT_ANIMATOR);
						pAnimator->GetAnimationController()->TransitionState("Campfire Start");
						SAFE_RELEASE(pAnimator);
						m_iSelfDialogueProgress = 11;
						m_fDialogueTimer;
					}
				}

				else if (m_iSelfDialogueProgress == 11)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 0.7f)
					{
						CAnimator* pAnimator = m_pCampfire->GetComponentFromType<CAnimator>(CT_ANIMATOR);
						if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Campfire Burn")
						{
							pAnimator->GetAnimationController()->TransitionState("Campfire Burn");
						}
						SAFE_RELEASE(pAnimator);

						if (m_fDialogueTimer >= 1.2f)
						{
							pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
							if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
							{
								pAnimator->GetAnimationController()->TransitionState("Stand");
							}
							SAFE_RELEASE(pAnimator);
							m_fDialogueTimer = 0.0f;
							m_iSelfDialogueProgress = 12;
						}
					}
				}

				else if (m_iSelfDialogueProgress == 12)
				{
					CTransform* pPlayerTransform = m_pPlayer->GetTransform();
					pPlayerTransform->SetWorldRotation(0.0f, PI, 0.0f);
					SAFE_RELEASE(pPlayerTransform);

					m_fDialogueTimer = 0.0f;
					m_iSelfDialogueProgress = 13;
				}
				else if (m_iSelfDialogueProgress == 13)
				{
					vTargetPosition = Vector3(16288.0f, -12768.0f, 0.0f);
					CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
					if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Run Slow")
					{
						pAnimator->GetAnimationController()->TransitionState("Run Slow");
					}
					SAFE_RELEASE(pAnimator);

					CTransform* pPlayerTransform = m_pPlayer->GetTransform();
					pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + Vector3(pPlayerTransform->right().x * 150 * fTime, 0.0f, 0.0f));

					SAFE_RELEASE(pPlayerTransform);

					if (vTargetPosition.x > vPlayerPosition.x)
					{
						CTransform* pPlayerTransform = m_pPlayer->GetTransform();
						pPlayerTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
						SAFE_RELEASE(pPlayerTransform);

						CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
						if (pAnimator->GetAnimationController()->GetCurrentState()->GetStateName() != "Stand")
						{
							pAnimator->GetAnimationController()->TransitionState("Stand");
						}
						SAFE_RELEASE(pAnimator);

						m_iSelfDialogueProgress = 14;
						m_fDialogueTimer = 0.0f;
					}
				}
				else if (m_iSelfDialogueProgress == 14)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 0.5f)
					{
						CAnimator* pAnimator = m_pPlayer->GetComponentFromType<CAnimator>(CT_ANIMATOR);
						pAnimator->GetAnimationController()->TransitionState("Sit");

						CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
						pPlayer->RenderHair(false);
						SAFE_RELEASE(pPlayer);
						SAFE_RELEASE(pAnimator);
						m_iSelfDialogueProgress = 15;
					}
				}
				else if (m_iSelfDialogueProgress == 15)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 2.0f)
					{
						m_iSelfDialogueProgress = 16;
					}
				}
				else if (m_iSelfDialogueProgress == 16)
				{
					m_pDialogue->OpenDialogue(PD_LEFT, PC_MADELINE, "Madeline Deadpan");
					m_pDialogue->SetAnimation("Madeline Deadpan Talk");
					m_pDialogue->SetString(L"...\n나에게는 어쩌면 무모한 도전이었는지도 모르겠네...     ");
					m_pDialogue->SetReserveAnimation("Madeline Deadpan");
					m_iSelfDialogueProgress = 17;
				}
				else if (m_iSelfDialogueProgress == 17)
				{
					if (GETASYNCKEYDOWN('C') && m_pDialogue->End())
					{
						m_iSelfDialogueProgress = 18;
						m_fDialogueTimer = 0.0f;
						m_pDialogue->CloseDialogue();

						GET_SINGLE(CSoundManager)->Play("mus_epilogue_loop");
					}
				}
				else if (m_iSelfDialogueProgress == 18)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 2.0f)
					{
						m_iSelfDialogueProgress = 19;
					}
				}
				else if (m_iSelfDialogueProgress == 19)
				{
					CLayer* pLayer = m_pScene->FindLayer("BackUI");
					GET_SINGLE(CGameData)->CloseScene(vPlayerPosition, m_pScene, pLayer, AE_STOP);
					SAFE_RELEASE(pLayer);
					m_iSelfDialogueProgress = 20;
					m_fDialogueTimer = 0.0f;
				}
				else if (m_iSelfDialogueProgress == 20)
				{
					m_fDialogueTimer += fTime;
					if (m_fDialogueTimer >= 2.0f)
					{
						m_fDialogueTimer = 0.0f;
						m_iSelfDialogueProgress = 21;
					}
				}
				
				else if (m_iSelfDialogueProgress == 21)
				{
					m_fDialogueTimer += fTime;
					float fProgress = m_fDialogueTimer / 0.5f;

					CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
					Vector3 pCametaPosition = pCameraTransform->GetWorldPosition();
					SAFE_RELEASE(pCameraTransform);
					CTransform* pTransform = m_pComplete->GetTransform();
					pTransform->SetWorldPosition(pCametaPosition);
					SAFE_RELEASE(pTransform);

					CRenderer* pRenderer = m_pComplete->GetComponentFromType<CRenderer>(CT_RENDERER);
					Vector4 vColor = pRenderer->GetDiffuse();
					vColor.w = sinf(PI * 0.5f * fProgress);
					pRenderer->SetDiffuse(vColor);
					SAFE_RELEASE(pRenderer);					
					
					if (fProgress >= 1.0f)
					{
						m_fDialogueTimer = 0.0f;
						m_iSelfDialogueProgress = 22;
					}
				}

				else if (m_iSelfDialogueProgress == 22)
				{
					m_fDialogueTimer += fTime;
					
					if (m_fDialogueTimer >= 2.0f)
					{
						m_fDialogueTimer = 0.0f;
						m_iSelfDialogueProgress = 23;
					}
				}

				else if (m_iSelfDialogueProgress == 23)
				{
					m_fDialogueTimer += fTime;
					float fProgress = m_fDialogueTimer / 1.0f;

					if (fProgress >= 3.0f)
					{
						GET_SINGLE(CSoundManager)->Stop(SOUND_BGM);
						GET_SINGLE(CSoundManager)->Stop(SOUND_ENV);
						GET_SINGLE(CSoundManager)->Stop(SOUND_SFX);

						GET_SINGLE(CSceneManager)->ChangeScene(SCC_TO_PREVIOUS);
						CIntroScene* pIntroScene = dynamic_cast<CIntroScene*>(GET_SINGLE(CSceneManager)->GetNextScene());
						CLayer* pIntroSceneUILayer = pIntroScene->FindLayer("UI");
						CGameObject* pManager = pIntroSceneUILayer->FindGameObjectWithTag("Main Menu Manager");
						CTitleSceneManager* pManagerComponent = pManager->GetComponentFromType<CTitleSceneManager>(CT_TITLE_SCENE_MANAGER);
						pManagerComponent->SetChapter1End(m_pScene);
						SAFE_RELEASE(pManagerComponent);
						SAFE_RELEASE(pManager);
						SAFE_RELEASE(pIntroSceneUILayer);

						m_iSelfDialogueProgress = 24;
					}		
				}

				SAFE_RELEASE(pPlayer);
			}
		}

#pragma endregion

	}


	return 0;
}
