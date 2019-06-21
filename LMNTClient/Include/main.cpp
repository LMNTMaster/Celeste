#include "Core.h"
#include "Client.h"
#include "resource.h"
#include "Scene\Chapter0.h"
#include "Scene\Chapter1.h"
#include "Scene\IntroScene.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Scene\TestScene.h"
#include "GameData.h"

LMNT_USING

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{	
	if (!GET_SINGLE(CCore)->Init(hInstance, RESOLUTION(1280, 720),
		L"LMNT", L"LMNT", IDI_ICON2, IDI_ICON2, true))
	{
		DESTROY_SINGLE(CCore);
		return 0;
	}

	GET_SINGLE(CGameData)->LoadData();

	GET_SINGLE(CCore)->SetRenderMode(RM_2D);

	CScene* pScene = GET_SINGLE(CSceneManager)->CreateScene<CIntroScene>("Intro Scene");
	//CScene* pScene = GET_SINGLE(CSceneManager)->CreateScene<CChapter1>("Main Scene");
	GET_SINGLE(CSceneManager)->SetCurrentScene(pScene);
	SAFE_RELEASE(pScene);

	
	int	iRet = GET_SINGLE(CCore)->Run();	
	DESTROY_SINGLE(CCore);
	DESTROY_SINGLE(CGameData);
	
	return iRet;
}

