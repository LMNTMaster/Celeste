#include "TestScene.h"
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



CTestScene::CTestScene()
{
}


CTestScene::~CTestScene()
{
}

bool CTestScene::Initialize()
{
	CLayer* pLayer = FindLayer("UI");

	CGameObject* pGameObject = CGameObject::CreateObject("Name", pLayer);
	CRectCollider2D* pCollider = pGameObject->AddComponent<CRectCollider2D>("COllider");
	SAFE_RELEASE(pCollider);
	SAFE_RELEASE(pGameObject);
	SAFE_RELEASE(pLayer);

	return true;
}
