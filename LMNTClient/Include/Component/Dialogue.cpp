#include "Dialogue.h"
#include "GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "Component/Camera.h"
#include "Device.h"
#include "Scene.h"
#include "Timer.h"
#include "TimerManager.h"
#include "SoundManager.h"


CDialogue::CDialogue():
	m_pBackPanel(NULL),
	m_pPortraitPanel(NULL)
{
	m_fTextTimer = 0.0f;
	m_pPortraitController = NULL;
	m_fTextInterval = 0.05f;
	m_bEnd = true;
}


CDialogue::~CDialogue()
{
	SAFE_RELEASE(m_pPortraitPanel);
	SAFE_RELEASE(m_pBackPanel);
}

DIALOGUE_STATE CDialogue::GetState() const
{
	return m_eState;
}

bool CDialogue::End() const
{
	return m_bEnd;
}

void CDialogue::SetReserveAnimation(const string & strAnimationKey)
{
	m_strAnimationKey = strAnimationKey;
}

void CDialogue::SetBackground(PORTRAIT_CHARACTER eCharacter)
{
	if (m_pBackPanel)
	{
		CRenderer* pRenderer = m_pBackPanel->GetComponentFromType<CRenderer>(CT_RENDERER);
		switch (eCharacter)
		{
		case PC_MADELINE:
			pRenderer->SetDiffuseTexture("DialogueBoxMadeline", L"Celeste\\Portraits\\textbox\\madeline.png");
			break;
		case PC_GRANNY:
			pRenderer->SetDiffuseTexture("DialogueBoxGranny", L"Celeste\\Portraits\\textbox\\granny.png");
			break;
		case PC_THEO:
			pRenderer->SetDiffuseTexture("DialogueBoxTheo", L"Celeste\\Portraits\\textbox\\theo.png");
			break;
		default:
			pRenderer->SetDiffuseTexture("DialogueBoxMadeline", L"Celeste\\Portraits\\textbox\\madeline.png");
			break;
		}
		SAFE_RELEASE(pRenderer);
	}
}

void CDialogue::SetString(const wstring & strText)
{
	m_fTextTimer = 0.0f;
	m_vIndexPosition.x = 0.0f;
	m_vIndexPosition.y = 0.0f;
	m_bEnd = false;

	while (!m_qString.empty())
	{
		m_qString.pop();
	}
	
	if (!m_vecText.empty())
	{
		size_t iSize = m_vecText.size();
		for (size_t i = 0; i < iSize; ++i)
		{
			m_vecText[i]->Destroy(true);
		}

		m_vecText.clear();
	}

	for (int i = 0; i < strText.length(); ++i)
	{
		m_qString.push(strText.at(i));
	}
}

void CDialogue::SetAnimation(const string & strAnimationKey)
{
	if (m_pPortraitController)
	{
		m_pPortraitController->TransitionState(strAnimationKey);
	}
}

CGameObject * CDialogue::DrawWord(const Vector3& vOffset, const wstring& strText)
{
	CGameObject* pReturn = NULL;
	CGameObject* pWord = CGameObject::CreateObject("DialogueWord", m_pLayer);
	CTransform* pWordTransform = pWord->GetTransform();
	pWordTransform->SetWorldPosition(vOffset + Vector3(m_vIndexPosition.x, m_vIndexPosition.y, 0.0f));
	SAFE_RELEASE(pWordTransform);
	
	CText* pText = pWord->AddComponent<CText>("DialogueWord");
	pText->AlphaBlend(true);
	pText->SetOpacity(0.0f);
	pText->SetText(strText);
	SAFE_RELEASE(pText);

	m_vecText.push_back(pWord);
	pReturn = pWord;
	SAFE_RELEASE(pWord);

	if (m_vIndexPosition.x > 720.0f || strText == L"\n")
	{
		m_vIndexPosition.x = 0;
		m_vIndexPosition.y += 40;
	}
	else
	{
		//특수문자
		if (strText == L" " || strText == L"'" || strText == L"\"" || strText == L"," || strText == L"." || strText == L"?" || strText == L"!")
		{
			m_vIndexPosition.x += 15.0f;
		}
		//else if ()
		//{
		//	m_vIndexPosition.x += 20.0f;
		//}
		else
		{
			m_vIndexPosition.x += 30.0f;
		}
	}
	return pReturn;
}

void CDialogue::DrawDialogueText()
{
	if (m_qString.empty())
		return;

	m_bEnd = false;

	float fFixedDeltaTime = GET_SINGLE(CTimerManager)->FindTimer("MainTimer")->GetFixedDeltaTime();

	m_fTextTimer += fFixedDeltaTime;

	if (m_fTextTimer >= m_fTextInterval)
	{
		m_fTextTimer = 0.0f;
		wchar_t strWord = m_qString.front();
		if (m_ePortraitDirection == PD_LEFT)
		{
			DrawWord(Vector3(370.0f, 60.0f, 0.0f), wstring(&strWord));
		}
		else
		{
			DrawWord(Vector3(200.0f, 60.0f, 0.0f), wstring(&strWord));
		}

		m_qString.pop();

		if (m_qString.empty())
		{
			m_bEnd = true;
			if (m_strAnimationKey != "")
			{
				m_pPortraitController->TransitionState(m_strAnimationKey);
			}
		}
	}

	if (m_vecText.empty())
	{
		return;
	}

	size_t iSize = m_vecText.size();
	for (size_t i = 0; i < iSize; ++i)
	{		
		CText* pText = m_vecText[i]->GetComponentFromType<CText>(CT_TEXT);
		CTransform* pTransform = m_vecText[i]->GetTransform();
		if (pText->GetOpacity() >= 1.0f)
		{
			SAFE_RELEASE(pText);
			SAFE_RELEASE(pTransform);
			continue;
		}
		else
		{
			pText->SetOpacity(pText->GetOpacity() + fFixedDeltaTime * 4.0f);
			pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f * fFixedDeltaTime * 4.0f, 0.0f));
		}
		SAFE_RELEASE(pText);
		SAFE_RELEASE(pTransform);	
	}
}

void CDialogue::	Next(PORTRAIT_DIRECTION eDirection, PORTRAIT_CHARACTER eCharacter, const string& strAnimationKey, const wstring& strText)
{
	//초기화 자동으로 해줌.
	SetString(strText);
	m_ePortraitDirection = eDirection;
	SetBackground(eCharacter);
	m_eCharacter = eCharacter;
	m_pPortraitController->TransitionState(strAnimationKey);
}

void CDialogue::OpenDialogue(PORTRAIT_DIRECTION eDirection, PORTRAIT_CHARACTER eCharacter, const string& strAnimationKey)
{
	m_ePortraitDirection = eDirection;
	SetBackground(eCharacter);

	if (m_pPortraitController)
	{
		m_pPortraitController->TransitionState(strAnimationKey);
	}

	if (eDirection == PD_RIGHT)
	{
		CTransform* pPortraitTransform = m_pPortraitPanel->GetTransform();
		pPortraitTransform->SetWorldRotation(PI / 2.0f, PI, 0.0f);
		pPortraitTransform->SetWorldPosition(1000.0f, 150.0f, 0.0f);
		SAFE_RELEASE(pPortraitTransform);
	}
	else
	{
		CTransform* pPortraitTransform = m_pPortraitPanel->GetTransform();
		pPortraitTransform->SetWorldRotation(PI / 2.0f, 0.0f, 0.0f);
		pPortraitTransform->SetWorldPosition(260.0f, 150.0f, 0.0f);
		SAFE_RELEASE(pPortraitTransform);
	}


	CRenderer* pBack = m_pBackPanel->GetComponentFromType<CRenderer>(CT_RENDERER);
	CRenderer* pPortrait = m_pPortraitPanel->GetComponentFromType<CRenderer>(CT_RENDERER);
	pBack->Enable(true);
	pPortrait->Enable(true);
	SAFE_RELEASE(pBack);
	SAFE_RELEASE(pPortrait);
	m_fTimer = 0.0f;
	m_eState = DS_OPEN;
}

void CDialogue::CloseDialogue()
{
	m_fTimer = 0.0f;
	m_eState = DS_CLOSE;

	while (!m_qString.empty())
	{
		m_qString.pop();
	}

	if (!m_vecText.empty())
	{
		size_t iSize = m_vecText.size();
		for (size_t i = 0; i < iSize; ++i)
		{
			m_vecText[i]->Destroy(true);
		}

		m_vecText.clear();
	}
}

bool CDialogue::Initialize()
{
	m_eState = DS_NONE;
	m_fTimer = 0.0f;

	//Load Sound Clip
	bool bLoadSound = true;

#pragma region Normal Sound Clip
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_01", false, "char_dia_madeline_normal_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_02", false, "char_dia_madeline_normal_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_03", false, "char_dia_madeline_normal_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_04", false, "char_dia_madeline_normal_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_05", false, "char_dia_madeline_normal_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_06", false, "char_dia_madeline_normal_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_07", false, "char_dia_madeline_normal_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_08", false, "char_dia_madeline_normal_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_09", false, "char_dia_madeline_normal_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_A_10", false, "char_dia_madeline_normal_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_01", false, "char_dia_madeline_normal_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_02", false, "char_dia_madeline_normal_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_03", false, "char_dia_madeline_normal_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_04", false, "char_dia_madeline_normal_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_05", false, "char_dia_madeline_normal_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_06", false, "char_dia_madeline_normal_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_07", false, "char_dia_madeline_normal_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_08", false, "char_dia_madeline_normal_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_09", false, "char_dia_madeline_normal_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_B_10", false, "char_dia_madeline_normal_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_01", false, "char_dia_madeline_normal_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_02", false, "char_dia_madeline_normal_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_03", false, "char_dia_madeline_normal_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_04", false, "char_dia_madeline_normal_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_05", false, "char_dia_madeline_normal_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_06", false, "char_dia_madeline_normal_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_07", false, "char_dia_madeline_normal_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_08", false, "char_dia_madeline_normal_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_09", false, "char_dia_madeline_normal_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_normal_mid_C_10", false, "char_dia_madeline_normal_mid_C_10.wav", SFX_PATH);
	}
#pragma endregion

#pragma region Angry Sound Clip
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_01", false, "char_dia_madeline_angry_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_02", false, "char_dia_madeline_angry_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_03", false, "char_dia_madeline_angry_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_04", false, "char_dia_madeline_angry_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_05", false, "char_dia_madeline_angry_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_06", false, "char_dia_madeline_angry_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_07", false, "char_dia_madeline_angry_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_08", false, "char_dia_madeline_angry_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_09", false, "char_dia_madeline_angry_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_A_10", false, "char_dia_madeline_angry_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_01", false, "char_dia_madeline_angry_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_02", false, "char_dia_madeline_angry_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_03", false, "char_dia_madeline_angry_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_04", false, "char_dia_madeline_angry_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_05", false, "char_dia_madeline_angry_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_06", false, "char_dia_madeline_angry_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_07", false, "char_dia_madeline_angry_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_08", false, "char_dia_madeline_angry_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_09", false, "char_dia_madeline_angry_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_B_10", false, "char_dia_madeline_angry_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_01", false, "char_dia_madeline_angry_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_02", false, "char_dia_madeline_angry_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_03", false, "char_dia_madeline_angry_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_04", false, "char_dia_madeline_angry_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_05", false, "char_dia_madeline_angry_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_06", false, "char_dia_madeline_angry_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_07", false, "char_dia_madeline_angry_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_08", false, "char_dia_madeline_angry_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_09", false, "char_dia_madeline_angry_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_angry_mid_C_10", false, "char_dia_madeline_angry_mid_C_10.wav", SFX_PATH);
	}
#pragma endregion

#pragma region Sad Sound Clip
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_01", false, "char_dia_madeline_sad_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_02", false, "char_dia_madeline_sad_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_03", false, "char_dia_madeline_sad_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_04", false, "char_dia_madeline_sad_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_05", false, "char_dia_madeline_sad_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_06", false, "char_dia_madeline_sad_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_07", false, "char_dia_madeline_sad_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_08", false, "char_dia_madeline_sad_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_09", false, "char_dia_madeline_sad_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_A_10", false, "char_dia_madeline_sad_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_01", false, "char_dia_madeline_sad_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_02", false, "char_dia_madeline_sad_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_03", false, "char_dia_madeline_sad_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_04", false, "char_dia_madeline_sad_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_05", false, "char_dia_madeline_sad_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_06", false, "char_dia_madeline_sad_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_07", false, "char_dia_madeline_sad_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_08", false, "char_dia_madeline_sad_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_09", false, "char_dia_madeline_sad_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_B_10", false, "char_dia_madeline_sad_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_01", false, "char_dia_madeline_sad_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_02", false, "char_dia_madeline_sad_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_03", false, "char_dia_madeline_sad_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_04", false, "char_dia_madeline_sad_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_05", false, "char_dia_madeline_sad_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_06", false, "char_dia_madeline_sad_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_07", false, "char_dia_madeline_sad_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_08", false, "char_dia_madeline_sad_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_09", false, "char_dia_madeline_sad_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_sad_mid_C_10", false, "char_dia_madeline_sad_mid_C_10.wav", SFX_PATH);
	}
#pragma endregion

#pragma region Upset Sound Clip
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_01", false, "char_dia_madeline_upset_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_02", false, "char_dia_madeline_upset_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_03", false, "char_dia_madeline_upset_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_04", false, "char_dia_madeline_upset_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_05", false, "char_dia_madeline_upset_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_06", false, "char_dia_madeline_upset_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_07", false, "char_dia_madeline_upset_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_08", false, "char_dia_madeline_upset_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_09", false, "char_dia_madeline_upset_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_A_10", false, "char_dia_madeline_upset_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_01", false, "char_dia_madeline_upset_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_02", false, "char_dia_madeline_upset_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_03", false, "char_dia_madeline_upset_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_04", false, "char_dia_madeline_upset_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_05", false, "char_dia_madeline_upset_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_06", false, "char_dia_madeline_upset_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_07", false, "char_dia_madeline_upset_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_08", false, "char_dia_madeline_upset_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_09", false, "char_dia_madeline_upset_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_B_10", false, "char_dia_madeline_upset_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_01", false, "char_dia_madeline_upset_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_02", false, "char_dia_madeline_upset_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_03", false, "char_dia_madeline_upset_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_04", false, "char_dia_madeline_upset_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_05", false, "char_dia_madeline_upset_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_06", false, "char_dia_madeline_upset_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_07", false, "char_dia_madeline_upset_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_08", false, "char_dia_madeline_upset_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_09", false, "char_dia_madeline_upset_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_upset_mid_C_10", false, "char_dia_madeline_upset_mid_C_10.wav", SFX_PATH);
	}
#pragma endregion

#pragma region Distracted
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_01", false, "char_dia_madeline_distracted_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_02", false, "char_dia_madeline_distracted_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_03", false, "char_dia_madeline_distracted_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_04", false, "char_dia_madeline_distracted_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_05", false, "char_dia_madeline_distracted_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_06", false, "char_dia_madeline_distracted_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_07", false, "char_dia_madeline_distracted_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_08", false, "char_dia_madeline_distracted_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_09", false, "char_dia_madeline_distracted_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_A_10", false, "char_dia_madeline_distracted_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_01", false, "char_dia_madeline_distracted_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_02", false, "char_dia_madeline_distracted_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_03", false, "char_dia_madeline_distracted_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_04", false, "char_dia_madeline_distracted_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_05", false, "char_dia_madeline_distracted_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_06", false, "char_dia_madeline_distracted_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_07", false, "char_dia_madeline_distracted_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_08", false, "char_dia_madeline_distracted_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_09", false, "char_dia_madeline_distracted_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_B_10", false, "char_dia_madeline_distracted_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_01", false, "char_dia_madeline_distracted_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_02", false, "char_dia_madeline_distracted_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_03", false, "char_dia_madeline_distracted_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_04", false, "char_dia_madeline_distracted_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_05", false, "char_dia_madeline_distracted_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_06", false, "char_dia_madeline_distracted_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_07", false, "char_dia_madeline_distracted_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_08", false, "char_dia_madeline_distracted_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_09", false, "char_dia_madeline_distracted_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_distracted_mid_C_10", false, "char_dia_madeline_distracted_mid_C_10.wav", SFX_PATH);

	}
#pragma endregion

#pragma region Deadpan
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_01", false, "char_dia_madeline_deadpan_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_02", false, "char_dia_madeline_deadpan_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_03", false, "char_dia_madeline_deadpan_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_04", false, "char_dia_madeline_deadpan_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_05", false, "char_dia_madeline_deadpan_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_06", false, "char_dia_madeline_deadpan_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_07", false, "char_dia_madeline_deadpan_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_08", false, "char_dia_madeline_deadpan_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_09", false, "char_dia_madeline_deadpan_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_A_10", false, "char_dia_madeline_deadpan_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_01", false, "char_dia_madeline_deadpan_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_02", false, "char_dia_madeline_deadpan_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_03", false, "char_dia_madeline_deadpan_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_04", false, "char_dia_madeline_deadpan_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_05", false, "char_dia_madeline_deadpan_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_06", false, "char_dia_madeline_deadpan_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_07", false, "char_dia_madeline_deadpan_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_08", false, "char_dia_madeline_deadpan_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_09", false, "char_dia_madeline_deadpan_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_B_10", false, "char_dia_madeline_deadpan_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_01", false, "char_dia_madeline_deadpan_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_02", false, "char_dia_madeline_deadpan_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_03", false, "char_dia_madeline_deadpan_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_04", false, "char_dia_madeline_deadpan_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_05", false, "char_dia_madeline_deadpan_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_06", false, "char_dia_madeline_deadpan_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_07", false, "char_dia_madeline_deadpan_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_08", false, "char_dia_madeline_deadpan_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_09", false, "char_dia_madeline_deadpan_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_madeline_deadpan_mid_C_10", false, "char_dia_madeline_deadpan_mid_C_10.wav", SFX_PATH);

	}
#pragma endregion


#pragma region Granny Normal Sound Clip
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_01", false, "char_dia_granny_normal_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_02", false, "char_dia_granny_normal_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_03", false, "char_dia_granny_normal_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_04", false, "char_dia_granny_normal_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_05", false, "char_dia_granny_normal_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_06", false, "char_dia_granny_normal_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_07", false, "char_dia_granny_normal_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_08", false, "char_dia_granny_normal_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_09", false, "char_dia_granny_normal_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_A_10", false, "char_dia_granny_normal_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_01", false, "char_dia_granny_normal_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_02", false, "char_dia_granny_normal_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_03", false, "char_dia_granny_normal_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_04", false, "char_dia_granny_normal_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_05", false, "char_dia_granny_normal_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_06", false, "char_dia_granny_normal_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_07", false, "char_dia_granny_normal_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_08", false, "char_dia_granny_normal_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_09", false, "char_dia_granny_normal_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_B_10", false, "char_dia_granny_normal_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_01", false, "char_dia_granny_normal_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_02", false, "char_dia_granny_normal_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_03", false, "char_dia_granny_normal_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_04", false, "char_dia_granny_normal_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_05", false, "char_dia_granny_normal_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_06", false, "char_dia_granny_normal_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_07", false, "char_dia_granny_normal_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_08", false, "char_dia_granny_normal_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_09", false, "char_dia_granny_normal_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_normal_mid_C_10", false, "char_dia_granny_normal_mid_C_10.wav", SFX_PATH);
	}
#pragma endregion


#pragma region Granny Mock Sound Clip
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_01", false, "char_dia_granny_mock_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_02", false, "char_dia_granny_mock_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_03", false, "char_dia_granny_mock_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_04", false, "char_dia_granny_mock_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_05", false, "char_dia_granny_mock_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_06", false, "char_dia_granny_mock_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_07", false, "char_dia_granny_mock_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_08", false, "char_dia_granny_mock_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_09", false, "char_dia_granny_mock_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_10", false, "char_dia_granny_mock_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_01", false, "char_dia_granny_mock_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_02", false, "char_dia_granny_mock_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_03", false, "char_dia_granny_mock_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_04", false, "char_dia_granny_mock_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_05", false, "char_dia_granny_mock_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_06", false, "char_dia_granny_mock_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_07", false, "char_dia_granny_mock_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_08", false, "char_dia_granny_mock_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_09", false, "char_dia_granny_mock_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_10", false, "char_dia_granny_mock_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_01", false, "char_dia_granny_mock_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_02", false, "char_dia_granny_mock_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_03", false, "char_dia_granny_mock_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_04", false, "char_dia_granny_mock_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_05", false, "char_dia_granny_mock_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_06", false, "char_dia_granny_mock_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_07", false, "char_dia_granny_mock_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_08", false, "char_dia_granny_mock_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_09", false, "char_dia_granny_mock_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_10", false, "char_dia_granny_mock_mid_C_10.wav", SFX_PATH);
	}
#pragma endregion


#pragma region Granny Mock Sound Clip
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_01", false, "char_dia_granny_mock_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_02", false, "char_dia_granny_mock_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_03", false, "char_dia_granny_mock_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_04", false, "char_dia_granny_mock_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_05", false, "char_dia_granny_mock_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_06", false, "char_dia_granny_mock_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_07", false, "char_dia_granny_mock_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_08", false, "char_dia_granny_mock_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_09", false, "char_dia_granny_mock_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_A_10", false, "char_dia_granny_mock_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_01", false, "char_dia_granny_mock_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_02", false, "char_dia_granny_mock_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_03", false, "char_dia_granny_mock_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_04", false, "char_dia_granny_mock_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_05", false, "char_dia_granny_mock_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_06", false, "char_dia_granny_mock_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_07", false, "char_dia_granny_mock_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_08", false, "char_dia_granny_mock_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_09", false, "char_dia_granny_mock_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_B_10", false, "char_dia_granny_mock_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_01", false, "char_dia_granny_mock_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_02", false, "char_dia_granny_mock_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_03", false, "char_dia_granny_mock_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_04", false, "char_dia_granny_mock_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_05", false, "char_dia_granny_mock_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_06", false, "char_dia_granny_mock_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_07", false, "char_dia_granny_mock_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_08", false, "char_dia_granny_mock_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_09", false, "char_dia_granny_mock_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_granny_mock_mid_C_10", false, "char_dia_granny_mock_mid_C_10.wav", SFX_PATH);
	
	
	
	
	
	
	}
#pragma endregion

#pragma region Theo Normal
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_01", false, "char_dia_theo_normal_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_02", false, "char_dia_theo_normal_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_03", false, "char_dia_theo_normal_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_04", false, "char_dia_theo_normal_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_05", false, "char_dia_theo_normal_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_06", false, "char_dia_theo_normal_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_07", false, "char_dia_theo_normal_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_08", false, "char_dia_theo_normal_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_09", false, "char_dia_theo_normal_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_A_10", false, "char_dia_theo_normal_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_01", false, "char_dia_theo_normal_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_02", false, "char_dia_theo_normal_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_03", false, "char_dia_theo_normal_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_04", false, "char_dia_theo_normal_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_05", false, "char_dia_theo_normal_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_06", false, "char_dia_theo_normal_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_07", false, "char_dia_theo_normal_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_08", false, "char_dia_theo_normal_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_09", false, "char_dia_theo_normal_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_B_10", false, "char_dia_theo_normal_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_01", false, "char_dia_theo_normal_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_02", false, "char_dia_theo_normal_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_03", false, "char_dia_theo_normal_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_04", false, "char_dia_theo_normal_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_05", false, "char_dia_theo_normal_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_06", false, "char_dia_theo_normal_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_07", false, "char_dia_theo_normal_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_08", false, "char_dia_theo_normal_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_09", false, "char_dia_theo_normal_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_DIALOGUE, "char_dia_theo_normal_mid_C_10", false, "char_dia_theo_normal_mid_C_10.wav", SFX_PATH);

	}
#pragma endregion


#pragma region Theo Excited
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_01", false, "char_dia_theo_excited_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_02", false, "char_dia_theo_excited_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_03", false, "char_dia_theo_excited_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_04", false, "char_dia_theo_excited_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_05", false, "char_dia_theo_excited_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_06", false, "char_dia_theo_excited_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_07", false, "char_dia_theo_excited_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_08", false, "char_dia_theo_excited_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_09", false, "char_dia_theo_excited_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_A_10", false, "char_dia_theo_excited_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_01", false, "char_dia_theo_excited_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_02", false, "char_dia_theo_excited_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_03", false, "char_dia_theo_excited_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_04", false, "char_dia_theo_excited_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_05", false, "char_dia_theo_excited_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_06", false, "char_dia_theo_excited_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_07", false, "char_dia_theo_excited_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_08", false, "char_dia_theo_excited_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_09", false, "char_dia_theo_excited_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_B_10", false, "char_dia_theo_excited_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_01", false, "char_dia_theo_excited_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_02", false, "char_dia_theo_excited_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_03", false, "char_dia_theo_excited_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_04", false, "char_dia_theo_excited_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_05", false, "char_dia_theo_excited_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_06", false, "char_dia_theo_excited_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_07", false, "char_dia_theo_excited_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_08", false, "char_dia_theo_excited_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_09", false, "char_dia_theo_excited_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_excited_mid_C_10", false, "char_dia_theo_excited_mid_C_10.wav", SFX_PATH);

	}
#pragma endregion


#pragma region Theo Thinking
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_01", false, "char_dia_theo_thinking_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_02", false, "char_dia_theo_thinking_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_03", false, "char_dia_theo_thinking_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_04", false, "char_dia_theo_thinking_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_05", false, "char_dia_theo_thinking_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_06", false, "char_dia_theo_thinking_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_07", false, "char_dia_theo_thinking_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_08", false, "char_dia_theo_thinking_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_09", false, "char_dia_theo_thinking_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_A_10", false, "char_dia_theo_thinking_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_01", false, "char_dia_theo_thinking_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_02", false, "char_dia_theo_thinking_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_03", false, "char_dia_theo_thinking_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_04", false, "char_dia_theo_thinking_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_05", false, "char_dia_theo_thinking_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_06", false, "char_dia_theo_thinking_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_07", false, "char_dia_theo_thinking_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_08", false, "char_dia_theo_thinking_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_09", false, "char_dia_theo_thinking_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_B_10", false, "char_dia_theo_thinking_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_01", false, "char_dia_theo_thinking_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_02", false, "char_dia_theo_thinking_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_03", false, "char_dia_theo_thinking_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_04", false, "char_dia_theo_thinking_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_05", false, "char_dia_theo_thinking_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_06", false, "char_dia_theo_thinking_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_07", false, "char_dia_theo_thinking_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_08", false, "char_dia_theo_thinking_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_09", false, "char_dia_theo_thinking_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_thinking_mid_C_10", false, "char_dia_theo_thinking_mid_C_10.wav", SFX_PATH);

	}
#pragma endregion

#pragma region Theo WTF
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_01", false, "char_dia_theo_wtf_mid_A_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_02", false, "char_dia_theo_wtf_mid_A_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_03", false, "char_dia_theo_wtf_mid_A_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_04", false, "char_dia_theo_wtf_mid_A_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_05", false, "char_dia_theo_wtf_mid_A_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_06", false, "char_dia_theo_wtf_mid_A_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_07", false, "char_dia_theo_wtf_mid_A_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_08", false, "char_dia_theo_wtf_mid_A_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_09", false, "char_dia_theo_wtf_mid_A_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_A_10", false, "char_dia_theo_wtf_mid_A_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_01", false, "char_dia_theo_wtf_mid_B_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_02", false, "char_dia_theo_wtf_mid_B_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_03", false, "char_dia_theo_wtf_mid_B_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_04", false, "char_dia_theo_wtf_mid_B_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_05", false, "char_dia_theo_wtf_mid_B_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_06", false, "char_dia_theo_wtf_mid_B_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_07", false, "char_dia_theo_wtf_mid_B_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_08", false, "char_dia_theo_wtf_mid_B_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_09", false, "char_dia_theo_wtf_mid_B_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_B_10", false, "char_dia_theo_wtf_mid_B_10.wav", SFX_PATH);

		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_01", false, "char_dia_theo_wtf_mid_C_01.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_02", false, "char_dia_theo_wtf_mid_C_02.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_03", false, "char_dia_theo_wtf_mid_C_03.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_04", false, "char_dia_theo_wtf_mid_C_04.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_05", false, "char_dia_theo_wtf_mid_C_05.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_06", false, "char_dia_theo_wtf_mid_C_06.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_07", false, "char_dia_theo_wtf_mid_C_07.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_08", false, "char_dia_theo_wtf_mid_C_08.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_09", false, "char_dia_theo_wtf_mid_C_09.wav", SFX_PATH);
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_wtf_mid_C_10", false, "char_dia_theo_wtf_mid_C_10.wav", SFX_PATH);

	}
#pragma endregion


#pragma region Theo Yolo
	{
		bLoadSound &= GET_SINGLE(CSoundManager)->LoadSound(SOUND_SFX, "char_dia_theo_yolo_solo", false, "char_dia_theo_yolo_solo.wav", SFX_PATH);
		
	}
#pragma endregion


	if (!bLoadSound)
		return false;

	m_pBackPanel = CGameObject::CreateObject("DialogueBackgroundPanel", m_pLayer);
	CTransform* pBackPanelTransform = m_pBackPanel->GetTransform();
	pBackPanelTransform->SetWorldPosition(DEVICE_RESOLUTION.iWidth / 2.0f, 150.0f, 0.0f);
	pBackPanelTransform->SetPivot(0.5f, 0.5f, 0.5f);
	pBackPanelTransform->SetWorldScale(1000.0f, 250.0f, 1.0f);
	SAFE_RELEASE(pBackPanelTransform);
	CRenderer* pBackPanelRenderer = m_pBackPanel->AddComponent<CRenderer>("Renderer");
	pBackPanelRenderer->Enable(false);
	pBackPanelRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pBackPanelRenderer->SetShader(UI_TEXTURE_SHADER);
	pBackPanelRenderer->SetInputLayout(POS_UV_LAYOUT);
	pBackPanelRenderer->SetRenderState(ALPHA_BLEND);
	pBackPanelRenderer->SetRenderMode(RM_2D);

	CMaterial* pBackPanelMaterial = pBackPanelRenderer->CreateMaterial();
	pBackPanelMaterial->SetDiffuseTexture("DialogueBoxGranny", L"Celeste\\Portraits\\textbox\\granny.png");
	pBackPanelMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pBackPanelMaterial);
	SAFE_RELEASE(pBackPanelRenderer);


	m_pPortraitPanel = CGameObject::CreateObject("DialoguePortraitPanel", m_pLayer);	
	CTransform* pPortraitTransform = m_pPortraitPanel->GetTransform();
	pPortraitTransform->SetPivot(0.5f, 0.5f, 0.5f);
	pPortraitTransform->SetWorldScale(160.0f, 160.0f, 1.0f);
	SAFE_RELEASE(pPortraitTransform);

	CRenderer* pPortraitPanelRenderer = m_pPortraitPanel->AddComponent<CRenderer>("Renderer");
	pPortraitPanelRenderer->Enable(false);
	pPortraitPanelRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pPortraitPanelRenderer->SetShader(UI_TEXTURE_SHADER);
	pPortraitPanelRenderer->SetInputLayout(POS_UV_LAYOUT);
	pPortraitPanelRenderer->SetRenderState(ALPHA_BLEND);
	pPortraitPanelRenderer->SetRenderMode(RM_2D);

	CMaterial* pPortraitPanelMaterial = pPortraitPanelRenderer->CreateMaterial();
	pPortraitPanelMaterial->SetDiffuseTexture("Portraits", L"Celeste\\Portraits\\Portraits.png");
	pPortraitPanelMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pPortraitPanelMaterial);
	SAFE_RELEASE(pPortraitPanelRenderer);


	CAnimator* pPortraitPanelAnimator = m_pPortraitPanel->AddComponent<CAnimator>("Animator");
	CAnimationController* pPortraitPanelController = new CAnimationController;
	pPortraitPanelAnimator->SetAnimationController(pPortraitPanelController);
	pPortraitPanelAnimator->SetSyncScale(false);
	pPortraitPanelAnimator->SetFixedScaleFactor(Vector3(CELESTE_PIXEL, CELESTE_PIXEL, 1.0f));
	m_pPortraitController = pPortraitPanelController;
	SAFE_RELEASE(pPortraitPanelAnimator);
#pragma region Madeline

	CAnimationState* pState = pPortraitPanelController->CreateAnimationState("Madeline Normal");
	pState->AddClip("Madeline Normal", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	//pPortraitPanelController->SetEntryState("Madeline Normal"); 
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal00");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Normal Blink");
	pPortraitPanelController->SetEntryState("Madeline Normal Blink");
	pState->AddClip("Madeline Normal Blink", AO_LOOP, 0.32f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal00");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal01");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal02");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal03");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Normal Talk");
	pState->AddClip("Madeline Normal Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal04");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal06");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal05");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\normal00");


	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_normal_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_normal_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_normal_mid_C_10", true);

	pState->GetClip()->SetAudioPackage("Talk");





	pState = pPortraitPanelController->CreateAnimationState("Madeline Angry");
	pState->AddClip("Madeline Angry", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry00");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Angry Blink");
	pState->AddClip("Madeline Angry Blink", AO_LOOP, 0.32f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry00");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry01");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry02");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry03");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Angry Talk");
	pState->AddClip("Madeline Angry Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry04");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry06");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry05");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\angry00");


	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_angry_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_angry_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_angry_mid_C_10", true);

	pState->GetClip()->SetAudioPackage("Talk");



	pState = pPortraitPanelController->CreateAnimationState("Madeline Sad");
	pState->AddClip("Madeline Sad", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad00");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Sad Blink");
	pState->AddClip("Madeline Sad Blink", AO_LOOP, 0.32f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad00");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad01");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad02");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad03");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Sad Talk");
	pState->AddClip("Madeline Sad Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad04");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad06");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad05");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\sad00");


	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_sad_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_sad_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_sad_mid_C_10", true);

	pState->GetClip()->SetAudioPackage("Talk");


	pState = pPortraitPanelController->CreateAnimationState("Madeline Upset");
	pState->AddClip("Madeline Upset", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset00");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Upset Blink");
	pState->AddClip("Madeline Upset Blink", AO_LOOP, 0.32f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset00");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset01");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset02");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset03");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Upset Talk");
	pState->AddClip("Madeline Upset Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset04");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset06");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset05");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\upset00");



	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_upset_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_upset_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_upset_mid_C_10", true);

	pState->GetClip()->SetAudioPackage("Talk");




	pState = pPortraitPanelController->CreateAnimationState("Madeline Distracted");
	pState->AddClip("Madeline Distracted", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted00");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Distracted Blink");
	pState->AddClip("Madeline Distracted Blink", AO_LOOP, 0.32f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted00");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted01");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted02");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted03");

	pState = pPortraitPanelController->CreateAnimationState("Madeline Distracted Talk");
	pState->AddClip("Madeline Distracted Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted04");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted06");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted05");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted00");

	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_distracted_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_distracted_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_distracted_mid_C_10", true);
	pState->GetClip()->SetAudioPackage("Talk");


	pState = pPortraitPanelController->CreateAnimationState("Madeline Deadpan");
	pState->AddClip("Madeline Deadpan", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\distracted00");
	
	pState = pPortraitPanelController->CreateAnimationState("Madeline Deadpan Talk");
	pState->AddClip("Madeline Deadpan Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\deadpan04");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\deadpan06");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\deadpan05");
	pState->GetClip()->AddCustomInfo("portraits\\madeline\\deadpan00");

	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_madeline_deadpan_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_madeline_deadpan_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_madeline_deadpan_mid_C_10", true);
	pState->GetClip()->SetAudioPackage("Talk");


	pState = pPortraitPanelController->CreateAnimationState("Granny Normal");
	pState->AddClip("Granny Normal", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal00");

	pState = pPortraitPanelController->CreateAnimationState("Granny Normal Talk");
	pState->AddClip("Granny Normal", AO_LOOP, 0.7f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal04");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal05");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal06");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal07");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal08");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal07");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal00");


	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_C_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_C_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_C_10", true);

	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_01", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_02", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_03", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_C_04", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_C_05", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_C_06", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_C_07", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_A_08", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_A_09", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_A_10", true);

	pState->GetClip()->SetAudioPackage("Talk");

#pragma endregion

#pragma region Theo 

	pState = pPortraitPanelController->CreateAnimationState("Theo Normal");
	pState->AddClip("Theo Normal", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\normal00");

	pState = pPortraitPanelController->CreateAnimationState("Theo Normal Talk");
	pState->AddClip("Theo Normal Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\normal04");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\normal06");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\normal05");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\normal00");

	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_normal_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_normal_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_normal_mid_C_10", true);

	pState->GetClip()->SetAudioPackage("Talk");

	pState = pPortraitPanelController->CreateAnimationState("Theo Excited");
	pState->AddClip("Theo Excited", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\excited00");

	pState = pPortraitPanelController->CreateAnimationState("Theo Excited Talk");
	pState->AddClip("Theo Excited Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\excited04");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\excited06");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\excited05");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\excited00");

	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_excited_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_excited_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_excited_mid_C_10", true);
	pState->GetClip()->SetAudioPackage("Talk");

	pState = pPortraitPanelController->CreateAnimationState("Theo Thinking");
	pState->AddClip("Theo Thinking", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\thinking00");

	pState = pPortraitPanelController->CreateAnimationState("Theo Thinking Talk");
	pState->AddClip("Theo Thinking Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\thinking04");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\thinking06");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\thinking05");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\thinking00");

	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_thinking_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_thinking_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_thinking_mid_C_10", true);
	pState->GetClip()->SetAudioPackage("Talk");


	pState = pPortraitPanelController->CreateAnimationState("Theo WTF");
	pState->AddClip("Theo WTF", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\wtf00");

	pState = pPortraitPanelController->CreateAnimationState("Theo WTF Talk");
	pState->AddClip("Theo WTF Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\wtf04");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\wtf06");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\wtf05");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\wtf00");

	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_wtf_mid_A_10", true);

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_theo_wtf_mid_B_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_theo_wtf_mid_C_10", true);
	pState->GetClip()->SetAudioPackage("Talk");


	pState = pPortraitPanelController->CreateAnimationState("Theo YOLO");
	pState->AddClip("Theo YOLO", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\yolo0");

	pState = pPortraitPanelController->CreateAnimationState("Theo YOLO Talk");
	pState->AddClip("Theo YOLO Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\yolo3");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\yolo2");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\yolo1");
	pState->GetClip()->AddCustomInfo("portraits\\theo\\yolo2");

	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_theo_yolo_solo", true);
	pState->GetClip()->SetAudioPackage("Talk");

#pragma endregion



#pragma region Granny


	pState = pPortraitPanelController->CreateAnimationState("Granny Normal Blink1");
	pState->AddClip("Granny Normal Blink1", AO_LOOP, 0.32f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal00");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal01");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal02");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal03");

	pState = pPortraitPanelController->CreateAnimationState("Granny Normal Blink2");
	pState->AddClip("Granny Normal Blink2", AO_LOOP, 0.64f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal00");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal09");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal10");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal11");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal02");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal12");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal13");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\normal14");


	pState = pPortraitPanelController->CreateAnimationState("Granny Mock");
	pState->AddClip("Granny Mock", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock00");

	pState = pPortraitPanelController->CreateAnimationState("Granny Mock Blink");
	pState->AddClip("Granny Mock Blink", AO_LOOP, 0.32f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock00");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock01");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock02");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock01");

	pState = pPortraitPanelController->CreateAnimationState("Granny Mock Talk");
	pState->AddClip("Granny Mock Talk", AO_LOOP, 0.8f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock03");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock05");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock04");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\mock00");


	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_01", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_02", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_03", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_04", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_05", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_06", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_07", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_08", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_09", true);
	pState->GetClip()->AddAudioClip(0, "Talk", "char_dia_granny_mock_mid_A_10", true);

	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_01", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_02", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_03", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_04", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_05", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_06", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_07", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_08", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_09", true);
	pState->GetClip()->AddAudioClip(2, "Talk", "char_dia_granny_mock_mid_C_10", true);

	pState->GetClip()->SetAudioPackage("Talk");




	pState = pPortraitPanelController->CreateAnimationState("Granny Creep A");
	pState->AddClip("Granny Creep A", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA00");

	pState = pPortraitPanelController->CreateAnimationState("Granny Creep A Talk");
	pState->AddClip("Granny Creep A Talk", AO_LOOP, 0.7f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA04");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA05");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA06");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA07");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA08");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA07");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA00");

	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_10", true);

	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_01", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_02", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_03", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_04", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_05", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_06", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_07", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_08", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_09", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_10", true);
	pState->GetClip()->SetAudioPackage("Talk");



	pState = pPortraitPanelController->CreateAnimationState("Granny Creep B");
	pState->AddClip("Granny Creep B", AO_LOOP, 0.1f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepA00");

	pState = pPortraitPanelController->CreateAnimationState("Granny Creep B Talk");
	pState->AddClip("Granny Creep B Talk", AO_LOOP, 0.7f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepB04");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepB05");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepB06");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepB07");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepB08");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepB07");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\creepB00");


	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_10", true);

	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_01", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_02", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_03", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_04", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_05", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_06", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_07", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_08", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_09", true);
	pState->GetClip()->AddAudioClip(3, "Talk", "char_dia_granny_normal_mid_B_10", true);

	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_01", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_02", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_03", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_04", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_05", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_06", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_07", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_08", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_09", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_10", true);
	pState->GetClip()->SetAudioPackage("Talk");


	pState = pPortraitPanelController->CreateAnimationState("Granny Laugh");
	pState->AddClip("Granny Laugh", AO_LOOP, 0.4f, "Portraits", L"Celeste\\Gameplay.png");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\laugh00");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\laugh01");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\laugh02");
	pState->GetClip()->AddCustomInfo("portraits\\granny\\laugh03");


	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_01", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_02", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_03", true);
	pState->GetClip()->AddAudioClip(5, "Talk", "char_dia_granny_normal_mid_C_04", true);


	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_01", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_02", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_03", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_04", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_05", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_06", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_07", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_08", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_09", true);
	pState->GetClip()->AddAudioClip(1, "Talk", "char_dia_granny_normal_mid_A_10", true);

	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_01", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_02", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_03", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_04", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_05", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_06", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_07", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_08", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_09", true);
	pState->GetClip()->AddAudioClip(4, "Talk", "char_dia_granny_normal_mid_B_10", true);
	pState->GetClip()->SetAudioPackage("Talk");


#pragma endregion

	return true;
}

int CDialogue::Update(float fTime)
{
	if (m_eState == DS_NONE)
	{
		CTransform* pBack = m_pBackPanel->GetTransform();
		CTransform* pPortrait = m_pPortraitPanel->GetTransform();
		pBack->SetWorldRotation(PI / 2.0f, pBack->GetWorldRotation().y, 0.0f);
		pPortrait->SetWorldRotation(PI / 2.0f, pPortrait->GetWorldRotation().y, 0.0f);
		SAFE_RELEASE(pBack);
		SAFE_RELEASE(pPortrait);
	}

	if (m_eState == DS_NORMAL)
	{
		DrawDialogueText();

		CTransform* pBack = m_pBackPanel->GetTransform();
		CTransform* pPortrait = m_pPortraitPanel->GetTransform();
		pBack->SetWorldRotation(0.0f, pBack->GetWorldRotation().y, 0.0f);
		pPortrait->SetWorldRotation(0.0f, pPortrait->GetWorldRotation().y, 0.0f);
		SAFE_RELEASE(pBack);
		SAFE_RELEASE(pPortrait);
	}


	if (m_eState == DS_OPEN)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.15f;

		CTransform* pBack = m_pBackPanel->GetTransform();
		CTransform* pPortrait = m_pPortraitPanel->GetTransform();
		
		pBack->SetWorldRotation(PI / 2.0f * (1.0f - fProgress), pBack->GetWorldRotation().y, 0.0f);
		pPortrait->SetWorldRotation(PI / 2.0f * (1.0f - fProgress), pPortrait->GetWorldRotation().y, 0.0f);
		SAFE_RELEASE(pBack);
		SAFE_RELEASE(pPortrait);

		if (fProgress >= 1.0f)
		{
			m_eState = DS_NORMAL;
			CTransform* pBack = m_pBackPanel->GetTransform();
			CTransform* pPortrait = m_pPortraitPanel->GetTransform();
			pBack->SetWorldRotation(0.0f, pBack->GetWorldRotation().y, 0.0f);
			pPortrait->SetWorldRotation(0.0f, pPortrait->GetWorldRotation().y, 0.0f);
			SAFE_RELEASE(pBack);
			SAFE_RELEASE(pPortrait);
		}
	}

	if (m_eState == DS_CLOSE)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.15f;
		CTransform* pBack = m_pBackPanel->GetTransform();
		CTransform* pPortrait = m_pPortraitPanel->GetTransform();
		pBack->SetWorldRotation(PI / 2.0f * fProgress, pBack->GetWorldRotation().y, 0.0f);
		pPortrait->SetWorldRotation(PI / 2.0f * fProgress, pPortrait->GetWorldRotation().y, 0.0f);
		SAFE_RELEASE(pBack);
		SAFE_RELEASE(pPortrait);

		if (fProgress >= 1.0f)
		{
			m_eState = DS_NONE;

			pBack = m_pBackPanel->GetTransform();
			pPortrait = m_pPortraitPanel->GetTransform();
			pBack->SetWorldRotation(PI / 2.0f, pBack->GetWorldRotation().y, 0.0f);
			pPortrait->SetWorldRotation(PI / 2.0f, pPortrait->GetWorldRotation().y, 0.0f);
			SAFE_RELEASE(pBack);
			SAFE_RELEASE(pPortrait);

			CRenderer* pBack = m_pBackPanel->GetComponentFromType<CRenderer>(CT_RENDERER);
			CRenderer* pPortrait = m_pPortraitPanel->GetComponentFromType<CRenderer>(CT_RENDERER);
			pBack->Enable(false);
			pPortrait->Enable(false);
			SAFE_RELEASE(pBack);
			SAFE_RELEASE(pPortrait);
		}
	}
	return 0;
}

void CDialogue::Render(float fTime)
{
	CTransform*	pCameraTransform = m_pScene->GetMainCameraTransform();
	Vector3	vCameraPos = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);
}
