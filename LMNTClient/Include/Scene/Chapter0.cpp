#include "Chapter0.h"
#include "Layer.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Transform.h"
#include "Core.h"
#include "../Client.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Device.h"
#include "Material.h"
#include "Component/Camera.h"
#include "Component/RectCollider2D.h"
#include "../Component/Player.h"

#include "Component/Animator.h"
#include "AnimationClip2D.h"
#include "AnimationState.h"
#include "AnimationController.h"
#include "Transition.h"
#include "Component/Text.h"
#include "../Component/Hair.h"
#include "../Option.h"
#include "../GameData.h"
#include "../Component/CameraMove.h"
#include "Core.h"
#include "../Component/CollisionDetecter.h"
#include "../Component/HairTrail.h"
#include "../Component/ParticleManager.h"
#include "../Component/StageManager00.h"
#include "../Component/FallBlock.h"
#include "../Component/Dialogue.h"
#include "../Component/Bird.h"
#include "SoundManager.h"

CChapter0::CChapter0()
{
}


CChapter0::~CChapter0()
{
	GET_SINGLE(COption)->DestroyInstance();
}

CGameObject * CChapter0::LoadPrefab(const string & strFileName, CLayer * pLayer, const Vector3 & vPosition)
{
	CGameObject* pReturn = GET_SINGLE(CGameData)->LoadPrefab(strFileName, this, pLayer);

	CTransform* pTransform = pReturn->GetTransform();
	pTransform->SetWorldPosition(vPosition);
	SAFE_RELEASE(pTransform);

	if (strFileName.find("PrologueBridge") != string::npos)
	{
		CFallBlock* pFallBlock = pReturn->AddComponent<CFallBlock>("FallBlock");
		SAFE_RELEASE(pFallBlock);
	}

	return pReturn;
}

bool CChapter0::LoadSoundFiles()
{
	bool bLoadSound = true;

	//BGM
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_BGM, "env_amb_00_main", true, "env_amb_00_main.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_BGM, "mus_lvl0_intro_loop", true, "mus_lvl0_intro_loop.wav", BGM_PATH);

	//SFX
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_dreamblock", false, "char_mad_jump_dreamblock.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_super", false, "char_mad_jump_super.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_wall_left", false, "char_mad_jump_wall_left.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_special", false, "char_mad_jump_special.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_wall_climblayer_right", false, "char_mad_jump_wall_climblayer_right.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_assisted", false, "char_mad_jump_assisted.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_wall_right", false, "char_mad_jump_wall_right.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump", false, "char_mad_jump.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_superslide", false, "char_mad_jump_superslide.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_wall_climblayer_left", false, "char_mad_jump_wall_climblayer_left.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_jump_superwall", false, "char_mad_jump_superwall.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_dash_pink_right", false, "char_mad_dash_pink_right.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_death", false, "char_mad_death.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_asphalt_01", false, "char_mad_foot_00_asphalt_01.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_asphalt_02", false, "char_mad_foot_00_asphalt_02.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_asphalt_03", false, "char_mad_foot_00_asphalt_03.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_asphalt_04", false, "char_mad_foot_00_asphalt_04.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_asphalt_05", false, "char_mad_foot_00_asphalt_05.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_asphalt_06", false, "char_mad_foot_00_asphalt_06.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_asphalt_07", false, "char_mad_foot_00_asphalt_07.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_dirt_01", false, "char_mad_foot_00_dirt_01.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_dirt_02", false, "char_mad_foot_00_dirt_02.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_dirt_03", false, "char_mad_foot_00_dirt_03.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_dirt_04", false, "char_mad_foot_00_dirt_04.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_dirt_05", false, "char_mad_foot_00_dirt_05.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_dirt_06", false, "char_mad_foot_00_dirt_06.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_dirt_07", false, "char_mad_foot_00_dirt_07.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_snowsoft_01", false, "char_mad_foot_00_snowsoft_01.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_snowsoft_02", false, "char_mad_foot_00_snowsoft_02.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_snowsoft_03", false, "char_mad_foot_00_snowsoft_03.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_snowsoft_04", false, "char_mad_foot_00_snowsoft_04.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_snowsoft_05", false, "char_mad_foot_00_snowsoft_05.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_snowsoft_06", false, "char_mad_foot_00_snowsoft_06.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_snowsoft_07", false, "char_mad_foot_00_snowsoft_07.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_woodwalkway_01", false, "char_mad_foot_00_woodwalkway_01.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_woodwalkway_02", false, "char_mad_foot_00_woodwalkway_02.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_woodwalkway_03", false, "char_mad_foot_00_woodwalkway_03.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_woodwalkway_04", false, "char_mad_foot_00_woodwalkway_04.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_woodwalkway_05", false, "char_mad_foot_00_woodwalkway_05.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_woodwalkway_06", false, "char_mad_foot_00_woodwalkway_06.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_mad_foot_00_woodwalkway_07", false, "char_mad_foot_00_woodwalkway_07.wav", SFX_PATH);

	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_fallingblock_prologue_impact", false, "game_00_fallingblock_prologue_impact.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_fallingblock_prologue_release", false, "game_00_fallingblock_prologue_release.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_fallingblock_prologue_shake", false, "game_00_fallingblock_prologue_shake.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_bridge_rumble_loop", false, "game_00_bridge_rumble_loop.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_bridge_supportbreak_01", false, "game_00_bridge_supportbreak_01.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_bridge_supportbreak_02", false, "game_00_bridge_supportbreak_02.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_bridge_supportbreak_03", false, "game_00_bridge_supportbreak_03.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_bridge_supportbreak_04", false, "game_00_bridge_supportbreak_04.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_bridge_supportbreak_05", false, "game_00_bridge_supportbreak_05.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_00_bridge_supportbreak_06", false, "game_00_bridge_supportbreak_06.wav", SFX_PATH);


	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_touch", false, "game_gen_strawberry_touch.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_postgame_strawberry_total_all", false, "ui_postgame_strawberry_total_all.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_game_increment_strawberry", false, "ui_game_increment_strawberry.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "ui_game_increment_dashcount", false, "ui_game_increment_dashcount.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_touchswitch_any", false, "game_gen_touchswitch_any.wav", SFX_PATH);


	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_wingflap_01", false, "game_gen_strawberry_wingflap_01.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_wingflap_02", false, "game_gen_strawberry_wingflap_02.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_wingflap_03", false, "game_gen_strawberry_wingflap_03.wav", SFX_PATH);

	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_red_get_1up", false, "game_gen_strawberry_red_get_1up.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_red_get_1000", false, "game_gen_strawberry_red_get_1000.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_red_get_2000", false, "game_gen_strawberry_red_get_2000.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_red_get_3000", false, "game_gen_strawberry_red_get_3000.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_red_get_4000", false, "game_gen_strawberry_red_get_4000.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_red_get_5000", false, "game_gen_strawberry_red_get_5000.wav", SFX_PATH);

	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_pulse", false, "game_gen_strawberry_pulse.wav", SFX_PATH);
	bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_strawberry_flyaway", false, "game_gen_strawberry_flyaway.wav", SFX_PATH);



	return bLoadSound;
}

bool CChapter0::Initialize()
{
	if (!LoadSoundFiles())
		return false;

	GET_SINGLE(COption)->GetInstance();

	CTransform* pCameraTransform = GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	Vector3 vScreenSize = Vector3(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight, 0);

#pragma region Layer

	CLayer* pUILayer = FindLayer("UI");

	CLayer* pTempLayer = NULL;
	pTempLayer = CreateLayer("BG0", -90);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("BG1", -85);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("BG2", -80);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("BG3", -75);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("BG4", -70);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("BG5", -65);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("BackTerrain", -2);
	SAFE_RELEASE(pTempLayer);
	CLayer* pLayer = FindLayer("Default"); // 0
	CLayer* pObjectLayer = CreateLayer("Object", 1);
	pTempLayer = CreateLayer("NPC", 2);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("ForeTerrain", 3);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("VFX0", 4);
	SAFE_RELEASE(pTempLayer);
	CLayer* pPlayerLayer = NULL;
	pPlayerLayer = CreateLayer("Player", 5);
	pTempLayer = CreateLayer("VFX1", 10);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("VFX2", 15);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("FG0", 65);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("FG1", 70);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("FG2", 75);
	SAFE_RELEASE(pTempLayer);

	pTempLayer = CreateLayer("BackUI", INT_MAX - 15);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("MiddleUI", INT_MAX - 10);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = CreateLayer("ForeUI", INT_MAX - 5);
	SAFE_RELEASE(pTempLayer);

	SAFE_RELEASE(pObjectLayer);
	SAFE_RELEASE(pUILayer);

#pragma endregion

#pragma region Particle
	{
		CLayer* pLayer = FindLayer("MiddleUI");
		CLayer* pFG1 = FindLayer("FG1");
		CLayer* pBackTerrain = FindLayer("BackTerrain");
		CLayer* pBG3 = FindLayer("BG3");

		CGameObject* pParticleManager = CGameObject::CreateObject("ParticleManager", pLayer);
		CParticleManager* pParticle = pParticleManager->AddComponent<CParticleManager>("Particle Manager");

		pParticle->SetSpawnCount(10);
		pParticle->SetSpawnInterval(0.5f);
		pParticle->AddTargetColor(Vector4(0.9f, 1.0f, 1.0f, 1.0f));
		pParticle->AddTargetColor(Vector4(0.6, 0.6, 0.8f, 1.0f));
		pParticle->AddTargetColor(Vector4(0.235f, 0.235f, 0.315, 1.0f));

		pParticle->AddTargetLayer(pFG1);
		pParticle->AddTargetLayer(pBackTerrain);
		pParticle->AddTargetLayer(pBG3);


		SAFE_RELEASE(pBG3);
		SAFE_RELEASE(pBackTerrain);
		SAFE_RELEASE(pFG1);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pParticle);
		SAFE_RELEASE(pParticleManager);
	}
#pragma endregion



#pragma region Map
	
	GET_SINGLE(CGameData)->LoadStage(L"00_Area_102_Backup.stg", DATA_PATH, (CScene*)this);
		
#pragma endregion

#pragma region Collision Matrix
	{
		CLayer* pBackTerrainLayer = FindLayer("BackTerrain");
		CLayer* pTempObjectLayer = FindLayer("Object");
		SetCollisionMatrix(pPlayerLayer, pLayer, true);
		SetCollisionMatrix(pTempObjectLayer, pLayer, true);
		SetCollisionMatrix(pPlayerLayer, pTempObjectLayer, true);
		SetCollisionMatrix(pPlayerLayer, pBackTerrainLayer, true);
		SAFE_RELEASE(pTempObjectLayer);
		SAFE_RELEASE(pBackTerrainLayer);
	}
#pragma endregion
	

#pragma region Text Test
	CGameObject* pDebugText = NULL;
	{
		CGameObject* pText = CGameObject::CreateObject("Text", pLayer);
		CTransform* pTransform = pText->GetTransform();
	
		GET_SINGLE(COption)->m_pDebugText = pText;
	
		pDebugText = pText;
	
		pTransform->SetWorldPosition(100.f, 100.f, 0.f);
		CText* pTextComponent = pText->AddComponent<CText>("Text");
	
		pTextComponent->SetRenderArea(-50.f, -100.f, 500.f, 0.f);
		pTextComponent->SetText(L"");
		pTextComponent->SetColor(Vector4::white);
		SAFE_RELEASE(pTextComponent);
		SAFE_RELEASE(pTransform);
		SAFE_RELEASE(pText);
	}
#pragma endregion
	
#pragma region Madlane
	{
		CGameObject* pHairBackground = CGameObject::CreateObject("MadlineHairBackground", pPlayerLayer);
		CTransform* pHairBackgroundTransform = pHairBackground->GetTransform();
		pHairBackgroundTransform->SetWorldScale(40, 36, 1.0f);
		pHairBackgroundTransform->SetLocalPosition(0, 0, 0);
		pHairBackgroundTransform->SetPivot(0.0f, 1.0f, 0.0f);
		SAFE_RELEASE(pHairBackgroundTransform);

		CRenderer* pHairBackgroundRenderer = pHairBackground->AddComponent<CRenderer>("HairRenderer");
		pHairBackgroundRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pHairBackgroundRenderer->SetShader(COLORED_TEXTURE_SHADER);
		pHairBackgroundRenderer->SetInputLayout(POS_UV_LAYOUT);
		pHairBackgroundRenderer->SetRenderState(ALPHA_BLEND);
		pHairBackgroundRenderer->SetRenderMode(RM_2D);
		CMaterial*	pHairBackgroundMaterial = pHairBackgroundRenderer->CreateMaterial();
		pHairBackgroundMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pHairBackgroundMaterial->SetSampler(SAMPLER_POINT);
		pHairBackgroundRenderer->SetImageInfo("characters\\player\\bangs00_outline");
		pHairBackgroundRenderer->SetDiffuse(Vector4::black);
		SAFE_RELEASE(pHairBackgroundMaterial);
		SAFE_RELEASE(pHairBackgroundRenderer);
		CHairTrail* pTrail = pHairBackground->AddComponent<CHairTrail>("Trail");
		SAFE_RELEASE(pTrail);




		CGameObject* pHair = CGameObject::CreateObject("MadlineHair", pPlayerLayer);
		CTransform* pHairTransform = pHair->GetTransform();
		pHairTransform->SetWorldScale(32, 32, 1.0f);
		pHairTransform->SetLocalPosition(0, 0, 0);
		pHairTransform->SetPivot(0.0f, 1.0f, 0.0f);
		SAFE_RELEASE(pHairTransform);

		CRenderer* pHairRenderer = pHair->AddComponent<CRenderer>("HairRenderer");
		pHairRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pHairRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pHairRenderer->SetInputLayout(POS_UV_LAYOUT);
		pHairRenderer->SetRenderState(ALPHA_BLEND);
		pHairRenderer->SetRenderMode(RM_2D);

		CMaterial*	pHairMaterial = pHairRenderer->CreateMaterial();
		pHairMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pHairMaterial->SetSampler(SAMPLER_POINT);
		pHairRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pHairMaterial);
		SAFE_RELEASE(pHairRenderer);


		CGameObject* pMadeline = CGameObject::CreateObject("Madeline", pPlayerLayer);

		GET_SINGLE(CGameData)->SetPlayer(pMadeline);
		////////////////////////////Ä«¸Þ¶ó///////////////////////////////////////////////

		CGameObject* pCamera = GetMainCameraObject();
		CCameraMove* pMove = pCamera->AddComponent<CCameraMove>("Camera Move");
		pMove->SetAreaData(GET_SINGLE(CGameData)->GetAreaData());
		pMove->SetPlayer(pMadeline);
		pMove->AddBackground(this);
		SAFE_RELEASE(pMove);
		SAFE_RELEASE(pCamera);
		////////////////////////////////////////////////////////////////////////////////

		CTransform* pTransform = pMadeline->GetTransform();
		pTransform->SetPivot(0.5f, 1.0f, 0.0f);
		pTransform->SetWorldScale(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f);
		pTransform->SetWorldPosition(64.0f, 32.0f, 0.0f);

		/////////////HAIR////////////////
		CHair* pHairComponent = pHair->AddComponent<CHair>("Hair");
		pHairComponent->SetBody(pMadeline); 
		SAFE_RELEASE(pHairComponent);
		pHairComponent = pHairBackground->AddComponent<CHair>("HairBackground");
		pHairComponent->SetBody(pMadeline);
		pHairComponent->UseAsShadow();
		SAFE_RELEASE(pHairComponent);
		///////////////////////////////////


		CRenderer* pRenderer = pMadeline->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial* pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_POINT);
		SAFE_RELEASE(pMaterial);

		CAnimator* pAnimator = pMadeline->AddComponent<CAnimator>("Animator");
		CAnimationController* pController = new CAnimationController;
		pAnimator->SetAnimationController(pController);
		pAnimator->SetSyncScale(true);
		pAnimator->SetFixedScaleFactor(Vector3(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f));
		CAnimationState* pState = pController->CreateAnimationState("Stand");
		pState->AddClip("Stand", AO_LOOP, 0.9f, "Gameplay", L"Celeste\\Gameplay.png");
		pController->SetEntryState("Stand");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle00");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle01");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle02");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle03");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle04");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle05");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle06");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle07");
		pState->GetClip()->AddCustomInfo("characters\\player\\idle08");


		pState = pController->CreateAnimationState("Look Up");
		pState->AddClip("Look Up", AO_STOP_AT_END, 0.6f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\lookUp02");
		pState->GetClip()->AddCustomInfo("characters\\player\\lookUp03");
		pState->GetClip()->AddCustomInfo("characters\\player\\lookUp04");
		pState->GetClip()->AddCustomInfo("characters\\player\\lookUp05");
		pState->GetClip()->AddCustomInfo("characters\\player\\lookUp06");
		pState->GetClip()->AddCustomInfo("characters\\player\\lookUp07");

		pState = pController->CreateAnimationState("Climb Wall Slide");
		pState->AddClip("Climb Wall Slide", AO_LOOP, 1.0f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb00");


		pState = pController->CreateAnimationState("Climb Look Back");
		pState->AddClip("Climb Look Back", AO_STOP_AT_END, 0.24f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb06");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb07");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb08");



		pState = pController->CreateAnimationState("Climb Up");
		pState->AddClip("Climb Up", AO_LOOP, 0.24f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb00");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb01");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb02");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb03");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb04");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb05");

		pState = pController->CreateAnimationState("Climb Push");
		pState->AddClip("Climb Push", AO_LOOP, 0.24f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb00");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb09");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb10");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb11");

		pState = pController->CreateAnimationState("Climb Pull");
		pState->AddClip("Climb Pull", AO_LOOP, 0.24f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb00");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb12");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb13");
		pState->GetClip()->AddCustomInfo("characters\\player\\climb14");


		pState = pController->CreateAnimationState("Dash");
		pState->AddClip("Dash", AO_STOP_AT_END, 0.36f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\dash00");
		pState->GetClip()->AddCustomInfo("characters\\player\\dash01");
		pState->GetClip()->AddCustomInfo("characters\\player\\dash02");
		pState->GetClip()->AddCustomInfo("characters\\player\\dash03");


		pState = pController->CreateAnimationState("Duck");
		pState->AddClip("Duck", AO_LOOP, 0.03f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\duck");


		pState = pController->CreateAnimationState("Dream Dash In");
		pState->AddClip("Dream Dash In", AO_LOOP, 0.16f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash00");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash01");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash02");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash03");

		pState = pController->CreateAnimationState("Dream Dash Loop");
		pState->AddClip("Dream Dash Loop", AO_LOOP, 0.39f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash04");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash05");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash06");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash07");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash08");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash09");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash10");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash11");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash12");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash13");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash14");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash15");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash16");


		pState = pController->CreateAnimationState("Dream Dash Out");
		pState->AddClip("Dream Dash Out", AO_LOOP, 0.16f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash17");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash18");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash19");
		pState->GetClip()->AddCustomInfo("characters\\player\\dreamDash20");


		pState = pController->CreateAnimationState("Walk");
		pState->AddClip("Walk", AO_LOOP, 1.0f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk00");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk01");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk02");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk03");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk04");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk05");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk06");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk07");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk08");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk09");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk10");
		pState->GetClip()->AddCustomInfo("characters\\player\\walk11");

		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_01", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_02", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_03", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_04", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_05", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_06", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_07", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_01", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_02", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_03", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_04", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_05", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_06", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_07", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_01", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_02", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_03", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_04", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_05", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_06", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_07", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_01", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_02", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_03", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_04", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_05", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_06", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_07", true);
		pState->GetClip()->SetAudioPackage("Asphalt");

		pState = pController->CreateAnimationState("Run Fast");
		pState->AddClip("Run Fast", AO_LOOP, 0.6f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast00");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast01");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast02");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast03");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast04");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast05");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast06");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast07");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast08");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast09");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast10");
		pState->GetClip()->AddCustomInfo("characters\\player\\runFast11");

		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_01", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_02", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_03", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_04", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_05", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_06", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_07", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_01", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_02", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_03", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_04", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_05", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_06", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_07", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_01", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_02", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_03", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_04", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_05", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_06", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_07", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_01", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_02", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_03", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_04", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_05", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_06", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_07", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_01", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_02", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_03", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_04", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_05", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_06", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_07", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_01", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_02", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_03", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_04", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_05", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_06", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_07", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_01", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_02", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_03", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_04", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_05", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_06", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_07", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_01", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_02", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_03", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_04", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_05", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_06", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_07", true);
		pState->GetClip()->SetAudioPackage("Asphalt");




		pState = pController->CreateAnimationState("Run Slow");
		pState->AddClip("Run Slow", AO_LOOP, 1.0f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow00");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow01");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow02");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow03");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow04");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow05");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow06");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow07");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow08");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow09");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow10");
		pState->GetClip()->AddCustomInfo("characters\\player\\runSlow11");

		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_01", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_02", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_03", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_04", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_05", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_06", true);
		pState->GetClip()->AddAudioClip(3, "Asphalt", "char_mad_foot_00_asphalt_07", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_01", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_02", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_03", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_04", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_05", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_06", true);
		pState->GetClip()->AddAudioClip(9, "Asphalt", "char_mad_foot_00_asphalt_07", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_01", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_02", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_03", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_04", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_05", true);
		pState->GetClip()->AddAudioClip(3, "Dirt", "char_mad_foot_00_dirt_06", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_07", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_01", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_02", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_03", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_04", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_05", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_06", true);
		pState->GetClip()->AddAudioClip(9, "Dirt", "char_mad_foot_00_dirt_07", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_01", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_02", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_03", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_04", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_05", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_06", true);
		pState->GetClip()->AddAudioClip(3, "SnowSoft", "char_mad_foot_00_snowsoft_07", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_01", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_02", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_03", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_04", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_05", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_06", true);
		pState->GetClip()->AddAudioClip(9, "SnowSoft", "char_mad_foot_00_snowsoft_07", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_01", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_02", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_03", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_04", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_05", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_06", true);
		pState->GetClip()->AddAudioClip(3, "WoodWalkway", "char_mad_foot_00_woodwalkway_07", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_01", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_02", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_03", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_04", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_05", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_06", true);
		pState->GetClip()->AddAudioClip(9, "WoodWalkway", "char_mad_foot_00_woodwalkway_07", true);
		pState->GetClip()->SetAudioPackage("Asphalt");



		pState = pController->CreateAnimationState("Spin");
		pState->AddClip("Spin", AO_LOOP, 1.0f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin00");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin01");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin02");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin03");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin04");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin05");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin06");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin07");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin08");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin09");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin10");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin11");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin12");
		pState->GetClip()->AddCustomInfo("characters\\player\\spin13");



		pState = pController->CreateAnimationState("Jump Fast Up");
		pState->AddClip("Jump Fast Up", AO_STOP_AT_END, 0.2f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\jumpFast00");
		pState->GetClip()->AddCustomInfo("characters\\player\\jumpFast01");

		pState = pController->CreateAnimationState("Jump Fast Down");
		pState->AddClip("Jump Fast Down", AO_STOP_AT_END, 0.2f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\jumpFast02");
		pState->GetClip()->AddCustomInfo("characters\\player\\jumpFast03");

		pState = pController->CreateAnimationState("Jump Slow Up");
		pState->AddClip("Jump Slow Up", AO_STOP_AT_END, 0.2f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\jumpSlow00");
		pState->GetClip()->AddCustomInfo("characters\\player\\jumpSlow01");

		pState = pController->CreateAnimationState("Jump Slow Down");
		pState->AddClip("Jump Slow Down", AO_STOP_AT_END, 0.2f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\jumpSlow02");
		pState->GetClip()->AddCustomInfo("characters\\player\\jumpSlow03");


		pState = pController->CreateAnimationState("Fall");
		pState->AddClip("Fall", AO_LOOP, 0.2f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\fall00");
		pState->GetClip()->AddCustomInfo("characters\\player\\fall01");
		pState->GetClip()->AddCustomInfo("characters\\player\\fall02");
		pState->GetClip()->AddCustomInfo("characters\\player\\fall03");
		pState->GetClip()->AddCustomInfo("characters\\player\\fall04");
		pState->GetClip()->AddCustomInfo("characters\\player\\fall05");
		pState->GetClip()->AddCustomInfo("characters\\player\\fall06");
		pState->GetClip()->AddCustomInfo("characters\\player\\fall07");

		pState = pController->CreateAnimationState("Push");
		pState->AddClip("Push", AO_LOOP, 1.6f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\push00");
		pState->GetClip()->AddCustomInfo("characters\\player\\push01");
		pState->GetClip()->AddCustomInfo("characters\\player\\push02");
		pState->GetClip()->AddCustomInfo("characters\\player\\push03");
		pState->GetClip()->AddCustomInfo("characters\\player\\push04");
		pState->GetClip()->AddCustomInfo("characters\\player\\push05");
		pState->GetClip()->AddCustomInfo("characters\\player\\push06");
		pState->GetClip()->AddCustomInfo("characters\\player\\push07");
		pState->GetClip()->AddCustomInfo("characters\\player\\push08");
		pState->GetClip()->AddCustomInfo("characters\\player\\push09");
		pState->GetClip()->AddCustomInfo("characters\\player\\push10");
		pState->GetClip()->AddCustomInfo("characters\\player\\push11");
		pState->GetClip()->AddCustomInfo("characters\\player\\push12");
		pState->GetClip()->AddCustomInfo("characters\\player\\push13");
		pState->GetClip()->AddCustomInfo("characters\\player\\push14");
		pState->GetClip()->AddCustomInfo("characters\\player\\push15");

		pState = pController->CreateAnimationState("Death");
		pState->AddClip("Death", AO_STOP_AT_END, 0.3f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h00");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h01");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h02");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h03");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h04");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h05");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h06");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h07");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h08");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h09");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h10");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h11");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h12");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h13");
		pState->GetClip()->AddCustomInfo("characters\\player\\death_h14");


		pState = pController->CreateAnimationState("Sit");
		pState->AddClip("Sit", AO_STOP_AT_END, 1.6f, "Gameplay", L"Celeste\\Gameplay.png");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown00");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown01");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown02");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown03");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown04");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown05");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown06");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown07");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown08");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown09");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown10");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown11");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown12");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown13");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown14");
		pState->GetClip()->AddCustomInfo("characters\\player\\sitDown15");

		CPlayer* pPlayer = pMadeline->AddComponent<CPlayer>("Player");
		pPlayer->m_pDebugText = pDebugText;
		pPlayer->AddHair(pHair);
		pPlayer->AddHair(pHairBackground, true);
		pPlayer->SetBeginPosition(Vector3(1376.0f, 512.0f, 0.0f));


		CRectCollider2D* pBottomCollider = pMadeline->AddComponent<CRectCollider2D>("MadelineBody");
		pBottomCollider->SetScale(20, 15.0f, 1.0f);
		pBottomCollider->SetPivot(0.5f, 1.0f, 0.0f);

		CRectCollider2D* pTopCollider = pMadeline->AddComponent<CRectCollider2D>("MadelineBody");
		pTopCollider->SetScale(10, 15.0f, 1.0f);
		pTopCollider->SetPivot(0.5f, 4.0f, 0.0f);

		CRectCollider2D* pRightCollider = pMadeline->AddComponent<CRectCollider2D>("MadelineBody");
		pRightCollider->SetScale(10.0f, 35.0f, 1.0f);
		pRightCollider->SetPivot(0.0f - 0.75f, 1.5f, 0.0f);

		CRectCollider2D* pLeftCollider = pMadeline->AddComponent<CRectCollider2D>("MadelineBody");
		pLeftCollider->SetScale(10.0f, 35.0f, 1.0f);
		pLeftCollider->SetPivot(1.0f + 0.75f, 1.5f, 0.0f);


		pPlayer->SetTopCollider(pTopCollider);
		pPlayer->SetBottomCollider(pBottomCollider);
		pPlayer->SetLeftCollider(pLeftCollider);
		pPlayer->SetRightCollider(pRightCollider);
		SAFE_RELEASE(pTopCollider);
		SAFE_RELEASE(pBottomCollider);
		SAFE_RELEASE(pRightCollider);
		SAFE_RELEASE(pLeftCollider);


		SAFE_RELEASE(pHair);
		SAFE_RELEASE(pHairBackground);

		SAFE_RELEASE(pTransform);
		SAFE_RELEASE(pAnimator);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pPlayer);
		SAFE_RELEASE(pMadeline);

	}
#pragma endregion

#pragma region OldLady
{
	CLayer* pNPCLayer = FindLayer("NPC");
	CGameObject* pOldLady = CGameObject::CreateObject("OldLady", pNPCLayer);
	
	CTransform* pOldLadyTransform = pOldLady->GetTransform();
	pOldLadyTransform->SetPivot(0.5f, 1.0f, 0.0f);
	pOldLadyTransform->SetWorldScale(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f);
	pOldLadyTransform->SetWorldPosition(5632.0f, 64.0f, 0.0f);
	SAFE_RELEASE(pOldLadyTransform);
	
	
	CRenderer* pOldLadyRenderer = pOldLady->AddComponent<CRenderer>("Renderer");
	pOldLadyRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pOldLadyRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pOldLadyRenderer->SetInputLayout(POS_UV_LAYOUT);
	pOldLadyRenderer->SetRenderState(ALPHA_BLEND);
	pOldLadyRenderer->SetRenderMode(RM_2D);
	
	CMaterial* pOldLadyMaterial = pOldLadyRenderer->CreateMaterial();
	pOldLadyMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pOldLadyMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pOldLadyMaterial);
	SAFE_RELEASE(pOldLadyRenderer);

	CAnimator* pOldLadyAnimator = pOldLady->AddComponent<CAnimator>("Animator");
	CAnimationController* pOldLadyController = new CAnimationController;
	pOldLadyAnimator->SetAnimationController(pOldLadyController);
	pOldLadyAnimator->SetSyncScale(true);
	pOldLadyAnimator->SetFixedScaleFactor(Vector3(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f));
	SAFE_RELEASE(pOldLadyAnimator);
	
	CAnimationState* pState = pOldLadyController->CreateAnimationState("OldLady Idle");
	pState->AddClip("OldLady Idle", AO_LOOP, 1.8f, "Gameplay", L"Celeste\\Gameplay.png");
	pOldLadyController->SetEntryState("OldLady Idle");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle00");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle01");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle02");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle03");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle04");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle05");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle06");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle07");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle08");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle09");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle10");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\idle11");														  
	
	pState = pOldLadyController->CreateAnimationState("OldLady Walk");
	pState->AddClip("OldLady Walk", AO_LOOP, 1.8f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\walk00");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\walk01");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\walk02");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\walk03");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\walk04");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\walk05");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\walk06");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\walk07");	
	
	pState = pOldLadyController->CreateAnimationState("OldLady Laugh");
	pState->AddClip("OldLady Laugh", AO_LOOP, 0.4f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\laugh00");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\laugh01");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\laugh02");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\laugh03");
	
	pState = pOldLadyController->CreateAnimationState("OldLady Air Quotes");
	pState->AddClip("OldLady Air Quotes", AO_LOOP, 1.92f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes00");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes01");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes02");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes03");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes04");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes05");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes06");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes07");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes08");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes09");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes10");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes11");
	
	pState = pOldLadyController->CreateAnimationState("OldLady Point Cane");
	pState->AddClip("OldLady Point Cane", AO_LOOP, 2.72f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes12");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes13");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes14");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes15");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes16");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes17");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes18");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes19");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes20");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes21");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes22");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes23");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes24");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes25");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes26");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes27");
	pState->GetClip()->AddCustomInfo("characters\\oldlady\\quotes28");
	
	
	SAFE_RELEASE(pOldLady);
	SAFE_RELEASE(pNPCLayer);
}
#pragma endregion

#pragma region Bird
{
	CLayer* pNPCLayer = FindLayer("NPC");
	CGameObject* pBird = CGameObject::CreateObject("Bird", pNPCLayer);

	CTransform* pBirdTransform = pBird->GetTransform();
	pBirdTransform->SetPivot(0.5f, 1.0f, 0.0f);
	pBirdTransform->SetWorldScale(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f);
	pBirdTransform->SetWorldPosition(3552.0f, 320.0f, 0.0f);
	pBirdTransform->SetWorldRotation(0.0f, PI, 0.0f);
	SAFE_RELEASE(pBirdTransform);


	CRenderer* pBirdRenderer = pBird->AddComponent<CRenderer>("Renderer");
	pBirdRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pBirdRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pBirdRenderer->SetInputLayout(POS_UV_LAYOUT);
	pBirdRenderer->SetRenderState(ALPHA_BLEND);
	pBirdRenderer->SetRenderMode(RM_2D);

	CMaterial* pBirdMaterial = pBirdRenderer->CreateMaterial();
	pBirdMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pBirdMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pBirdMaterial);
	SAFE_RELEASE(pBirdRenderer);

	CAnimator* pBirdAnimator = pBird->AddComponent<CAnimator>("Animator");
	CAnimationController* pBirdController = new CAnimationController;
	pBirdAnimator->SetAnimationController(pBirdController);
	pBirdAnimator->SetSyncScale(true);
	pBirdAnimator->SetFixedScaleFactor(Vector3(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f));
	SAFE_RELEASE(pBirdAnimator);

	CAnimationState* pState = pBirdController->CreateAnimationState("Bird Idle");
	pState->AddClip("Bird Idle", AO_STOP_AT_END, 0.5f, "Gameplay", L"Celeste\\Gameplay.png");
	pBirdController->SetEntryState("Bird Idle");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");

	pState = pBirdController->CreateAnimationState("Bird Croak");
	pState->AddClip("Bird Croak", AO_STOP_AT_END, 1.12f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow01");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow02");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow03");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow04");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow05");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow06");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow07");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow08");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow09");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow10");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow11");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow12");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow13");

	pState = pBirdController->CreateAnimationState("Bird Peck");
	pState->AddClip("Bird Peck", AO_STOP_AT_END, 1.04f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow14");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow15");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow16");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow17");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow18");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow19");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow20");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow21");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow22");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow23");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow24");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow25");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow26");

	pState = pBirdController->CreateAnimationState("Bird Jump");
	pState->AddClip("Bird Jump", AO_STOP_AT_END, 0.56f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow29");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow30");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow31");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow32");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow33");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow34");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow35");

	pState = pBirdController->CreateAnimationState("Bird Fly");
	pState->AddClip("Bird Fly", AO_LOOP, 0.4f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow03");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow00");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow01");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow02");

	pState = pBirdController->CreateAnimationState("Bird Fall");
	pState->AddClip("Bird Fall", AO_LOOP, 0.21f, "Gameplay", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow42");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow43");
	pState->GetClip()->AddCustomInfo("characters\\bird\\crow44");

	CBird* pBirdComponent = pBird->AddComponent<CBird>("Bird");
	pBirdComponent->SetAnimationController(pBirdController);
	SAFE_RELEASE(pBirdComponent);
	SAFE_RELEASE(pBird);
	SAFE_RELEASE(pNPCLayer);


}
#pragma endregion




	SAFE_RELEASE(pPlayerLayer);
	SAFE_RELEASE(pLayer);

	GET_SINGLE(CCore)->SetClearColor(4, 4, 4, 255);

	{
		CLayer* pPlayerLayer = FindLayer("Player");
		CLayer* pTempUILayer = FindLayer("UI");
		CLayer* pTempObjectLayer = FindLayer("Object");

		CGameObject* pStageManager = CGameObject::CreateObject("StageManager", pTempUILayer);

		CDialogue* pDialogue = pStageManager->AddComponent<CDialogue>("Dialogue");

		CStageManager00* pStageManagerComponent = pStageManager->AddComponent<CStageManager00>("StageManager");
		pStageManagerComponent->SetDialogue(pDialogue);
		SAFE_RELEASE(pDialogue);


		CLayer* pNPCLayer = FindLayer("NPC");
		CGameObject* pGranny = pNPCLayer->FindGameObjectWithTag("OldLady");
		pStageManagerComponent->SetGranny(pGranny);
		SAFE_RELEASE(pGranny);

		CGameObject* pBird = pNPCLayer->FindGameObjectWithTag("Bird");
		pStageManagerComponent->SetBird(pBird);
		SAFE_RELEASE(pBird);

		SAFE_RELEASE(pNPCLayer);


		CGameObject* pFallBlock = pTempObjectLayer->FindGameObjectWithTag("PrologueFallBlockObject");
		pStageManagerComponent->SetFallBlock(pFallBlock);
		SAFE_RELEASE(pFallBlock);

		CGameObject* pPlayer = pPlayerLayer->FindGameObjectWithTag("Madeline");
		pStageManagerComponent->SetPlayer(pPlayer);
		SAFE_RELEASE(pPlayer);

		Vector3 vOffset(6369.0f, 128.0f, 0.0f);
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge00", pTempObjectLayer, vOffset + Vector3(0.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge01", pTempObjectLayer, vOffset + Vector3(64.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge02", pTempObjectLayer, vOffset + Vector3(96.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge03", pTempObjectLayer, vOffset + Vector3(128.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge04", pTempObjectLayer, vOffset + Vector3(160.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge05", pTempObjectLayer, vOffset + Vector3(192.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge06", pTempObjectLayer, vOffset + Vector3(224.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge07", pTempObjectLayer, vOffset + Vector3(256.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge08", pTempObjectLayer, vOffset + Vector3(288.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge09", pTempObjectLayer, vOffset + Vector3(320.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge10", pTempObjectLayer, vOffset + Vector3(384.0f, 0.0f, 0.0f)));

		vOffset += Vector3(416.0f, 0.0f, 0.0f) ;
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge00", pTempObjectLayer, vOffset + Vector3(0.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge01", pTempObjectLayer, vOffset + Vector3(64.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge02", pTempObjectLayer, vOffset + Vector3(96.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge03", pTempObjectLayer, vOffset + Vector3(128.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge04", pTempObjectLayer, vOffset + Vector3(160.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge05", pTempObjectLayer, vOffset + Vector3(192.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge06", pTempObjectLayer, vOffset + Vector3(224.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge07", pTempObjectLayer, vOffset + Vector3(256.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge08", pTempObjectLayer, vOffset + Vector3(288.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge09", pTempObjectLayer, vOffset + Vector3(320.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge10", pTempObjectLayer, vOffset + Vector3(384.0f, 0.0f, 0.0f)));


		vOffset += Vector3(416.0f, 0.0f, 0.0f);
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge00", pTempObjectLayer, vOffset + Vector3(0.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge01", pTempObjectLayer, vOffset + Vector3(64.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge02", pTempObjectLayer, vOffset + Vector3(96.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge03", pTempObjectLayer, vOffset + Vector3(128.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge04", pTempObjectLayer, vOffset + Vector3(160.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge05", pTempObjectLayer, vOffset + Vector3(192.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge06", pTempObjectLayer, vOffset + Vector3(224.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge07", pTempObjectLayer, vOffset + Vector3(256.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge08", pTempObjectLayer, vOffset + Vector3(288.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge09", pTempObjectLayer, vOffset + Vector3(320.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge10", pTempObjectLayer, vOffset + Vector3(384.0f, 0.0f, 0.0f)));


		vOffset += Vector3(416.0f, 0.0f, 0.0f);
		//pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge00", pTempObjectLayer, vOffset + Vector3(0.0f, 0.0f, 0.0f)));
		//pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge09", pTempObjectLayer, vOffset + Vector3(320.0f, 0.0f, 0.0f)));
		pStageManagerComponent->AddFallBlockBridge(LoadPrefab("PrologueBridge10", pTempObjectLayer, vOffset + Vector3(384.0f, 0.0f, 0.0f)));

		pStageManagerComponent->InitializeDialogue();

		SAFE_RELEASE(pStageManagerComponent);
		SAFE_RELEASE(pStageManager);
		
		SAFE_RELEASE(pPlayerLayer);
		SAFE_RELEASE(pTempUILayer);
		SAFE_RELEASE(pTempObjectLayer);
	}



	return true;
}

void CChapter0::Start()
{
	GET_SINGLE(CSoundManager)->Play("env_amb_00_main");
}
