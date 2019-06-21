#pragma once
#include "Component.h"

LMNT_USING

class CEditorCursor :
	public CComponent
{
	friend class CGameObject;

private:
	CEditorCursor();
	~CEditorCursor();

private:
	bool m_bOnDrag;
	Vector3 m_vBeginDragPosition;
	Vector3 m_vEndDragPosition;

	list<class CGameObject*> m_CollisionList;
	class CGameObject* m_pDragTarget;

	Vector2 vPrevCursorPosition;
	bool m_bCameraMove;

private:
	bool CheckCollisionList(class CGameObject* pTarget) const;
	void RemoveFromCollisionList(class CGameObject* pTarget);
	class CGameObject* TopObject() const;
public:
	bool Initialize() override;
	int Update(float fTime) override;


};

