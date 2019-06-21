#include "TitleSceneManager.h"
#include "Input.h"
#include "../Client.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component/Renderer.h"
#include "SceneManager.h"
#include "../Scene/Chapter0.h"

#include "../Chapter0LoadingThread.h"
#include "../Chapter1LoadingThread.h"
#include "ThreadManager.h"
#include "SceneManager.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "Layer.h"
#include "Core.h"
#include "Device.h"


#include "Component/Animator.h"
#include "AnimationState.h"
#include "AnimationController.h"
#include "Transition.h"
#include "AnimationClip2D.h"

#include "SoundManager.h"
#include "CollisionManager.h"


CTitleSceneManager::CTitleSceneManager():
	m_pTitle(NULL),
	m_eState(MS_TITLE),
	m_eMenuType(MT_NOT_IN_MENU),
	m_fTimer(0.0f),
	m_fBlinkTimer(0.0f),
	m_vMenuOffset(-400.0f, 0.0f, 0.0f),
	m_bChapter0OnLoading(false),
	m_bChapter1OnLoading(false),
	m_pChapter0LoadingThread(NULL),
	m_pChapter1LoadingThread(NULL),
	m_iChapter(-1),
	m_iReserveToStartChapter(-1),
	m_bBlowoutChapter0(false),
	m_bBlowoutChapter1(false),
	m_pChapter0(NULL),
	m_pChapter1(NULL)
{
	//Climb
	m_pLocalOffset[0] = Vector3(130.0f, 210.0f, 0.0f);
	m_pLocalOffset[1] = Vector3(137.5f, 290.0f, 0.0f);

	//Options
	m_pLocalOffset[2] = Vector3(120.0f, 360.0f, 0.0f);
	m_pLocalOffset[3] = Vector3(175.0f, 365.0f, 0.0f);
	
	//Credits
	m_pLocalOffset[4] = Vector3(120.0f, 410.0f, 0.0f);
	m_pLocalOffset[5] = Vector3(175.0f, 415.0f, 0.0f);
	
	//Exit
	m_pLocalOffset[6] = Vector3(120.0f, 460.0f, 0.0f);
	m_pLocalOffset[7] = Vector3(175.0f, 465.0f, 0.0f);


	m_eComponentType = CT_TITLE_SCENE_MANAGER;
	SetTag("ParticleMove");
}

CTitleSceneManager::CTitleSceneManager(const CTitleSceneManager & component)
{
}


CTitleSceneManager::~CTitleSceneManager()
{
	if (m_pChapter0LoadingThread)
	{
		m_pChapter0LoadingThread->ReleaseLoadedScene();
	}

	if (m_pChapter1LoadingThread)
	{
		m_pChapter1LoadingThread->ReleaseLoadedScene();
	}
}

CTitleSceneManager * CTitleSceneManager::Clone() const
{
	return new CTitleSceneManager(*this);
}

void CTitleSceneManager::UpdateMenuType( const Vector4& vColor)
{
	size_t iSize = m_vecMenuText.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		CRenderer* pRenderer = m_vecMenuText[i]->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->SetDiffuse(Vector4::white);
		SAFE_RELEASE(pRenderer);
	}


	CRenderer* pRenderer = m_vecMenuText[(int)m_eMenuType]->GetComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetDiffuse(vColor);
	SAFE_RELEASE(pRenderer);

}

void CTitleSceneManager::SetTargetScale(const Vector3 & vScale)
{
	m_vTargetScale = vScale;
}

void CTitleSceneManager::UpdateMenuPosition(float fProgress)
{
	m_vMenuOffset = XMVectorLerp(Vector3(-400.0f, 0.0f, 0.0f), Vector3(), fProgress);

	size_t iIconSize = m_vecMenuIcon.size();
	size_t iTextSize = m_vecMenuText.size();


	for (size_t i = 0; i < iIconSize; ++i)
	{
		CTransform* pTransform = m_vecMenuIcon[i]->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[i * 2]);
		SAFE_RELEASE(pTransform);
	}

	for (size_t i = 0; i < iTextSize; ++i)
	{
		CTransform* pTransform = m_vecMenuText[i]->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[i * 2 + 1]);
		SAFE_RELEASE(pTransform);
	}
}

void CTitleSceneManager::SetChapter0End(CScene* pScene)
{
	GET_SINGLE(CSoundManager)->Play("env_amb_00_main");
	GET_SINGLE(CSoundManager)->Play("TitleIntro");
	m_pTitle = NULL ;
	m_eState = MS_ENTER_SELECT_CHAPTER0;
	m_eMenuType = MT_CLIMB;
	m_fTimer = 0.0f;
	m_fBlinkTimer = 0.0f;
	m_bChapter0OnLoading = false;
	m_bChapter1OnLoading = false;
	m_pChapter0LoadingThread = NULL;
	m_pChapter1LoadingThread = NULL;
	m_iChapter = 0;
	m_iReserveToStartChapter = -1;


	CTransform* pTransform = m_pInfo->GetTransform();
	pTransform->SetWorldPosition(5000.0f, 100.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	m_bBlowoutChapter0 = true;
	m_pChapter0 = pScene;
}

void CTitleSceneManager::SetChapter1End(CScene* pScene)
{
	GET_SINGLE(CSoundManager)->Play("env_amb_00_main");
	GET_SINGLE(CSoundManager)->Play("TitleIntro");
	m_pTitle = NULL;
	m_eState = MS_ENTER_SELECT_CHAPTER1;
	m_eMenuType = MT_CLIMB;
	m_fTimer = 0.25f;
	m_fBlinkTimer = 0.0f;
	m_bChapter0OnLoading = false;
	m_bChapter1OnLoading = false;
	m_pChapter0LoadingThread = NULL;
	m_pChapter1LoadingThread = NULL;
	m_iChapter = 1;
	m_iReserveToStartChapter = -1;

	CTransform* pTransform = m_pInfo->GetTransform();
	pTransform->SetWorldPosition(5000.0f, 100.0f, 0.0f);
	SAFE_RELEASE(pTransform);
	
	m_bBlowoutChapter1 = true;
	m_pChapter1 = pScene;
}

void CTitleSceneManager::SetMenuBackground(CGameObject * pBackground)
{
	m_pMenuBackground = pBackground;
}

void CTitleSceneManager::SetOriginalScale(const Vector3 & vScale)
{
	m_vOriginScale = vScale - Vector3(300.0f, 300.0f, 0.0f);
}

void CTitleSceneManager::SetBackground(CGameObject * pBackground)
{
	m_pBackground = pBackground;
}

void CTitleSceneManager::SetTitle(CGameObject * pTitleObject)
{
	m_pTitle = pTitleObject;
}

bool CTitleSceneManager::Initialize()
{
	GET_SINGLE(CSoundManager)->LoadSound(SOUND_BGM, "TitleIntro",	true,"TItleIntro.mp3", BGM_PATH);
	GET_SINGLE(CSoundManager)->LoadSound(SOUND_ENV, "env_amb_00_main", true, "env_amb_00_main.wav", SFX_PATH);
	GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_main_title_firstinput", false, "ui_main_title_firstinput.wav", UI_SFX_PATH);
	GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_main_roll_up", false, "ui_main_roll_up.wav", UI_SFX_PATH);
	GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_main_button_climb", false, "ui_main_button_climb.wav", UI_SFX_PATH);
	GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_world_chapter_level_select", false, "ui_world_chapter_level_select.wav", UI_SFX_PATH);
	GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_world_chapter_back", false, "ui_world_chapter_back.wav", UI_SFX_PATH);
	GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_world_chapter_checkpoint_start", false, "ui_world_chapter_checkpoint_start.wav", UI_SFX_PATH);

	GET_SINGLE(CSoundManager)->Play("env_amb_00_main");
	GET_SINGLE(CSoundManager)->Play("TitleIntro");

	GET_SINGLE(CSoundManager)->Volume(SOUND_BGM, 1.0f);
	GET_SINGLE(CSoundManager)->Volume(SOUND_ENV, 0.1f);

	 
	CLayer* pLayer = m_pScene->FindLayer("UI");

	//Climb Icon 
	{
		CGameObject* pGameObject = CGameObject::CreateObject("MenuTexture", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[0]);
		pTransform->SetPivot(0.0f, 0.5f, 0.0f);
		pTransform->SetWorldScale(170.0f, 150.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetImageInfo("ui\\menu\\climb_icon");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_vecMenuIcon.push_back(pGameObject);
		SAFE_RELEASE(pGameObject);
	}

	//Climb Text 
	{
		CGameObject* pGameObject = CGameObject::CreateObject("MenuTexture", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[1]);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(200.0f, 55.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetImageInfo("ui\\menu\\climb_text");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_vecMenuText.push_back(pGameObject);
		SAFE_RELEASE(pGameObject);
	}


	//Options Icon 
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Menu", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[2]);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(40.0f, 40.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetImageInfo("ui\\menu\\options_icon");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_vecMenuIcon.push_back(pGameObject);
		SAFE_RELEASE(pGameObject);
	}

	//Options Text 
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Menu", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[3]);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(140.0f, 40.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetImageInfo("ui\\menu\\options_text");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_vecMenuText.push_back(pGameObject);
		SAFE_RELEASE(pGameObject);
	}

	//Credits Icon 
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Menu", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[4]);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(40.0f, 40.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetImageInfo("ui\\menu\\credits_icon");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_vecMenuIcon.push_back(pGameObject);
		SAFE_RELEASE(pGameObject);
	}
	
	
	//Credits Text 
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Menu", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[5]);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(140.0f, 40.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetImageInfo("ui\\menu\\exit_text");// 실수;;

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_vecMenuText.push_back(pGameObject);
		SAFE_RELEASE(pGameObject);
	}



	//Exit Icon 
	{

		CGameObject* pGameObject = CGameObject::CreateObject("Menu", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[6]);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(40.0f, 40.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetImageInfo("ui\\menu\\exit_icon");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_vecMenuIcon.push_back(pGameObject);
		SAFE_RELEASE(pGameObject);
	}

	
	

	//Exit Text 
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Menu", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[7]);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(140.0f, 40.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetImageInfo("ui\\menu\\credits_text"); // 실수;;

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_vecMenuText.push_back(pGameObject);
		SAFE_RELEASE(pGameObject);
	}



	//Info Pannel
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Info", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(2000.0f, 300.0f, 0.0f);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Menu_Chapter0", L"Celeste\\Menu_Chapter0.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_pInfo = pGameObject;
		SAFE_RELEASE(pGameObject);
	}





	//Running
	{
		CLayer* pLayer = m_pScene->FindLayer("Default");

		CGameObject* pObject = CGameObject::CreateObject("Running", pLayer);
		SAFE_RELEASE(pLayer);

		CTransform* pTransform = pObject->GetTransform();
		pTransform->SetWorldScale(96, 128, 1.0f);
		pTransform->SetPivot(0.5f, 1.0f, 0.0f);
		pTransform->SetWorldPosition(-1000.0f, -1000.0f, 0.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		SAFE_RELEASE(pMaterial);

		pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
		SAFE_RELEASE(pRenderer);

		CAnimator* pAnimator = pObject->AddComponent<CAnimator>("Animator");
		pAnimator->CreateAnimationController();
		CAnimationState* pState = pAnimator->CreateAnimationStateInCurrentController("Running");
		pState->AddClip("Running", AO_LOOP, 1.0f, "Gameplay", L"Celeste\\Gameplay.png");
		pAnimator->SetEntryState("Running");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal00");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal01");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal02");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal03");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal04");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal05");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal06");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal07");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal08");
		pState->GetClip()->AddCustomInfo("ui\\loading\\normal09");
		SAFE_RELEASE(pAnimator);
		m_pRunning = pObject;
		SAFE_RELEASE(pObject);
	}


	//Loading
	{
		CLayer* pLayer = m_pScene->FindLayer("Default");

		CGameObject* pObject = CGameObject::CreateObject("Loading", pLayer);
		SAFE_RELEASE(pLayer);

		CTransform* pTransform = pObject->GetTransform();
		pTransform->SetWorldScale(100.0f, 100.0f, 1.0f);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		pTransform->SetWorldPosition(-5000.0f, 620.0f, 0.0f);
		SAFE_RELEASE(pTransform);

		CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
		SAFE_RELEASE(pMaterial);

		pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		SAFE_RELEASE(pRenderer);

		CAnimator* pAnimator = pObject->AddComponent<CAnimator>("Animator");
		pAnimator->CreateAnimationController();
		CAnimationState* pState = pAnimator->CreateAnimationStateInCurrentController("Loading");
		pState->AddClip("Loading", AO_LOOP, 1.0f, "Gameplay", L"Celeste\\Gameplay.png");
		pAnimator->SetEntryState("Loading");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu00");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu01");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu02");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu03");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu04");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu05");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu06");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu07");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu08");
		pState->GetClip()->AddCustomInfo("ui\\loading\\menu09");
		SAFE_RELEASE(pAnimator);
		m_pLoading = pObject;
		SAFE_RELEASE(pObject);
	}



	//Fade
	{
		CGameObject* pGameObject = CGameObject::CreateObject("Fade", pLayer);
		CTransform* pTransform = pGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vMenuOffset + m_pLocalOffset[7]);
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
		pMaterial->SetSampler(SAMPLER_LINEAR);
		pRenderer->SetDiffuse(Vector4::clear);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		m_pFader = pGameObject;
		SAFE_RELEASE(pGameObject);
	}

	SAFE_RELEASE(pLayer);

	return true;
}

int CTitleSceneManager::Update(float fTime)
{
	if (m_bBlowoutChapter0)
	{
		if (GET_SINGLE(CSceneManager)->GetNextScene() == m_pChapter0)
		{
			GET_SINGLE(CSceneManager)->ClearNextScene();
			GET_SINGLE(CCollisionManager)->DestroyInstance();
			GET_SINGLE(CCollisionManager)->Initialize();
			m_bBlowoutChapter0 = false;
		}
		else if (GET_SINGLE(CSceneManager)->GetPrevScene() == m_pChapter0)
		{
			GET_SINGLE(CSceneManager)->ClearPrevScene();
			GET_SINGLE(CCollisionManager)->DestroyInstance();
			GET_SINGLE(CCollisionManager)->Initialize();
			m_bBlowoutChapter0 = false;
		}
	}

	if (m_bBlowoutChapter1)
	{
		if (GET_SINGLE(CSceneManager)->GetNextScene() == m_pChapter1)
		{
			GET_SINGLE(CSceneManager)->ClearNextScene();
			m_bBlowoutChapter1 = false;
		}
		else if (GET_SINGLE(CSceneManager)->GetPrevScene() == m_pChapter1)
		{
			GET_SINGLE(CSceneManager)->ClearPrevScene();
			m_bBlowoutChapter1 = false;
		}
	}







	if (m_bChapter0OnLoading)
	{
		if (m_pChapter0LoadingThread)
		{
			if (m_pChapter0LoadingThread->GetLoadingScene() == NULL)
			{
				CTransform* pLoadingTransform = m_pLoading->GetTransform();
				pLoadingTransform->SetWorldPosition(1180.0f, 620.0f, 0.0f);
				SAFE_RELEASE(pLoadingTransform);
			}
			else
			{
				CTransform* pLoadingTransform = m_pLoading->GetTransform();
				pLoadingTransform->SetWorldPosition(5180.0f, 620.0f, 0.0f);
				SAFE_RELEASE(pLoadingTransform);
				m_bChapter0OnLoading = false;
			}
		}
	}
	
	if(m_bChapter1OnLoading)
	{
		if (m_pChapter1LoadingThread)
		{
			if (m_pChapter1LoadingThread->GetLoadingScene() == NULL)
			{
				CTransform* pLoadingTransform = m_pLoading->GetTransform();
				pLoadingTransform->SetWorldPosition(1180.0f, 620.0f, 0.0f);
				SAFE_RELEASE(pLoadingTransform);
			}
			else
			{
				CTransform* pLoadingTransform = m_pLoading->GetTransform();
				pLoadingTransform->SetWorldPosition(5180.0f, 620.0f, 0.0f);
				SAFE_RELEASE(pLoadingTransform);
				m_bChapter1OnLoading = false;
			}
		}
	}


	if (m_iReserveToStartChapter == 0)
	{
		if (m_pChapter0LoadingThread->GetLoadingScene() != NULL)
		{
			GET_SINGLE(CSoundManager)->Stop(SOUND_BGM);
			GET_SINGLE(CSoundManager)->Stop(SOUND_ENV);
			GET_SINGLE(CSceneManager)->ChangeScene(m_pChapter0LoadingThread->GetLoadingScene());
			m_pChapter0LoadingThread = NULL;

			if(m_pChapter1LoadingThread)
				m_pChapter1LoadingThread->ReleaseLoadedScene();
			
			m_pChapter1LoadingThread = NULL;
		}
	}
	else if (m_iReserveToStartChapter == 1)
	{
		if (m_pChapter1LoadingThread->GetLoadingScene() != NULL)
		{
			GET_SINGLE(CSoundManager)->Stop(SOUND_BGM);
			GET_SINGLE(CSoundManager)->Stop(SOUND_ENV);
			GET_SINGLE(CSceneManager)->ChangeScene(m_pChapter1LoadingThread->GetLoadingScene());
			m_pChapter1LoadingThread = NULL;

			if (m_pChapter0LoadingThread)
				m_pChapter0LoadingThread->ReleaseLoadedScene();
			m_pChapter0LoadingThread = NULL;
		}
	}

	if (GETASYNCKEYDOWN('C'))
	{
		if (m_eState == MS_TITLE)
		{
			GET_SINGLE(CSoundManager)->Play("ui_main_title_firstinput");
			m_eState = MS_TITLE_TO_MENU;
		}
	}

	if (m_eState == MS_TITLE_TO_MENU)
	{
		//Transition In 0.75 Second
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.75f;

		float fScale = cosf(PI * 0.5f * fProgress) * m_vOriginScale.x + 300.0f;
		float fAlpha = cosf(PI * 0.5f * fProgress);

		CTransform* pTitleTransform = m_pTitle->GetTransform();
		pTitleTransform->SetWorldScale(fScale, fScale, 1.0f);
		SAFE_RELEASE(pTitleTransform);

		CRenderer* pTitleRenderer = m_pTitle->GetComponentFromType<CRenderer>(CT_RENDERER);
		Vector4 vColor = pTitleRenderer->GetDiffuse();
		vColor.w = fAlpha;
		pTitleRenderer->SetDiffuse(vColor);
		SAFE_RELEASE(pTitleRenderer);		

		CRenderer* pBackgroundRenderer = m_pBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
		vColor = pBackgroundRenderer->GetDiffuse();
		vColor.w = fAlpha;
		if (vColor.w <= 0.5f)
		{
			vColor.w = 0.5f;
		}
		pBackgroundRenderer->SetDiffuse(vColor);
		SAFE_RELEASE(pBackgroundRenderer);

		if (fProgress >= 1.0f)
		{
			m_eState = MS_TITLE_FADE_OUT;
			m_fTimer = 0.0f;
		}
	}

	if (m_eState == MS_TITLE_FADE_OUT)
	{
		//Transition In 0.75 Second
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.75f;
		float fAlpha = cosf(PI * 0.5f * fProgress);

		CRenderer* pBackgroundRenderer = m_pMenuBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
		Vector4 vColor = pBackgroundRenderer->GetDiffuse();
		vColor.w = 1.0f - fAlpha;
		pBackgroundRenderer->SetDiffuse(vColor);
		SAFE_RELEASE(pBackgroundRenderer);
		if (fProgress >= 1.0f)
		{
			m_eState = MS_MENU_ENTER;
			m_eMenuType = MT_CLIMB;
			m_fTimer = 0.0f;
		}
	}

	if (m_eState == MS_MENU_ENTER)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;
		UpdateMenuPosition(fProgress);
	
		if (fProgress >= 1.0f)
		{
			UpdateMenuPosition(1.0f);
			m_eState = MS_MENU;
			m_fTimer = 0.0f;
		}	
	}

	//Blink Menu Text
	if (m_eState == MS_MENU_ENTER || m_eState == MS_MENU)
	{
		m_fBlinkTimer += fTime;

		if (m_fBlinkTimer >= 0.1f)
		{
			UpdateMenuType(Vector4(170.0f / 255.0f, 1.0f, 133.0f / 255.0f, 1.0f));
		}			
		if (m_fBlinkTimer >= 0.2f)
		{
			UpdateMenuType(Vector4(1.0f, 1.0f, 139.0f / 255.0f, 1.0f));
			m_fBlinkTimer = 0.0f;
		}
	}

	if (m_eState == MS_MENU)
	{
		if (GETASYNCKEYDOWN(VK_UP))
		{
			if (m_eMenuType > 0)
			{
				GET_SINGLE(CSoundManager)->Play("ui_main_roll_up");
				m_eMenuType = (MENU_TYPE)((int)m_eMenuType - 1);
			}			
		}

		if (GETASYNCKEYDOWN(VK_DOWN))
		{
			if (m_eMenuType < 3)
			{
				GET_SINGLE(CSoundManager)->Play("ui_main_roll_up");
				m_eMenuType = (MENU_TYPE)((int)m_eMenuType + 1);
			}
		}

		if (GETASYNCKEYDOWN('C'))
		{
			GET_SINGLE(CSoundManager)->Play("ui_main_button_climb");

			if (m_eMenuType == MT_CLIMB)
			{
				m_fTimer = 0.0f;
				m_eState = MS_SELECT_CLIMB;
			}
			if (m_eMenuType == MT_OPTIONS)
			{
				m_fTimer = 0.0f;
				m_eState = MS_SELECT_OPTIONS;
			}
			if (m_eMenuType == MT_CREDITS)
			{
				m_fTimer = 0.0f;
				m_eState = MS_SELECT_CREDITS;
			}
			if (m_eMenuType == MT_EXIT)
			{
				m_eState = MS_SELECT_EXIT;
			}
		}
	}



	if (m_eState == MS_SELECT_CLIMB)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;
		UpdateMenuPosition(1.0f - fProgress);

		if (fProgress >= 1.0f)
		{
			m_eState = MS_EXIT_MENU;
			UpdateMenuPosition(0.0f);
			m_fTimer = 0.0f;

			CTransform* pTransform = m_pFader->GetTransform();
			pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
			pTransform->SetWorldScale(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight, 1.0f);
			SAFE_RELEASE(pTransform);
		}
	}
	if (m_eState == MS_SELECT_OPTIONS)
	{

	}
	if (m_eState == MS_SELECT_CREDITS)
	{

	}
	if (m_eState == MS_SELECT_EXIT)
	{
		GET_SINGLE(CCore)->ExitProcess();
	}



	if (m_eState == MS_EXIT_MENU)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.5f;


		CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
		SAFE_RELEASE(pRenderer);

		if (fProgress >= 1.0f)
		{
			pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			SAFE_RELEASE(pRenderer);

			m_eState = MS_ENTER_SELECT_CHAPTER0;
			m_fTimer = 0.0f;


			pRenderer = m_pMenuBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuseTexture("MenuChapter0Select", L"Celeste\\Chapter0_00.png");
			SAFE_RELEASE(pRenderer);

		}
	}


	if (m_eState == MS_ENTER_SELECT_CHAPTER0)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.5f;

		CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, 1.0f - fProgress));
		SAFE_RELEASE(pRenderer);

		pRenderer = m_pMenuBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->SetDiffuseTexture("MenuChapter0Select", L"Celeste\\Chapter0_00.png");
		SAFE_RELEASE(pRenderer);

		if (fProgress >= 1.0f)
		{
			m_eState = MS_CHAPTER0;
			m_fTimer = 0.0f;

			CTransform* pTransform = m_pRunning->GetTransform();
			pTransform->SetWorldPosition(500.0f, 300.0f, 0.0f);
			SAFE_RELEASE(pTransform);
		}
	}


	if (m_eState == MS_CHAPTER0)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;


		if (fProgress <= 1.0f)
		{
			CRenderer* pRenderer = m_pRunning->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, fProgress));
			SAFE_RELEASE(pRenderer);			
		}
		else
		{
			if (GETASYNCKEYDOWN('X'))
			{
				GET_SINGLE(CSoundManager)->Play("ui_world_chapter_back");
				m_eState = MS_EXIT_SELECT_CHAPTER0;
				m_fTimer = 0.0f;
			}
			if (GETASYNCKEYDOWN('C'))
			{
				GET_SINGLE(CSoundManager)->Play("ui_world_chapter_level_select");
				m_eState = MS_CONFIRM_CHAPTER0;
				m_fTimer = 0.0f;
				m_bChapter0OnLoading = true;
				m_iChapter = 0;
				m_iLoadingChapter = 0;
				if (!m_pChapter0LoadingThread)
					m_pChapter0LoadingThread = GET_SINGLE(CThreadManager)->CreateThread<CChapter0LoadingThread>("LoadingChapter0");
			}
			if (GETASYNCKEYDOWN(VK_RIGHT))
			{
				GET_SINGLE(CSoundManager)->Play("ui_main_roll_up");
				m_eState = MS_ENTER_SELECT_CHAPTER1;
				m_fTimer = 0.0f;
			}
		}
	}


	//Chapter 선택 창에서 메뉴로 돌아감
	if (m_eState == MS_EXIT_SELECT_CHAPTER0)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;
		//0 -> 1
		if (fProgress <= 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);
		}

		if (fProgress >= 1.0f)
		{
			CTransform* pRunnungTransform = m_pRunning->GetTransform();
			pRunnungTransform->SetWorldPosition(-1000.0f, -1000.0f, 0.0f);
			SAFE_RELEASE(pRunnungTransform);
			CRenderer* pRenderer = m_pRunning->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
			SAFE_RELEASE(pRenderer);

			pRenderer = m_pMenuBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuseTexture("MenuBackground", L"Celeste\\Menu.png");
			SAFE_RELEASE(pRenderer);


			pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, 1.0f - (fProgress - 1.0f)));
			SAFE_RELEASE(pRenderer);

			if (fProgress >= 2.0f)
			{
				m_eState = MS_MENU_ENTER;
				m_eMenuType = MT_CLIMB;
				m_fTimer = 0.0f;
			}
		}		
	}
	
	if (m_eState == MS_ENTER_SELECT_CHAPTER1)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;
		//기존의 background Image 교체. 
		if (fProgress <= 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);

			pRenderer = m_pRunning->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f - fProgress));
			SAFE_RELEASE(pRenderer);
		}

		if (fProgress >= 1.5f)
		{
			float fSecondProgress = fProgress - 1.5f;

			CRenderer* pRenderer = m_pMenuBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuseTexture("MenuChapter1Select", L"Celeste\\Chapter1_00.png");
			SAFE_RELEASE(pRenderer);


			pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, 1.0f - fSecondProgress));
			SAFE_RELEASE(pRenderer);
			if (fSecondProgress >= 1.0f)
			{
				m_eState = MS_SELECT_CHAPTER1;
				m_fTimer = 0.0f;


				CTransform* pTransform = m_pRunning->GetTransform();
				pTransform->SetWorldPosition(600.0f, 350.0f, 0.0f);
				SAFE_RELEASE(pTransform);
			}
		}
	}

	if (m_eState == MS_SELECT_CHAPTER1)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;

		CRenderer* pRenderer = m_pRunning->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, fProgress));
		SAFE_RELEASE(pRenderer);

		if (fProgress >= 1.0f)
		{
			m_eState = MS_CHAPTER1;
			m_fTimer = 0.0f;
		}
	}

	if (m_eState == MS_CHAPTER1)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;

		if (GETASYNCKEYDOWN('X'))
		{
			GET_SINGLE(CSoundManager)->Play("ui_world_chapter_back");
			m_eState = MS_EXIT_SELECT_CHAPTER1;
			m_fTimer = 0.0f;
		}
		if (GETASYNCKEYDOWN('C'))
		{
			GET_SINGLE(CSoundManager)->Play("ui_world_chapter_level_select");
			m_eState = MS_CONFIRM_CHAPTER1;
			m_fTimer = 0.0f;
			m_bChapter1OnLoading = true;
			m_iChapter = 1;
			m_iLoadingChapter = 1;
			if(!m_pChapter1LoadingThread)
				m_pChapter1LoadingThread = GET_SINGLE(CThreadManager)->CreateThread<CChapter1LoadingThread>("LoadingChapter1");
		}
		if (GETASYNCKEYDOWN(VK_LEFT))
		{
			GET_SINGLE(CSoundManager)->Play("ui_main_roll_up");
			m_eState = MS_TEMPSTATE00; // CHAPTER1 의 Enterance와 내용이 호환되지 않아 임시 상태로 이전 후 이동시킨다.
			m_fTimer = 0.0f;
		}
	}

	//Chapter 선택 창에서 메뉴로 돌아감
	if (m_eState == MS_EXIT_SELECT_CHAPTER1)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;
		//0 -> 1
		if (fProgress <= 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);
		}

		if (fProgress >= 1.0f)
		{
			CTransform* pRunnungTransform = m_pRunning->GetTransform();
			pRunnungTransform->SetWorldPosition(-1000.0f, -1000.0f, 0.0f);
			SAFE_RELEASE(pRunnungTransform);
			CRenderer* pRenderer = m_pRunning->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
			SAFE_RELEASE(pRenderer);

			pRenderer = m_pMenuBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuseTexture("MenuBackground", L"Celeste\\Menu.png");
			SAFE_RELEASE(pRenderer);


			pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, 1.0f - (fProgress - 1.0f)));
			SAFE_RELEASE(pRenderer);

			if (fProgress >= 2.0f)
			{
				m_eState = MS_MENU_ENTER;
				m_eMenuType = MT_CLIMB;
				m_fTimer = 0.0f;
			}
		}
	}


	//Chapter1 ->Chapter0의 중계 State
	if (m_eState == MS_TEMPSTATE00)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;
		if (fProgress <= 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);

			pRenderer = m_pRunning->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f,1.0f - fProgress));
			SAFE_RELEASE(pRenderer);
		}

		if (fProgress >= 1.0f)
		{
			CTransform* pRunnungTransform = m_pRunning->GetTransform();
			pRunnungTransform->SetWorldPosition(500.0f, 300.0f, 0.0f);
			SAFE_RELEASE(pRunnungTransform);
		}

		if (fProgress >= 1.5f)
		{
			m_eState = MS_ENTER_SELECT_CHAPTER0;
			m_fTimer = 0.0f;
		}
	}



	if (m_eState == MS_CONFIRM_CHAPTER0)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;

		if (fProgress < 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);

			CTransform* pRunningTransform = m_pRunning->GetTransform();
			pRunningTransform->SetWorldPosition((Vector3)XMVectorLerp(Vector3(500.0f, 300.0f, 0.0f), Vector3(400.0f, 230.0f, 0.0f),fProgress));
			SAFE_RELEASE(pRunningTransform);
		}
		else if(fProgress >= 1.25f && fProgress < 2.25f)
		{
			float fSecondProgress = fProgress - 1.25f;

			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, 1.0f - fSecondProgress));
			SAFE_RELEASE(pRenderer);

			pRenderer = m_pMenuBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuseTexture("Chapter0_01", L"Celeste\\Chapter0_01.png");
			SAFE_RELEASE(pRenderer);

			pRenderer = m_pInfo->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuseTexture("Menu_Chapter0", L"Celeste\\Menu_Chapter0.png");
			SAFE_RELEASE(pRenderer);

			CTransform* pInfoTransform = m_pInfo->GetTransform();
			pInfoTransform->SetWorldScale(582.0f, 370.0f, 1.0f);
			pInfoTransform->SetWorldPosition((Vector3)XMVectorLerp(Vector3(DEVICE_RESOLUTION.iWidth, 100.0f, 0.0f), Vector3(DEVICE_RESOLUTION.iWidth - 582.0f, 100.0f, 0.0f), fSecondProgress));
			SAFE_RELEASE(pInfoTransform);
		}
		else
		{
			if (GETASYNCKEYDOWN('X'))
			{
				m_iChapter = -1;
				m_fTimer = 0.0f;
				m_eState = MS_TEMPSTATE01;
				GET_SINGLE(CSoundManager)->Play("ui_world_chapter_back");
			}
			if (GETASYNCKEYDOWN('C'))
			{
				m_fTimer = 0.0f;
				m_eState = MS_START_CHAPTER0;
				GET_SINGLE(CSoundManager)->Play("ui_world_chapter_checkpoint_start");
			}
		}
	}

	if (m_eState == MS_CONFIRM_CHAPTER1)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;

		if (fProgress < 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);

			CTransform* pRunningTransform = m_pRunning->GetTransform();
			pRunningTransform->SetWorldPosition((Vector3)XMVectorLerp(Vector3(600.0f, 350.0f, 0.0f), Vector3(310, 250.0f, 0.0f), fProgress));
			SAFE_RELEASE(pRunningTransform);
		}
		else if (fProgress >= 1.25f && fProgress < 2.25f)
		{
			float fSecondProgress = fProgress - 1.25f;

			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, 1.0f - fSecondProgress));
			SAFE_RELEASE(pRenderer);

			pRenderer = m_pMenuBackground->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuseTexture("Chapter1_01", L"Celeste\\Chapter1_01.png");
			SAFE_RELEASE(pRenderer);

			pRenderer = m_pInfo->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuseTexture("Menu_Chapter1", L"Celeste\\Menu_Chapter1.png");
			SAFE_RELEASE(pRenderer);

			CTransform* pInfoTransform = m_pInfo->GetTransform();
			pInfoTransform->SetWorldScale(582.0f, 527.0f, 1.0f);
			pInfoTransform->SetWorldPosition((Vector3)XMVectorLerp(Vector3(DEVICE_RESOLUTION.iWidth, 100.0f, 0.0f), Vector3(DEVICE_RESOLUTION.iWidth - 582.0f, 100.0f, 0.0f), fSecondProgress));
			SAFE_RELEASE(pInfoTransform);
		}
		else
		{
			if (GETASYNCKEYDOWN('X'))
			{
				m_iChapter = -1;
				m_fTimer = 0.0f;
				m_eState = MS_TEMPSTATE02;
				GET_SINGLE(CSoundManager)->Play("ui_world_chapter_back");
			}
			if (GETASYNCKEYDOWN('C'))
			{
				//STAGE START
				m_fTimer = 0.0f;
				m_eState = MS_START_CHAPTER1;
				GET_SINGLE(CSoundManager)->Play("ui_world_chapter_checkpoint_start");
			}
		}
	}


	if (m_eState == MS_START_CHAPTER0)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 1.0f;

		if (fProgress < 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);
		}
		else if(fProgress >= 4.0f)
		{
			//STAGE START
			m_iReserveToStartChapter = 0;
		}
	}

	if (m_eState == MS_START_CHAPTER1)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 1.0f;

		if (fProgress < 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);
		}
		else if (fProgress >= 4.0f)
		{
			//STAGE START
			m_iReserveToStartChapter = 1;
		}
	}


	if (m_eState == MS_TEMPSTATE01)
	{
		//CONFIRM0 -> SELECT

		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;

		if (fProgress < 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);

			CTransform* pRunningTransform = m_pRunning->GetTransform();
			pRunningTransform->SetWorldPosition((Vector3)XMVectorLerp(Vector3(400.0f, 230.0f, 0.0f), Vector3(500.0f, 300.0f, 0.0f),  fProgress));
			SAFE_RELEASE(pRunningTransform);

			CTransform* pInfoTransform = m_pInfo->GetTransform();
			pInfoTransform->SetWorldScale(582.0f, 370.0f, 1.0f);
			pInfoTransform->SetWorldPosition((Vector3)XMVectorLerp(Vector3(DEVICE_RESOLUTION.iWidth - 582.0f, 100.0f, 0.0f), Vector3(DEVICE_RESOLUTION.iWidth, 100.0f, 0.0f), fProgress));
			SAFE_RELEASE(pInfoTransform);
		}
		else
		{
			m_eState = MS_ENTER_SELECT_CHAPTER0;
			m_fTimer = 0.0f;
		}
	}
	if (m_eState == MS_TEMPSTATE02)
	{
		//CONFIRM1 -> SELECT

		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.25f;

		if (fProgress < 1.0f)
		{
			CRenderer* pRenderer = m_pFader->GetComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, fProgress));
			SAFE_RELEASE(pRenderer);

			CTransform* pRunningTransform = m_pRunning->GetTransform();
			pRunningTransform->SetWorldPosition((Vector3)XMVectorLerp(Vector3(310, 250.0f, 0.0f), Vector3(600.0f, 350.0f, 0.0f), fProgress));
			SAFE_RELEASE(pRunningTransform);

			CTransform* pInfoTransform = m_pInfo->GetTransform();
			pInfoTransform->SetWorldScale(582.0f, 527.0f, 1.0f);
			pInfoTransform->SetWorldPosition((Vector3)XMVectorLerp(Vector3(DEVICE_RESOLUTION.iWidth - 582.0f, 100.0f, 0.0f), Vector3(DEVICE_RESOLUTION.iWidth, 100.0f, 0.0f) , fProgress));
			SAFE_RELEASE(pInfoTransform);
		}
		else
		{
			m_eState = MS_ENTER_SELECT_CHAPTER1;
			m_fTimer = 0.0f;
		}
	}





	return 0;
}
