#pragma once
#include "../Ref.h"

LMNT_BEGIN

class LMNT_DLL CSceneComponent :
	public CRef
{
protected:
	friend class CScene;

protected:
	CSceneComponent();
	virtual ~CSceneComponent() = 0;

protected:
	class CScene* m_pScene;	

public:
	void SetScene(class CScene* pScene);

public:
	virtual bool Initialize();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual int Collision(float fTime);
	virtual void Render(float fTime);


};

LMNT_END

