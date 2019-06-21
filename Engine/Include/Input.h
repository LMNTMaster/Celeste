#pragma once

#include "Engine.h"

LMNT_BEGIN


typedef struct _tagKeyState
{
	bool bDown;
	bool bPressed;
	bool bUp;

	_tagKeyState() :
		bDown(false),
		bPressed(false),
		bUp(false)
	{
	}

}KEYSTATE, *PKEYSTATE;


class LMNT_DLL CInput
{
	DECLARE_SINGLE(CInput)

private:
	friend class CCore;

private:
	unordered_map<int, PKEYSTATE> m_mapKeyState;
	Vector2 m_vPrevCursorPositionAsWindow;
	Vector2 m_vPrevCursorPositionAsDevice;
	bool m_bEnableInput;
	bool m_bCursorVisible;

	
private:
	void SetWheelState(int iKey);
	void ClearWheelState();	

public:
	bool GetCursorVisible() const;
	void SetCursorVisible(bool bVisible);
	static Vector2 GetCursorPosition(SPACE eSpace);
	Vector2 GetCursorDeltaPosition(SPACE eSpace) const;
	
public:
	void EnableInput();
	void DisableInput();
	bool GetAsyncKeyDown(int iKey);
	bool GetAsyncKeyUp(int iKey);
	bool GetAsyncKey(int iKey);
	bool GetKeyDown(int iKey);
	bool GetKeyUp(int iKey);
	bool GetKey(int iKey);
	void Update();
	bool Initialize();

};


LMNT_END	