#include "IntroScene.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Transform.h"
#include "Core.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Device.h"
#include "Material.h"
#include "Component/Camera.h"
#include "Component/RectCollider2D.h"
#include "../Component/TitleSceneManager.h"
#include "../Component/ParticleManager.h"

#include "Component/Animator.h"
#include "AnimationClip2D.h"
#include "AnimationState.h"
#include "AnimationController.h"
#include "Transition.h"

#include "ThreadManager.h"
#include "../Chapter0LoadingThread.h"

CIntroScene::CIntroScene()
{
}


CIntroScene::~CIntroScene()
{
	int i = 0;
}

bool CIntroScene::Initialize()
{

	GET_SINGLE(CCore)->SetClearColor(4, 4, 4, 255);

	CTransform* pCameraTransform	 = GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	Vector3 vScreenSize = Vector3(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight, 0);


	CLayer* pBackgroundLayer = CreateLayer("Background", -1);
	CLayer*	pLayer = FindLayer("Default"); // Z : 0
	CLayer* pForegroundLayer = CreateLayer("Foreground", 1);
	CLayer* pUILayer = FindLayer("UI");
#pragma region GameManager
	
		CGameObject* pManager = CGameObject::CreateObject("Main Menu Manager", pUILayer);
		CTitleSceneManager* pManagerComponent = pManager->AddComponent<CTitleSceneManager>("TitleManager");
			
#pragma endregion


#pragma region Main Menu Phase
		{
			CGameObject* pTexture = CGameObject::CreateObject("MenuTexture", pBackgroundLayer);
			pManagerComponent->SetMenuBackground(pTexture);
			CTransform* pTransform = pTexture->GetTransform();
			pTransform->SetWorldPosition(vCameraPosition.x + vScreenSize.x / 2.0f, vCameraPosition.y + vScreenSize.y / 2.0f, 0);
			pTransform->SetPivot(0.5f, 0.5f, 0.0f);
			pTransform->SetWorldScale(vScreenSize.x, vScreenSize.y, 1.0f);
			SAFE_RELEASE(pTransform);

			CRenderer* pRenderer = pTexture->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);

			CMaterial*	pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("MenuBackground", L"Celeste\\Menu.png");
			pMaterial->SetSampler(SAMPLER_LINEAR);

			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			SAFE_RELEASE(pTexture);
		}
#pragma endregion


#pragma region Background Texture
	{
		CGameObject* pBackgroundTexture = CGameObject::CreateObject("BackgroundTexture", pBackgroundLayer);
		pManagerComponent->SetBackground(pBackgroundTexture);
	
		CTransform* pTransform = pBackgroundTexture->GetTransform();
		pTransform->SetWorldPosition(vCameraPosition.x + vScreenSize.x / 2.0f, vCameraPosition.y + vScreenSize.y / 2.0f, 0);
		pTransform->SetPivot(0.5f, 0.5f, 0.0f);
		pTransform->SetWorldScale(vScreenSize.x, vScreenSize.y, 1.0f);
		SAFE_RELEASE(pTransform);
	
		CRenderer* pRenderer = pBackgroundTexture->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);
		
		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("TitleBackground", L"Celeste\\TitleBackground.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);			
		pRenderer->SetDiffuse(Vector4(0.25f, 0.25f, 0.25f, 1.0f));
	
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pBackgroundTexture);
	}
#pragma endregion


#pragma region Title Texture
	{
		CGameObject* pTexture = CGameObject::CreateObject("Title", pLayer);
		CTransform* pTransform = pTexture->GetTransform();
		Vector3 position = vCameraPosition + vScreenSize / 2.0f;
		position.z = 0;
		pTransform->SetWorldPosition(position);
		pTransform->SetPivot(0.5f, 0.5f, 0.0f);
		//Screen height 에 1/2 크기로. (정사각형이기때문에 한쪽방향에 맞춰야 함)
		pTransform->SetWorldScale(vScreenSize.y , vScreenSize.y , 1.0f);
	
		pManagerComponent->SetTitle(pTexture);
		pManagerComponent->SetOriginalScale(pTransform->GetWorldScale());
	
		SAFE_RELEASE(pTransform);
	
		CRenderer* pRenderer = pTexture->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(UI_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);
	
		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Title", L"Celeste/Title.png");
		pMaterial->SetSampler(SAMPLER_LINEAR);
	
		pRenderer->SetDiffuse(Vector4(1.0f, 1.25f, 1.75f, 1.0f));
	
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pTexture);
	}
#pragma endregion


#pragma region SnowManager
	{
		CGameObject* pManager = CGameObject::CreateObject("SnowManager", pForegroundLayer);
		CParticleManager* pParticle = pManager->AddComponent<CParticleManager>("Snow Manager");
	
		pParticle->SetSpawnCount(3);
		pParticle->SetSpawnInterval(0.5f);
		pParticle->UseSnow();
		pParticle->SetSpawnLimit(40);
	
		SAFE_RELEASE(pParticle);
		SAFE_RELEASE(pManager)
	}
#pragma endregion



	SAFE_RELEASE(pManagerComponent);
	SAFE_RELEASE(pManager);
	SAFE_RELEASE(pUILayer);
	SAFE_RELEASE(pBackgroundLayer);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pForegroundLayer);



	return true;
}
