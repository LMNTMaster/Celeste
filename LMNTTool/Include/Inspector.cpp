// Inspector.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Inspector.h"
#include "Transform.h"
#include "GameObject.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneManager.h"
#include "Component/Camera.h"
#include "SceneManager.h"
#include "Component.h"
#include "Component/Tile.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "Engine.h"
#include "ResourcesManager.h"
#include "Component/RectCollider2D.h"
#include "Collider.h"
#include "GridRenderer.h"
#include "Transmitter.h"
#include "MainFrm.h"
#include "Component/Text.h"
#include "PathManager.h"

#include "Component/Animator.h"
#include "AnimationClip2D.h"
#include "AnimationState.h"
#include "AnimationController.h"
#include "Transition.h"


using namespace LMNT;

// CInspector

IMPLEMENT_DYNCREATE(CInspector, CFormView)

CInspector::CInspector()
	: CFormView(IDD_DIALOG_FORM),
	m_vPosition(Vector3(0.0f,0.0f,0.0f)),
	m_vRotation(Vector3(0.0f,0.0f,0.0f)),
	m_vScale(Vector3(0.0f,0.0f,0.0f))
	, m_bFixedMove(FALSE)
	, m_iGridSize(32)
	, m_bDrawMode(FALSE)
	, m_iTileIndex(0),
	m_pTargetGameObject(NULL)
	, m_strTagName(_T(""))
	, m_strAtlasKey(_T("")),
	m_pBackgroundMap(NULL),
	m_pBackgroundMap1(NULL),
	m_pBackgroundMap2(NULL),
	m_pBackgroundMap3(NULL),
	m_pBackgroundMap4(NULL),
	m_pBackgroundMap5(NULL),
	m_pBackgroundMap6(NULL),
	m_pBackgroundMap7(NULL),
	m_bShowArea(true),
	m_bShowTerrain(true),
	m_bShowCollider(true),
	m_bShowBackground(true)
{

}

CInspector::~CInspector()
{
	for (int i = 0; i < m_vecPrefab.size(); ++i)
	{
		delete m_vecPrefab[i];
	}
	for (int i = 0; i < m_vecStrawberry.size(); ++i)
	{
		delete m_vecStrawberry[i];
	}


	SAFE_RELEASE(m_pBackgroundMap);
	SAFE_RELEASE(m_pBackgroundMap1);
	SAFE_RELEASE(m_pBackgroundMap2);
	SAFE_RELEASE(m_pBackgroundMap3);
	SAFE_RELEASE(m_pBackgroundMap4);
	SAFE_RELEASE(m_pBackgroundMap5);
	SAFE_RELEASE(m_pBackgroundMap6);
	SAFE_RELEASE(m_pBackgroundMap7);
}

void CInspector::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_POSITIONX, m_vPosition.x);
	DDX_Text(pDX, IDC_EDIT_POSITIONY, m_vPosition.y);
	DDX_Text(pDX, IDC_EDIT_POSITIONZ, m_vPosition.z);
	DDX_Text(pDX, IDC_EDIT_ROTATIONX, m_vRotation.x);
	DDX_Text(pDX, IDC_EDIT_ROTATIONY, m_vRotation.y);
	DDX_Text(pDX, IDC_EDIT_ROTATIONZ, m_vRotation.z);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_vScale.z);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_vScale.z);

	DDX_Check(pDX, IDC_CHECK1, m_bFixedMove);
	DDX_Text(pDX, IDC_EDIT_GRIDSIZE, m_iGridSize);
	DDX_Check(pDX, IDC_CHECK_DRAW_MODE, m_bDrawMode);
	DDX_Text(pDX, IDC_EDIT_TILEINDEX, m_iTileIndex);
	DDX_Control(pDX, IDC_COMBO_TILESET, m_TilesetCombo);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ButtonSave);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_ButtonLoad);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strTagName);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_LayerCombo);
	DDX_Text(pDX, IDC_EDIT_ATLASKEY, m_strAtlasKey);
	DDX_Control(pDX, IDC_COMBO_TILESET_TYPE, m_TileTypeCombo);
}

BEGIN_MESSAGE_MAP(CInspector, CFormView)
	ON_EN_CHANGE(IDC_EDIT_POSITIONX, &CInspector::OnEnChangeEditPositionx)
	ON_EN_CHANGE(IDC_EDIT_POSITIONY, &CInspector::OnEnChangeEditPositiony)
	ON_EN_CHANGE(IDC_EDIT_POSITIONZ, &CInspector::OnEnChangeEditPositionz)
	ON_EN_CHANGE(IDC_EDIT_ROTATIONX, &CInspector::OnEnChangeEditRotationx)
	ON_EN_CHANGE(IDC_EDIT_ROTATIONY, &CInspector::OnEnChangeEditRotationy)
	ON_EN_CHANGE(IDC_EDIT_ROTATIONZ, &CInspector::OnEnChangeEditRotationz)
	ON_EN_CHANGE(IDC_EDIT_SCALEX, &CInspector::OnEnChangeEditScalex)
	ON_EN_CHANGE(IDC_EDIT_SCALEY, &CInspector::OnEnChangeEditScaley)
	ON_EN_CHANGE(IDC_EDIT_SCALEZ, &CInspector::OnEnChangeEditScalez)
	ON_STN_CLICKED(IDC_STATIC_TRANSFORM, &CInspector::OnStnClickedStaticTransform)
	ON_CBN_EDITCHANGE(IDC_COMBO_TILESET, &CInspector::OnCbnEditchangeComboTileset)

	ON_CBN_CLOSEUP(IDC_COMBO_TILESET, &CInspector::OnCbnCloseupComboTileset)
	ON_EN_CHANGE(IDC_EDIT_GRIDSIZE, &CInspector::OnEnChangeEditGridsize)
	ON_BN_CLICKED(IDC_CHECK_DRAW_MODE, &CInspector::OnBnClickedCheckDrawMode)
	ON_EN_CHANGE(IDC_EDIT_TILEINDEX, &CInspector::OnEnChangeEditTileindex)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CInspector::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CInspector::OnBnClickedButtonSave)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CInspector::OnEnChangeEditName)
	ON_CBN_CLOSEUP(IDC_COMBO_LAYER, &CInspector::OnCbnCloseupComboLayer)
	ON_EN_CHANGE(IDC_EDIT_ATLASKEY, &CInspector::OnEnChangeEditAtlaskey)
	ON_CBN_CLOSEUP(IDC_COMBO_TILESET_TYPE, &CInspector::OnCbnCloseupComboTilesetType)
END_MESSAGE_MAP()


// CInspector 진단입니다.

#ifdef _DEBUG
void CInspector::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInspector::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInspector 메시지 처리기입니다.


void CInspector::OnEnChangeEditPositionx()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vPosition);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::OnEnChangeEditPositiony()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vPosition);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::OnEnChangeEditPositionz()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vPosition);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::OnEnChangeEditRotationx()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldRotation(m_vRotation);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::OnEnChangeEditRotationy()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldRotation(m_vRotation);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::OnEnChangeEditRotationz()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldRotation(m_vRotation);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::OnEnChangeEditScalex()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldScale(m_vScale);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::OnEnChangeEditScaley()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldScale(m_vScale);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::OnEnChangeEditScalez()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldScale(m_vScale);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::SetGameObject(LMNT::CGameObject * pTarget)
{
	if (m_pTargetGameObject == pTarget)
	{
		return;
	}
	m_pTargetGameObject = pTarget;
	
	CLayer* pLayer = m_pTargetGameObject->GetLayer();
	CString strLayer = CA2CT(pLayer->GetTag().c_str());
	int idx = m_LayerCombo.FindString(-1, strLayer);
	m_LayerCombo.SetCurSel(idx);
	SAFE_RELEASE(pLayer);

	CString strTag = CA2CT(m_pTargetGameObject->GetTag().c_str());
	m_strTagName = strTag;

	CTransform* pTransform = m_pTargetGameObject->GetTransform();
	m_vPosition = pTransform->GetWorldPosition();
	m_vRotation = pTransform->GetWorldRotation();
	m_vScale = pTransform->GetWorldScale();

	SetCombo();
	UpdateData(FALSE);
	SAFE_RELEASE(pTransform);
}


void CInspector::OnStnClickedStaticTransform()
{
}


void CInspector::OnCbnEditchangeComboTileset()
{
	UpdateData(TRUE);
	CString strTilesetName;
	m_TilesetCombo.GetLBText(m_TilesetCombo.GetCurSel(), strTilesetName);
	string temp = CT2CA(strTilesetName);
	CTile* pTile = m_pTargetGameObject->GetComponentFromType<CTile>(CT_TILE);
	int iCount = pTile->SetTileSetName(temp);
	SetTileIndexCombo(iCount);
	pTile->SetTileIndex(0);
	SAFE_RELEASE(pTile);
}

void CInspector::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_TilesetCombo.AddString(L"bgBrick");
	m_TilesetCombo.AddString(L"bgBrickRuined");
	m_TilesetCombo.AddString(L"bgCliffside");
	m_TilesetCombo.AddString(L"bgCore");
	m_TilesetCombo.AddString(L"bgDirt");
	m_TilesetCombo.AddString(L"bgPool");
	m_TilesetCombo.AddString(L"bgResortStone");
	m_TilesetCombo.AddString(L"bgSnow");
	m_TilesetCombo.AddString(L"bgSummit");
	m_TilesetCombo.AddString(L"bgTempleA");
	m_TilesetCombo.AddString(L"bgTempleB");
	m_TilesetCombo.AddString(L"bgWood");	
	m_TilesetCombo.AddString(L"cement");
	m_TilesetCombo.AddString(L"cliffside");
	m_TilesetCombo.AddString(L"cliffsideAlt");
	m_TilesetCombo.AddString(L"core");
	m_TilesetCombo.AddString(L"deadgrass");
	m_TilesetCombo.AddString(L"dirt");
	m_TilesetCombo.AddString(L"girder");
	m_TilesetCombo.AddString(L"grass");
	m_TilesetCombo.AddString(L"poolEdges");
	m_TilesetCombo.AddString(L"reflection");
	m_TilesetCombo.AddString(L"reflectionAlt");
	m_TilesetCombo.AddString(L"rock");
	m_TilesetCombo.AddString(L"scenery");
	m_TilesetCombo.AddString(L"snow");
	m_TilesetCombo.AddString(L"starJump");
	m_TilesetCombo.AddString(L"stone");
	m_TilesetCombo.AddString(L"summit");
	m_TilesetCombo.AddString(L"summitNoSnow");
	m_TilesetCombo.AddString(L"template");
	m_TilesetCombo.AddString(L"templeA");
	m_TilesetCombo.AddString(L"templeB");
	m_TilesetCombo.AddString(L"tower");
	m_TilesetCombo.AddString(L"wood");
	m_TilesetCombo.AddString(L"woodStoneEdges");


	m_TilesetCombo.SetCurSel(0);



	m_LayerCombo.AddString(L"FG2");
	m_LayerCombo.AddString(L"FG1");
	m_LayerCombo.AddString(L"FG0");
	m_LayerCombo.AddString(L"VFX1");
	m_LayerCombo.AddString(L"VFX0");
	m_LayerCombo.AddString(L"ForeTerrain");
	m_LayerCombo.AddString(L"Default");
	m_LayerCombo.AddString(L"BackTerrain");
	m_LayerCombo.AddString(L"BG5");
	m_LayerCombo.AddString(L"BG4");
	m_LayerCombo.AddString(L"BG3");
	m_LayerCombo.AddString(L"BG2");
	m_LayerCombo.AddString(L"BG1");
	m_LayerCombo.AddString(L"BG0");

	m_TileTypeCombo.AddString(L"Top Edge");
	m_TileTypeCombo.AddString(L"Bottom Edge");
	m_TileTypeCombo.AddString(L"Left Edge");
	m_TileTypeCombo.AddString(L"Right Edge");
	m_TileTypeCombo.AddString(L"Top Single");
	m_TileTypeCombo.AddString(L"Bottom Single");
	m_TileTypeCombo.AddString(L"Left Single");
	m_TileTypeCombo.AddString(L"Right Single");
	m_TileTypeCombo.AddString(L"Single");
	m_TileTypeCombo.AddString(L"Outer Left Top Corner");
	m_TileTypeCombo.AddString(L"Outer Right Top Corner");
	m_TileTypeCombo.AddString(L"Outer Left Bottom Corner");
	m_TileTypeCombo.AddString(L"Outer Right Bottom Corner");
	m_TileTypeCombo.AddString(L"Inner Left Top Corner");
	m_TileTypeCombo.AddString(L"Inner Right Top Corner");
	m_TileTypeCombo.AddString(L"Inner Left Bottom Corner");
	m_TileTypeCombo.AddString(L"Inner Right Bottom Corner");
	m_TileTypeCombo.AddString(L"Inner Block");
	m_TileTypeCombo.AddString(L"None");
}

CGameObject * CInspector::CreateTile()
{

	CGameObject* pReturn = NULL;

	RECT tempRect = {};
	CMainFrame* pMainFrame = GET_SINGLE(CTransmitter)->GetMainFrame();
	pMainFrame->m_pToolView->GetDC()->GetClipBox(&tempRect);

	//카메라 위치에 오브젝트 생성. 타겟으로 설정 함.
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	RECT rc = tempRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Default");

	LMNT::CGameObject* pGameObject = LMNT::CGameObject::CreateObject("Game Object", pLayer);
	CTransform* pTransform = pGameObject->GetTransform();
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
	pTransform->SetWorldScale(32.0f, 32.0f, 1.0f);

	CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>("TileRenderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CTile* pTile = pGameObject->AddComponent<CTile>("Tile");
	CString strTilesetName;
	m_TilesetCombo.GetLBText(m_TilesetCombo.GetCurSel(), strTilesetName);	
	string temp = CT2CA(strTilesetName);
	int iCount = pTile->SetTileSetName(temp);
	//SetTileIndexCombo(iCount);
	//int iIndex = rand() % iCount;

	pTile->SetTileIndex(GetTileIndexFromCombo());
	SAFE_RELEASE(pTile);

	//CCollider* pCollider = pGameObject->AddComponent<CRectCollider2D>("Collider");
	//pCollider->SetScale(pTransform->GetWorldScale());
	//SAFE_RELEASE(pCollider);

	SetGameObject(pGameObject);

	UpdateTransform();
	pReturn = pGameObject;
	SAFE_RELEASE(pTransform);

	//AddTile(pGameObject);
	SAFE_RELEASE(pGameObject);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	return pReturn;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CInspector::OnGameobjectCreategameobject()
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Default");
	CGameObject* pEmptyObject = CGameObject::CreateObject("Game Object", pLayer);
	CTransform* pTransform = pEmptyObject->GetTransform();
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));	
	SAFE_RELEASE(pTransform);
	CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);


	SetGameObject(pEmptyObject);
	UpdateTransform();
	SAFE_RELEASE(pEmptyObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);
}

void CInspector::UpdateTransform()
{
	UpdateData(TRUE);
	CTransform* pTransform = m_pTargetGameObject->GetTransform();
	m_vPosition = pTransform->GetWorldPosition();
	m_vRotation = pTransform->GetWorldRotation();
	m_vScale = pTransform->GetWorldScale();
	SAFE_RELEASE(pTransform);
	UpdateData(FALSE);
}

void CInspector::CummitTransform()
{
	UpdateData(TRUE);
	if (m_pTargetGameObject)
	{
		CTransform* pTransform = m_pTargetGameObject->GetTransform();
		pTransform->SetWorldPosition(m_vPosition);
		pTransform->SetWorldRotation(m_vRotation);
		pTransform->SetWorldScale(m_vScale);
		SAFE_RELEASE(pTransform);
	}
}

void CInspector::SetTileIndexCombo(int iCount)
{
	m_iTileIndex = 0;
	UpdateData(FALSE);
}

void CInspector::SetTileIndexCombo(int iCount, int iIndex)
{
	m_iTileIndex = iIndex;
	UpdateData(FALSE);
}

void CInspector::SetCombo()
{
	CTile* pTile = m_pTargetGameObject->GetComponentFromType<CTile>(CT_TILE);
	if (pTile)
	{
		int iCount = pTile->SetTileSetName(pTile->GetTilesetName());
		CString strTemp = CA2CT(pTile->GetTilesetName().c_str());
		m_TilesetCombo.SetCurSel(m_TilesetCombo.FindString(-1, strTemp));
		SetTileIndexCombo(iCount, pTile->GetTileIndex());
		m_TileTypeCombo.SetCurSel(GetCurselIndexFromTileIndex(pTile->GetTileIndex()));
		SAFE_RELEASE(pTile);
		UpdateData(FALSE);
	}
}

void CInspector::ResetInspector()
{
	if (!m_pTargetGameObject)
	{
		m_vPosition = Vector3::Zero;
		m_vRotation = Vector3::Zero;
		m_vScale = Vector3::Zero;
		m_TilesetCombo.SetCurSel(0);
		m_iTileIndex = 0;
		UpdateData(FALSE);
	}
}



void CInspector::OnCbnCloseupComboTileset()
{
	UpdateData(TRUE);

	CString strTilesetName;
	int iIndex = m_TilesetCombo.GetCurSel();
	m_TilesetCombo.GetLBText(iIndex, strTilesetName);
	string temp = CT2CA(strTilesetName);

	if (m_pTargetGameObject)
	{
		CTile* pTile = m_pTargetGameObject->GetComponentFromType<CTile>(CT_TILE);
		if (pTile)
		{
			int iCount = pTile->SetTileSetName(temp);
			SetTileIndexCombo(iCount);
			pTile->SetTileIndex(m_TilesetCombo.GetCurSel());
		}
		//???
		SAFE_RELEASE(pTile);
	}
}

void CInspector::OnEnChangeEditGridsize()
{
	UpdateData(TRUE);
	GET_SINGLE(CTransmitter)->GetMainFrame()->m_pToolView->m_pGrid->SetGridSize(m_iGridSize);
}


void CInspector::OnBnClickedCheckDrawMode()
{
	UpdateData(TRUE);
}


void CInspector::OnEnChangeEditTileindex()
{
	UpdateData(TRUE);
	CTile* pTile = m_pTargetGameObject->GetComponentFromType<CTile>(CT_TILE);	
	pTile->SetTileIndex(m_iTileIndex);
	SAFE_RELEASE(pTile);
}


void CInspector::OnBnClickedButtonLoad()
{
	CFileDialog	dlg(TRUE, L"stg", L"map", OFN_HIDEREADONLY,
		L"STAGE(*.stg)|*.stg|OBJECT(*object)|*.object|AllFiles(*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		CString	strFullPath = dlg.GetPathName();

		FILE* pFile = NULL;
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer* pLayer = pScene->FindLayer("Default");

		char	astrPath[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, strFullPath, -1,
			astrPath, lstrlen(strFullPath) * 2, 0, 0);

		fopen_s(&pFile, astrPath, "rb");

		if (pFile)
		{
			{
				int iCount = 0;
				fread(&iCount, 4, 1, pFile);
				for (int i = 0; i < iCount; ++i)
				{
					CGameObject* pTile = CGameObject::CreateObject("Tile", pLayer);
					CRenderer* pTileRenderer = pTile->AddComponent<CRenderer>("TileRenderer");
					pTileRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
					pTileRenderer->SetShader(STANDARD_TEXTURE_SHADER);
					pTileRenderer->SetInputLayout(POS_UV_LAYOUT);
					pTileRenderer->SetRenderState(ALPHA_BLEND);
					pTileRenderer->SetRenderMode(RM_2D);

					CMaterial*	pMaterial = pTileRenderer->CreateMaterial();
					pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
					pMaterial->SetSampler(SAMPLER_POINT);
					SAFE_RELEASE(pMaterial);
					SAFE_RELEASE(pTileRenderer);

					CTransform* pTileTransform = pTile->GetTransform();
					pTileTransform->Load(pFile);
					SAFE_RELEASE(pTileTransform);

					CTile* pTileComponent = pTile->AddComponent<CTile>("Tile");
					pTileComponent->Load(pFile);
					SAFE_RELEASE(pTileComponent);
					SAFE_RELEASE(pTile);
				}
			}

			{
				//나머지 오브젝트 저장
				int iCount = 0;
				fread(&iCount, 4, 1, pFile);
				for (int i = 0; i < iCount; ++i)
				{
					int iLength = 0;
					char strTagBuffer[256] = {};
					fread(&iLength, 4, 1, pFile);
					fread(strTagBuffer, 1, iLength, pFile);

					//Layer읽음
					iLength = 0;
					char cLayerBuffer[256] = {};
					fread(&iLength, 4, 1, pFile);
					fread(cLayerBuffer, sizeof(char), iLength, pFile);

					CLayer* pTempLayer = pScene->FindLayer(string(cLayerBuffer));
					CGameObject* pObject = CGameObject::CreateObject(string(strTagBuffer), pTempLayer);
					SAFE_RELEASE(pTempLayer);

					CTransform* pTransform = pObject->GetTransform();
					pTransform->Load(pFile);
					SAFE_RELEASE(pTransform);

					CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
					pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
					pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
					pRenderer->SetInputLayout(POS_UV_LAYOUT);
					pRenderer->SetRenderState(ALPHA_BLEND);
					pRenderer->SetRenderMode(RM_2D);

					CMaterial*	pMaterial = pRenderer->CreateMaterial();
					pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
					pMaterial->SetSampler(SAMPLER_POINT);
					SAFE_RELEASE(pMaterial);

					char strAtlasKey[256] = {};
					iLength = 0;
					fread(&iLength, 4, 1, pFile);
					fread(strAtlasKey, sizeof(char), iLength, pFile);
					pRenderer->SetImageInfo(string(strAtlasKey));
					SAFE_RELEASE(pRenderer);
					SAFE_RELEASE(pObject);
				}
			}
			//구역 읽기
			{
				int iSize = 0;
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					Vector4 vArea;
					fread(&vArea, sizeof(Vector4), 1, pFile);

					CGameObject* pArea = CreateArea();
					CTransform* pAreaTransform = pArea->GetTransform();
					pAreaTransform->SetWorldPosition(vArea.x, vArea.y, 0.0f);
					pAreaTransform->SetWorldScale(vArea.z, vArea.w, 1.0f);

					UpdateTransform();
					CummitTransform();
					SAFE_RELEASE(pAreaTransform);
				}
			}
			
			
			//충돌체 읽기
			{
				int iSize = 0;
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					int iLength = 0;
					char cTagBuffer[256] = {};

					fread(&iLength, 4, 1, pFile);
					fread(&cTagBuffer, iLength, 1, pFile);
					Vector4 vArea;
					fread(&vArea, sizeof(Vector4), 1, pFile);

					CGameObject* pArea = CreateCollider(cTagBuffer);
					CTransform* pAreaTransform = pArea->GetTransform();
					pAreaTransform->SetWorldPosition(vArea.x, vArea.y, 0.0f);
					pAreaTransform->SetWorldScale(vArea.z, vArea.w, 1.0f);

					UpdateTransform();
					CummitTransform();
					SAFE_RELEASE(pAreaTransform);
				}
			}


			//가시 읽기
			{
				int iSize = m_vecDanger.size();
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					DIRECTION eDirection;
					fread(&eDirection, sizeof(DIRECTION), 1, pFile);

					CGameObject* pDanger = CreateDanger(eDirection);
					CTransform* pTransform = pDanger->GetTransform();
					pTransform->Load(pFile);
					SAFE_RELEASE(pTransform);
					UpdateTransform();
					CummitTransform();
				}
			}

			//Jumpthru 읽기
			{
				int iSize = m_vecDanger.size();
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					CGameObject* pJumpthru = CreateJumpthru();
					CTransform* pTransform = pJumpthru->GetTransform();
					pTransform->Load(pFile);
					SAFE_RELEASE(pTransform);

					int iLength = 0;
					char cBuffer[256] = {};

					fread(&iLength, 4, 1, pFile);
					fread(cBuffer, 1, iLength, pFile);
					string strAtlasKey(cBuffer);

					CRenderer* pRenderer = pJumpthru->GetComponentFromType<CRenderer>(CT_RENDERER);
					pRenderer->SetImageInfo(strAtlasKey);
					m_strAtlasKey = CA2CT(cBuffer);
					SAFE_RELEASE(pRenderer);

					UpdateTransform();
					CummitTransform();
				}
			}

			//Prefab
			{
				int iSize = 0;
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					//파일 명 읽음
					int iLength = 0;
					fread(&iLength, 4, 1, pFile);
					char cBuffer[512] = {};
					fread(cBuffer, iLength, 1, pFile);					

					//레이어 읽음
					char cLayerBuffer[512] = {};
					fread(&iLength, 4, 1, pFile);
					fread(cLayerBuffer, iLength, 1, pFile);
					CLayer* pTempLayer = pScene->FindLayer(cLayerBuffer);

					//Ref 안오름
					CGameObject* pPrefab = LoadPrefab(cBuffer, pTempLayer);
					
					SAFE_RELEASE(pTempLayer);

					CTransform* pTransform = pPrefab->GetTransform();
					pTransform->Load(pFile);
					SAFE_RELEASE(pTransform);
				}
			}


			//Spring
			{
				int iSize = 0;
				fread(&iSize, 4, 1, pFile);
			
				for (int i = 0; i < iSize; ++i)
				{
					CGameObject* pGameObject = CreateSpring();
					CTransform* pSpringTransform = pGameObject->GetTransform();
					pSpringTransform->Load(pFile);
					SAFE_RELEASE(pSpringTransform);
				}
			
			}

			//CheckPoint
			{
				int iSize = 0;
				fread(&iSize, 4, 1, pFile);
			
				for (int i = 0; i < iSize; ++i)
				{
					Vector3 vPosition;
					fread(&vPosition, sizeof(Vector3), 1, pFile);
					CGameObject* pCheckPoint = CreateCheckPoint();
					CTransform* pCheckPointTransform = pCheckPoint->GetTransform();
					pCheckPointTransform->SetWorldPosition(vPosition);
					SAFE_RELEASE(pCheckPointTransform);
				}
			}

			//Refill
			{
				int iSize = 0;
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					CGameObject* pGameObject = CreateRefillCristal();
					CTransform* pTransform = pGameObject->GetTransform();
					Vector3 position;
					fread(&position, sizeof(Vector3), 1, pFile);
					pTransform->SetWorldPosition(position);
					SAFE_RELEASE(pTransform);
				}
			}

			//Strawberry
			{
				int iSize = 0;
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					BERRY_TYPE eType;
					fread(&eType, sizeof(BERRY_TYPE), 1, pFile);
					Vector3 position;
					fread(&position, sizeof(Vector3), 1, pFile);
					CGameObject* pBerry = CreateStrawberry(eType);
					CTransform* pBerryTransform = pBerry->GetTransform();
					pBerryTransform->SetWorldPosition(position);
					SAFE_RELEASE(pBerryTransform);
				}
			}

			fclose(pFile);
		}
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

}


void CInspector::OnBnClickedButtonSave()
{
	CFileDialog   dlg(FALSE, L"stg", L"map", OFN_OVERWRITEPROMPT,
		L"STAGE(*.stg)|*.stg|OBJECT(*object)|*.object|AllFiles(*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		CString   strFullPath = dlg.GetPathName();

		FILE* pFile = NULL;	
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		

		char	astrPath[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, strFullPath, -1,
			astrPath, lstrlen(strFullPath) * 2, 0, 0);
		
		fopen_s(&pFile, astrPath, "wb");
		
		if(pFile)
		{

			list<CGameObject*> TileObjectList;
			list<CGameObject*> OtherObjectList;
			{
				int iLayerCount = pScene->GetLayerCount();
				for (int i = 0; i < iLayerCount; ++i)
				{
					CLayer* pIterLayer = pScene->FindLayer(i);
					string pLayerTag = pIterLayer->GetTag();
					if (pLayerTag == "Grid" || pLayerTag == "Area" || pLayerTag == "Collider" || pLayerTag == "BackgroundMap")
					{
						SAFE_RELEASE(pIterLayer);
						continue;
					}

					//오브젝트중 타일만 뽑아둔다.
					list<CGameObject*>::const_iterator iter = pIterLayer->GetAllGameObjects()->begin();
					list<CGameObject*>::const_iterator iterEnd = pIterLayer->GetAllGameObjects()->end();

					for (; iter != iterEnd; ++iter) 
					{
						CTransform* pIterTransform = (*iter)->GetTransform();
						CTransform* pParentTransform = pIterTransform->GetParent();
						string strParentTag;
						if (pParentTransform)
						{
							strParentTag = pParentTransform->GetGameObject()->GetTag();
						}
						SAFE_RELEASE(pIterTransform);

						if ((*iter)->CheckComponentFromType(CT_TILE) && !IsPrefab(strParentTag))
						{
							TileObjectList.push_back((*iter));
						}
						else
						{
							//커서랑 그리드는 저장하지 않음.
							if ((*iter)->GetTag() == "Cursor")
								continue;
							else if ((*iter)->GetTag() == "Grid")
								continue;
							else if ((*iter)->GetTag() == "Jumpthru")
								continue;
							else if ((*iter)->GetTag().find("Spike") != string::npos)
								continue;
							else if ((*iter)->GetTag() == "Crumble")
								continue;
							else if ((*iter)->GetTag() == "Spring")
								continue;
							else if ((*iter)->GetTag() == "CheckPoint")
								continue;
							else if ((*iter)->GetTag() == "Refill")
								continue;
							else if ((*iter)->GetTag().find("berry") != string::npos)
								continue;
							else if (IsPrefab((*iter)->GetTag()))
								continue;
							else if (IsPrefab(strParentTag))
								continue;
							else
								OtherObjectList.push_back((*iter));
						}
					}
					SAFE_RELEASE(pIterLayer);
				}
			}

			{
				int iCount = TileObjectList.size();
				//타일 갯수 저장
				fwrite(&iCount, 4, 1, pFile);
				list<CGameObject*>::iterator iter;
				list<CGameObject*>::iterator iterEnd = TileObjectList.end();
				for (iter = TileObjectList.begin(); iter != iterEnd; ++iter)
				{
					CTransform* pTileTransform = (*iter)->GetTransform();
					pTileTransform->Save(pFile);
					SAFE_RELEASE(pTileTransform);

					CTile* pTile = (*iter)->GetComponentFromType<CTile>(CT_TILE);
					pTile->Save(pFile);
					SAFE_RELEASE(pTile);
				}
			}

			{
				//오브젝트 저장
				int iCount = OtherObjectList.size();
				fwrite(&iCount, 4, 1, pFile);
				list<CGameObject*>::iterator iter;
				list<CGameObject*>::iterator iterEnd = OtherObjectList.end();
				for (iter = OtherObjectList.begin(); iter != iterEnd; ++iter)
				{
					//이름 저장
					string strTag = (*iter)->GetTag();
					int iLength = strTag.length();
					fwrite(&iLength, 4, 1, pFile);
					fwrite(strTag.c_str(), sizeof(char), iLength, pFile);
					

					//레이어 저장
					CLayer* pLayer = (*iter)->GetLayer();
					string strLayer = pLayer->GetTag();
					
					iLength = strLayer.length();
					fwrite(&iLength, 4, 1, pFile);
					fwrite(strLayer.c_str(), sizeof(char), iLength, pFile);
					SAFE_RELEASE(pLayer);

					CTransform* pTransform = (*iter)->GetTransform();
					pTransform->Save(pFile);
					SAFE_RELEASE(pTransform);

					CRenderer* pRenderer = (*iter)->GetComponentFromType<CRenderer>(CT_RENDERER);
					string strKey = pRenderer->GetAtlasKey();
					iLength = strKey.length();
					fwrite(&iLength, 4, 1, pFile);
					fwrite(strKey.c_str(), sizeof(char), iLength, pFile);
					SAFE_RELEASE(pRenderer);
				}
			}


			//구역 저장
			{
				int iSize = m_vecArea.size();
				fwrite(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pTransform = m_vecArea[i]->GetTransform();
					Vector4 vArea;
					Vector3 vPosition = pTransform->GetWorldPosition();
					Vector3 vScale = pTransform->GetWorldScale();

					vArea.x = vPosition.x;
					vArea.y = vPosition.y;
					vArea.z = vScale.x;
					vArea.w =vScale.y;

					fwrite(&vArea, sizeof(Vector4), 1, pFile);
					SAFE_RELEASE(pTransform);
				}
			}


			//타일 충돌체 저장
			//{
			//	int iAAAASize = 0;
			//	fwrite(&iAAAASize, 4, 1, pFile);
			//}

			{
				//Prefab이 아닌 오브젝트를 거른 갯수
				int iNotPrefab = m_vecCollider.size();
				int iSize = m_vecCollider.size();
				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pTransform = m_vecCollider[i]->GetTransform();
					CTransform* pParentTransform = pTransform->GetParent();
					string strParentTag;
					if (pParentTransform)
					{
						strParentTag = pParentTransform->GetGameObject()->GetTag();
					}
			
					if (IsPrefab(strParentTag))
					{
						--iNotPrefab;
					}
					SAFE_RELEASE(pTransform);
				}
			
				fwrite(&iNotPrefab, 4, 1, pFile);
			
				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pTransform = m_vecCollider[i]->GetTransform();
					CTransform* pParentTransform = pTransform->GetParent();
					string strParentTag;
					if (pParentTransform)
					{
						strParentTag = pParentTransform->GetGameObject()->GetTag();
					}
			
					if (IsPrefab(strParentTag))
					{
						SAFE_RELEASE(pTransform);
						continue;
					}
			
					string strTag = m_vecCollider[i]->GetTag();
					int iLength = strTag.length();

					Vector4 vArea;
					Vector3 vPosition = pTransform->GetWorldPosition();
					Vector3 vScale = pTransform->GetWorldScale();
			
					vArea.x = vPosition.x;
					vArea.y = vPosition.y;
					vArea.z = vScale.x;
					vArea.w = vScale.y;
					fwrite(&iLength, 4, 1, pFile);
					fwrite(strTag.c_str(), iLength, 1, pFile);			
					fwrite(&vArea, sizeof(Vector4), 1, pFile);
					SAFE_RELEASE(pTransform);
				}
			}



			//가시 저장
			{
				int iSize = m_vecDanger.size();
				int iNotPrefab = iSize;
				//Prefab인 가시는 저장하지 않음
				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pTransform = m_vecDanger[i]->pObject->GetTransform();
					CTransform* pTempParentTransform = pTransform->GetParent();

					if (pTempParentTransform)
					{
						--iNotPrefab;
					}
					SAFE_RELEASE(pTransform);
				}

				fwrite(&iNotPrefab, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pTempTransform = m_vecDanger[i]->pObject->GetTransform();
					CTransform* pTempParentTransform = pTempTransform->GetParent();

					if (pTempParentTransform)
					{
						SAFE_RELEASE(pTempTransform);
						continue;
					}
					SAFE_RELEASE(pTempTransform);
					

					fwrite(&m_vecDanger[i]->eDirection, sizeof(DIRECTION), 1, pFile);
					CTransform* pTransform = m_vecDanger[i]->pObject->GetTransform();
					pTransform->Save(pFile);
					SAFE_RELEASE(pTransform);
				}			
			}

			//Jumpthru 추가
			{
				int iSize = m_vecJumpthru.size();
				fwrite(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pTransform = m_vecJumpthru[i]->GetTransform();
					pTransform->Save(pFile);
					SAFE_RELEASE(pTransform);
					CRenderer* pRenderer = m_vecJumpthru[i]->GetComponentFromType<CRenderer>(CT_RENDERER);
					string strAtlasKey = pRenderer->GetAtlasKey();
					SAFE_RELEASE(pRenderer);
					int iLength = strAtlasKey.length();
					fwrite(&iLength, 4, 1, pFile);
					fwrite(strAtlasKey.c_str(), 1, iLength, pFile);
				}
			}

			//Prefab
			{
				int iSize = m_vecPrefab.size();
				fwrite(&iSize, 4, 1, pFile);
			
				for (int i = 0; i < iSize; ++i)
				{
					int iLength = m_vecPrefab[i]->strFileName.length();
					fwrite(&iLength, 4, 1, pFile);
					fwrite(m_vecPrefab[i]->strFileName.c_str(), iLength, 1, pFile);
			
					CLayer* pObjectLayer = m_vecPrefab[i]->pObject->GetLayer();
					iLength = pObjectLayer->GetTag().length();
					fwrite(&iLength, 4, 1, pFile);
					fwrite(pObjectLayer->GetTag().c_str(), iLength, 1, pFile);
					SAFE_RELEASE(pObjectLayer);
			
					CTransform* pTransform = m_vecPrefab[i]->pObject->GetTransform();
					pTransform->Save(pFile);
					SAFE_RELEASE(pTransform);
				}
			}


			//Spring 저장
			{
				int iSize = m_vecSpring.size();
				fwrite(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pTransform = m_vecSpring[i]->GetTransform();
					pTransform->Save(pFile);
					SAFE_RELEASE(pTransform);
				}
			}

			{
				int iSize = m_vecCheckPoint.size();
				fwrite(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pCheckPoint = m_vecCheckPoint[i]->GetTransform();
					Vector3 position = pCheckPoint->GetWorldPosition();
					SAFE_RELEASE(pCheckPoint);
					fwrite(&position, sizeof(Vector3), 1, pFile);
				}
			}

			//Refill
			{
				int iSize = m_vecRefill.size();
				fwrite(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					CTransform* pRefill = m_vecRefill[i]->GetTransform();
					Vector3 position = pRefill->GetWorldPosition();
					SAFE_RELEASE(pRefill);
					fwrite(&position, sizeof(Vector3), 1, pFile);
				}
			}

			//Strawberry
			{
				int iSize = m_vecStrawberry.size();
				fwrite(&iSize, 4, 1, pFile);
			
				for (int i = 0; i < iSize; ++i)
				{
					fwrite(&m_vecStrawberry[i]->eType, sizeof(BERRY_TYPE), 1, pFile);
					CTransform* pBerry = m_vecStrawberry[i]->pGameObject->GetTransform();
					Vector3 position = pBerry->GetWorldPosition();
					SAFE_RELEASE(pBerry);
					fwrite(&position, sizeof(Vector3), 1, pFile);
				}
			}

			fclose(pFile);
		}	
	SAFE_RELEASE(pScene);

	}
}



//이름 변경 시 변경
void CInspector::OnEnChangeEditName()
{
	UpdateData(TRUE);

	string strTag = CT2CA(m_strTagName);

	if (m_pTargetGameObject)
	{
		m_pTargetGameObject->SetTag(strTag);
	}
}

//레이어 선택 시 적용됨
void CInspector::OnCbnCloseupComboLayer()
{
	UpdateData(TRUE);

	if (m_pTargetGameObject)
	{
		if (m_LayerCombo.GetCurSel() != -1)
		{
			CString strLayer;
			m_LayerCombo.GetLBText(m_LayerCombo.GetCurSel(), strLayer);
			string astrLayer = CT2CA(strLayer);

			CScene* pScene = m_pTargetGameObject->GetScene();
			CLayer* pLayer = pScene->FindLayer(astrLayer);
			CLayer* pObjectLayer = m_pTargetGameObject->GetLayer();
		
			if (pLayer->GetTag() != pObjectLayer->GetTag())
			{
				m_pTargetGameObject->ChangeLayer(pLayer);
			}
			SAFE_RELEASE(pObjectLayer);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
		}
	}

}


void CInspector::OnEnChangeEditAtlaskey()
{
	UpdateData(TRUE);

	if (m_pTargetGameObject)
	{
		CRenderer* pRenderer = m_pTargetGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer)
		{
			string strKey = CT2CA(m_strAtlasKey);
			pRenderer->SetImageInfo(strKey);
			PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData(strKey);
		
			if (pData)
			{
				CTransform* pTransform = m_pTargetGameObject->GetTransform();
				pTransform->SetWorldScale(pData->wWidth * 4, pData->wHeight * 4, 1.0f);
				SAFE_RELEASE(pTransform);
				UpdateTransform();
			}			
		}
		SAFE_RELEASE(pRenderer);
	}

	UpdateData(FALSE);
}


//타일 방향 적용
void CInspector::OnCbnCloseupComboTilesetType()
{
	int idx = GetTileIndexFromCombo();
	if (m_pTargetGameObject)
	{
		CTile* pTile = m_pTargetGameObject->GetComponentFromType<CTile>(CT_TILE);
		pTile->SetTileIndex(idx);
		SAFE_RELEASE(pTile);
	}
	m_iTileIndex = idx;
	UpdateData(FALSE);
}

//void CInspector::AddTile(CGameObject * pTarget)
//{
//	CTransform* pTransform = pTarget->GetTransform();
//	Vector3 position = pTransform->GetWorldPosition();
//	SAFE_RELEASE(pTransform);
//
//	POINT tPos = { (int)position.x, (int)position.y };
//
//	m_mapTile.insert(make_pair(tPos, pTarget));
//}
//
//void CInspector::RemoveTile( int iX, int iY)
//{
//	POINT tPos = { iX, iY };
//	unordered_map<POINT, CGameObject*>::iterator iter = m_mapTile.find(tPos);
//
//	if (iter == m_mapTile.end())
//		return;
//
//	m_mapTile.erase(iter);
//}
//
//CGameObject * CInspector::FindIndex(int iX, int iY)
//{
//	POINT tPos = { iX, iY };
//	unordered_map<POINT, CGameObject*>::iterator iter = m_mapTile.find(tPos);
//	if (iter == m_mapTile.end())
//		return NULL;
//
//	return iter->second;
//}
//
//int CInspector::CalculateTile(CGameObject * pTarget)
//{
//	int iTargetIndex = 0;
//
//	CTransform* pTargetTransform = pTarget->GetTransform();
//	Vector3 position	 = pTargetTransform->GetWorldPosition();
//	POINT tPos = { (int)position.x, (int)position.y };
//	SAFE_RELEASE(pTargetTransform);
//
//	CGameObject* pL = FindIndex(tPos.x - m_iGridSize, tPos.y);
//	CGameObject* pT = FindIndex(tPos.x , tPos.y - m_iGridSize);
//	CGameObject* pR = FindIndex(tPos.x + m_iGridSize, tPos.y);
//	CGameObject* pB = FindIndex(tPos.x , tPos.y + m_iGridSize);
//
//	//주변의 모든 타일이 존재하지 않을 때
//	if (pL == NULL && pT == NULL && pR == NULL && pB == NULL)
//	{
//		return GetSingle();
//	}
//	else if (pL != NULL && pT == NULL && pR == NULL && pB == NULL)
//	{
//		return GetRightSingle();
//	}
//	else if (pL == NULL && pT != NULL && pR == NULL && pB == NULL)
//	{
//		return GetBottomSingle();
//	}
//	else if (pL == NULL && pT == NULL && pR != NULL && pB == NULL)
//	{
//		return GetLeftSingle();
//	}
//	else if (pL == NULL && pT == NULL && pR == NULL && pB != NULL)
//	{
//		return GetTopSingle();
//	}
//}
//
//void CInspector::ChangeIndex(CGameObject * pTarget, int iOriginX, int iOriginY)
//{
//	RemoveTile(iOriginX, iOriginY);
//	AddTile(pTarget);
//}

int CInspector::GetTopEdge()
{
	int Temp[4] = { 0,1,2,3 };
	return Temp[rand() % 4];
}

int CInspector::GetBottomEdge()
{
	int Temp[4] = { 6,7,8,9 };
	return Temp[rand() % 4];
}

int CInspector::GetLeftEdge()
{
	int Temp[4] = { 12,13,14,15 };
	return Temp[rand() % 4];
}

int CInspector::GetRightEdge()
{
	int Temp[4] = {18,19,20,21};
	return Temp[rand() % 4];
}

int CInspector::GetTopSingle()
{
	int Temp[4] = {36,37,38,39};
	return Temp[rand() % 4];
}

int CInspector::GetBottomSingle()
{
	int Temp[4] = {42,43,44,45};
	return Temp[rand() % 4];
}

int CInspector::GetLeftSingle()
{
	int Temp[4] = {48,49,50,51};
	return Temp[rand() % 4];
}

int CInspector::GetRightSingle()
{
	int Temp[4] = {54,55,56,57};
	return Temp[rand() % 4];
}

int CInspector::GetSingle()
{
	int Temp[4] = {60,61,62,63};
	return Temp[rand() % 4];
}

int CInspector::GetOuterLeftTopCorner()
{
	int Temp[4] = {66,67,68,69};
	return Temp[rand() % 4];
}

int CInspector::GetOuterRightTopCorner()
{
	int Temp[4] = {72,73,74,75};
	return Temp[rand() % 4];
}

int CInspector::GetOuterLeftBottomCorner()
{
	int Temp[4] = {78,79,80,81};
	return Temp[rand() % 4];
}

int CInspector::GetOuterRightBottomCorner()
{
	int Temp[4] = {84,85,86,87};
	return Temp[rand() % 4];
}

int CInspector::GetInnerLeftTopCorner()
{
	return 22;
}

int CInspector::GetInnerRightTopCorner()
{
	return 10;
}

int CInspector::GetInnerLeftBottomCorner()
{
	return 16;
}

int CInspector::GetInnerRightBottomCorner()
{
	return 4;
}

int CInspector::GetInnerBlock()
{
	int Temp[12] = {5, 11, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71};
	return Temp[rand() % 12];
}

int CInspector::GetEmptyBlock()
{
	int Temp[3] = {77, 83, 89};
	return Temp[rand() % 3];
}

bool CInspector::TopEdge(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 0 || idx == 1 || idx == 2 || idx == 3)
	{
		result = true;
	}	
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::BottomEdge(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 6 || idx == 7 || idx == 8 || idx == 9)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::LeftEdge(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 12 || idx == 13 || idx == 14 || idx == 15)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::RightEdge(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 18 || idx == 19 || idx == 20 || idx == 21)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::TopSingle(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 36 || idx == 37 || idx == 38 || idx == 39)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::BottomSingle(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 42 || idx == 43 || idx == 44 || idx == 45)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::LeftSingle(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 48 || idx == 49 || idx == 50 || idx == 51)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::RightSingle(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 54 || idx == 55 || idx == 56 || idx == 57)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::Single(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 60 || idx == 61 || idx == 62 || idx == 63)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::OuterLeftTopCorner(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 66 || idx == 67 || idx == 68 || idx == 69)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::OuterRightTopCorner(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 72 || idx == 73 || idx == 74 || idx == 75)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::OuterLeftBottomCorner(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 78 || idx == 79 || idx == 80 || idx == 81)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::OuterRightBottomCorner(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 84 || idx == 85 || idx == 86 || idx == 87)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::InnerLeftTopCorner(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 22)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::InnerRightTopCorner(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 10)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::InnerLeftBottomCorner(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 16)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::InnerRightBottomCorner(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 4)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::InnerBlock(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 5 || idx == 11 || idx == 17 || idx == 23 || idx == 29 || idx == 35 || 
		idx == 41 || idx == 47 || idx == 53 || idx == 59 || idx == 65 || idx == 71)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

bool CInspector::EmptyBlock(CGameObject * pTarget)
{
	if (!pTarget)
		return false;

	bool result = false;
	CTile* pTile = pTarget->GetComponentFromType<CTile>(CT_TILE);
	int idx = pTile->GetTileIndex();
	if (idx == 77 || idx == 83 || idx == 89)
	{
		result = true;
	}
	SAFE_RELEASE(pTile);

	return result;
}

int CInspector::GetCurselIndexFromTileIndex(int idx)
{

	if (idx == 0 || idx == 1 || idx == 2 || idx == 3)
	{
	return 0;
	}
	else if (idx == 6 || idx == 7 || idx == 8 || idx == 9)
	{
	//Bottom Edge
	return 1;
	}
	else if (idx == 12 || idx == 13 || idx == 14 || idx == 15)
	{
	//Left Edge
	return 2;
	}
	else if (idx == 18 || idx == 19 || idx == 20 || idx == 21)
	{
	//Right Edge
	return 3;
	}
	else if (idx == 36 || idx == 37 || idx == 38 || idx == 39)
	{
	//Top Single
	return 4;
	}
	else if (idx == 42 || idx == 43 || idx == 44 || idx == 45)
	{
	//Bottom Single
	return 5;
	}

	else if (idx == 48 || idx == 49 || idx == 50 || idx == 51)
	{
	//Left Single
	return 6;
	}
	else if (idx == 54 || idx == 55 || idx == 56 || idx == 57)
	{
	//Right Single
	return 7;
	}
	else if (idx == 60 || idx == 61 || idx == 62 || idx == 63)
	{
	//Single
	return 8;
	}
	else if (idx == 66 || idx == 67 || idx == 68 || idx == 69)
	{
	//Outer Left Top
	return 9;
	}
	else if (idx == 72 || idx == 73 || idx == 74 || idx == 75)
	{
	//Outer Right Top
	return 10;
	}
	else if (idx == 78 || idx == 79 || idx == 80 || idx == 81)
	{
	//Outer Left Bottom
	return 11;
	}
	else if (idx == 84 || idx == 85 || idx == 86 || idx == 87)
	{
	//Outer Right Bottom
	return 12;
	}
	else if (idx == 22)
	{
	// Inner Left Top
	return 13;
	}
	else if (idx == 10)
	{
	// Inner Right Top
	return 14;
	}
	else if (idx == 16)
	{
	// Inner Left Bottom
	return 15;
	}
	else 	if (idx == 4)
	{
	// Inner Right Bottom
	return 16;
	}
	else if (idx == 5 || idx == 11 || idx == 17 || idx == 23 || idx == 29 || idx == 35 ||
	idx == 41 || idx == 47 || idx == 53 || idx == 59 || idx == 65 || idx == 71)
	{
	// Inner
	return 17;
	}
	else if (idx == 77 || idx == 83 || idx == 89)
	{
	// Empty
	return 18;
	}
	else
	return -1;
}

int CInspector::GetTileIndexFromCombo()
{
	UpdateData(TRUE);

	int idx = m_TileTypeCombo.GetCurSel();
	if (idx == -1)
	{
		m_TileTypeCombo.SetCurSel(0);

		return GetTopEdge();
	}
	
	if (idx == 0)
		return GetTopEdge();	
	else if (idx == 1)
		return GetBottomEdge();
	else if (idx == 2)
		return GetLeftEdge();
	else if (idx == 3)
		return GetRightEdge();
	else if (idx == 4)
		return GetTopSingle();
	else if (idx == 5)
		return GetBottomSingle();
	else if (idx == 6)
		return GetLeftSingle();
	else if (idx == 7)
		return GetRightSingle();
	else if (idx == 8)
		return GetSingle();
	else if (idx == 9)
		return GetOuterLeftTopCorner();
	else if (idx == 10)
		return GetOuterRightTopCorner();
	else if (idx == 11)
		return GetOuterLeftBottomCorner();
	else if (idx == 12)
		return GetOuterRightBottomCorner();
	else if (idx == 13)
		return GetInnerLeftTopCorner();
	else if (idx == 14)
		return GetInnerRightTopCorner();
	else if (idx == 15)
		return GetInnerLeftBottomCorner();
	else if (idx == 16)
		return GetInnerRightBottomCorner();
	else if (idx == 17)
		return GetInnerBlock();
	else
		return GetEmptyBlock();
	
}


class CGameObject* CInspector::CreateArea()
{
	CGameObject* pReturn = NULL;
	int iIndex = m_vecArea.size();
	char cIndex[8] = {};
	itoa(iIndex, cIndex, 10);
	
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Area");
	CGameObject* pEmptyObject = CGameObject::CreateObject(string("Area" + string(cIndex)), pLayer);
	CTransform* pTransform = pEmptyObject->GetTransform();
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
	pTransform->SetWorldScale(1280.0f, 720.0f, 1.0f);
	SAFE_RELEASE(pTransform);
	CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Area", L"Celeste\\Area.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	
	int iEnumerationalColor = iIndex % 4;
	if (iEnumerationalColor == 0)
		pRenderer->SetDiffuse(Vector4(0.25f, 0.25f, 0.75f, 0.5f));
	else if (iEnumerationalColor == 1)
		pRenderer->SetDiffuse(Vector4(0.25f, 0.75f, 0.75f, 0.5f));
	else if (iEnumerationalColor == 2)
		pRenderer->SetDiffuse(Vector4(0.25f, 0.75f, 0.25f, 0.5f));
	else if (iEnumerationalColor == 3)
		pRenderer->SetDiffuse(Vector4(1.0f, 0.75f, 0.0f, 0.5f));
	
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CText* pText = pEmptyObject->AddComponent<CText>("Text");	
	pText->SetText(wstring(L"Area ") + wstring(CA2W(cIndex)));
	SAFE_RELEASE(pText);

	SetGameObject(pEmptyObject);
	UpdateTransform();

	m_vecArea.push_back(pEmptyObject);
	pReturn = pEmptyObject;
	SAFE_RELEASE(pEmptyObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);

	return pReturn;
}

CGameObject * CInspector::CreateCollider()
{
	CGameObject* pReturn = NULL;
	int iIndex = m_vecCollider.size();
	char cIndex[8] = {};
	itoa(iIndex, cIndex, 10);

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Collider");
	CGameObject* pEmptyObject = CGameObject::CreateObject(string("Collider" + string(cIndex)), pLayer);
	CTransform* pTransform = pEmptyObject->GetTransform();
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
	pTransform->SetWorldScale(64.0f, 64.0f, 1.0f);
	SAFE_RELEASE(pTransform);
	CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Collider", L"Celeste\\Area.png");
	pMaterial->SetSampler(SAMPLER_POINT);

	pRenderer->SetDiffuse(Vector4(0.25f, 0.75f, 0.25f, 0.5f));

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	//CText* pText = pEmptyObject->AddComponent<CText>("Text");
	//pText->SetText(wstring(L"Collider ") + wstring(CA2W(cIndex)));
	//SAFE_RELEASE(pText);

	SetGameObject(pEmptyObject);
	UpdateTransform();

	m_vecCollider.push_back(pEmptyObject);
	pReturn = pEmptyObject;
	SAFE_RELEASE(pEmptyObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);

	return pReturn;
}

CGameObject * CInspector::CreateCollider(const string & strName)
{
	CGameObject* pReturn = NULL;
	int iIndex = m_vecCollider.size();
	char cIndex[8] = {};
	itoa(iIndex, cIndex, 10);

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Collider");
	CGameObject* pEmptyObject = CGameObject::CreateObject(strName, pLayer);
	CTransform* pTransform = pEmptyObject->GetTransform();
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
	pTransform->SetWorldScale(64.0f, 64.0f, 1.0f);
	SAFE_RELEASE(pTransform);
	CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Collider", L"Celeste\\Area.png");
	pMaterial->SetSampler(SAMPLER_POINT);

	if (strName == "Dirt")
	{
		pRenderer->SetDiffuse(Vector4(1.0f, 0.4f, 0.0f, 0.5f));
	}
	else if (strName == "SnowSoft")
	{
		pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else if (strName == "Asphalt")
	{
		pRenderer->SetDiffuse(Vector4(0.5f, 0.5f, 0.5f, 0.5f));
	}
	else if (strName == "WoodWalkWay")
	{
		pRenderer->SetDiffuse(Vector4(1.0f, 0.4f, 0.0f, 0.5f));
	}
	else if (strName == "Brick")
	{
		pRenderer->SetDiffuse(Vector4(0.0f, 0.4f, 0.0f, 0.5f));
	}
	else if (strName == "MetalGirder")
	{
		pRenderer->SetDiffuse(Vector4(0.0f, 0.0f, 0.0f, 0.5f));
	}
	else if (strName == "ZipMover")
	{
		pRenderer->SetDiffuse(Vector4(0.5f, 0.5f, 0.5f, 0.5f));
	}
	else if (strName == "DeadZone")
	{
		pRenderer->SetDiffuse(Vector4(1.0f, 0.0f, 0.0f, 0.5f));
	}



	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	//CText* pText = pEmptyObject->AddComponent<CText>("Text");
	//pText->SetText(wstring(L"Collider ") + wstring(CA2W(cIndex)));
	//SAFE_RELEASE(pText);

	SetGameObject(pEmptyObject);
	UpdateTransform();

	m_vecCollider.push_back(pEmptyObject);
	pReturn = pEmptyObject;
	SAFE_RELEASE(pEmptyObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);

	return pReturn;
}

CGameObject * CInspector::CreateJumpthru()
{
	CGameObject* pReturn = NULL;
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Default");
	CGameObject* pEmptyObject = CGameObject::CreateObject("Jumpthru", pLayer);
	CTransform* pTransform = pEmptyObject->GetTransform();
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));

	CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);

	PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData("objects\\jumpthru\\wood00");
	pRenderer->SetImageInfo("objects\\jumpthru\\wood00");
	m_strAtlasKey = "objects\\jumpthru\\wood00";
	SAFE_RELEASE(pRenderer);
	
	pTransform->SetWorldScale(pData->wWidth * 4, pData->wHeight * 4, 1.0f);
	SAFE_RELEASE(pTransform);

	SetGameObject(pEmptyObject);
	UpdateTransform();

	m_vecJumpthru.push_back(pEmptyObject);
	pReturn = pEmptyObject;
	SAFE_RELEASE(pEmptyObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);

	return pReturn;
}

CGameObject * CInspector::CreateCrumble()
{
	CGameObject* pReturn = NULL;
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Default");
	CGameObject* pEmptyObject = CGameObject::CreateObject("Crumble", pLayer);
	CTransform* pTransform = pEmptyObject->GetTransform();
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));

	CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);

	PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData("objects\\crumbleBlock\\default");
	pRenderer->SetImageInfo("objects\\crumbleBlock\\default");
	m_strAtlasKey = "objects\\crumbleBlock\\default";
	SAFE_RELEASE(pRenderer);

	pTransform->SetWorldScale(pData->wWidth * 4, pData->wHeight * 4, 1.0f);
	pTransform->SetPivot(0.0f, 1.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	SetGameObject(pEmptyObject);
	UpdateTransform();

	m_vecCrumble.push_back(pEmptyObject);
	pReturn = pEmptyObject;
	SAFE_RELEASE(pEmptyObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);

	return pReturn;
}

CGameObject * CInspector::CreateDanger()
{
	//CGameObject* pReturn = NULL;
	//CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	//
	//RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	//Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	//CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	//Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	//SAFE_RELEASE(pCameraTransform);
	//
	//CLayer* pLayer = pScene->FindLayer("Default");
	//CGameObject* pEmptyObject = CGameObject::CreateObject("Spike", pLayer);
	//CTransform* pTransform = pEmptyObject->GetTransform();
	//pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
	//
	//CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	//pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	//pRenderer->SetInputLayout(POS_UV_LAYOUT);
	//pRenderer->SetRenderState(ALPHA_BLEND);
	//pRenderer->SetRenderMode(RM_2D);
	//
	//CMaterial*	pMaterial = pRenderer->CreateMaterial();
	//pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	//pMaterial->SetSampler(SAMPLER_POINT);
	//SAFE_RELEASE(pMaterial);
	//
	//PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData("danger\\spikes\\default_up00");
	//pRenderer->SetImageInfo("danger\\spikes\\default_up00");
	//m_strAtlasKey = "danger\\spikes\\default_up00";
	//SAFE_RELEASE(pRenderer);
	//
	//pTransform->SetWorldScale(pData->wWidth * 4, pData->wHeight * 4, 1.0f);
	//pTransform->SetPivot(0.0f, - 1.0f / 3.0f, 0.0f);
	//SAFE_RELEASE(pTransform);
	//
	//SetGameObject(pEmptyObject);
	//UpdateTransform();
	//
	//m_vecDanger.push_back(pEmptyObject);
	//pReturn = pEmptyObject;
	//SAFE_RELEASE(pEmptyObject);
	//SAFE_RELEASE(pScene);
	//SAFE_RELEASE(pLayer);

	return NULL;
}

CGameObject * CInspector::CreateDanger(DIRECTION eFaceDir)
{
	
	string strDirection;
	string strAtlasKey;
	Vector3 vPivot;
	switch (eFaceDir)
	{
	case LMNT::DIR_LEFT:
		strDirection = "Left";
		strAtlasKey = "danger\\spikes\\default_left00";
		vPivot.x = -1.0f / 3.0f;;
		vPivot.y = 0.0f;
		vPivot.z = 0.0f;
		break;
	case LMNT::DIR_TOP:
		strDirection = "Up";
		strAtlasKey = "danger\\spikes\\default_up00";
		vPivot.x = 0.0f;
		vPivot.y = - 1.0f / 3.0f;
		vPivot.z = 0.0f;
		break;
	case LMNT::DIR_RIGHT:
		strDirection = "Right";
		strAtlasKey = "danger\\spikes\\default_right00";
		vPivot.x = 0.0f;
		vPivot.y = 0.0f;
		vPivot.z = 0.0f;
		break;
	case LMNT::DIR_BOTTOM:
		strDirection = "Down";
		strAtlasKey = "danger\\spikes\\default_down00";
		vPivot.x = 0.0f;
		vPivot.y = 0.0f;
		vPivot.z = 0.0f;
		break;
	default:
		strDirection = "Up";
		break;
	}
	
	CGameObject* pReturn = NULL;
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();



	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Default");
	CGameObject* pEmptyObject = CGameObject::CreateObject("Spike" + strDirection, pLayer);
	CTransform* pTransform = pEmptyObject->GetTransform();
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));

	CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);

	PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData(strAtlasKey);
	pRenderer->SetImageInfo(strAtlasKey);
	m_strAtlasKey = CA2CT(strAtlasKey.c_str());
	SAFE_RELEASE(pRenderer);

	pTransform->SetWorldScale(pData->wWidth * 4, pData->wHeight * 4, 1.0f);
	pTransform->SetPivot(vPivot);
	SAFE_RELEASE(pTransform);

	SetGameObject(pEmptyObject);
	UpdateTransform();

	PSPIKE pSpike = new SPIKE;
	pSpike->eDirection = eFaceDir;
	pSpike->pObject = pEmptyObject;

	m_vecDanger.push_back(pSpike);
	pReturn = pEmptyObject;
	SAFE_RELEASE(pEmptyObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);

	return pReturn;
}

CGameObject * CInspector::CreateSpring()
{
	CGameObject* pReturn = NULL;
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	CLayer* pLayer = pScene->FindLayer("Default");
	CGameObject* pEmptyObject = CGameObject::CreateObject("Spring", pLayer);
	CTransform* pTransform = pEmptyObject->GetTransform();
	pTransform->SetPivot(0.0f, 1.0f, 0.0f);
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));

	CRenderer* pRenderer = pEmptyObject->AddComponent<CRenderer>("Renderer"); pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);

	PATLASDATA pData = GET_SINGLE(CResourcesManager)->GetAtlasData("objects\\spring\\00");
	pRenderer->SetImageInfo("objects\\spring\\00");
	m_strAtlasKey = "objects\\spring\\00";
	SAFE_RELEASE(pRenderer);

	pTransform->SetWorldScale(pData->wWidth * 4, pData->wHeight * 4, 1.0f);
	pTransform->SetPivot(0.5f, 1.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	SetGameObject(pEmptyObject);
	UpdateTransform();

	m_vecSpring.push_back(pEmptyObject);
	pReturn = pEmptyObject;
	SAFE_RELEASE(pEmptyObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);

	return pReturn;
}

CGameObject * CInspector::LoadPrefab()
{
	CGameObject* pReturn = NULL;
	CFileDialog	dlg(TRUE, L"", L"", OFN_HIDEREADONLY,
		L"OBJECT(*object)|*.object|AllFiles(*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		CString strTitle = dlg.GetFileTitle();
		CString	strFullPath = dlg.GetPathName();

		FILE* pFile = NULL;
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer* pLayer = pScene->FindLayer("Default");

		char	astrPath[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, strFullPath, -1,
			astrPath, lstrlen(strFullPath) * 2, 0, 0);

		fopen_s(&pFile, astrPath, "rb");

		if (pFile)
		{
			CGameObject* pParent = CGameObject::CreateObject("Object", pLayer);
			CTransform* pParentTransform = pParent->GetTransform();
			{
				int iCount = 0;
				fread(&iCount, 4, 1, pFile);
				for (int i = 0; i < iCount; ++i)
				{
					CGameObject* pTile = CGameObject::CreateObject("Tile", pLayer);
					CRenderer* pTileRenderer = pTile->AddComponent<CRenderer>("TileRenderer");
					pTileRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
					pTileRenderer->SetShader(STANDARD_TEXTURE_SHADER);
					pTileRenderer->SetInputLayout(POS_UV_LAYOUT);
					pTileRenderer->SetRenderState(ALPHA_BLEND);
					pTileRenderer->SetRenderMode(RM_2D);

					CMaterial*	pMaterial = pTileRenderer->CreateMaterial();
					pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
					pMaterial->SetSampler(SAMPLER_POINT);
					SAFE_RELEASE(pMaterial);
					SAFE_RELEASE(pTileRenderer);

					CTransform* pTileTransform = pTile->GetTransform();
					pTileTransform->Load(pFile);
					pTileTransform->SetParent(pParent);
					pTileTransform->SetLocalPosition(pTileTransform->GetWorldPosition());
					pTileTransform->SetLocalScale(pTileTransform->GetWorldScale());
					pTileTransform->SetLocalRotation(pTileTransform->GetWorldRotation());
					pTileTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
					pTileTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					pTileTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
					SAFE_RELEASE(pTileTransform);

					CTile* pTileComponent = pTile->AddComponent<CTile>("Tile");
					pTileComponent->Load(pFile);
					SAFE_RELEASE(pTileComponent);
					SAFE_RELEASE(pTile);
				}
			}


			{
				//나머지 오브젝트 저장
				int iCount = 0;
				fread(&iCount, 4, 1, pFile);
				for (int i = 0; i < iCount; ++i)
				{
					int iLength = 0;
					char strTagBuffer[256] = {};
					fread(&iLength, 4, 1, pFile);
					fread(strTagBuffer, 1, iLength, pFile);

					//Layer읽음
					iLength = 0;
					char cLayerBuffer[256] = {};
					fread(&iLength, 4, 1, pFile);
					fread(cLayerBuffer, sizeof(char), iLength, pFile);

					CLayer* pTempLayer = pScene->FindLayer(string(cLayerBuffer));
					CGameObject* pObject = CGameObject::CreateObject(string(strTagBuffer), pTempLayer);
					SAFE_RELEASE(pTempLayer);

					CTransform* pTransform = pObject->GetTransform();
					pTransform->Load(pFile);
					pTransform->SetParent(pParent);
					pTransform->SetLocalPosition(pTransform->GetWorldPosition());
					pTransform->SetLocalScale(pTransform->GetWorldScale());
					pTransform->SetLocalRotation(pTransform->GetWorldRotation());
					pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
					pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
					SAFE_RELEASE(pTransform);

					CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
					pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
					pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
					pRenderer->SetInputLayout(POS_UV_LAYOUT);
					pRenderer->SetRenderState(ALPHA_BLEND);
					pRenderer->SetRenderMode(RM_2D);

					CMaterial*	pMaterial = pRenderer->CreateMaterial();
					pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
					pMaterial->SetSampler(SAMPLER_POINT);
					SAFE_RELEASE(pMaterial);

					char strAtlasKey[256] = {};
					iLength = 0;
					fread(&iLength, 4, 1, pFile);
					fread(strAtlasKey, sizeof(char), iLength, pFile);
					pRenderer->SetImageInfo(string(strAtlasKey));
					SAFE_RELEASE(pRenderer);
					SAFE_RELEASE(pObject);
				}
			}

			//Dummy
			{
				int iCount = 0;
				fread(&iCount, 4, 1, pFile);
			}

			//충돌체 읽기
			{
				int iSize = 0;
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					int iLength = 0;
					char cTagBuffer[256] = {};

					fread(&iLength, 4, 1, pFile);
					fread(&cTagBuffer, iLength, 1, pFile);
					Vector4 vArea;
					fread(&vArea, sizeof(Vector4), 1, pFile);

					CGameObject* pArea = CreateCollider(cTagBuffer);
					CTransform* pAreaTransform = pArea->GetTransform();
					pAreaTransform->SetParent(pParent);
					pAreaTransform->SetLocalPosition(vArea.x, vArea.y, 0.0f);
					pAreaTransform->SetLocalScale(vArea.z, vArea.w, 1.0f);

					UpdateTransform();
					CummitTransform();
					SAFE_RELEASE(pAreaTransform);
				}
			}


			//가시 읽기
			{
				int iSize = m_vecDanger.size();
				fread(&iSize, 4, 1, pFile);

				for (int i = 0; i < iSize; ++i)
				{
					DIRECTION eDirection;
					fread(&eDirection, sizeof(DIRECTION), 1, pFile);

					CGameObject* pDanger = CreateDanger(eDirection);
					CTransform* pTransform = pDanger->GetTransform();
					pTransform->Load(pFile);
					pTransform->SetParent(pParent);
					pTransform->SetLocalPosition(pTransform->GetWorldPosition());
					pTransform->SetLocalScale(pTransform->GetWorldScale());
					pTransform->SetLocalRotation(pTransform->GetWorldRotation());
					pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
					pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
					pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
					SAFE_RELEASE(pTransform);
					UpdateTransform();
					CummitTransform();
				}
			}



			//Jumpthru 읽기
			//PrefabDummy
			//Spring
			//CheckPoint
			//Refill
			//Strawberry

			pReturn = pParent;
			//파일 명 추출


			PPREFAB pPrefab = new PREFAB;
			pPrefab->strFileName = CT2CA(strTitle);
			pPrefab->pObject = pParent;
			m_vecPrefab.push_back(pPrefab);
			

			RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
			Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
			CTransform* pCameraTransform = pScene->GetMainCameraTransform();
			Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
			SAFE_RELEASE(pCameraTransform);

			pParentTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
			
			SAFE_RELEASE(pParentTransform);
			SAFE_RELEASE(pParent);
			fclose(pFile);
		}
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	return pReturn;
}

CGameObject * CInspector::LoadPrefab(const string & strFileName, class CLayer* pLayer)
{
	CGameObject* pReturn = NULL;
	string strPath = GET_SINGLE(CPathManager)->FindPathMultiByte(PREFAB_PATH);

	strPath += strFileName;
	strPath += string(".object");


	FILE* pFile = NULL;
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	
	fopen_s(&pFile, strPath.c_str(), "rb");

	if (pFile)
	{
		CGameObject* pParent = CGameObject::CreateObject("Object", pLayer);
		CTransform* pParentTransform = pParent->GetTransform();
		{
			int iCount = 0;
			fread(&iCount, 4, 1, pFile);
			for (int i = 0; i < iCount; ++i)
			{
				CGameObject* pTile = CGameObject::CreateObject("Tile", pLayer);
				CRenderer* pTileRenderer = pTile->AddComponent<CRenderer>("TileRenderer");
				pTileRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
				pTileRenderer->SetShader(STANDARD_TEXTURE_SHADER);
				pTileRenderer->SetInputLayout(POS_UV_LAYOUT);
				pTileRenderer->SetRenderState(ALPHA_BLEND);
				pTileRenderer->SetRenderMode(RM_2D);

				CMaterial*	pMaterial = pTileRenderer->CreateMaterial();
				pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
				pMaterial->SetSampler(SAMPLER_POINT);
				SAFE_RELEASE(pMaterial);
				SAFE_RELEASE(pTileRenderer);

				CTransform* pTileTransform = pTile->GetTransform();
				pTileTransform->Load(pFile);
				pTileTransform->SetParent(pParent);
				pTileTransform->SetLocalPosition(pTileTransform->GetWorldPosition());
				pTileTransform->SetLocalScale(pTileTransform->GetWorldScale());
				pTileTransform->SetLocalRotation(pTileTransform->GetWorldRotation());
				pTileTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
				pTileTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				pTileTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
				SAFE_RELEASE(pTileTransform);

				CTile* pTileComponent = pTile->AddComponent<CTile>("Tile");
				pTileComponent->Load(pFile);
				SAFE_RELEASE(pTileComponent);
				SAFE_RELEASE(pTile);
			}
		}


		{
			//나머지 오브젝트 저장
			int iCount = 0;
			fread(&iCount, 4, 1, pFile);
			for (int i = 0; i < iCount; ++i)
			{
				int iLength = 0;
				char strTagBuffer[256] = {};
				fread(&iLength, 4, 1, pFile);
				fread(strTagBuffer, 1, iLength, pFile);

				//Layer읽음
				iLength = 0;
				char cLayerBuffer[256] = {};
				fread(&iLength, 4, 1, pFile);
				fread(cLayerBuffer, sizeof(char), iLength, pFile);

				CLayer* pTempLayer = pScene->FindLayer(string(cLayerBuffer));
				CGameObject* pObject = CGameObject::CreateObject(string(strTagBuffer), pTempLayer);
				SAFE_RELEASE(pTempLayer);

				CTransform* pTransform = pObject->GetTransform();
				pTransform->Load(pFile);
				pTransform->SetParent(pParent);
				pTransform->SetLocalPosition(pTransform->GetWorldPosition());
				pTransform->SetLocalScale(pTransform->GetWorldScale());
				pTransform->SetLocalRotation(pTransform->GetWorldRotation());
				pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
				pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
				SAFE_RELEASE(pTransform);

				CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
				pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
				pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
				pRenderer->SetInputLayout(POS_UV_LAYOUT);
				pRenderer->SetRenderState(ALPHA_BLEND);
				pRenderer->SetRenderMode(RM_2D);

				CMaterial*	pMaterial = pRenderer->CreateMaterial();
				pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
				pMaterial->SetSampler(SAMPLER_POINT);
				SAFE_RELEASE(pMaterial);

				char strAtlasKey[256] = {};
				iLength = 0;
				fread(&iLength, 4, 1, pFile);
				fread(strAtlasKey, sizeof(char), iLength, pFile);
				pRenderer->SetImageInfo(string(strAtlasKey));
				SAFE_RELEASE(pRenderer);
				SAFE_RELEASE(pObject);
			}
		}

		//Dummy
		{
			int iCount = 0;
			fread(&iCount, 4, 1, pFile);
		}
		
		//충돌체 읽기
		{
			int iSize = 0;
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				int iLength = 0;
				char cTagBuffer[256] = {};

				fread(&iLength, 4, 1, pFile);
				fread(&cTagBuffer, iLength, 1, pFile);
				Vector4 vArea;
				fread(&vArea, sizeof(Vector4), 1, pFile);

				CGameObject* pArea = CreateCollider(cTagBuffer);
				CTransform* pAreaTransform = pArea->GetTransform();
				pAreaTransform->SetParent(pParent);
				pAreaTransform->SetLocalPosition(vArea.x, vArea.y, 0.0f);
				pAreaTransform->SetLocalScale(vArea.z, vArea.w, 1.0f);

				UpdateTransform();
				CummitTransform();
				SAFE_RELEASE(pAreaTransform);
			}
		}


		//가시 읽기
		{
			int iSize = m_vecDanger.size();
			fread(&iSize, 4, 1, pFile);

			for (int i = 0; i < iSize; ++i)
			{
				DIRECTION eDirection;
				fread(&eDirection, sizeof(DIRECTION), 1, pFile);

				CGameObject* pDanger = CreateDanger(eDirection);
				CTransform* pTransform = pDanger->GetTransform();
				pTransform->Load(pFile);
				pTransform->SetParent(pParent);
				pTransform->SetLocalPosition(pTransform->GetWorldPosition());
				pTransform->SetLocalScale(pTransform->GetWorldScale());
				pTransform->SetLocalRotation(pTransform->GetWorldRotation());
				pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
				pTransform->SetWorldRotation(0.0f, 0.0f, 0.0f);
				pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
				SAFE_RELEASE(pTransform);
				UpdateTransform();
				CummitTransform();
			}
		}

		pReturn = pParent;
		//파일 명 추출

		PPREFAB pPrefab = new PREFAB;
		pPrefab->strFileName = strFileName;
		pPrefab->pObject = pParent;
		m_vecPrefab.push_back(pPrefab);


		SAFE_RELEASE(pParentTransform);
		SAFE_RELEASE(pParent);
		fclose(pFile);
	}
	SAFE_RELEASE(pScene);
	return pReturn;
}

class CGameObject* CInspector::CreateCheckPoint()
{
	CGameObject* pReturn = NULL;
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->FindLayer("Default");

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);


	CGameObject* pObject = CGameObject::CreateObject("CheckPoint", pLayer);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	CTransform* pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(96, 128, 1.0f);
	pTransform->SetPivot(0.5f, 1.0f, 0.0f);
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);	
	SAFE_RELEASE(pRenderer);

	CAnimator* pAnimator = pObject->AddComponent<CAnimator>("Animator");
	pAnimator->CreateAnimationController();
	CAnimationState* pState = pAnimator->CreateAnimationStateInCurrentController("CheckPoint");
	pState->AddClip("CheckPoint", AO_LOOP, 1.0f, "Gameplay", L"Celeste\\Gameplay.png");
	pAnimator->SetEntryState("CheckPoint");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal00");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal01");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal02");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal03");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal04");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal05");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal06");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal07");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal08");
	pState->GetClip()->AddCustomInfo("ui\\loading\\normal09");	
	SAFE_RELEASE(pAnimator);

	m_vecCheckPoint.push_back(pObject);
	pReturn = pObject;
	SAFE_RELEASE(pObject);

	return pReturn;
}

CGameObject * CInspector::CreateRefillCristal()
{
	CGameObject* pReturn = NULL;
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->FindLayer("Default");

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);


	CGameObject* pObject = CGameObject::CreateObject("Refill", pLayer);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	CTransform* pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(40.0f, 40.0f, 1.0f);
	pTransform->SetPivot(0.5f, 0.5f, 0.0f);
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);

	pRenderer->SetImageInfo("objects\\refill\\idle00");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	
	m_vecRefill.push_back(pObject);
	pReturn = pObject;
	SAFE_RELEASE(pObject);

	return pReturn;
}

CGameObject * CInspector::CreateStrawberry(BERRY_TYPE eType)
{
	CGameObject* pReturn = NULL;
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->FindLayer("Default");

	RECT rc = GET_SINGLE(CTransmitter)->m_pMainFrame->m_tRect;
	Vector2 vScreenSize = Vector2(rc.right - rc.left, rc.bottom - rc.top);
	CTransform* pCameraTransform = pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);


	CGameObject* pObject = CGameObject::CreateObject("Strawberry", pLayer);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	CTransform* pTransform = pObject->GetTransform();
	pTransform->SetWorldScale(40.0f, 52.0f, 1.0f);
	pTransform->SetPivot(0.5f, 0.5f, 0.0f);
	pTransform->SetWorldPosition(vCameraPosition + Vector3(vScreenSize.x / 2.0f, vScreenSize.y / 2.0f, 0.0f));
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pMaterial->SetSampler(SAMPLER_POINT);

	pRenderer->SetImageInfo("collectables\\strawberry\\normal00");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	PSTRAWBERRY  pBerry = new STRAWBERRY;
	pBerry->eType = eType;
	pBerry->pGameObject = pObject;
	m_vecStrawberry.push_back(pBerry);

	pReturn = pObject;
	SAFE_RELEASE(pObject);

	return pReturn;
}

bool CInspector::IsPrefab(const string & strTag)
{
	size_t iSize = m_vecPrefab.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecPrefab[i]->pObject->GetTag() == strTag || m_vecPrefab[i]->strFileName == strTag)
		{
			return true;
		}		
	}
	return false;
}

void CInspector::ShowBackgroundMap1(bool bShow)
{
	if (bShow)
	{
		if (!m_pBackgroundMap)
		{
			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			CLayer* pLayer = pScene->FindLayer("BackgroundMap");
			m_pBackgroundMap = CGameObject::CreateObject("Background", pLayer);
			
			CTransform* pTransform = m_pBackgroundMap->GetTransform();
			pTransform->SetPivot(0.0f, 1.0f, 0.0f);
			pTransform->SetWorldScale(4096.0f, 4096.0f, 1.0f);
			pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
			SAFE_RELEASE(pTransform);

			CRenderer* pRenderer = m_pBackgroundMap->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);


			CMaterial*	pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("Stage1_00", L"Stage1_00.png");
			pMaterial->SetSampler(SAMPLER_POINT);
			
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.6f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			//Child1
			m_pBackgroundMap1 = CGameObject::CreateObject("Background", pLayer);

			pTransform = m_pBackgroundMap1->GetTransform();
			pTransform->SetPivot(0.0f, 1.0f, 0.0f);
			pTransform->SetWorldScale(4096.0f, 4096.0f, 1.0f);
			pTransform->SetWorldPosition(4096.0f, 0.0f, 0.0f);
			SAFE_RELEASE(pTransform);

			pRenderer = m_pBackgroundMap1->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);

			pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("Stage1_01", L"Stage1_01.png");
			pMaterial->SetSampler(SAMPLER_POINT);

			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.6f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);


			//Child
			m_pBackgroundMap2 = CGameObject::CreateObject("Background", pLayer);
			
			pTransform = m_pBackgroundMap2->GetTransform();
			pTransform->SetPivot(0.0f, 1.0f, 0.0f);
			pTransform->SetWorldScale(4096.0f, 4096.0f, 1.0f);
			pTransform->SetWorldPosition(0.0f, -3912.0f, 0.0f);
			SAFE_RELEASE(pTransform);
			
			pRenderer = m_pBackgroundMap2->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);
			
			pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("Stage1_02", L"Stage1_02.png");
			pMaterial->SetSampler(SAMPLER_POINT);
			
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.6f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			
			//Child
			m_pBackgroundMap3 = CGameObject::CreateObject("Background", pLayer);
			
			pTransform = m_pBackgroundMap3->GetTransform();
			pTransform->SetPivot(0.0f, 1.0f, 0.0f);
			pTransform->SetWorldScale(4096.0f, 4096.0f, 1.0f);
			pTransform->SetWorldPosition(4096.0f, -3912.0f, 0.0f);
			SAFE_RELEASE(pTransform);
			
			pRenderer = m_pBackgroundMap3->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);
			
			pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("Stage1_03", L"Stage1_03.png");
			pMaterial->SetSampler(SAMPLER_POINT);
			
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.6f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			
			
			//Child
			m_pBackgroundMap4 = CGameObject::CreateObject("Background", pLayer);
			
			pTransform = m_pBackgroundMap4->GetTransform();
			pTransform->SetPivot(0.0f, 1.0f, 0.0f);
			pTransform->SetWorldScale(4096.0f, 4096.0f, 1.0f);
			pTransform->SetWorldPosition(8192.0f, -4232.0f, 0.0f);
			SAFE_RELEASE(pTransform);
			
			pRenderer = m_pBackgroundMap4->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);
			
			pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("Stage1_04", L"Stage1_04.png");
			pMaterial->SetSampler(SAMPLER_POINT);
			
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.6f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			
			//Child
			m_pBackgroundMap5 = CGameObject::CreateObject("Background", pLayer);
			
			pTransform = m_pBackgroundMap5->GetTransform();
			pTransform->SetPivot(0.0f, 1.0f, 0.0f);
			pTransform->SetWorldScale(4096.0f, 4096.0f, 1.0f);
			pTransform->SetWorldPosition(8192.0f, -8192.0f, 0.0f);
			SAFE_RELEASE(pTransform);
			
			pRenderer = m_pBackgroundMap5->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);
			
			pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("Stage1_05", L"Stage1_05.png");
			pMaterial->SetSampler(SAMPLER_POINT);
			
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.6f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			
			//Child
			m_pBackgroundMap6 = CGameObject::CreateObject("Background", pLayer);
			
			pTransform = m_pBackgroundMap6->GetTransform();
			pTransform->SetPivot(0.0f, 1.0f, 0.0f);
			pTransform->SetWorldScale(4096.0f, 4096.0f, 1.0f);
			pTransform->SetWorldPosition(12284.0f, -4096.0f, 0.0f);
			SAFE_RELEASE(pTransform);
			
			pRenderer = m_pBackgroundMap6->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);
			
			pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("Stage1_06", L"Stage1_06.png");
			pMaterial->SetSampler(SAMPLER_POINT);
			
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.6f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			
			
			//Child
			m_pBackgroundMap7 = CGameObject::CreateObject("Background", pLayer);
			
			pTransform = m_pBackgroundMap7->GetTransform();
			pTransform->SetPivot(0.0f, 1.0f, 0.0f);
			pTransform->SetWorldScale(4519.0f, 5297.0f, 1.0f);
			pTransform->SetWorldPosition(12288.0f, -8192.0f, 0.0f);
			SAFE_RELEASE(pTransform);
			
			pRenderer = m_pBackgroundMap7->AddComponent<CRenderer>("Renderer");
			pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
			pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
			pRenderer->SetInputLayout(POS_UV_LAYOUT);
			pRenderer->SetRenderState(ALPHA_BLEND);
			pRenderer->SetRenderMode(RM_2D);
			
			pMaterial = pRenderer->CreateMaterial();
			pMaterial->SetDiffuseTexture("Stage1_07", L"Stage1_07.png");
			pMaterial->SetSampler(SAMPLER_POINT);
			
			pRenderer->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 0.6f));
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);







			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
		}
	}
}
