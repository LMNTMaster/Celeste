#pragma once
#include "Ref.h"

LMNT_BEGIN

typedef struct _tagAudioClip
{
	int iFrame;
	bool bRandom;
	string strAudioPackage;
	string strKey;
}AUDIOCLIP, *PAUDIOCLIP;

class LMNT_DLL CAnimationClip2D
{
private:
	friend class CAnimationState;
	friend class CAnimator;

private:
	CAnimationClip2D();
	CAnimationClip2D(const CAnimationClip2D& clip);
	~CAnimationClip2D();

	//Transition과의 연계를 위해 일부 항목(ANIMATION_OPTION) 조정 필요함.
	//Clip은 State를 컨트롤 하는 부분에 있어서는 능동적으로 처리하면 안된다.
	//Clip은 AnimationState에 의해 컨트롤 되어야 하기 때문에, Clip은 단순하게 애니메이션을 돌리는 부분만 하도록 하며, 
	//추후, AnimationState에 StateMachine을 컴포짓 패턴의 형태로 추가하여 사용할 수 있도록 한다.

private:
	class CAnimationState* m_pAnimationState;
	class CAnimationController* m_pAnimationController;
	class CAnimator* m_pAnimator;

	string				m_strName;
	ANIMATION_TYPE		m_eType;
	ANIMATION_OPTION	m_eOption;
	class CTexture*		m_pTexture;
	float				m_fAnimationTime;
	float				m_fAnimationFrameTime;
	float				m_fAnimationProgressTime;
	float				m_fAnimationProgress;
	int					m_iFrameX;
	int					m_iFrameY;
	int					m_iStartX;
	int					m_iStartY;
	int					m_iFrameLengthX;
	int					m_iFrameLengthY;
	int					m_iFrameMaxX;
	int					m_iFrameMaxY;
	int					m_iTextureRegister;
	ANIMATION2DCBUFFER	m_tCBuffer;
	vector<CUSTOMANIMATIONINFO>	m_vecCustomInfo;
	vector<string> m_vecAtlasKey;

	vector<function<void(void)>> m_vecFunction;
	string m_strAudioPackage;
	vector<PAUDIOCLIP> m_vecAudioClip;
	bool m_bPause;

private:
	void SyncScale(const Vector3& vTargetScale);
	void PlayAudioClip(int iFrame) const;

public:
	float GetProgress() const;
	void Pause();
	void Play();
	void SetAudioPackage(const string& strPackage);
	void AddAudioClip(int iFrame, const string& strAudioPackage, const string& strKey, bool bRandom);	

	void AddFunction(function<void(void)> func);
	void AddFunction();

	const string& GetAtlasKey(int iFrame);
	const string& GetClipName() const;
	unsigned int GetCurrentFrameCount() const;
	unsigned int GetCustomFrameCount() const;
	PANIMATION2DCBUFFER GetAnimation2DCBuffer();
	void SetClipInfo(const string& strClipName, ANIMATION_OPTION eOption, float fFrameTime,
		const string& strKey, const wchar_t* pFileName = NULL, const string& strPathKey = TEXTURE_PATH);

	void SetClipInfo(const string& strClipName,
		ANIMATION_OPTION eOption,
		int iFrameMaxX, int iFrameMaxY, int iStartX, int iStartY,
		int iLengthX, int iLengthY, float fFrameTime,
		const string& strKey, const wchar_t* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void SetClipInfo(const string& strClipName, ANIMATION_OPTION eOption,
		int iFrameMax, float fFrameTime,
		const string& strKey, const vector<wstring>* vecFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);

	void ResetClip();
	//Atlas용
	void AddCustomInfo(int iStartX, int iStartY, int iLengthX, int iLengthY, int iMaxSizeX, int iMaxSizeY);
	void AddCustomInfo(ATLASDATA tData);
	void AddCustomInfo(const string& strAtlasKey);
	//Function
	void AddCustomInfo(int iStartX, int iStartY, int iLengthX, int iLengthY, int iMaxSizeX, int iMaxSizeY, function<void(void)> func);
	void AddCustomInfo(ATLASDATA tData, function<void(void)> func);
	void AddCustomInfo(const string& strAtlasKey, function<void(void)> func);

private:
	bool Init();
	bool Update(float fTime);
	CAnimationClip2D* Clone()	const;

};


LMNT_END
