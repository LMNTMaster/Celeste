#include "GameData.h"
#include "PathManager.h"
#include "ResourcesManager.h"
#include "Component/Tile.h"
#include "SceneManager.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
//#include "Scene.h"
#include "Layer.h"
#include "Component/RectCollider2D.h"
#include "Component\Celeste.h"
#include "Component\Death.h"
#include "Component\SceneEffect.h"
#include "Component\Player.h"
#include "Component\FallBlock.h"
#include "Component/Camera.h"
#include "Component\CameraMove.h"	

#include "Component/Animator.h"
#include "AnimationClip2D.h"
#include "AnimationState.h"
#include "AnimationController.h"
#include "Transition.h"
#include "Component\ParticleMove.h"
#include "Component\Refill.h"
#include "Component\Strawberry.h"
#include "SoundManager.h"
#include "Component/Crumble.h"
#include "Component\Distortion.h"

DEFINITION_SINGLE(CGameData)

CGameData::CGameData():
	m_tCurrentArea(NULL),
	m_tPrevArea(NULL)
{
}

CGameData::~CGameData()
{
	Safe_Delete_VecList(m_vecArea);
}

void CGameData::SetPlayer(CGameObject * pPlayer)
{
	m_pPlayer = pPlayer;
}

void CGameData::AddObjectIntoAreaData(CGameObject * pGameObject)
{
	CTransform* pTransform = pGameObject->GetTransform();
	Vector3 vPosition = pTransform->GetWorldPosition();
	SAFE_RELEASE(pTransform);

	PAREA pArea = GetAreaData(vPosition);
	
	pArea->vecObjects.push_back(pGameObject);
}

PAREA CGameData::GetAreaData(const Vector3 & vPosition)
{
	size_t iSize = m_vecArea.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (vPosition.x > m_vecArea[i]->vRange.x && vPosition.x < m_vecArea[i]->vRange.x + m_vecArea[i]->vRange.z &&
			vPosition.y > m_vecArea[i]->vRange.y && vPosition.y < m_vecArea[i]->vRange.y + m_vecArea[i]->vRange.w)
		{
			return m_vecArea[i];
		}
	}
}

vector<PAREA>* CGameData::GetAreaData()
{
	return &m_vecArea;
}

void CGameData::SetGameManager(CCeleste * pCeleste)
{
	m_pGameManager = pCeleste;
}

CCeleste * CGameData::GetGameManager() const
{
	return m_pGameManager;
}

void CGameData::CreateSpike(FILE* pFile, DIRECTION eDirection, class CLayer* pLayer) const
{
	CreateSpike(pFile, eDirection, pLayer);
}

CGameObject * CGameData::CreateSpike(FILE * pFile, DIRECTION eDirection, CLayer * pLayer)
{
	string strDirection;
	string strAtlasKey;
	Vector3 vPivot;
	switch (eDirection)
	{
	case LMNT::DIR_LEFT:
		strDirection = "Left";
		strAtlasKey = "danger\\spikes\\default_left00";
		vPivot.x = -1.0f / 3.0f;;
		vPivot.y = 0.0f;
		vPivot.z = 0.0f;
		break;
	case LMNT::DIR_TOP:
		strDirection = "Up";
		strAtlasKey = "danger\\spikes\\default_up00";
		vPivot.x = 0.0f;
		vPivot.y = -1.0f / 3.0f;
		vPivot.z = 0.0f;
		break;
	case LMNT::DIR_RIGHT:
		strDirection = "Right";
		strAtlasKey = "danger\\spikes\\default_right00";
		vPivot.x = 0.0f;
		vPivot.y = 0.0f;
		vPivot.z = 0.0f;
		break;
	case LMNT::DIR_BOTTOM:
		strDirection = "Down";
		strAtlasKey = "danger\\spikes\\default_down00";
		vPivot.x = 0.0f;
		vPivot.y = 0.0f;
		vPivot.z = 0.0f;
		break;
	default:
		strDirection = "Up";
		break;
	}

	CGameObject* pDanger = CGameObject::CreateObject("Spike", pLayer);
	CTransform* pTransform = pDanger->GetTransform();
	pTransform->Load(pFile);
	pTransform->SetPivot(vPivot);
	SAFE_RELEASE(pTransform);

	PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData(strAtlasKey);
	CRenderer* pRenderer = pDanger->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	pRenderer->SetImageInfo(strAtlasKey);
	SAFE_RELEASE(pRenderer);

	CRectCollider2D* pCollider = pDanger->AddComponent<CRectCollider2D>("Spike");
	pCollider->SetScale(pData->wWidth * CELESTE_PIXEL, pData->wHeight * CELESTE_PIXEL, 1.0f);
	SAFE_RELEASE(pCollider);
	CGameObject* pReturn = pDanger;
	SAFE_RELEASE(pDanger);

	return pReturn;

}

void CGameData::CreateFootDust(const Vector3 & vPosition)
{
	Vector3 LT = vPosition - Vector3(20.0f, 16.0f, 0.0f);

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->FindLayer("Default");

	int bRandomIndex[40] = {};

	for (int i = 0; i < 40; ++i)
	{
		bRandomIndex[i] = i;
 	}

	for (int i = 0; i < 100; ++i)
	{
		int idx1 = rand() % 40;
		int idx2 = rand() % 40;
		int iTemp = bRandomIndex[idx1];
		bRandomIndex[idx1] = bRandomIndex[idx2];
		bRandomIndex[idx2] = iTemp;
	}

	for (int i = 0; i < 30; ++i)
	{
		int X = bRandomIndex[i] % 10;
		int Y = (int)bRandomIndex[i] / (int)10;
		Vector3 vSpawnPosition = LT + Vector3((float)X * 4, (float)Y * 4, 0.0f);
		//Vector3 vDirection = (vSpawnPosition - vPosition).Normalize();
		//Vector3 vTargetPosition = vPosition + (vDirection* 10.0f);
		Vector3 vTargetPosition = vSpawnPosition + Vector3(0.0f, -1.0f, 0.0f) * (rand() % 40);
 		CGameObject* pParticle = CreateParticle(pLayer, vSpawnPosition, vTargetPosition, Vector4::white, true, true, 2, 4, rand() % 20 + 10);
		SAFE_RELEASE(pParticle);
	}

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}


void CGameData::CreateDistortionEffect(const Vector3& vPosition, float fTime, float fScale)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pVFXLayer = pScene->FindLayer("VFX2");

	CGameObject* pDistortion = CGameObject::CreateObject("Distortion", pVFXLayer);

	CTransform* pTransform = pDistortion->GetTransform();
	pTransform->SetPivot(0.5f, 0.5f, 0.5f);
	pTransform->SetWorldPosition(vPosition);
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pDistortion->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);

	pRenderer->SetImageInfo("util\\displacementCircle");
	SAFE_RELEASE(pRenderer);

	CDistortion* pComponent = pDistortion->AddComponent<CDistortion>("Distortion");
	pComponent->SetTargetScale(fScale);
	pComponent->SetTotalTime(fTime);
	SAFE_RELEASE(pComponent);
		
	SAFE_RELEASE(pDistortion);
	SAFE_RELEASE(pVFXLayer);
	SAFE_RELEASE(pScene);
}

void CGameData::CreateStrawberry(const Vector3 & position, BERRY_TYPE eType, CLayer * pLayer)
{
	CGameObject* pStrawberry = CGameObject::CreateObject("Strawberry", pLayer);

	CTransform* pStrawberryTransform = pStrawberry->GetTransform();
	pStrawberryTransform->SetPivot(0.5f, 0.5f, 0.5f);
	pStrawberryTransform->SetWorldPosition(position);
	SAFE_RELEASE(pStrawberryTransform);
	CStrawberry* pStrawberryComponent = pStrawberry->AddComponent<CStrawberry>("Refill");
	pStrawberryComponent->SetStrawberryType(eType);
	SAFE_RELEASE(pStrawberryComponent);
	SAFE_RELEASE(pStrawberry);
}

void CGameData::CreateRefill(const Vector3& position, CLayer * pLayer)
{
	CGameObject* pRefill = CGameObject::CreateObject("Refill", pLayer);
	CTransform* pRefillTransform = pRefill->GetTransform();
	pRefillTransform->SetPivot(0.5f, 0.5f, 0.5f);
	pRefillTransform->SetWorldPosition(position);
	SAFE_RELEASE(pRefillTransform);
	CRefill* pRefillComponent = pRefill->AddComponent<CRefill>("Refill");
	SAFE_RELEASE(pRefillComponent);
	SAFE_RELEASE(pRefill);	
}

//해제 해줘야함
CGameObject * CGameData::CreateParticle(CLayer * pLayer, const Vector3 vSpawnPosition, Vector3 vTargetPosition, const Vector4 vColor, bool bUseFadeout, bool bUseSpeedReduce, float fFrequency, float fAmplitude, float fSpeed)
{
	CGameObject* pParticle = CGameObject::CreateObject("Particle", pLayer);

	CTransform* pTransform = pParticle->GetTransform();
	pTransform->SetWorldScale(CELESTE_PIXEL, CELESTE_PIXEL, CELESTE_PIXEL);
	pTransform->SetWorldPosition(vSpawnPosition);
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pParticle->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);

	pRenderer->SetImageInfo("util\\particle");
	pRenderer->SetDiffuse(vColor);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);


	CParticleMove* pMove = pParticle->AddComponent<CParticleMove>("Particle");
	pMove->SetTargetPosition(vTargetPosition);
	pMove->SetFadeout(bUseFadeout);
	pMove->SetAmplitude(fAmplitude);
	pMove->SetFrequency(fFrequency);
	pMove->SetMoveSpeed(fSpeed);
	pMove->SetReduceSpeed(bUseSpeedReduce);
	SAFE_RELEASE(pMove);

	return pParticle;
}

void CGameData::ResetArea(PAREA pArea)
{
	CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
	pPlayer->Reset();
	SAFE_RELEASE(pPlayer);

	size_t iSize = pArea->vecObjects.size();
	for (size_t i = 0; i < iSize; ++i)
	{
		CFallBlock* pComponent = pArea->vecObjects[i]->GetComponentFromType<CFallBlock>(CT_FALL_BLOCK);
		if (pComponent)
		{
			pComponent->Reset();
		}
	}
}

void CGameData::ResetArea(const Vector3 & pPosition)
{
	ResetArea(GetAreaData(pPosition));
}


void CGameData::ResetArea()
{
	CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
	pPlayer->Reset();

	PAREA pArea = pPlayer->GetCurrentArea();

	size_t iSize = pArea->vecObjects.size();
	for (size_t i = 0; i < iSize; ++i)
	{
		CFallBlock* pComponent = pArea->vecObjects[i]->GetComponentFromType<CFallBlock>(CT_FALL_BLOCK);
		if (pComponent)
		{
			pComponent->Reset();
		}
	}

	SAFE_RELEASE(pPlayer);

}



PAREA CGameData::GetCurrentArea() const
{
	return m_tCurrentArea;
}

PAREA CGameData::GetPrevArea() const
{
	return m_tPrevArea;
}

//구역이 변경되면 true, 변경되지 않으면 false
bool CGameData::UpdateAreaData()
{
	if (m_pPlayer)
	{
		CTransform* pTransform = m_pPlayer->GetTransform();
		Vector3 vPlayerPosition = pTransform->GetWorldPosition();
		SAFE_RELEASE(pTransform);


		for (size_t i = 0; i < m_vecArea.size(); ++i)
		{
			if (vPlayerPosition.x > m_vecArea[i]->vRange.x && vPlayerPosition.x < m_vecArea[i]->vRange.x + m_vecArea[i]->vRange.z &&
				vPlayerPosition.y > m_vecArea[i]->vRange.y && vPlayerPosition.y < m_vecArea[i]->vRange.y + m_vecArea[i]->vRange.w)
			{
				m_tCurrentArea = m_vecArea[i];
			}
		}

		if (m_tCurrentArea != m_tPrevArea)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void CGameData::SetPrevArea()
{
	m_tPrevArea = m_tCurrentArea;
}

Vector3 CGameData::GetCheckPointPosition(PAREA pArea, const Vector3 & vTargetPosition)
{
	int iSize = pArea->vecCheckPoint.size();
	
	float fMinDistance = FLT_MAX;
	Vector3 vMinPosition = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	for (int i = 0; i < iSize; ++i)
	{
		float fDistance = (vTargetPosition - pArea->vecCheckPoint[i]).Length();
		if (fDistance <= fMinDistance)
		{
			fMinDistance = fDistance;
			vMinPosition = pArea->vecCheckPoint[i];
		}
	}

	return vMinPosition;
}

void CGameData::ShakeCamera(float fDuration, float fAmplitude)
{
	CScene* pScene = m_pPlayer->GetScene();
	CGameObject* pCamera = pScene->GetMainCameraObject();
	CCameraMove* pCameraMove = pCamera->GetComponentFromType<CCameraMove>(CT_CAMERA_MOVE);
	pCameraMove->Shake(fDuration, fAmplitude);
	SAFE_RELEASE(pCameraMove);
	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pScene);
}

CGameObject * CGameData::GetPlayer() const
{
	return m_pPlayer;
}

bool CGameData::LoadData()
{
	if(!LoadData("Celeste\\Gameplay11.ap"))
		return false;
	if(!LoadData("Celeste\\Portraits.ap"))
		return false;

	return true;
}

bool CGameData::LoadData(const string& strPath)
{
	string strFullPath = GET_SINGLE(CPathManager)->FindPathMultiByte(TEXTURE_PATH);

	strFullPath += strPath;

	FILE* pFile = NULL;

	fopen_s(&pFile, strFullPath.c_str(), "rb");

	if (!pFile)
		return false;
	else
	{
		int iCount = 0;
		int iAtlasSizeX = 0;
		int iAtlasSizeY = 0;
		fread(&iCount, 4, 1, pFile);
		fread(&iAtlasSizeX, 4, 1, pFile);
		fread(&iAtlasSizeY, 4, 1, pFile);

		for (int i = 0; i < iCount; ++i)
		{
			PATLASDATA pData = new ATLASDATA;
			int iLength = 0;
			fread(&iLength, 4, 1, pFile);
			fread(&pData->pDirectory, 1, iLength, pFile);
			fread(&pData->wX, 2, 1, pFile);
			fread(&pData->wY, 2, 1, pFile);
			fread(&pData->wWidth, 2, 1, pFile);
			fread(&pData->wHeight, 2, 1, pFile);
			pData->iAtlasSizeX = iAtlasSizeX;
			pData->iAtlasSizeY = iAtlasSizeY;
			
			if (pData->wHeight == 0 && pData->wWidth == 0 && pData->wX == 0 && pData->wY == 0)
			{
				delete pData;
				continue;
			}

			GET_SINGLE(CResourcesManager)->InsertAtlasData(pData);
		}

		fclose(pFile);
		return true;
	}
}

bool CGameData::LoadStage(const wchar_t* pFileName, const string& strPathKey, CScene* pTargetScene)
{


	GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "game_gen_spring", false, "game_gen_spring.wav", SFX_PATH);

	m_vecArea.clear();

	string strPath = GET_SINGLE(CPathManager)->FindPathMultiByte(DATA_PATH);


	char	astrPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, astrPath, lstrlenW(pFileName) * 2, 0, 0);

	strPath += string(astrPath);

	FILE* pFile = NULL;
	CScene* pScene = pTargetScene;//GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->FindLayer("Default");


	fopen_s(&pFile, strPath.c_str(), "rb");

	if (pFile)
	{
		{
			int iCount = 0;
			fread(&iCount, 4, 1, pFile);
			for (int i = 0; i < iCount; ++i)
			{
				CGameObject* pTile = CGameObject::CreateObject("Tile", pLayer);
				CRenderer* pTileRenderer = pTile->AddComponent<CRenderer>("TileRenderer");
				pTileRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
				pTileRenderer->SetShader(STANDARD_TEXTURE_SHADER);
				pTileRenderer->SetInputLayout(POS_UV_LAYOUT);
				pTileRenderer->SetRenderState(ALPHA_BLEND);
				pTileRenderer->SetRenderMode(RM_2D);

				CMaterial*	pMaterial = pTileRenderer->CreateMaterial();
				pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
				pMaterial->SetSampler(SAMPLER_POINT);
				SAFE_RELEASE(pMaterial);
				SAFE_RELEASE(pTileRenderer);

				CTransform* pTileTransform = pTile->GetTransform();
				pTileTransform->Load(pFile); 
				SAFE_RELEASE(pTileTransform);

				CTile* pTileComponent = pTile->AddComponent<CTile>("Tile");
				pTileComponent->Load(pFile);
				SAFE_RELEASE(pTileComponent);

				//CRectCollider2D* pCollider = pTile->AddComponent<CRectCollider2D>("Tile");
				//pCollider->SetScale(pTileTransform->GetWorldScale());
				//pCollider->SetPivot(0.0f, 0.0f, 0.0f);				
				//SAFE_RELEASE(pCollider);
				SAFE_RELEASE(pTile);
			}
		}


		{
			//나머지 오브젝트 저장
			int iCount = 0;
			fread(&iCount, 4, 1, pFile);
			for (int i = 0; i < iCount; ++i)
			{
				int iLength = 0;
				char strTagBuffer[256] = {};
				fread(&iLength, 4, 1, pFile);
				fread(strTagBuffer, 1, iLength, pFile);

				//Layer읽음
				iLength = 0;
				char cLayerBuffer[256] = {};
				fread(&iLength, 4, 1, pFile);
				fread(cLayerBuffer, sizeof(char), iLength, pFile);

				CLayer* pTempLayer = pScene->FindLayer(string(cLayerBuffer));
				string pTag = pTempLayer->GetTag();

				CGameObject* pObject = CGameObject::CreateObject(pTag, pTempLayer);
				SAFE_RELEASE(pTempLayer);

				CTransform* pTransform = pObject->GetTransform();
				pTransform->Load(pFile);
				SAFE_RELEASE(pTransform);

				CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
				pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
				pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
				pRenderer->SetInputLayout(POS_UV_LAYOUT);
				pRenderer->SetRenderState(ALPHA_BLEND);
				pRenderer->SetRenderMode(RM_2D);

				CMaterial*	pMaterial = pRenderer->CreateMaterial();
				pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
				pMaterial->SetSampler(SAMPLER_POINT);
				SAFE_RELEASE(pMaterial);

				char strAtlasKey[256] = {};
				iLength = 0;
				fread(&iLength, 4, 1, pFile);
				fread(strAtlasKey, sizeof(char), iLength, pFile);
				pRenderer->SetImageInfo(string(strAtlasKey));
				SAFE_RELEASE(pRenderer);
				SAFE_RELEASE(pObject);
			}
		}


		//구역 읽기
		{
			int iSize = m_vecArea.size();
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				PAREA pArea = new AREA;
				Vector4 vArea;
				fread(&vArea, sizeof(Vector4), 1, pFile);
				pArea->vRange.x = vArea.x;
				pArea->vRange.y = vArea.y;
				pArea->vRange.z = vArea.z;
				pArea->vRange.w = vArea.w;
				m_vecArea.push_back(pArea);
			}
		}

		//충돌체 읽기
		{
			int iSize = m_vecArea.size();
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				int iLength = 0;
				char cTagBuffer[256] = {};
				fread(&iLength, 4, 1, pFile);
				fread(&cTagBuffer, iLength, 1, pFile);

				Vector4 vArea;
				fread(&vArea, sizeof(Vector4), 1, pFile);
				
				CGameObject* pTileCollider = CGameObject::CreateObject(cTagBuffer, pLayer);

				CRectCollider2D* pCollider = pTileCollider->AddComponent<CRectCollider2D>("Tile Collider");
				pCollider->SetScale(vArea.z, vArea.w, 1.0f);
				pCollider->SetPivot(0.0f, 0.0f, 0.0f);
				SAFE_RELEASE(pCollider);

				CTransform* pColliderTransform = pTileCollider->GetTransform();
				pColliderTransform->SetWorldScale(vArea.z,vArea.w, 0.0f);
				pColliderTransform->SetWorldPosition(vArea.x,	 vArea.y, 0.0f);
				SAFE_RELEASE(pColliderTransform);				
				
				SAFE_RELEASE(pTileCollider);
			}
		}


		//Danger
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				DIRECTION eDirection;
				fread(&eDirection, sizeof(DIRECTION), 1, pFile);
				CreateSpike(pFile, eDirection, pLayer);
			}
		}

		//Jumpthru
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				CGameObject* pJumpthru = CGameObject::CreateObject("Jumpthru", pLayer);
				CTransform* pTransform = pJumpthru->GetTransform();
				pTransform->Load(pFile);
				pTransform->SetPivot(0.0f, 0.0f, 0.0f);
				SAFE_RELEASE(pTransform);

				int iLength = 0;
				char cBuffer[256] = {};
				fread(&iLength, 4, 1, pFile);
				fread(cBuffer, 1, iLength, pFile);
				string strAtlasKey(cBuffer);
				PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData(strAtlasKey);
				CRenderer* pRenderer = pJumpthru->AddComponent<CRenderer>("Renderer");
				pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
				pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
				pRenderer->SetInputLayout(POS_UV_LAYOUT);
				pRenderer->SetRenderState(ALPHA_BLEND);
				pRenderer->SetRenderMode(RM_2D);

				CMaterial*	pMaterial = pRenderer->CreateMaterial();
				pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
				pMaterial->SetSampler(SAMPLER_POINT);
				SAFE_RELEASE(pMaterial);
				pRenderer->SetImageInfo(strAtlasKey);
				SAFE_RELEASE(pRenderer);

				CRectCollider2D* pCollider = pJumpthru->AddComponent<CRectCollider2D>("Jumpthru");
				pCollider->SetScale(pData->wWidth * CELESTE_PIXEL, pData->wHeight * CELESTE_PIXEL, 1.0f);
				SAFE_RELEASE(pCollider);
				SAFE_RELEASE(pJumpthru);
			}
		}


		//Prefab
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				//파일 명 읽음
				int iLength = 0;
				fread(&iLength, 4, 1, pFile);
				char cBuffer[512] = {};
				fread(cBuffer, iLength, 1, pFile);

				//레이어 읽음
				char cLayerBuffer[512] = {};
				fread(&iLength, 4, 1, pFile);
				fread(cLayerBuffer, iLength, 1, pFile);
				//CLayer* pTempLayer = pScene->FindLayer(cLayerBuffer);
				CLayer* pTempLayer = pScene->FindLayer("Object");

				//Ref 안오름
				CGameObject* pPrefab = LoadPrefab(cBuffer, pScene, pTempLayer);
				SAFE_RELEASE(pTempLayer);

				CTransform* pTransform = pPrefab->GetTransform();
				pTransform->Load(pFile);
				SAFE_RELEASE(pTransform);

				//파일 명에 따라 추가 컴포넌트 달아줌
				if (string(cBuffer) == "PrologueFallBlockObject")
				{
					CFallBlock* pFallBlock = pPrefab->AddComponent<CFallBlock>("FallBlock");
					SAFE_RELEASE(pFallBlock);
				}
				else if (string(cBuffer).find("Stage1_FallBlock") != string::npos)
				{
					CFallBlock* pFallBlock = pPrefab->AddComponent<CFallBlock>("FallBlock");					
					SAFE_RELEASE(pFallBlock);
				}
				else if (string(cBuffer).find("Crumble") != string::npos)
				{
					CCrumble* pCrumble = pPrefab->AddComponent<CCrumble>("Crumble");
					SAFE_RELEASE(pCrumble);
				}
			}
		}


		//Spring 로드
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				CGameObject* pSpring = CGameObject::CreateObject("Spring", pLayer);
				CTransform* pSpringTransform = pSpring->GetTransform();
				pSpringTransform->Load(pFile);
				pSpringTransform->SetWorldPosition(pSpringTransform->GetWorldPosition() + Vector3(0.0f, 0.0f, 0.0f));
				pSpringTransform->SetPivot(0.5f, 1.0f, 0.0f);

				CRenderer* pRenderer = pSpring->AddComponent<CRenderer>("Renderer");
				pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
				pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
				pRenderer->SetInputLayout(POS_UV_LAYOUT);
				pRenderer->SetRenderState(ALPHA_BLEND);
				pRenderer->SetRenderMode(RM_2D);

				CMaterial*	pMaterial = pRenderer->CreateMaterial();
				pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
				pMaterial->SetSampler(SAMPLER_POINT);
				SAFE_RELEASE(pMaterial);
				SAFE_RELEASE(pRenderer);

				CAnimator* pSpringAnimator = pSpring->AddComponent<CAnimator>("Animator");
				CAnimationController* pController = new CAnimationController;
				pSpringAnimator->SetAnimationController(pController);
				pSpringAnimator->SetSyncScale(true);
				pSpringAnimator->SetFixedScaleFactor(Vector3(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f));
				CAnimationState* pState = pController->CreateAnimationState("Spring Normal");
				pState->AddClip("Spring Normal", AO_LOOP, 1.0f, "Gameplay", L"Celeste\\Gameplay.png");
				pController->SetEntryState("Spring Normal");
				pState->GetClip()->AddCustomInfo("objects\\spring\\00");

				pState = pController->CreateAnimationState("Spring Active");
				pState->AddClip("Spring Active", AO_STOP_AT_END, 0.5f, "Gameplay", L"Celeste\\Gameplay.png");
				pState->GetClip()->AddCustomInfo("objects\\spring\\00");
				pState->GetClip()->AddCustomInfo("objects\\spring\\01");
				pState->GetClip()->AddCustomInfo("objects\\spring\\02");
				pState->GetClip()->AddCustomInfo("objects\\spring\\03");
				pState->GetClip()->AddCustomInfo("objects\\spring\\04");
				pState->GetClip()->AddCustomInfo("objects\\spring\\05");

				pState->GetClip()->SetAudioPackage("Spring");
				pState->GetClip()->AddAudioClip(1, "Spring", "game_gen_spring", false);

				SAFE_RELEASE(pSpringAnimator);

				SAFE_RELEASE(pRenderer);

				CRectCollider2D* pCollider = pSpring->AddComponent<CRectCollider2D>("Spring");
				pCollider->SetScale(pSpringTransform->GetWorldScale());
				pCollider->SetPivot(0.5f, 1.0f, 0.0f);
				SAFE_RELEASE(pSpringTransform);
				SAFE_RELEASE(pCollider);
				SAFE_RELEASE(pSpring);
			}		
		}

		
		//CheckPoint
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				Vector3 vPosition;
				fread(&vPosition, sizeof(Vector3), 1, pFile);

				int iContainerSize = m_vecArea.size();
				for (int j = 0; j < iContainerSize; ++j)
				{
					if (vPosition.x >= m_vecArea[j]->vRange.x && vPosition.x <= m_vecArea[j]->vRange.x + m_vecArea[j]->vRange.z &&
						vPosition.y >= m_vecArea[j]->vRange.y && vPosition.y <= m_vecArea[j]->vRange.y + m_vecArea[j]->vRange.w)
					{
						m_vecArea[j]->vecCheckPoint.push_back(vPosition);
						break;
					}
				}
			}
		}


		//Refill
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				Vector3 vPosition;
				fread(&vPosition, sizeof(Vector3), 1, pFile);
				CreateRefill(vPosition, pLayer);
			}
		}

		//Berry
		{

			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				BERRY_TYPE eType;
				fread(&eType, sizeof(BERRY_TYPE), 1, pFile);
				Vector3 vPosition;
				fread(&vPosition, sizeof(Vector3), 1, pFile); 				
				CreateStrawberry(vPosition, eType, pLayer);
			}
		}


		fclose(pFile);
	}
	SAFE_RELEASE(pLayer);

	
	return true;
}

void CGameData::CreateDeathEffect(const Vector3& vPosition, CScene * pScene, CLayer * pLayer)
{
	CGameObject* pCenter = CGameObject::CreateObject("DeathCenter", pLayer);
	CTransform* pCenterTransform = pCenter->GetTransform();
	pCenterTransform->SetWorldPosition(vPosition);
	SAFE_RELEASE(pCenterTransform);
	CDeath* pDeath = pCenter->AddComponent<CDeath>("Death");


	for (int i = 0; i < 8; ++i)
	{
		CGameObject* pBlob = CGameObject::CreateObject("DeathBlob", pLayer);
		CTransform* pBlobTransform = pBlob->GetTransform();
		pBlobTransform->SetPivot(0.5f, 0.5f, 0.5f);
		SAFE_RELEASE(pBlobTransform);

		CRenderer* pRenderer = pBlob->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
		pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
		pRenderer->SetInputLayout(POS_UV_LAYOUT);
		pRenderer->SetRenderState(ALPHA_BLEND);
		pRenderer->SetRenderMode(RM_2D);

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
		pMaterial->SetSampler(SAMPLER_POINT);
		SAFE_RELEASE(pMaterial);
		pRenderer->SetImageInfo("particles\\blob");
		SAFE_RELEASE(pRenderer);

		pDeath->AddBlob(pBlob);
		SAFE_RELEASE(pBlob);
	}

	SAFE_RELEASE(pDeath);
	SAFE_RELEASE(pCenter);
}

CGameObject * CGameData::LoadPrefab(const string & strFileName, CScene* pScene, CLayer * pLayer)
{
	CGameObject* pReturn = NULL;
	string strPath = GET_SINGLE(CPathManager)->FindPathMultiByte(PREFAB_PATH);

	strPath += strFileName;
	strPath += string(".object");


	FILE* pFile = NULL;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (pFile)
	{
		CGameObject* pParent = CGameObject::CreateObject(strFileName, pLayer);
		CTransform* pParentTransform = pParent->GetTransform();
		{
			int iCount = 0;
			fread(&iCount, 4, 1, pFile);
			for (int i = 0; i < iCount; ++i)
			{
				CGameObject* pTile = CGameObject::CreateObject("Tile", pLayer);
				CRenderer* pTileRenderer = pTile->AddComponent<CRenderer>("TileRenderer");
				pTileRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
				pTileRenderer->SetShader(STANDARD_TEXTURE_SHADER);
				pTileRenderer->SetInputLayout(POS_UV_LAYOUT);
				pTileRenderer->SetRenderState(ALPHA_BLEND);
				pTileRenderer->SetRenderMode(RM_2D);

				CMaterial*	pMaterial = pTileRenderer->CreateMaterial();
				pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
				pMaterial->SetSampler(SAMPLER_POINT);
				SAFE_RELEASE(pMaterial);
				SAFE_RELEASE(pTileRenderer);

				CTransform* pTileTransform = pTile->GetTransform();
				pTileTransform->Load(pFile);
				pTileTransform->SetParent(pParent);
				pTileTransform->SetLocalPosition(pTileTransform->GetWorldPosition());
				pTileTransform->SetLocalScale(pTileTransform->GetWorldScale());
				pTileTransform->SetLocalRotation(pTileTransform->GetWorldRotation());
				pTileTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
				pTileTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				pTileTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
				SAFE_RELEASE(pTileTransform);

				CTile* pTileComponent = pTile->AddComponent<CTile>("Tile");
				pTileComponent->Load(pFile);
				SAFE_RELEASE(pTileComponent);
				SAFE_RELEASE(pTile);
			}
		}

		{
			//나머지 오브젝트 저장
			int iCount = 0;
			fread(&iCount, 4, 1, pFile);
			for (int i = 0; i < iCount; ++i)
			{
				int iLength = 0;
				char strTagBuffer[256] = {};
				fread(&iLength, 4, 1, pFile);
				fread(strTagBuffer, 1, iLength, pFile);

				//Layer읽음
				iLength = 0;
				char cLayerBuffer[256] = {};
				fread(&iLength, 4, 1, pFile);
				fread(cLayerBuffer, sizeof(char), iLength, pFile);

				//CLayer* pTempLayer = pScene->FindLayer(string(cLayerBuffer));				
				CGameObject* pObject = CGameObject::CreateObject(string(strTagBuffer), pLayer);
				//SAFE_RELEASE(pTempLayer);

				CTransform* pTransform = pObject->GetTransform();
				pTransform->Load(pFile);
				pTransform->SetParent(pParent);
				pTransform->SetLocalPosition(pTransform->GetWorldPosition());
				pTransform->SetLocalScale(pTransform->GetWorldScale());
				pTransform->SetLocalRotation(pTransform->GetWorldRotation());
				pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
				pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
				SAFE_RELEASE(pTransform);

				CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
				pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
				pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
				pRenderer->SetInputLayout(POS_UV_LAYOUT);
				pRenderer->SetRenderState(ALPHA_BLEND);
				pRenderer->SetRenderMode(RM_2D);

				CMaterial*	pMaterial = pRenderer->CreateMaterial();
				pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
				pMaterial->SetSampler(SAMPLER_POINT);
				SAFE_RELEASE(pMaterial);

				char strAtlasKey[256] = {};
				iLength = 0;
				fread(&iLength, 4, 1, pFile);
				fread(strAtlasKey, sizeof(char), iLength, pFile);
				pRenderer->SetImageInfo(string(strAtlasKey));
				SAFE_RELEASE(pRenderer);
				SAFE_RELEASE(pObject);
			}
		}

		//Dummy
		{
			int iCount = 0;
			fread(&iCount, 4, 1, pFile);
		}

		//충돌체 읽기
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				int iLength = 0;
				char cTagBuffer[256] = {};

				fread(&iLength, 4, 1, pFile);
				fread(&cTagBuffer, iLength, 1, pFile);
				Vector4 vArea;
				fread(&vArea, sizeof(Vector4), 1, pFile);

				CRectCollider2D* pCollider = pParent->AddComponent<CRectCollider2D>(cTagBuffer);
				pCollider->SetScale(vArea.z, vArea.w, 1.0f);
				pCollider->SetPivot(vArea.x, vArea.y, 0.0f);
				SAFE_RELEASE(pCollider);
			}
		}

		//가시 읽기
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				DIRECTION eDirection;
				fread(&eDirection, sizeof(DIRECTION), 1, pFile);

				CLayer* pSpikeLayer = pScene->FindLayer("BackTerrain");
				CGameObject* pDanger = CreateSpike(pFile, eDirection, pSpikeLayer);
				SAFE_RELEASE(pSpikeLayer);
				CTransform* pTransform = pDanger->GetTransform();
				//pTransform->Load(pFile);
				pTransform->SetParent(pParent);
				pTransform->SetLocalPosition(pTransform->GetWorldPosition());
				pTransform->SetLocalScale(pTransform->GetWorldScale());
				pTransform->SetLocalRotation(pTransform->GetWorldRotation());
				pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
				pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
				SAFE_RELEASE(pTransform);
			}
		}


		pReturn = pParent;

		SAFE_RELEASE(pParentTransform);
		SAFE_RELEASE(pParent);
		fclose(pFile);
	}
	return pReturn;
}

void CGameData::OpenScene(const Vector3& vPosition, CScene * pScene, CLayer * pLayer)
{
	CGameObject* pCenter = CGameObject::CreateObject("DeathCenter", pLayer);
	CTransform* pCenterTransform = pCenter->GetTransform();
	pCenterTransform->SetWorldPosition(vPosition);
	SAFE_RELEASE(pCenterTransform);

	CRenderer* pRenderer = pCenter->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("SpawnCircle", L"Celeste\\SpawnCircle.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CSceneEffect* pVFX = pCenter->AddComponent<CSceneEffect>("Open");
	pVFX->SetOpen(true);
	SAFE_RELEASE(pVFX);
	SAFE_RELEASE(pCenter);
}

void CGameData::OpenScene(CScene * pScene, CLayer * pLayer)
{
	CTransform* pPlayerTransform = m_pPlayer->GetTransform();
	OpenScene(pPlayerTransform->GetWorldPosition(), pScene, pLayer);
	SAFE_RELEASE(pPlayerTransform);
}

void CGameData::OpenScene()
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->FindLayer("UI");
	OpenScene(pScene, pLayer);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}

void CGameData::CloseScene(const Vector3& vPosition, CScene * pScene, CLayer * pLayer, SCENE_AFTER_EFFECT eAfterEffect)
{
	CGameObject* pCenter = CGameObject::CreateObject("DeathCenter", pLayer);
	CTransform* pCenterTransform = pCenter->GetTransform();
	pCenterTransform->SetWorldPosition(vPosition);
	SAFE_RELEASE(pCenterTransform);

	CRenderer* pRenderer = pCenter->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("SpawnCircle", L"Celeste\\SpawnCircle.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CSceneEffect* pVFX = pCenter->AddComponent<CSceneEffect>("Open");
	pVFX->SetOpen(false);
	pVFX->SetAfterEffect(eAfterEffect);
	SAFE_RELEASE(pVFX);
	SAFE_RELEASE(pCenter);
}
