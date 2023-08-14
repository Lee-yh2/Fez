#include "stdafx.h"
#include "GomezHouse.h"
#include "Camera.h"
#include "Player.h"
#include "MainCamera.h"
#include "InterBox.h"
#include "VirtualCube.h"

#include "SoundMgr.h"

CGomezHouse::CGomezHouse(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
	: CScene(pDevice), m_vecTile(vecTile)
{
}

HRESULT CGomezHouse::Ready_Scene()
{
	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlaySound(L"BGM_GomezHouse.mp3", CSoundMgr::BGM);
	//if (FAILED(Ready_Layer_Player()))
	//{
	//	PRINT_LOG(L"GomezHouse", L"Failed to Ready_Layer_Player");
	//	return E_FAIL;
	//}
	if (FAILED(Ready_Layer_Terrain()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Terrain");
		return E_FAIL;
	}
	
	if (FAILED(Ready_Layer_GomezHouse()))
	{
		PRINT_LOG(L"GomezHouse", L"Failed to Ready_Layer_GomezHouse");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Camera()))
	{
		PRINT_LOG(L"GomezHouse", L"Failed to Ready_Layer_Camera");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_VirtualCube()))
	{
		PRINT_LOG(L"GomezHouse", L"Failed to Ready_Layer_VirtualCube");
		return E_FAIL;
	}


	if (FAILED(Ready_Layer_Portal()))
	{
		PRINT_LOG(L"GomezHouse", L"Failed to Ready_Layer_Portal");
		return E_FAIL;
	}

	/*if (FAILED(Ready_Layer_Obj()))
	{
		PRINT_LOG(L"GomezHouse", L"Failed to Ready_Layer_Obj");
		return E_FAIL;
	}*/


	CManagement* pManagement = CManagement::Get_Instance();

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

	if (pPlayer)
	{
		pPlayer->Set_PortalList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Portal"));
		//pPlayer->Set_RightRot();
		pPlayer->Set_PlayerPos(_vec3(12.f, 7.f, 14.f));
		pPlayer->Set_FallPos(_vec3(12.f, 7.f, 14.f));
		pPlayer->Set_CollisionTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Collision_Terrain"));
		pPlayer->Set_CurrentState(CPlayer::GETOUT);
		pPlayer->Set_Frame(0.f);
		pPlayer->Set_Camera();
		pPlayer->Set_SceneChange(false);
	}

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
	CAMERA_DESC	tCamDesc = pCamera->Get_Desc();
	tCamDesc.bToolmode = false;
	pCamera->Set_Desc(tCamDesc);
	static_cast<CMainCamera*>(pCamera)->Set_Start(false);
	static_cast<CMainCamera*>(pCamera)->Set_CameraAngle(_vec3(0.f, 0.f, 0.f));

	::SetWindowText(g_hWnd, L"House Scene");
	return S_OK;
}

_uint CGomezHouse::Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Update_Scene(fDeltaTime))
		return iEvent;

	return iEvent;
}

_uint CGomezHouse::Late_Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Late_Update_Scene(fDeltaTime))
		return iEvent;

	return iEvent;
}

CGomezHouse * CGomezHouse::Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
{
	if (!pDevice)
	{
		PRINT_LOG(L"GomezHouse", L"pDevice is nullptr");
		return nullptr;
	}

	CGomezHouse* pInstance = new CGomezHouse(pDevice, vecTile);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"GomezHouse", L"Failed To Create Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGomezHouse::Free()
{
	CScene::Free();
}

HRESULT CGomezHouse::Ready_Layer_Player()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"GomezHouse", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(12.f, 7.f, 14.f);
	tDesc.vScale = { 1.5f, 1.5f, 1.5f };
	tDesc.fMoveSpdPerSec = 2.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

	return pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Player", ePrototypeType::Static, L"Layer_Player", &tDesc);
}

HRESULT CGomezHouse::Ready_Layer_VirtualCube()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.fMoveSpdPerSec = 10.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

	return pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_VirtualCube", ePrototypeType::NoneStatic, L"Layer_VirtualCube", &tDesc);
}

HRESULT CGomezHouse::Ready_Layer_Portal()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}
	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(11.4f, 7.14f, 6.9f);
	tDesc.vScale = { 1.3f, 1.3f, 1.f };
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
	tDesc.iVineDir = (_uint)eMapDir::NORTH;
	tDesc.iNum = (_uint)eSceneType::Stage;

	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
		, L"GameObject_Portal", ePrototypeType::NoneStatic, L"Layer_Portal", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGomezHouse::Ready_Layer_Camera()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"GomezHouse", L"pManagement is nullptr");
		return E_FAIL;
	}

	CAMERA_DESC	tCameraDesc;
	tCameraDesc.vUp = _vec3(0.f, 1.f, 0.f);
	tCameraDesc.fFovY = D3DXToRadian(90.f);
	tCameraDesc.fAspect = 1.f;
	tCameraDesc.fNear = 1.f;
	tCameraDesc.fFar = 1000.f;
	//tCameraDesc.bToolmode = false;

	return pManagement->Add_GameObjectInLayer(ePrototypeType::Static, L"GameObject_MainCamera"
		, ePrototypeType::NoneStatic, L"Layer_Camera", &tCameraDesc);
}




HRESULT CGomezHouse::Ready_Layer_GomezHouse()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"GomezHouse", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vScale = { 6.3f, 7.f, 9.f };
	tDesc.vPos = _vec3(9.5f, 9.5f, 11.5f);

	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_GomezHouse"
		, ePrototypeType::NoneStatic, L"Layer_Room", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGomezHouse::Ready_Layer_Obj()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"GomezHouse", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;

	tDesc.vPos = _vec3(3.f, 0.f, 3.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_InterBox"
		, ePrototypeType::NoneStatic, L"Layer_InterObj", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGomezHouse::Ready_Layer_Terrain()
{
	TRANSFORM_DESC	tDesc;
	wstring wsLayerName;
	wstring wsPrototypeName;

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	for each (TILEINFO* pTile in m_vecTile)
	{
		if (pTile->byOption & TILE_COL) {
			wsPrototypeName = L"GameObject_Monster";
			wsLayerName = L"Layer_Collision_Terrain";
		}
		else if (pTile->byOption & TILE_INTER)
		{
			wsPrototypeName = L"GameObject_InterBox";
			wsLayerName = L"Layer_InterObj";
		}
		else if (pTile->byOption & TILE_VINE0)
		{
			wsPrototypeName = L"GameObject_Monster";
			wsLayerName = L"Layer_Vine_Terrain";
		}
		else {
			wsPrototypeName = L"GameObject_Monster";
			wsLayerName = L"Layer_InterObj";
		}


		TILEINFO	tTile;
		tTile.vSize = pTile->vSize;
		tTile.vColSize = pTile->vColSize;
		tTile.vRot = pTile->vRot;
		tTile.vPos = pTile->vPos;
		tTile.byOption = pTile->byOption;
		tTile.byPlayDrawID = pTile->byPlayDrawID;
		tTile.byDevDrawID = pTile->byDevDrawID;
		tTile.iStageNum = pTile->iStageNum;
		tTile.iIndex = pTile->iIndex;

		if (pTile->iStageNum == 0) {
			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, wsPrototypeName
				, ePrototypeType::NoneStatic, wsLayerName, &tTile)))
				return E_FAIL;
		}
	}
	return S_OK;
}
