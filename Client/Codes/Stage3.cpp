#include "stdafx.h"
#include "Player.h"
#include "Stage3.h"

#include "SoundMgr.h"
#include "MainCamera.h"


CStage3::CStage3(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
	: CScene(pDevice), m_vecTile(vecTile)
{
}

HRESULT CStage3::Ready_Scene()
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Monster");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Camera()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Camera");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Skybox()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Skybox");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_VirtualCube()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Skybox");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_UIcube()))		//추가
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
		return UPDATE_ERROR;
	}

	if (FAILED(Ready_Layer_UIcubepiece()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
		return UPDATE_ERROR;
	}

	if (FAILED(Ready_Layer_UIkey()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Item");
		return UPDATE_ERROR;
	}

	if (FAILED(Ready_Layer_Item()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
		return UPDATE_ERROR;
	}
	if (FAILED(Ready_Layer_Portal()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Portal");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Effect()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Effect");
		return E_FAIL;
	}

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	if (pPlayer)
	{
		pPlayer->Set_PlayerPos(_vec3(0.f, 1.f, 0.f)); // Staring Pos (0, 1, 0)
		pPlayer->Set_FallPos(_vec3(0.f, 1.0f, 0.f));
		pPlayer->Set_Camera();
		pPlayer->Set_CollisionTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Collision_Terrain"));
		pPlayer->Set_ItemList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Item"));
		pPlayer->Set_SceneChange(false);
		pPlayer->Set_PlayerOnBonusStage();
		pPlayer->Set_OnNewBonusStage(false);
	}

	::SetWindowText(g_hWnd, L"Last Stage Scene");

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
	CAMERA_DESC	tCamDesc = pCamera->Get_Desc();

	tCamDesc.bToolmode = false;
	pCamera->Set_Desc(tCamDesc);
	static_cast<CMainCamera*>(pCamera)->Set_Start(false);
	static_cast<CMainCamera*>(pCamera)->Set_CameraAngle(_vec3(0.f, 0.f, 0.f));

	_vec3	vParentPos = _vec3(0.f, 35.f, 0.f);
	static_cast<CMainCamera*>(pCamera)->Set_ParentPos(vParentPos);

	return S_OK;
}

_uint CStage3::Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Update_Scene(fDeltaTime))
		return iEvent;
	CSoundMgr::Get_Instance()->PlaySound(L"BGM_Stage3.mp3", CSoundMgr::BGM);

	return iEvent;
}

_uint CStage3::Late_Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Late_Update_Scene(fDeltaTime))
		return iEvent;

	return iEvent;
}

CStage3 * CStage3::Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
{
	if (!pDevice)
	{
		PRINT_LOG(L"Error", L"pDevice is nullptr");
		return nullptr;
	}

	CStage3* pInstance = new CStage3(pDevice, vecTile);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage3::Free()
{
	CScene::Free();
}

HRESULT CStage3::Ready_Layer_Portal()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(2.f, -14.f, 2.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
		, L"GameObject_Portal", ePrototypeType::NoneStatic, L"Layer_Portal", &tDesc);
}

HRESULT CStage3::Ready_Layer_Terrain()
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
		_vec3 vPos = pTile->vPos;
		BYTE bTemp = 0;
		_bool bSouth = true;
		_bool bNorth = true;
		_bool bEast = true;
		_bool bWest = true;

		for each(TILEINFO* pTile in m_vecTile) {
			if (vPos.y == pTile->vPos.y) {
				if (vPos.x > pTile->vPos.x)
					bWest = false;
				if (vPos.x < pTile->vPos.x)
					bEast = false;
				if (vPos.z < pTile->vPos.z)
					bNorth = false;
				if (vPos.z > pTile->vPos.z)
					bSouth = false;
			}
		}

		if (bSouth)
			bTemp |= TILECOL_DIR_SOUTH;
		if (bNorth)
			bTemp |= TILECOL_DIR_NORTH;
		if (bEast)
			bTemp |= TILECOL_DIR_EAST;
		if (bWest)
			bTemp |= TILECOL_DIR_WEST;

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
			wsPrototypeName = L"GameObject_Vine";
			wsLayerName = L"Layer_Vine_Terrain";
		}
		else{
			wsPrototypeName = L"GameObject_Monster";
			wsLayerName = L"Layer_InterObj";
		}

		TILEINFO	tTile;
		tTile.btCollisionDir = bTemp;
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

	static_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front())->Set_AllTerrainList(&m_vecTile);

	return S_OK;
}

HRESULT CStage3::Ready_Layer_Monster()
{
	// 여기서 보스 세팅하면 될듯 ?

	return S_OK;
}


HRESULT CStage3::Ready_Layer_Skybox()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vScale *= 40.f;
	tDesc.iNum = 1;

	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Skybox"
		, ePrototypeType::NoneStatic, L"Layer_Skybox", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage3::Ready_Layer_Obj()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
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

HRESULT CStage3::Ready_Layer_Camera()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
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

HRESULT CStage3::Ready_Layer_VirtualCube()
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

HRESULT CStage3::Ready_Layer_UIcube()
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
		, L"GameObject_UIcube", ePrototypeType::NoneStatic, L"Layer_UIcube", &tDesc);
}

HRESULT CStage3::Ready_Layer_UIcubepiece()
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
		, L"GameObject_UIcubepiece", ePrototypeType::NoneStatic, L"Layer_UIcubepiece", &tDesc);
}

HRESULT CStage3::Ready_Layer_UIkey()
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
		, L"GameObject_UIkey", ePrototypeType::NoneStatic, L"Layer_UIkey", &tDesc);
}

HRESULT CStage3::Ready_Layer_Item()
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
	tDesc.vPos = _vec3(0.f, 100.f, 0.f);
	tDesc.iVineDir = (_uint)eMapDir::SOUTH;
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);

	//tDesc.vPos = _vec3(37.85f, 41.94f, 37.40f);
	//pManagement->Add_GameObjectInLayer(ePrototypeType::Static
	//	, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);

	//tDesc.vPos = _vec3(30.85f, 29.99f, 27.27f);
	//pManagement->Add_GameObjectInLayer(ePrototypeType::Static
	//	, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	//tDesc.vPos = _vec3(19.07f, 43.10f, 21.63f);
	//pManagement->Add_GameObjectInLayer(ePrototypeType::Static
	//	, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	//tDesc.vPos = _vec3(26.03f, 48.2f, 39.92f);
	//pManagement->Add_GameObjectInLayer(ePrototypeType::Static
	//	, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	//tDesc.vPos = _vec3(39.52f, 51.36f, 29.7f);
	//pManagement->Add_GameObjectInLayer(ePrototypeType::Static
	//	, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	//tDesc.vPos = _vec3(41.64f, 22.44f, 38.18f);
	//pManagement->Add_GameObjectInLayer(ePrototypeType::Static
	//	, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	//tDesc.vPos = _vec3(21.86f, 42.86f, 34.85f);
	//pManagement->Add_GameObjectInLayer(ePrototypeType::Static
	//	, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	return S_OK;
}

HRESULT CStage3::Ready_Layer_Effect()
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
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Effect", ePrototypeType::NoneStatic, L"Layer_Effect", &tDesc);

	return S_OK;
}