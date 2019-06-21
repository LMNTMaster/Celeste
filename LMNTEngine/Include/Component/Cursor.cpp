#include "Cursor.h"
#include "../Device.h"
#include "../Input.h"

LMNT_USING

CCursor::CCursor():
	m_bVisible(true)
{
	m_eComponentType = CT_CURSOR;
	SetTag("Cursor");
}


CCursor::~CCursor()
{
}

void CCursor::SetCursorVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool CCursor::GetCursorVisible() const
{
	return m_bVisible;
}

bool CCursor::Initialize()
{
	m_bVisible = false;
	return true;
}

int CCursor::LateUpdate(float fTime)
{
	Vector2 vCursorPosition = GET_SINGLE(CInput)->GetCursorPosition(SPACE_DEVICE);

	if (vCursorPosition.x < 0 || vCursorPosition.x > DEVICE_RESOLUTION.iWidth ||
		vCursorPosition.y < 0 || vCursorPosition.y > DEVICE_RESOLUTION.iHeight)
	{
		if (!m_bVisible)
		{
			while (ShowCursor(TRUE) < 0)
			{
			}
			m_bVisible = TRUE;
		}
	}

	else
	{
		 while (ShowCursor(FALSE) >= 0);
		 {
		 }
		 m_bVisible = FALSE;
	}

	return 0;
}
