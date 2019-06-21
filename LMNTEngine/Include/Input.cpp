#include "Input.h"
#include "Core.h"
#include "Device.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"
#include "Component\Cursor.h"

LMNT_USING

DEFINITION_SINGLE(CInput)


CInput::CInput():
	m_bCursorVisible(true),
	m_bEnableInput(true)
{
}


CInput::~CInput()
{	
	unordered_map<int, PKEYSTATE>::iterator iter;
	unordered_map<int, PKEYSTATE>::iterator iterEnd = m_mapKeyState.end();

	for (iter = m_mapKeyState.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second)
			delete iter->second;
	}
	m_mapKeyState.clear();	
}

bool CInput::Initialize()
{
	for (int i = 1; i < 256; ++i)
	{
		m_mapKeyState.insert(make_pair(i, new KEYSTATE));
	}

	m_mapKeyState.insert(make_pair(MOUSE_WHEEL_UP, new KEYSTATE));
	m_mapKeyState.insert(make_pair(MOUSE_WHEEL_DOWN, new KEYSTATE));


	//CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	//CLayer* pLayer = pScene->FindLayer("UI");
	//CGameObject* pCursorObject = CGameObject::CreateObject("Cursor", pLayer);
	//CCursor* pCursor = pCursorObject->AddComponent<CCursor>("Cursor");
	//SAFE_RELEASE(pCursor);
	//SAFE_RELEASE(pCursorObject);
	//SAFE_RELEASE(pLayer);
	//SAFE_RELEASE(pScene);


	m_vPrevCursorPositionAsDevice = GetCursorPosition(SPACE_DEVICE);
	m_vPrevCursorPositionAsWindow = GetCursorPosition(SPACE_WINDOW);

	return true;
}


void CInput::Update()
{
	unordered_map<int, PKEYSTATE>::iterator iter;
	unordered_map<int, PKEYSTATE>::iterator iterEnd = m_mapKeyState.end();

	for (iter = m_mapKeyState.begin(); iter != iterEnd; ++iter)
	{
		bool bKeyState = GetAsyncKeyState(iter->first) & 0x8000;
		if (!iter->second->bDown && bKeyState && !iter->second->bPressed)
		{
			iter->second->bDown = true;
			iter->second->bPressed = true;
			iter->second->bUp = false;
		}
		else if (iter->second->bDown && bKeyState)
		{
			iter->second->bDown = false;
			iter->second->bPressed = true;
			iter->second->bUp = false;
		}
		else if (iter->second->bPressed && !bKeyState)
		{
			iter->second->bDown = false;
			iter->second->bPressed = false;
			iter->second->bUp = true;
		}
		else if (iter->second->bUp && !bKeyState)
		{
			iter->second->bDown = false;
			iter->second->bPressed = false;
			iter->second->bUp = false;
		}
	}

	m_vPrevCursorPositionAsDevice = GetCursorPosition(SPACE_DEVICE);
	m_vPrevCursorPositionAsWindow = GetCursorPosition(SPACE_WINDOW);

	//Input에 이 옵션을 묶어서 Scene 및 Layer에 상관 없이 처리할 수 있도록 함.
	//UI와의 충돌 처리는 GetCursorPosition으로 위치를 받아서 처리하기 때문에 Cursor의 오브젝트가 필요하지 않음.
	if (!m_bCursorVisible)
	{
		Vector2 vCursorPosition = GET_SINGLE(CInput)->GetCursorPosition(SPACE_DEVICE);

		if (vCursorPosition.x < 0 || vCursorPosition.x > DEVICE_RESOLUTION.iWidth ||
			vCursorPosition.y < 0 || vCursorPosition.y > DEVICE_RESOLUTION.iHeight)
		{
			while (ShowCursor(TRUE) < 0)
			{
			}
			m_bCursorVisible = TRUE;		
		}
		else
		{
			while (ShowCursor(FALSE) >= 0);
			{
			}
			m_bCursorVisible = FALSE;
		}
	}
	else
	{
		while (ShowCursor(TRUE) < 0)
		{
		}
		m_bCursorVisible = TRUE;
	}
}


void CInput::SetWheelState(int iKey)
{
	unordered_map<int, PKEYSTATE>::iterator iter = m_mapKeyState.find(iKey);
	if (iter == m_mapKeyState.end())
	{
		assert(false);
		return;
	}

	iter->second->bDown = true;

}

void CInput::ClearWheelState()
{
	unordered_map<int, PKEYSTATE>::iterator iter = m_mapKeyState.find(MOUSE_WHEEL_UP);
	if (iter == m_mapKeyState.end())
	{
		assert(false);
		return;
	}
	iter->second->bDown = false;
		
	iter = m_mapKeyState.find(MOUSE_WHEEL_DOWN);
	if (iter == m_mapKeyState.end())
	{
		assert(false);
		return;
	}
	iter->second->bDown = false;
}

bool CInput::GetCursorVisible() const
{
	return m_bCursorVisible;
}

void CInput::SetCursorVisible(bool bVisible)
{
	m_bCursorVisible = bVisible;
}

Vector2 CInput::GetCursorPosition(SPACE eSpace)
{
	
	POINT pos;
	GetCursorPos(&pos);
	HWND hWnd = GET_SINGLE(CCore)->GetWindowHandle();
	ScreenToClient(hWnd, &pos);

	switch (eSpace)
	{
		case LMNT::SPACE_WINDOW:
		{
			return Vector2(pos.x, pos.y);
		}	break;
		case LMNT::SPACE_DEVICE:
		{
			RESOLUTION tDeviceResolution = GET_SINGLE(CDevice)->GetDeviceResolution();
			RECT tWindowRect;
			GetClientRect(hWnd, &tWindowRect);

			Vector2 vWindowResolution = Vector2(tWindowRect.right - tWindowRect.left, tWindowRect.bottom - tWindowRect.top);
			Vector2 vResolutionRate = Vector2((float)tDeviceResolution.iWidth / vWindowResolution.x, (float)tDeviceResolution.iHeight / vWindowResolution.y);
		
			return Vector2(pos.x * vResolutionRate.x, pos.y * vResolutionRate.y);

		}
		break;
		default:
		{
			return Vector2(pos.x, pos.y);
		}
		break;
	}

	
	
	return Vector2(pos.x, pos.y);


}

Vector2 CInput::GetCursorDeltaPosition(SPACE eSpace) const
{
	if (eSpace == SPACE_DEVICE)
		return GetCursorPosition(eSpace) - m_vPrevCursorPositionAsDevice;
	else if (eSpace == SPACE_WINDOW)
		return GetCursorPosition(eSpace) - m_vPrevCursorPositionAsWindow;
}

void CInput::EnableInput()
{
	m_bEnableInput = true;
}

void CInput::DisableInput()
{
	m_bEnableInput = false;
}

bool CInput::GetAsyncKeyDown(int iKey)
{
	unordered_map<int, PKEYSTATE>::iterator iter = m_mapKeyState.find(iKey);
	if (iter == m_mapKeyState.end())
	{
		return false;
	}
	return iter->second->bDown;
}

bool CInput::GetAsyncKeyUp(int iKey)
{
	unordered_map<int, PKEYSTATE>::iterator iter = m_mapKeyState.find(iKey);
	if (iter == m_mapKeyState.end())
	{
		return false;
	}
	return iter->second->bUp;
}

bool CInput::GetAsyncKey(int iKey)
{
	unordered_map<int, PKEYSTATE>::iterator iter = m_mapKeyState.find(iKey);
	if (iter == m_mapKeyState.end())
	{
		return false;
	}
	return iter->second->bPressed;
}

bool CInput::GetKeyDown(int iKey)
{
	if (m_bEnableInput)
	{
		unordered_map<int, PKEYSTATE>::iterator iter = m_mapKeyState.find(iKey);
		if (iter == m_mapKeyState.end())
		{
			return false;
		}
		return iter->second->bDown;
	}
	else
	{
		return false;
	}
}

bool CInput::GetKeyUp(int iKey)
{
	if (m_bEnableInput)
	{
		unordered_map<int, PKEYSTATE>::iterator iter = m_mapKeyState.find(iKey);
		if (iter == m_mapKeyState.end())
		{
			return false;
		}
		return iter->second->bUp;
	}
	else
	{
		return false;
	}
}

bool CInput::GetKey(int iKey)
{
	if (m_bEnableInput)
	{
		unordered_map<int, PKEYSTATE>::iterator iter = m_mapKeyState.find(iKey);
		if (iter == m_mapKeyState.end())
		{
			return false;
		}
		return iter->second->bPressed;
	}
	else
	{
		return false;
	}
}
