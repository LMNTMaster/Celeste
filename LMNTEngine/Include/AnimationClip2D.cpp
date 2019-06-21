#include "AnimationClip2D.h"
#include "Texture.h"
#include "ResourcesManager.h"
#include "Timer.h"
#include "Component\Animator.h"
#include "AnimationController.h"
#include "AnimationState.h"
#include "GameObject.h"
#include "Transform.h"
#include "SoundManager.h"

LMNT_USING

CAnimationClip2D::CAnimationClip2D():
	m_pAnimator(NULL),
	m_pAnimationController(NULL),
	m_pAnimationState(NULL),
	m_bPause(false)
{
	m_eType = AT_ATLAS;
	m_eOption = AO_LOOP;
	m_pTexture = NULL;
	m_fAnimationTime = 0.f;
	m_fAnimationFrameTime = 1.f;
	m_fAnimationProgressTime = 0.f;
	m_fAnimationProgress = 0.f;
	m_iFrameX = 0;
	m_iFrameY = 0;
	m_iStartX = 0;
	m_iStartY = 0;
	m_iFrameLengthX = 0;
	m_iFrameLengthY = 0;
	m_iFrameMaxX = 0;
	m_iFrameMaxY = 0;
	m_iTextureRegister = 0;
}

CAnimationClip2D::CAnimationClip2D(const CAnimationClip2D & clip)
{
	*this = clip;

	m_tCBuffer = clip.m_tCBuffer;
}

CAnimationClip2D::~CAnimationClip2D()
{
	SAFE_RELEASE(m_pTexture);
	
	int iSize = m_vecAudioClip.size();
	for (int i = 0; i < iSize; ++i)
	{
		delete m_vecAudioClip[i];
	}
	m_vecAudioClip.clear();
}

CAnimationClip2D * CAnimationClip2D::Clone() const
{
	return new CAnimationClip2D(*this);
}

void CAnimationClip2D::SyncScale(const Vector3& vTargetScale)
{
	CGameObject* pGameObject = m_pAnimator->GetGameObject();
	CTransform* pTransform = pGameObject->GetTransform();

	pTransform->SetWorldScale(vTargetScale * m_pAnimator->GetFixedScaleFactor() * m_pAnimator->GetLocalScaleFactor());

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pGameObject);
}

void CAnimationClip2D::PlayAudioClip(int iFrame) const
{
	vector<PAUDIOCLIP> vecRandomTarget;

	int iSize = m_vecAudioClip.size();
	for (int i = 0; i < iSize; ++i)
	{
		if (m_vecAudioClip[i]->strAudioPackage == m_strAudioPackage)
		{
			if (m_vecAudioClip[i]->iFrame == iFrame)
			{
				if (!m_vecAudioClip[i]->bRandom)
				{
					GET_SINGLE(CSoundManager)->Play(m_vecAudioClip[i]->strKey);
				}
				else
				{
					vecRandomTarget.push_back(m_vecAudioClip[i]);
				}
			}
		}
	}

	if (vecRandomTarget.size() > 0)
	{
		int iRandom = rand() % vecRandomTarget.size();
		GET_SINGLE(CSoundManager)->Play(vecRandomTarget[iRandom]->strKey);
	}
}

float CAnimationClip2D::GetProgress() const
{
	return m_fAnimationProgress;
}

void CAnimationClip2D::Pause()
{
	m_bPause = true;
}

void CAnimationClip2D::Play()
{
	m_bPause = false;
}

void CAnimationClip2D::SetAudioPackage(const string & strPackage)
{
	m_strAudioPackage = strPackage;
}

void CAnimationClip2D::AddAudioClip(int iFrame, const string& strAudioPackage, const string & strKey, bool bRandom)
{
	PAUDIOCLIP pAudioClip = new AUDIOCLIP;
	pAudioClip->iFrame = iFrame;
	pAudioClip->strAudioPackage = strAudioPackage;
	pAudioClip->strKey = strKey;
	pAudioClip->bRandom = bRandom;

	m_vecAudioClip.push_back(pAudioClip);
}

void CAnimationClip2D::AddFunction(function<void(void)> Function)
{
	m_vecFunction.push_back(Function);
}

void CAnimationClip2D::AddFunction()
{
	auto Func = [&]() {};
	m_vecFunction.push_back(Func);
}

const string & CAnimationClip2D::GetAtlasKey(int iFrame)
{
	return m_vecAtlasKey[iFrame];
}

const string & CAnimationClip2D::GetClipName() const
{
	return m_strName;
}

unsigned int CAnimationClip2D::GetCurrentFrameCount() const
{
	return m_iFrameY * (m_iFrameMaxX * m_iFrameMaxY) + m_iFrameX;
}

unsigned int CAnimationClip2D::GetCustomFrameCount() const
{
	return m_vecCustomInfo.size();
}

PANIMATION2DCBUFFER CAnimationClip2D::GetAnimation2DCBuffer()
{
	return &m_tCBuffer;
}

void CAnimationClip2D::SetClipInfo(const string & strClipName, ANIMATION_OPTION eOption, float fFrameTime,
	const string & strKey, const wchar_t * pFileName, const string & strPathKey)
{
	m_strName = strClipName;
	m_eType = AT_CUSTOM;
	m_iTextureRegister = 0;
	m_eOption = eOption;
	SAFE_RELEASE(m_pTexture);

	if (pFileName)
	{
		m_pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
	}
	else
	{
		m_pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
	}
	m_fAnimationTime = 0.f;
	m_fAnimationFrameTime = fFrameTime;
	m_fAnimationProgress = 0.f;
}

void CAnimationClip2D::SetClipInfo(const string & strClipName, ANIMATION_OPTION eOption,
	int iFrameMaxX, int iFrameMaxY, int iStartX, int iStartY, int iLengthX, int iLengthY, float fFrameTime,
	const string & strKey, const wchar_t * pFileName, const string & strPathKey)
{
	m_strName = strClipName;
	m_eType = AT_ATLAS;
	m_iTextureRegister = 0;
	m_eOption = eOption;

	SAFE_RELEASE(m_pTexture);
	if (pFileName)
	{
		m_pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
	}

	else
		m_pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strKey);

	m_fAnimationTime = 0.f;
	m_fAnimationFrameTime = fFrameTime;
	m_fAnimationProgress = 0.f;
	m_iFrameX = 0;
	m_iFrameY = 0;
	m_iStartX = iStartX;
	m_iStartY = iStartY;
	m_iFrameLengthX = iLengthX;
	m_iFrameLengthY = iLengthY;
	m_iFrameMaxX = iFrameMaxX;
	m_iFrameMaxY = iFrameMaxY;

	m_fAnimationProgressTime = fFrameTime / (iLengthX * iLengthY);

	m_tCBuffer.iType = m_eType;
	m_tCBuffer.iFrameX = m_iFrameX;
	m_tCBuffer.iFrameY = m_iFrameY;
	m_tCBuffer.iStartX = m_iStartX;
	m_tCBuffer.iStartY = m_iStartY;
	m_tCBuffer.iLengthX = m_iFrameLengthX;
	m_tCBuffer.iLengthY = m_iFrameLengthY;
	m_tCBuffer.iMaxX = m_iFrameMaxX;
	m_tCBuffer.iMaxY = m_iFrameMaxY; 
	
}

void CAnimationClip2D::SetClipInfo(const string & strClipName, ANIMATION_OPTION eOption,
	int iFrameMax, float fFrameTime, const string & strKey, const vector<wstring>* vecFileName, const string & strPathKey)
{
	m_strName = strClipName;
	m_eType = AT_FRAME;
	m_iTextureRegister = 9;
	m_eOption = eOption;
	SAFE_RELEASE(m_pTexture);

	if (vecFileName)
		m_pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, *vecFileName, strPathKey);

	else
		m_pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strKey);

	m_fAnimationTime = 0.f;
	m_fAnimationFrameTime = fFrameTime;
	m_fAnimationProgress = 0.f;
	m_iFrameX = 0;
	m_iFrameY = 0;
	m_iStartX = 0;
	m_iStartY = 0;
	m_iFrameLengthX = iFrameMax;
	m_iFrameLengthY = 1;
	m_iFrameMaxX = iFrameMax;
	m_iFrameMaxY = 1;

	m_fAnimationProgressTime = fFrameTime / iFrameMax;

	m_tCBuffer.iType = m_eType;
	m_tCBuffer.iFrameX = m_iFrameX;
	m_tCBuffer.iFrameY = m_iFrameY;
	m_tCBuffer.iStartX = m_iStartX;
	m_tCBuffer.iStartY = m_iStartY;
	m_tCBuffer.iLengthX = m_iFrameLengthX;
	m_tCBuffer.iLengthY = m_iFrameLengthY;
	m_tCBuffer.iMaxX = m_iFrameMaxX;
	m_tCBuffer.iMaxY = m_iFrameMaxY;
}


void CAnimationClip2D::ResetClip()
{
	m_fAnimationTime = 0.f;
	m_iFrameX = 0;
	m_iFrameY = 0;
	m_bPause = false;
}

void CAnimationClip2D::AddCustomInfo(int iStartX, int iStartY, int iLengthX, int iLengthY, int iMaxSizeX, int iMaxSizeY)
{
	CUSTOMANIMATIONINFO	tInfo = {};
	tInfo.iStartX = iStartX;
	tInfo.iStartY = iStartY;
	tInfo.iSizeX = iLengthX;
	tInfo.iSizeY = iLengthY;
	tInfo.iMaxSizeX = iMaxSizeX;
	tInfo.iMaxSizeY = iMaxSizeY;
	
	m_vecCustomInfo.push_back(tInfo);

	m_iFrameLengthX = m_vecCustomInfo.size();
	m_fAnimationProgressTime = m_fAnimationFrameTime / (float) m_vecCustomInfo.size();

	AddFunction();
}

void CAnimationClip2D::AddCustomInfo(ATLASDATA tData)
{
	AddCustomInfo(tData.wX, tData.wY, tData.wWidth, tData.wHeight, tData.iAtlasSizeX, tData.iAtlasSizeY);
	AddFunction();
}

void CAnimationClip2D::AddCustomInfo(const string & strAtlasKey)
{
	PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData(strAtlasKey);

	if (!pData)
		return;

	else
	{
		m_vecAtlasKey.push_back(strAtlasKey);
		AddCustomInfo(pData->wX, pData->wY, pData->wWidth, pData->wHeight, pData->iAtlasSizeX, pData->iAtlasSizeY);
		AddFunction();
	}
}

void CAnimationClip2D::AddCustomInfo(int iStartX, int iStartY, int iLengthX, int iLengthY, int iMaxSizeX, int iMaxSizeY, function<void(void)> func)
{
	AddCustomInfo(iStartX, iStartY, iLengthX, iLengthY, iMaxSizeX, iMaxSizeY);
	AddFunction(func);
}

void CAnimationClip2D::AddCustomInfo(ATLASDATA tData, function<void(void)> func)
{
	AddCustomInfo(tData);
	AddFunction(func);
}

void CAnimationClip2D::AddCustomInfo(const string & strAtlasKey, function<void(void)> func)
{
	AddCustomInfo(strAtlasKey);
	AddFunction(func);
}

bool CAnimationClip2D::Init()
{
	return true;
}

bool CAnimationClip2D::Update(float fTime)
{
	bool	bEnd = false;

	if (m_pAnimator->GetSyncScale() && m_eType == AT_CUSTOM)
	{
		SyncScale(Vector3(m_vecCustomInfo[m_iFrameX].iSizeX, m_vecCustomInfo[m_iFrameX].iSizeY, 1.0f));
	}


	if (!m_bPause)
	{
		m_fAnimationTime += fTime;

		while (m_fAnimationTime >= m_fAnimationProgressTime)
		{
			m_fAnimationTime -= m_fAnimationProgressTime;


			++m_iFrameX;

			//AudioClip 
			if (m_eType == AT_CUSTOM)
			{
				PlayAudioClip(m_iFrameX);
			}
			else
			{
				PlayAudioClip(m_iFrameY * m_iFrameLengthX + m_iFrameX);
			}


			if (m_eOption == AO_STOP_AT_END)
			{
				if (m_iFrameX >= m_iFrameLengthX)
				{
					m_iFrameX = m_iFrameLengthX - 1;
					Pause();
				}
			}
			
			if (m_iFrameX >= m_iFrameLengthX)
			{
				m_iFrameX = 0;
				++m_iFrameY;

				if (m_eType == AT_CUSTOM)
				{
					PlayAudioClip(m_iFrameX);
				}
				else
				{
					PlayAudioClip(m_iFrameY * m_iFrameLengthX + m_iFrameX);
				}

				if (m_iFrameY >= m_iFrameLengthY)
				{
					m_iFrameY = 0;
					bEnd = true;
				
				}
			}
			// 애니메이션 진행률을 구해준다.
			if (m_eType == AT_CUSTOM)
			{
				//m_vecFunction[m_iFrameX + 1]();
				m_fAnimationProgress = (float)(m_iFrameX + 1) / (float)m_vecCustomInfo.size();
			}
			else
			{
				m_vecFunction[m_iFrameY * m_iFrameLengthX + m_iFrameX]();
				m_fAnimationProgress = (float)(m_iFrameY * m_iFrameLengthX + m_iFrameX) / (float)(m_iFrameLengthX * m_iFrameLengthY);
			}
			//m_fAnimationProgress = m_fAnimationTime / m_fAnimationFrameTime;
		}
	}

	
	

	m_tCBuffer.iType = m_eType;
	m_tCBuffer.iFrameX = m_iFrameX;
	m_tCBuffer.iFrameY = m_iFrameY;
	m_tCBuffer.iStartX = m_iStartX;
	m_tCBuffer.iStartY = m_iStartY;
	m_tCBuffer.iLengthX = m_iFrameLengthX;
	m_tCBuffer.iLengthY = m_iFrameLengthY;
	m_tCBuffer.iMaxX = m_iFrameMaxX;
	m_tCBuffer.iMaxY = m_iFrameMaxY;


	if (m_tCBuffer.iType == AT_CUSTOM )
	{
		m_tCBuffer.iFrameX = m_vecCustomInfo[m_iFrameX].iStartX;
		m_tCBuffer.iFrameY = m_vecCustomInfo[m_iFrameX].iStartY;
		m_tCBuffer.iLengthX = m_vecCustomInfo[m_iFrameX].iSizeX;
		m_tCBuffer.iLengthY = m_vecCustomInfo[m_iFrameX].iSizeY;
		m_tCBuffer.iMaxX = m_vecCustomInfo[m_iFrameX].iMaxSizeX;
		m_tCBuffer.iMaxY = m_vecCustomInfo[m_iFrameX].iMaxSizeY;
	}

	if (bEnd)
	{
		if (m_eOption == AO_STOP_AT_END)
		{
			m_iFrameX = m_iFrameLengthX;
			Pause();
		}

		if (m_eOption == AO_LOOP)
		{
			ResetClip();
		}
		else if (m_eOption == AO_ONCE_DESTROY)
		{
			ResetClip();
			m_pAnimator->GetGameObject()->Destroy(true);
		}
	}

	return bEnd;
}