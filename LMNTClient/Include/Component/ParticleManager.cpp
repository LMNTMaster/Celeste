#include "ParticleManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "ParticleMove.h"
#include "Scene.h"
#include "Component/Camera.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "Device.h"
#include "../Client.h"


CParticleManager::CParticleManager() :
	m_fTimer(0),
	m_fSpawnInterval(0.5f),
	m_iSpawnCount(3),
	m_bUseSnow(false),
	m_iParticleCount(0),
	m_iLimitCount(200)

{
	m_eComponentType = CT_PARTICLE_MANAGER;
	SetTag("Particle Manager");
}


CParticleManager::~CParticleManager()
{
}

void CParticleManager::SetSpawnLimit(int iCount)
{
	m_iLimitCount = iCount;
}

void CParticleManager::CreateParticle(class CLayer* pTargetLayer)
{
	CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	Vector3 vScreenSize = Vector3(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight, 0);
	SAFE_RELEASE(pCameraTransform);

	float fSpawnMinPositionX = vScreenSize.x + vCameraPosition.x;
	float fSpawnMinPositionY = vCameraPosition.y;
	float fSpawnMaxPositionX = vScreenSize.x + vCameraPosition.x;
	float fSpawnMaxPositionY = vScreenSize.y + vCameraPosition.y;
	float fTargetPositionX = - 200.0f;
	float fTargetPositionY = RandomRange(fSpawnMinPositionY, fSpawnMaxPositionY);
	float fTransparency = RandomRange(0.35f, 0.75f);
	float fPendulateDistance = RandomRange(30.0f, 100.0f);
	float fPendulateTime = RandomRange(1.5f, 3.0f);
	float fMoveSpeed = RandomRange(5.0f, 35.0f) * 30.0f;

	Vector3 vTargetPosition = Vector3(fTargetPositionX, fTargetPositionY, 0);
	Vector3 position = Vector3(RandomRange(fSpawnMinPositionX, fSpawnMaxPositionX), RandomRange(fSpawnMinPositionY, fSpawnMaxPositionY), 0);

	CGameObject* pSnow = CGameObject::CreateObject("Snow", pTargetLayer);
	CTransform* pTransform = pSnow->GetTransform();
	pTransform->SetWorldScale(4, 4, 1.0f);
	pTransform->SetWorldPosition(position);
	SAFE_RELEASE(pTransform);

	CParticleMove* pMove = pSnow->AddComponent<CParticleMove>("Move");
	pMove->SetAmplitude(fPendulateDistance);
	pMove->SetFrequency(fPendulateTime);
	pMove->SetMoveSpeed(fMoveSpeed);
	pMove->SetTargetPosition(vTargetPosition);
	pMove->SetManager(this);
	pMove->UseSnow();
	SAFE_RELEASE(pMove);

	CRenderer* pRenderer = pSnow->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);	
	pRenderer->SetImageInfo("util\\particle");
	pRenderer->SetDiffuse(GetRandomColor());
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pSnow);
}

void CParticleManager::CreateSnow()
{
	CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	Vector3 vScreenSize = Vector3(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight, 0);
	SAFE_RELEASE(pCameraTransform);


	float fSpawnMinPositionX = vScreenSize.x + vCameraPosition.x;
	float fSpawnMinPositionY = vCameraPosition.y;
	float fSpawnMaxPositionX = vScreenSize.x + vCameraPosition.x;
	float fSpawnMaxPositionY = vScreenSize.y + vCameraPosition.y;

	//float fSpawnMinPositionX = vCameraPosition.x + vScreenSize.x;
	//float fSpawnMinPositionY = vCameraPosition.y - vScreenSize.y / 2.0f;
	//float fSpawnMaxPositionX = fSpawnMinPositionX + vScreenSize.x / 2.0f;
	//float fSpawnMaxPositionY = fSpawnMinPositionY + vScreenSize.y * 2.0f;
	float fTargetPositionX = vCameraPosition.x - 200.0f;
	float fTargetPositionY = RandomRange(fSpawnMinPositionY, fSpawnMaxPositionY);
	float fScale = RandomRange(5.0f, 35.0f);
	float fTransparency = RandomRange(0.35f, 0.75f);
	float fPendulateDistance = RandomRange(30.0f, 100.0f);
	float fPendulateTime = RandomRange(1.5f, 3.0f);
	float fMoveSpeed = fScale * 20.0f;

	Vector3 vTargetPosition = Vector3(fTargetPositionX, fTargetPositionY, 0);
	Vector3 position = Vector3(RandomRange(fSpawnMinPositionX, fSpawnMaxPositionX), RandomRange(fSpawnMinPositionY, fSpawnMaxPositionY), 0);
	
	CGameObject* pSnow = CGameObject::CreateObject("Snow", m_pLayer);
	CTransform* pTransform = pSnow->GetTransform();
	pTransform->SetWorldScale(fScale, fScale, 1.0f);
	pTransform->SetWorldPosition(position);
	SAFE_RELEASE(pTransform);

	CParticleMove* pMove = pSnow->AddComponent<CParticleMove>("Move");
	pMove->SetAmplitude(fPendulateDistance);
	pMove->SetFrequency(fPendulateTime);
	pMove->SetMoveSpeed(fMoveSpeed);
	pMove->SetTargetPosition(vTargetPosition);
	pMove->SetManager(this);
	pMove->UseSnow();
	SAFE_RELEASE(pMove);

	CRenderer* pRenderer = pSnow->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND); 
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Snow", L"Celeste/Snow.png");
	pMaterial->SetSampler(SAMPLER_LINEAR);

	pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, fTransparency));
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pSnow);
}

Vector4 CParticleManager::GetRandomColor()
{
	int idx = rand() % m_vecTargetColor.size();
	return m_vecTargetColor[idx];
}

void CParticleManager::ReduceCount()
{
	--m_iParticleCount;
}

void CParticleManager::UseSnow()
{
	m_bUseSnow = true;
}

void CParticleManager::AddTargetLayer(CLayer* pLayer)
{
	m_vecTargetLayer.push_back(pLayer);
}

void CParticleManager::AddTargetColor(const Vector4& vColor)
{
	m_vecTargetColor.push_back(vColor);
}

void CParticleManager::SetSpawnCount(int iCount)
{
	m_iSpawnCount = iCount;
}

void CParticleManager::SetSpawnInterval(float fInterval)
{
	m_fSpawnInterval = fInterval;
}

bool CParticleManager::Initialize()
{














	return true;
}

int CParticleManager::Update(float fTime)
{
	m_fTimer += fTime;

	if (m_fTimer >= m_fSpawnInterval)
	{
		m_fTimer = 0;

		for (int i = 0; i < m_iSpawnCount; ++i)
		{
			if (m_bUseSnow)
			{
				if (m_iLimitCount > m_iParticleCount)
				{
					CreateSnow();
					++m_iParticleCount;
				}
			}
			else
			{
				size_t iCount = m_vecTargetLayer.size();
				for (size_t i = 0; i < iCount; ++i)
				{
					if (m_iLimitCount > m_iParticleCount)
					{
						CreateParticle(m_vecTargetLayer[i]);		
						++m_iParticleCount;
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	return 0;
}
