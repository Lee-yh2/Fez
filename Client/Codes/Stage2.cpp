#include "stdafx.h"
#include "Player.h"
#include "MainCamera.h"
#include "Stage2.h"

#include "SoundMgr.h"

CStage2::CStage2(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
	: CScene(pDevice), m_vecTile(vecTile)
{
}

HRESULT CStage2::Ready_Scene()
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
	if (FAILED(Ready_Layer_UIcube()))		//Ãß°¡
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
		return UPDATE_ERROR;
	}

	if (FAILED(Ready_Layer_UIcubepiece()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
		return UPDATE_ERROR;
	}

	if (FAILED(Ready_Layer_BlackMonster()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_BlackMonster");
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
		pPlayer->Set_RightRot();
		pPlayer->Set_PlayerPos(_vec3(6.f, 16.8f, 18.f));
		pPlayer->Set_FallPos(_vec3(6.f, 16.8f, 18.f));
		pPlayer->Set_Camera();
		pPlayer->Set_PortalList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Portal"));
		//pPlayer->Set_InterObjList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj"));
		pPlayer->Set_CollisionTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Collision_Terrain"));
		pPlayer->Set_ItemList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Item"));
		//pPlayer->Set_VineTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Vine_Terrain"));
		pPlayer->Set_SceneChange(false);
	}

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
	CAMERA_DESC	tCamDesc = pCamera->Get_Desc();
	tCamDesc.bToolmode = false;
	pCamera->Set_Desc(tCamDesc);
	static_cast<CMainCamera*>(pCamera)->Set_Start(false);
	static_cast<CMainCamera*>(pCamera)->Set_CameraAngle(_vec3(0.f, 0.f, 0.f));

	::SetWindowText(g_hWnd, L"Stage2 Scene");

	return S_OK;
}

_uint CStage2::Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Update_Scene(fDeltaTime))
		return iEvent;

	CSoundMgr::Get_Instance()->PlaySound(L"BGM_Stage2.mp3", CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlaySound(L"ENV_Stage2.mp3", CSoundMgr::ENVIRONMENT);

	return iEvent;
}

_uint CStage2::Late_Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Late_Update_Scene(fDeltaTime))
		return iEvent;

	return iEvent;
}

CStage2 * CStage2::Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
{
	if (!pDevice)
	{
		PRINT_LOG(L"Error", L"pDevice is nullptr");
		return nullptr;
	}

	CStage2* pInstance = new CStage2(pDevice, vecTile);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage2::Free()
{
	CScene::Free();
}

HRESULT CStage2::Ready_Layer_Portal()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(22.f, 45.f, 32.f);
	tDesc.vScale = { 1.5f, 2.f, 1.f };
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
	tDesc.iVineDir = (_uint)eMapDir::NORTH;
	tDesc.iNum = (_uint)eSceneType::Stage2;

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
		, L"GameObject_Portal", ePrototypeType::NoneStatic, L"Layer_Portal", &tDesc);
}

HRESULT CStage2::Ready_Layer_Terrain()
{
	TRANSFORM_DESC   tDesc;
	wstring wsLayerName;
	wstring wsPrototypeName = L"GameObject_Monster";

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

		if (pTile->byPlayDrawID == 4)
			wsLayerName = L"Layer_NoneCollision";
		else if (pTile->byPlayDrawID == 5)
			wsLayerName = L"Layer_Collision_Terrain";
		else
			wsLayerName = L"Layer_Vine";

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

HRESULT CStage2::Ready_Layer_Monster()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;

	for (int j = 0; j < 7; ++j) {
		for (int i = 0; i < 7; ++i) {
			tDesc.vPos = _vec3((_float)i, -20.f, (_float)j);
			tDesc.vLook = { 0.f, 0.f, 1.f };
			tDesc.fMoveSpdPerSec = 5.f;
			tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Monster"
				, ePrototypeType::NoneStatic, L"Layer_Collision_Terrain", &tDesc)))
				return E_FAIL;
		}
	}

	for (int j = 0; j < 3; ++j) {
		for (int i = 0; i < 3; ++i) {
			tDesc.vPos = _vec3((_float)i + 1, -19.f, (_float)j + 1);
			tDesc.vLook = { 0.f, 0.f, 1.f };
			tDesc.fMoveSpdPerSec = 5.f;
			tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Monster"
				, ePrototypeType::NoneStatic, L"Layer_Vine_Terrain", &tDesc)))
				return E_FAIL;
		}
	}
	for (int j = 0; j < 3; ++j) {
		for (int i = 0; i < 3; ++i) {
			tDesc.vPos = _vec3((_float)i + 1, -18.f, (_float)j + 1);
			tDesc.vLook = { 0.f, 0.f, 1.f };
			tDesc.fMoveSpdPerSec = 5.f;
			tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Monster"
				, ePrototypeType::NoneStatic, L"Layer_Vine_Terrain", &tDesc)))
				return E_FAIL;
		}
	}

	for (int j = 0; j < 3; ++j) {
		for (int i = 0; i < 3; ++i) {
			tDesc.vPos = _vec3((_float)i + 1, -17.f, (_float)j + 1);
			tDesc.vLook = { 0.f, 0.f, 1.f };
			tDesc.fMoveSpdPerSec = 5.f;
			tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Monster"
				, ePrototypeType::NoneStatic, L"Layer_Vine_Terrain", &tDesc)))
				return E_FAIL;
		}
	}

	for (int j = 0; j < 3; ++j) {
		for (int i = 0; i < 3; ++i) {
			tDesc.vPos = _vec3((_float)i + 1, -16.f, (_float)j + 1);
			tDesc.vLook = { 0.f, 0.f, 1.f };
			tDesc.fMoveSpdPerSec = 5.f;
			tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Monster"
				, ePrototypeType::NoneStatic, L"Layer_Vine_Terrain", &tDesc)))
				return E_FAIL;
		}
	}

	for (int j = 0; j < 5; ++j) {
		for (int i = 0; i < 5; ++i) {
			tDesc.vPos = _vec3((_float)i, -15.f, (_float)j);
			tDesc.vLook = { 0.f, 0.f, 1.f };
			tDesc.fMoveSpdPerSec = 5.f;
			tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Monster"
				, ePrototypeType::NoneStatic, L"Layer_Collision_Terrain", &tDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}


HRESULT CStage2::Ready_Layer_Skybox()
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

HRESULT CStage2::Ready_Layer_Obj()
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

HRESULT CStage2::Ready_Layer_Camera()
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

HRESULT CStage2::Ready_Layer_VirtualCube()
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

HRESULT CStage2::Ready_Layer_UIcube()
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

HRESULT CStage2::Ready_Layer_UIcubepiece()
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

HRESULT CStage2::Ready_Layer_UIkey()
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

HRESULT CStage2::Ready_Layer_Item()
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
	tDesc.vPos = _vec3(25.f, 44.f, 28.f);
	tDesc.iVineDir = (_uint)eMapDir::SOUTH;
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(31.f, 18.f, 11.8f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(31.f, 22.f, 5.f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(31.f, 28.f, 13.f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(28.f, 29.f, 7.4f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(35.f, 36.f, 10.f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(23.f, 36.f, 18.f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(23.f, 38.f, 24.f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	return S_OK;
}

HRESULT CStage2::Ready_Layer_Effect()
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

HRESULT CStage2::Ready_Layer_BlackMonster()
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
		, L"GameObject_BlackMonster", ePrototypeType::NoneStatic, L"Layer_BlackMonster", &tDesc);

	return S_OK;
}