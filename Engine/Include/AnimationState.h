#pragma once
#include "Ref.h"
#include "AnimationController.h"

LMNT_BEGIN


class LMNT_DLL CAnimationState
{
private:
	friend class CAnimationController;

private:
	CAnimationState();
	~CAnimationState();

private:
	class CAnimationController* m_pController;
	class CAnimator* m_pAnimator;
	string m_strStateName;

	//여기에서 클립 돌려줌
	class CAnimationClip2D*	m_pAnimationClip;
	vector<class CTransition*> m_vecTransition;
	class CTransition* m_pTransition;
	bool m_bReserveTransition;
	bool m_bEnd;

public:
	bool End() const;
	class CAnimationClip2D* GetClip() const;

	bool AddClip(const string& strClipName, ANIMATION_OPTION eOption, float fFrameTime,
		const string& strTextureKey, const wchar_t* pFileName = NULL, const string& strPathKey = TEXTURE_PATH);


	bool AddClip(const string& strClipName, ANIMATION_OPTION eOption,
		int iFrameMaxX, int iFrameMaxY, int iStartX, int iStartY,
		int iLengthX, int iLengthY, float fFrameTime,
		const string& strTexKey, const wchar_t* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);

	bool AddClip(const string& strClipName, ANIMATION_OPTION eOption,
		int iFrameMax, float fFrameTime,
		const string& strTexKey, vector<wstring>* vecFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void AddTransition(class CTransition* pTransition);
	void ResetState();
	const string& GetStateName() const;

	void UpdateCondition(float fTime);


};

LMNT_END