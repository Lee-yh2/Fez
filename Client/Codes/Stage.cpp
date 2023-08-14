#include "stdafx.h"
#include "Stage.h"
#include "MainCamera.h"
#include "Player.h"
#include "Vine.h"
#include "InterBox.h"
#include "Item.h"
#include "VirtualCube.h"
#include "UIcube.h"
#include "UIcubepiece.h"
#include "UIkey.h"
#include "Monster.h"
#include "Bird.h"
#include "Cat.h"
#include "Effect.h"
#include "SpeechBubble.h"
#include "NPC.h"

#include "SoundMgr.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile, _bool _bFirst)
	: CScene(pDevice), m_vecTile(vecTile), m_bStart(true), m_bStartPosFinished(false), m_bIntroFinished(false), m_fStartTime(0.f), m_bFirst(_bFirst)
{
}

HRESULT CStage::Ready_Scene()
{


	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Player");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_VirtualCube()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Terrain()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_Terrain");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Monster()))
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

	if (FAILED(Ready_Layer_NPC()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Bird()))
	{
		PRINT_LOG(L"Error", L"Failed to Bird");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Cat()))
	{
		PRINT_LOG(L"Error", L"Failed to Cat");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_SpeechBubble()))
	{
		PRINT_LOG(L"Error", L"Failed to SpeechBubble");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Effect()))
	{
		PRINT_LOG(L"Error", L"Failed to Effect");
		return E_FAIL;
	}
	CManagement* pManagement = CManagement::Get_Instance();

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	//CCamera* pCamera = dynamic_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
	// CInterBox* pBox = dynamic_cast<CInterBox*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj")->front());
	{
		pPlayer->Set_Camera();
		//pPlayer->Set_InterObjList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj"));
		pPlayer->Set_CollisionTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Collision_Terrain"));
		pPlayer->Set_Alpha(true);
		if (!m_bFirst)											//추가
		{
			pPlayer->Set_PlayerPos(_vec3(38.3f, 22.7f, 40.5f));
			pPlayer->Set_FallPos(_vec3(38.3f, 22.7f, 40.5f));
		}
	}

	::SetWindowText(g_hWnd, L"Stage Scene");

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
	CAMERA_DESC	tCamDesc = pCamera->Get_Desc();

	if (m_bFirst)			//추가
	{
		tCamDesc.bToolmode = true;
		pCamera->Set_Desc(tCamDesc);
	}
	else
	{
		tCamDesc.bToolmode = false;
		pCamera->Set_Desc(tCamDesc);
		static_cast<CMainCamera*>(pCamera)->Set_Start(false);
		static_cast<CMainCamera*>(pCamera)->Set_CameraAngle(_vec3(0.f, 0.f, 0.f));
	}

	_vec3	vParentPos = _vec3(0.f, 35.f, 0.f);
	static_cast<CMainCamera*>(pCamera)->Set_ParentPos(vParentPos);

	if (m_bFirst)			//추가
	{
#pragma region	Tile
		list<CGameObject*>*	listTileLayer = static_cast<list<CGameObject*>*>
			(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Collision_Terrain"));
		if (!listTileLayer->empty())
		{
			vParentPos = _vec3(35.f, 35.f, 35.f);
			for (auto& Tile1 : *listTileLayer)
			{
				_float	fTotalTime = (rand() % 100) * 0.05f + 2.5f;
				static_cast<CMonster*>(Tile1)->Set_StartPos(vParentPos, fTotalTime);
			}
		}

		listTileLayer = static_cast<list<CGameObject*>*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj"));
		if (!listTileLayer->empty())
		{
			_float	fTotalTime = (rand() % 100) * 0.1f + 5.f;
			vParentPos = _vec3(35.f, 35.f, 35.f);
			for (auto& Tile1 : *listTileLayer)
			{
				_float	fTotalTime = (rand() % 100) * 0.05f + 2.5f;
				static_cast<CMonster*>(Tile1)->Set_StartPos(vParentPos, fTotalTime);
			}
		}

		/*listTileLayer = static_cast<list<CGameObject*>*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Vine_Terrain"));
		if (!listTileLayer->empty())
		{
			_float	fTotalTime = (rand() % 100) * 0.1f + 5.f;
			vParentPos = _vec3(35.f, 35.f, 35.f);
			for (auto& Tile1 : *listTileLayer)
			{
				_float	fTotalTime = (rand() % 100) * 0.05f + 2.5f;
				static_cast<CMonster*>(Tile1)->Set_StartPos(vParentPos, fTotalTime);
			}
		}*/

		listTileLayer = static_cast<list<CGameObject*>*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj"));
		if (!listTileLayer->empty())
		{
			_float	fTotalTime = (rand() % 100) * 0.1f + 5.f;
			vParentPos = _vec3(35.f, 35.f, 35.f);
			for (auto& Tile1 : *listTileLayer)
			{
				_float	fTotalTime = (rand() % 100) * 0.05f + 2.5f;
				static_cast<CMonster*>(Tile1)->Set_StartPos(vParentPos, fTotalTime);
			}
		}
#pragma endregion
	}
	return S_OK;
}

_uint CStage::Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Update_Scene(fDeltaTime))
		return iEvent;

	CSoundMgr::Get_Instance()->PlaySound(L"ENV_Stage1.mp3", CSoundMgr::ENVIRONMENT);
	CSoundMgr::Get_Instance()->PlaySound(L"BGM_Stage1.wav", CSoundMgr::BGM);

	m_fStartTime += fDeltaTime;

	if (m_bStart && m_bStartPosFinished && m_bIntroFinished)
	{ 
#pragma region	Ready_Layer

		if (FAILED(Ready_Layer_Portal()))
		{
			PRINT_LOG(L"Error", L"Failed to Ready_Layer_Portal");
			return UPDATE_ERROR;
		}

		if (FAILED(Ready_Layer_Obj()))
		{
			PRINT_LOG(L"Error", L"Failed to Ready_Layer_Obj");
			return UPDATE_ERROR;
		}

		if (FAILED(Ready_Layer_Vine()))
		{
			PRINT_LOG(L"Error", L"Failed to Ready_Layer_Vine");
			return UPDATE_ERROR;
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

		if (FAILED(Ready_Layer_Item()))
		{
			PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
			return UPDATE_ERROR;
		}

		if (FAILED(Ready_Layer_UIcubepiece()))
		{
			PRINT_LOG(L"Error", L"Failed to Ready_Layer_VirtualCube");
			return E_FAIL;
		}
#pragma endregion

#pragma region	Player
		CManagement* pManagement = CManagement::Get_Instance();

		CPlayer* pPlayer = static_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

		pPlayer->Set_PortalList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Portal"));
		pPlayer->Set_VineTerrainList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Vine"));
		pPlayer->Set_ItemList(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Item"));   //플레이어에 아이템 전달
		pPlayer->Set_Alpha(false);
		pPlayer->Set_SceneChange(false);		//추가
		pPlayer->Set_CurrentState(CPlayer::GETOUT);
		pPlayer->Set_Frame(0.f);
#pragma endregion

#pragma region	Camera
		if (m_bFirst) //추가
		{
			CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
			CAMERA_DESC	tCamDesc = pCamera->Get_Desc();
			tCamDesc.bToolmode = false;
			pCamera->Set_Desc(tCamDesc);
			static_cast<CMainCamera*>(pCamera)->Set_Start(false);
			static_cast<CMainCamera*>(pCamera)->Set_CameraAngle(_vec3(0.f, 0.f, 0.f));
		}
#pragma endregion

		m_bStart = false;
	}
	if (m_bFirst)	//추가
	{
		if (13.5f <= m_fStartTime && !m_bIntroFinished)
			m_bIntroFinished = true;
		else if (5.f <= m_fStartTime && !m_bStartPosFinished)
			m_bStartPosFinished = true;


		/* 배경음 추가 */
		if (!m_bIntroFinished)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BGM_Move.wav", CSoundMgr::BGM);
		}
		else
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BGM_Stage1.wav", CSoundMgr::BGM);
		}

		if (10.5f > m_fStartTime)
		{

			CManagement* pManagement = CManagement::Get_Instance();
			CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
			CAMERA_DESC	tCamDesc = pCamera->Get_Desc();
			CTransform*	pCamTransform = static_cast<CTransform*>(pCamera->Get_Component(L"Com_Transform"));
			pCamTransform->RotateY(fDeltaTime);
			_vec3		vCamPos = pCamTransform->Get_Pos();
			vCamPos.y -= fDeltaTime * 35.f / 10.5f;
			pCamTransform->Set_Pos(vCamPos);

			_vec3	vCamAngle = static_cast<CMainCamera*>(pCamera)->Get_CameraAngle();
			vCamAngle.y += -50.f * fDeltaTime / 10.5f;
			static_cast<CMainCamera*>(pCamera)->Set_CameraAngle(vCamAngle);
		}
	}
	else
	{
		m_bIntroFinished = true;
		m_bStartPosFinished = true;
	}

	return iEvent;
}

_uint CStage::Late_Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Late_Update_Scene(fDeltaTime))
		return iEvent;




	return iEvent;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile, _bool _bFirst)
{
	if (!pDevice)
	{
		PRINT_LOG(L"Error", L"pDevice is nullptr");
		return nullptr;
	}

	CStage* pInstance = new CStage(pDevice, vecTile, _bFirst);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage::Free()
{
	CScene::Free();
}

HRESULT CStage::Ready_Layer_Player()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(34.f, 25.f, 30.f);
	tDesc.vScale = { 1.f, 1.f, 1.f };
	tDesc.fMoveSpdPerSec = 2.5f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

	return pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Player", ePrototypeType::Static, L"Layer_Player", &tDesc);
}

HRESULT CStage::Ready_Layer_VirtualCube()
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

HRESULT CStage::Ready_Layer_Portal()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(33.f, 13.f, 29.39f);
	tDesc.vScale = { 4.f, 5.f, 1.f };
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
	tDesc.iVineDir = (_uint)eMapDir::SOUTH;
	tDesc.iNum = (_uint)eSceneType::Stage2;
	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
		, L"GameObject_Portal", ePrototypeType::NoneStatic, L"Layer_Portal", &tDesc)))
		return E_FAIL;

	tDesc.vPos = _vec3(38.3f, 22.3f, 39.51f);
	tDesc.vScale = { 1.5f, 1.6f, 1.f };
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
	tDesc.iVineDir = (_uint)eMapDir::NORTH;
	tDesc.iNum = (_uint)eSceneType::GomezHouse;
	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
		, L"GameObject_Portal", ePrototypeType::NoneStatic, L"Layer_Portal", &tDesc)))
		return E_FAIL;

	tDesc.vPos = _vec3(33.f, 13.f, 29.4f);
	tDesc.vScale = { 4.f, 5.f, 1.f };
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
		, L"GameObject_PortalBack", ePrototypeType::NoneStatic, L"Layer_PortalBack", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage::Ready_Layer_Camera()
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

HRESULT CStage::Ready_Layer_Monster()
{
	return S_OK;
}


HRESULT CStage::Ready_Layer_Skybox()
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

HRESULT CStage::Ready_Layer_Obj()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	//TRANSFORM_DESC	tDesc;

	//tDesc.vPos = _vec3(3.f, 0.f, 3.f);
	//tDesc.vLook = { 0.f, 0.f, 1.f };
	//tDesc.fMoveSpdPerSec = 5.f;
	//tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

	//if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_InterBox"
	//	, ePrototypeType::NoneStatic, L"Layer_InterObj", &tDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStage::Ready_Layer_Terrain()
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
		else {
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

HRESULT CStage::Ready_Layer_Vine()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}
	for (_int j = 30; j < 35; ++j)
	{
		for (_int i = 30; i < 33; ++i)
		{
			TRANSFORM_DESC	tDesc;
			tDesc.vPos = _vec3((_float)i, (_float)j, 29.4f);
			tDesc.vLook = { 0.f, 0.f, 1.f };
			tDesc.fMoveSpdPerSec = 5.f;
			tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
			tDesc.iVineDir = (_uint)eMapDir::SOUTH;

			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
				, L"GameObject_Vine", ePrototypeType::NoneStatic, L"Layer_Vine", &tDesc)))
				return E_FAIL;
		}
	}
	for (_int j = 30; j < 34; ++j)
	{
		TRANSFORM_DESC	tDesc;
		tDesc.vPos = _vec3(33, (_float)j, 29.4f);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
		tDesc.iVineDir = (_uint)eMapDir::SOUTH;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
			, L"GameObject_Vine", ePrototypeType::NoneStatic, L"Layer_Vine", &tDesc)))
			return E_FAIL;
	}

	for (_uint j = 21; j > 13; --j) {
		for (_int i = 32; i < 35; ++i) {
			TRANSFORM_DESC    tDesc;
			tDesc.vPos = _vec3((_float)i, (_float)j, 38.6f);
			tDesc.vLook = { 0.f, 0.f, 1.f };
			tDesc.fMoveSpdPerSec = 5.f;
			tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
			tDesc.iVineDir = (_uint)eMapDir::NORTH;

			if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
				, L"GameObject_Vine", ePrototypeType::NoneStatic, L"Layer_Vine", &tDesc)))
				return E_FAIL;
		}
	}
	for (_int i = 33; i < 35; ++i) {
		TRANSFORM_DESC    tDesc;
		tDesc.vPos = _vec3((_float)i, 13, 38.6f);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
		tDesc.iVineDir = (_uint)eMapDir::NORTH;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
			, L"GameObject_Vine", ePrototypeType::NoneStatic, L"Layer_Vine", &tDesc)))
			return E_FAIL;
	}

	for (_int i = 22; i < 25; ++i) {
		TRANSFORM_DESC    tDesc;
		tDesc.vPos = _vec3(32.f, (_float)i, 34.6f);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
		tDesc.iVineDir = (_uint)eMapDir::NORTH;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic
			, L"GameObject_Vine", ePrototypeType::NoneStatic, L"Layer_Vine", &tDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStage::Ready_Layer_UIcube()
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

HRESULT CStage::Ready_Layer_UIcubepiece()
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

HRESULT CStage::Ready_Layer_UIkey()
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

HRESULT CStage::Ready_Layer_Item()
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
	tDesc.vPos = _vec3(44.95f, 32.92f, 50.f);
	tDesc.iVineDir =(_uint) eMapDir::SOUTH;
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);

	tDesc.vPos = _vec3(37.85f, 41.94f, 37.40f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);

	tDesc.vPos = _vec3(30.85f, 29.99f, 27.27f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(19.07f, 43.10f, 21.63f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(26.03f, 48.2f, 39.92f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(39.52f, 51.36f, 29.7f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(41.64f, 22.44f, 38.18f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	tDesc.vPos = _vec3(21.86f, 42.86f, 34.85f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Item", ePrototypeType::NoneStatic, L"Layer_Item", &tDesc);
	return S_OK;
}

HRESULT CStage::Ready_Layer_NPC()
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
		, L"GameObject_NPC", ePrototypeType::NoneStatic, L"Layer_NPC", &tDesc);
}

HRESULT CStage::Ready_Layer_Bird()
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
	tDesc.vPos = _vec3(20.25f, 42.06f, 28.23f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Bird", ePrototypeType::NoneStatic, L"Layer_Bird", &tDesc);
	tDesc.vPos = _vec3(27.95, 30.02, 33.25);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Bird", ePrototypeType::NoneStatic, L"Layer_Bird", &tDesc);
	tDesc.vPos = _vec3(35.87f, 22.19f, 31.94f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Bird", ePrototypeType::NoneStatic, L"Layer_Bird", &tDesc);

	return S_OK;
}

HRESULT CStage::Ready_Layer_Cat()
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
	tDesc.vPos = _vec3(32.25f, 42.06f, 28.23f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Cat", ePrototypeType::NoneStatic, L"Layer_Cat", &tDesc);
	tDesc.vPos = _vec3(33.5f, 30.02f, 36.25f);
	pManagement->Add_GameObjectInLayer(ePrototypeType::Static
		, L"GameObject_Cat", ePrototypeType::NoneStatic, L"Layer_Cat", &tDesc);

	return S_OK;
}

HRESULT CStage::Ready_Layer_SpeechBubble()
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
		, L"GameObject_SpeechBubble", ePrototypeType::NoneStatic, L"Layer_SpeechBubble", &tDesc);

	return S_OK;
}

HRESULT CStage::Ready_Layer_Effect()
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