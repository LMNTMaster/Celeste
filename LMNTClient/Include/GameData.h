#pragma once
#include "Client.h"
#include "Scene.h"

LMNT_USING

//Client에서 Engine의 컨테이너에 추가하기 위한 중계용 싱글턴 클래스
class CGameData
{
	DECLARE_SINGLE(CGameData)

private:
	class CCeleste* m_pGameManager;
	vector<PAREA> m_vecArea;
	class CGameObject* m_pPlayer;
	PAREA m_tCurrentArea;
	PAREA m_tPrevArea;

public:
	void SetPlayer(class CGameObject* pPlayer);

public:
	void AddObjectIntoAreaData(class CGameObject* pGameObject);
	PAREA GetAreaData(const Vector3& vPosition);
	vector<PAREA>* GetAreaData();
	void SetGameManager(class CCeleste* pCeleste);
	class CCeleste* GetGameManager() const;


private:
	void CreateSpike(FILE* pFile, DIRECTION eDirection,  class CLayer* pLayer) const;
	class CGameObject* CreateSpike(FILE* pFile, DIRECTION eDirection, class CLayer* pLayer);



public:
	void CreateFootDust(const Vector3& vPosition);
	void CreateDistortionEffect(const Vector3& vPosition, float fTime, float fScale);
	void CreateStrawberry(const Vector3& position, BERRY_TYPE eType, class CLayer* pLayer);
	void CreateRefill(const Vector3& position, class CLayer* pLayer);
	class CGameObject* CreateParticle(class CLayer* pLayer, const Vector3 vSpawnPosition, Vector3 vTargetPosition, const Vector4 vColor, bool bUseFadeout, bool bUseSpeedReduce, float fFrequency, float fAmplitude, float fSpeed);
	void ResetArea(PAREA pArea); // 미구현	
	void ResetArea(const Vector3& pPosition); 
	void ResetArea();
	PAREA GetCurrentArea() const;
	PAREA GetPrevArea() const ;
	bool UpdateAreaData();
	void SetPrevArea();
	Vector3 GetCheckPointPosition(PAREA pArea, const Vector3& vTargetPosition);
	void ShakeCamera(float fDuration, float fAmplitude);
	class CGameObject* GetPlayer() const;
	bool LoadData(const string& strPath);
	bool LoadData();
	bool LoadStage(const wchar_t* pFileName, const string& strPathKey, CScene* pScene);
	void CreateDeathEffect(const Vector3& vPosition, class CScene* pScene, class CLayer* pLayer);
	class CGameObject* LoadPrefab(const string& strFileName, class CScene* pScene, class CLayer* pLayer);
	void OpenScene(const Vector3& vPosition, CScene * pScene, CLayer * pLayer);
	void OpenScene(CScene * pScene, CLayer * pLayer);
	void OpenScene();
	void CloseScene(const Vector3& vPosition, CScene * pScene, CLayer * pLayer , SCENE_AFTER_EFFECT eAfterEffect);
};

