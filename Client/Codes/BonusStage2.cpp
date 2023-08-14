#include "stdafx.h"
#include "BonusStage2.h"
#include "MainCamera.h"
#include "Player.h"
#include "Monster.h"
#include "SoundMgr.h"
#include "InterBox.h"
#include "SoundMgr.h"

CBonusStage2::CBonusStage2(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
	: CScene(pDevice), m_vecTile(vecTile), m_bClear(false)
{
}

HRESULT CBonusStage2::Ready_Scene()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error-BonusStage2-Ready_Scene", L"pManagement is nullptr");
		return E_FAIL;
	}

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	//bgm 잘 넣어주세욧;;

	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Camera");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Terrain()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Monster");
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

	if (FAILED(Ready_Layer_UIcube()))
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
	Ready_Layer_Effect();

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	if (pPlayer)
	{
		//pPlayer->Set_RightRot();
		pPlayer->Set_PlayerPos(_vec3(25.f, 42.f, 18.f));
		pPlayer->Set_FallPos(_vec3(25.f, 42.f, 18.f));
		/*pPlayer->Set_CurrentState(CPlayer::GETOUT);
		pPlayer->Set_Frame(0.f);*/
		pPlayer->Set_Camera();
		//pPlayer->Set_PortalList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Portal"));
		pPlayer->Set_InterObjList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj"));
		pPlayer->Set_CollisionTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Collision_Terrain"));
		//pPlayer->Set_ItemList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Item"));
		//pPlayer->Set_VineTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Vine_Terrain"));
		pPlayer->Set_SceneChange(false);
		pPlayer->Set_OnNewBonusStage(false);
	}

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
	CAMERA_DESC	tCamDesc = pCamera->Get_Desc();
	tCamDesc.bToolmode = false;
	pCamera->Set_Desc(tCamDesc);
	static_cast<CMainCamera*>(pCamera)->Set_Start(false);
	static_cast<CMainCamera*>(pCamera)->Set_CameraAngle(_vec3(0.f, 0.f, 0.f));

	list<CGameObject*>* InterObjList = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj");

	for each(CGameObject* pInterObj in *InterObjList)
	{
		static_cast<CInterBox*>(pInterObj)->Set_CollisionTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Collision_Terrain"));
		static_cast<CInterBox*>(pInterObj)->Set_InterObjList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj"));
	}

	::SetWindowText(g_hWnd, L"BonusStage2 Scene");

	return S_OK;
}

_uint CBonusStage2::Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Update_Scene(fDeltaTime))
		return iEvent;

	if (!m_bClear && (iEvent = Check_Clear()))
		return iEvent;
	CSoundMgr::Get_Instance()->PlaySound(L"BGM_Bonus2.mp3", CSoundMgr::BGM);

	return iEvent;
}

_uint CBonusStage2::Late_Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Late_Update_Scene(fDeltaTime))
		return iEvent;

	return iEvent;
}

CBonusStage2 * CBonusStage2::Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
{
	if (!pDevice)
	{
		PRINT_LOG(L"Error-BonusStage2-Create", L"pDevice is nullptr");
		return nullptr;
	}

	CBonusStage2*	pInstance = new CBonusStage2(pDevice, vecTile);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error-BonusStage2-Create", L"Failed to Create BonusStage2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBonusStage2::Free()
{
	CScene::Free();
}

HRESULT CBonusStage2::Ready_Layer_Terrain()
{
	TRANSFORM_DESC   tDesc;
	wstring wsLayerName;
	wstring wsPrototypeName;

	CManagement*   pManagement = CManagement::Get_Instance();
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

		for each(TILEINFO* pTile in m_vecTile)
		{
			if (vPos.y == pTile->vPos.y)
			{
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

		if (TILE_INTER & pTile->byOption)
		{
			wsPrototypeName = L"GameObject_InterBox";
			wsLayerName = L"Layer_InterObj";
		}
		else if (TILE_COL & pTile->byOption) {
			wsPrototypeName = L"GameObject_Monster";
			wsLayerName = L"Layer_Collision_Terrain";
		}
		else
		{
			wsPrototypeName = L"GameObject_Monster";
			wsLayerName = L"Layer_NoneCollision";
		}

		TILEINFO   tTile;
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

HRESULT CBonusStage2::Ready_Layer_Camera()
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

HRESULT CBonusStage2::Ready_Layer_Skybox()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vScale *= 40.f;

	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Skybox"
		, ePrototypeType::NoneStatic, L"Layer_Skybox", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBonusStage2::Ready_Layer_VirtualCube()
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

HRESULT CBonusStage2::Ready_Layer_UIcube()
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

HRESULT CBonusStage2::Ready_Layer_UIcubepiece()
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

HRESULT CBonusStage2::Ready_Layer_UIkey()
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

HRESULT CBonusStage2::Ready_Layer_Portal()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(30.f, 38.5f, 17.f);
	tDesc.vScale = { 1.5f, 2.f, 1.f };
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
	tDesc.iVineDir = (_uint)eMapDir::SOUTH;
	tDesc.iNum = (_uint)eSceneType::Stage3;

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
		, L"GameObject_Portal", ePrototypeType::NoneStatic, L"Layer_Portal", &tDesc);
}

HRESULT CBonusStage2::Ready_Layer_Effect()
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

_uint CBonusStage2::Check_Clear()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	list<CGameObject*>*	listbyLayer = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj");
	if (!listbyLayer)
		return NO_EVENT;

	_bool	bCorrect1 = false;
	_bool	bCorrect2 = false;
	_bool	bCorrect3 = false;

	_bool	bLeftCorrect = false;
	_bool	bRightCorrect = false;

	for (auto& iter : *listbyLayer)
	{
		_vec3	vTilePos = static_cast<CInterBox*>(iter)->Get_TileInfo().vPos;
		_vec3	vLength = _vec3(0.f, 0.f, 0.f);
		_float	fLength = 0.f;

#pragma region	bCorrect1
		vLength = _vec3(25.f, 42.f, 26.f) - vTilePos;
		fLength = D3DXVec3Length(&vLength);
		if (0.5f > fLength)
			bCorrect1 = true;
#pragma endregion

#pragma region	bCorrect2
		vLength = _vec3(25.f, 43.f, 26.f) - vTilePos;
		fLength = D3DXVec3Length(&vLength);
		if (0.5f > fLength)
			bCorrect2 = true;
#pragma endregion

#pragma region	bLeftCorrect
		vLength = _vec3(24.f, 42.f, 25.f) - vTilePos;
		fLength = D3DXVec3Length(&vLength);
		if (0.5f > fLength)
			bLeftCorrect = true;
#pragma endregion

#pragma region	bRightCorrect
		if (!bLeftCorrect)
		{
			vLength = _vec3(26.f, 42.f, 25.f) - vTilePos;
			fLength = D3DXVec3Length(&vLength);
			if (0.5f > fLength)
				bRightCorrect = true;
		}
#pragma endregion

#pragma region	bCorrect3
		if (bLeftCorrect)
		{
			vLength = _vec3(26.f, 42.f, 24.f) - vTilePos;
			fLength = D3DXVec3Length(&vLength);
			if (0.5f > fLength)
				bCorrect3 = true;
		}
		else if (bRightCorrect)
		{
			vLength = _vec3(24.f, 42.f, 24.f) - vTilePos;
			fLength = D3DXVec3Length(&vLength);
			if (0.5f > fLength)
				bCorrect3 = true;
		}
#pragma endregion
	}

#pragma region	Cleared
	if (bCorrect1 && bCorrect2 && bCorrect3)
	{
		if (FAILED(Ready_Layer_Portal()))
		{
			PRINT_LOG(L"Error", L"Failed to Ready_Layer_Portal");
			return UPDATE_ERROR;
		}

		CPlayer* pPlayer = static_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
		if (pPlayer)
			pPlayer->Set_PortalList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Portal"));

		m_bClear = true;
	}
#pragma endregion

	return NO_EVENT;
}
