#include "stdafx.h"
#include "LoadingScene.h"

#pragma	region	Scene Headers
#include "Logo.h"
#include "Menu.h"
#include "Stage.h"
#include "GomezHouse.h"
#include "Stage2.h"
#include "Stage3.h"
#include "BonusStage.h"
#include "BonusStage2.h"

#pragma endregion

#pragma region	GameObject Headers
#include "VirtualCube.h"
#include "Terrain.h"
#include "Monster.h"
#include "Grass.h"
#include "Skybox.h"
#include "Player.h"
#include "InterBox.h"
#include "Vine.h"
#include "Portal.h"
#include "PortalBack.h"
#include "Item.h"
#include "NPC.h"
#include "Bird.h"
#include "Cat.h"
#include "Effect.h"
#include "SpeechBubble.h"
#include "Heli.h"
#include "Bullet.h"
#include "Meteor.h"
#include "Explosion.h"
#include "BlackMonster.h"	//공격몬스터추가

#include "UIFail.h"
#include "UISuccess.h"
#include "BonusMonster.h"
#include "BonusSkybox.h"
#include "BonusPlayer.h"
#include "BonusCamera.h"

#include "GomezRoom.h"
#pragma endregion

#pragma region UI Headers
#include "Font.h"
#include "Font2.h"
#include "Mouse.h"
#include "StarBack.h"
#include "Rect.h"
#include "RectCol.h"
#include "RectRow.h"
#include "RectScaleFree.h"
#include "RectScaleFreeSmall.h"
#include "UIcube.h"
#include "UIcubepiece.h"
#include "UIkey.h"
#pragma endregion

#pragma region	Loading Headers
#include "LoadingBackground.h"
#include "LoadingBar.h"
#include "LoadingRect.h"
#pragma endregion

#pragma region Manager
#include "SoundMgr.h"
#pragma endregion

CLoadingScene::CLoadingScene(LPDIRECT3DDEVICE9 pDevice, eSceneType eNextScene)
	: CScene(pDevice), m_eNextScene(eNextScene), m_bIsFinished(false), m_iFileCount(0)
{
}

HRESULT CLoadingScene::Ready_Scene()
{
	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ENVIRONMENT);

	/* 동영상 */
	CManagement*	pManagement = CManagement::Get_Instance();
	pManagement->Set_CreateOn();

	if (FAILED(Ready_Layer_LoadingBackground()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_LoadingBackground");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_LoadingBar()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_LoadingBar");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_LoadingRect()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_Layer_LoadingRect");
		return E_FAIL;
	}

	wstring wsFilePath = L"../../Data/TileInfo.dat";

	switch (m_eNextScene)
	{
	case eSceneType::Logo:
		break;
	case eSceneType::Menu:
		break;
	case eSceneType::Loading:
		break;
	case eSceneType::Stage:
		wsFilePath = L"../../Data/TileInfo.dat";
		break;
	case eSceneType::GomezHouse:
		wsFilePath = L"../../Data/GomezHouse.dat";
		break;
	case eSceneType::Stage2:
		wsFilePath = L"../../Data/Stage2.dat";
		break;
	case eSceneType::Stage3:
		wsFilePath = L"../../Data/Stage3.dat";
		break;
	case eSceneType::BonusStage:
		wsFilePath = L"../../Data/Stage3.dat";
		break;
	case eSceneType::BonusStage2:
		wsFilePath = L"../../Data/BonusStage2_TileInfo.dat";
		break;
	case eSceneType::NumOfScene:
		break;
	default:
		break;
	}


	if (FAILED(LoadTileData(wsFilePath)))
	{
		PRINT_LOG(L"Error", L"Terrain Files Load Failed");
		return E_FAIL;
	}

	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);
	if (!m_hLoadingThread)
	{
		PRINT_LOG(L"Error", L"Failed to Create m_LoadingThread");
		return E_FAIL;
	}


	InitializeCriticalSection(&m_tCriticalSection);

	return S_OK;
}

_uint CLoadingScene::Update_Scene(_float & fDeltaTime)
{
	CScene::Update_Scene(fDeltaTime);

	if (m_bIsFinished)
	{
		CManagement*	pManagement = CManagement::Get_Instance();
		if (!pManagement)
		{
			PRINT_LOG(L"LoadingScene", L"pManagement is nullptr");
			return UPDATE_ERROR;
		}

		{
			list<CGameObject*>*	listLayer1 = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBackground");
			if (listLayer1->empty())
				PRINT_LOG(L"Error", L"listLayer1 is empty");

			CLoadingBackground*	pBackground = static_cast<CLoadingBackground*>(listLayer1->front());
			pBackground->Free();
			listLayer1->clear();

			list<CGameObject*>*	listLayer2 = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBar");
			if (listLayer2->empty())
				PRINT_LOG(L"Error", L"listLayer2 is empty");

			CLoadingBar*	pBar = static_cast<CLoadingBar*>(listLayer2->front());
			pBar->Free();
			listLayer2->clear();

			list<CGameObject*>*	listLayer3 = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingRect");
			if (listLayer3->empty())
				PRINT_LOG(L"Error", L"listLayer3 is empty");

			CLoadingRect*	pRect = static_cast<CLoadingRect*>(listLayer3->front());
			pRect->Free();
			listLayer3->clear();
		}

		CScene*	pNextScene = nullptr;
		switch (m_eNextScene)
		{
		case eSceneType::Logo:
			pNextScene = CLogo::Create(m_pDevice);
			break;
		case eSceneType::Menu:
			pNextScene = CMenu::Create(m_pDevice);
			break;
		case eSceneType::Loading:
			break;
		case eSceneType::Stage:
			bStage = true;
			pManagement->Set_Stage(bStage);
			pNextScene = CStage::Create(m_pDevice, m_vecTile, m_bFirst);
			break;
		case eSceneType::GomezHouse:
			bStage = false;
			pManagement->Set_Stage(bStage);
			pNextScene = CGomezHouse::Create(m_pDevice, m_vecTile);
			break;
		case eSceneType::Stage2:
			bStage = true;
			pManagement->Set_Stage(bStage);
			pNextScene = CStage2::Create(m_pDevice, m_vecTile);
			break;
		case eSceneType::Stage3:
			bStage = true;
			pManagement->Set_Stage(bStage);
			pNextScene = CStage3::Create(m_pDevice, m_vecTile);
			break;
		case eSceneType::BonusStage:
			bStage = true;
			pManagement->Set_Stage(bStage);
			pNextScene = CBonusStage::Create(m_pDevice, m_vecTile);
			break;
		case eSceneType::BonusStage2:
			bStage = true;
			pManagement->Set_Stage(bStage);
			pNextScene = CBonusStage2::Create(m_pDevice, m_vecTile);
			break;
		case eSceneType::NumOfScene:
			break;
		default:
			break;
		}

		if (FAILED(pManagement->Setup_CurrentScene((_int)m_eNextScene, pNextScene)))
		{
			PRINT_LOG(L"LoadingScene", L"Failed to Setup_CurrentScene pNextScene");
			return UPDATE_ERROR;
		}

		return SCENE_CHANGE;
	}

	return NO_EVENT;
}

_uint CLoadingScene::Late_Update_Scene(_float & fDeltaTime)
{
	CScene::Late_Update_Scene(fDeltaTime);

	return NO_EVENT;
}

HRESULT CLoadingScene::Ready_Layer_LoadingBackground()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vScale *= 2.f;

	return pManagement->Add_GameObjectInLayer(ePrototypeType::Static, L"GameObject_LoadingBackground"
		, ePrototypeType::NoneStatic, L"Layer_LoadingBackground", &tDesc);
}

HRESULT CLoadingScene::Ready_Layer_LoadingBar()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vScale.x *= 1.5f;
	tDesc.vScale.y *= 0.04f;
	tDesc.vPos.x -= 0.25f;
	tDesc.vPos.y -= 0.725f;

	return pManagement->Add_GameObjectInLayer(ePrototypeType::Static, L"GameObject_LoadingBar"
		, ePrototypeType::NoneStatic, L"Layer_LoadingBar", &tDesc);
}

HRESULT CLoadingScene::Ready_Layer_LoadingRect()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vScale *= 0.1f;
	tDesc.vPos.x += 0.8f;
	tDesc.vPos.y -= 0.7f;
	tDesc.fRotSpdPerSec = D3DXToRadian(270.f);

	return pManagement->Add_GameObjectInLayer(ePrototypeType::Static, L"GameObject_LoadingRect"
		, ePrototypeType::NoneStatic, L"Layer_LoadingRect", &tDesc);
}

HRESULT CLoadingScene::LoadTileData(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;

	TILEINFO* pTile;

	while (true)
	{
		pTile = new TILEINFO;

		ReadFile(hFile, &pTile->vSize, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &pTile->vColSize, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &pTile->vRot, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &pTile->vPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &pTile->byOption, sizeof(BYTE), &dwByte, nullptr);
		ReadFile(hFile, &pTile->byPlayDrawID, sizeof(BYTE), &dwByte, nullptr);
		ReadFile(hFile, &pTile->byDevDrawID, sizeof(BYTE), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iStageNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pTile->iIndex, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}
		m_vecTile.emplace_back(pTile);
	}
	CloseHandle(hFile);
	return S_OK;
}

CLoadingScene * CLoadingScene::Create(LPDIRECT3DDEVICE9 pDevice, eSceneType eNextScene, _bool _bFirst)
{
	CLoadingScene*	pInstance = new CLoadingScene(pDevice, eNextScene);
	pInstance->m_bFirst = _bFirst;
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed to Create CLoadingScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingScene::Free()
{
	CloseHandle(m_hLoadingThread);
	DeleteCriticalSection(&m_tCriticalSection);

	CScene::Free();
}

_uint CLoadingScene::ThreadMain(void * pParam)
{
	CLoadingScene*	pCurrentScene = (CLoadingScene*)pParam;
	if (!pCurrentScene)
	{
		PRINT_LOG(L"Error", L"pCurrentScene is nullptr");
		return UPDATE_ERROR;
	}

	EnterCriticalSection(&pCurrentScene->m_tCriticalSection);

	HRESULT	hr = 0;

	switch (pCurrentScene->m_eNextScene)
	{
	case eSceneType::Logo:
		hr = pCurrentScene->Ready_LogoResources();
		break;
	case eSceneType::Menu:
		hr = pCurrentScene->Ready_MenuResources();
		break;
	case eSceneType::Stage:
		hr = pCurrentScene->Ready_StageResources();
		break;
	case eSceneType::GomezHouse:
		hr = pCurrentScene->Ready_GomezHouseResources();
		break;
	case eSceneType::Stage2:
		hr = pCurrentScene->Ready_Stage2Resources();
		break;
	case eSceneType::Stage3:
		hr = pCurrentScene->Ready_Stage3Resources();
		break;
	case eSceneType::BonusStage:
		hr = pCurrentScene->Ready_BonusStageResources();
		break;
	case eSceneType::BonusStage2:
		hr = pCurrentScene->Ready_BonusStage2Resources();
		break;
	default:
		break;
	}

	LeaveCriticalSection(&pCurrentScene->m_tCriticalSection);

	if (FAILED(hr))
	{
		PRINT_LOG(L"Error", L"Failed to Loading to Resources");
		return UPDATE_ERROR;
	}

	pCurrentScene->m_bIsFinished = true;

	return NO_EVENT;
}

HRESULT CLoadingScene::Ready_LogoResources()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	list<CGameObject*>*	listLayer = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBar");
	CLoadingBar*		pLoadingBar = static_cast<CLoadingBar*>(listLayer->front());
	CTransform*			pTransform = static_cast<CTransform*>(pLoadingBar->Get_Component(L"Com_Transform"));
	TRANSFORM_DESC		tOldTempDesc = pTransform->Get_Desc();
	TRANSFORM_DESC		tTempDesc = pTransform->Get_Desc();
	m_iFileCount = 200;
	_float	fFileCount = 0.f;
	tTempDesc.vScale.x = 0.f;
	tTempDesc.vPos.x = tOldTempDesc.vPos.x * (tOldTempDesc.vScale.x + 2.f);
	pTransform->Set_Desc(tTempDesc);


#pragma region Hell

	
	/* Texture_Boss */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static, L"Component_Texture_Hell"
		, CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Hell/Boss.png"))))
	{
		PRINT_LOG(L"Error-MFCToolView-Ready_StaticResources", L"Failed to Add Component_Texture_Hell Prototype");
		return E_FAIL;
	}

	/* Texture_Missile */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static, L"Component_Texture_Missile"
		, CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Hell/Missile.png"))))
	{
		PRINT_LOG(L"Error-MFCToolView-Ready_StaticResources", L"Failed to Add Component_Texture_Missile Prototype");
		return E_FAIL;
	}

	/* Texture_Cannon */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static, L"Component_Texture_Cannon"
		, CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Hell/Cannon.png"))))
	{
		PRINT_LOG(L"Error-MFCToolView-Ready_StaticResources", L"Failed to Add Component_Texture_Cannon Prototype");
		return E_FAIL;
	}

	/* Texture_Explosion */ // 찬수 추가분
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Explosion", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Hell/Explosion_%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Explosion Prototype");
		return E_FAIL;
	}

#pragma endregion

#pragma region	Player
	/*Player*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_Player", CPlayer::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Player Prototype");
		return E_FAIL;
	}

	/*Texture_Player*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_Player", CPlayer::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Player Prototype");
		return E_FAIL;
	}

	/*Texture_Player*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_Idle", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/Idle/Idle%d.png", 10))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 10.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (10.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_Walk", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/Walk/Walk%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 6.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (6.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_Jump", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/Jump/Jump%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 6.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (6.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_BeginFall", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/BeginFall/BeginFall%d.png", 3))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 3.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (3.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_EndFall", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/EndFall/EndFall%d.png", 4))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 4.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (4.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_FallDie", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/FallDie/FallDie%d.png", 9))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 9.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (9.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_Climb", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/Climb/Climb%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 6.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (6.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_GetIn", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/GetIn/GetIn%d.png", 13))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 13.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (13.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_GetOut", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/GetOut/GetOut%d.png", 14))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 14.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (14.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_GrapIdle", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/GrapIdle/GrapIdle%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_GrapMove", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/GrapMove/GrapMove%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 6.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (6.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_GrapUp", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/GrapUp/GrapUp%d.png", 7))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 7.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (7.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_LiftBox", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/LiftBox/LiftBox%d.png", 10))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 10.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (10.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_LiftBoxMove", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/LiftBoxMove/LiftBoxMove%d.png", 8))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 8.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (8.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_PutDownBox", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/PutDownBox/PutDownBox%d.png", 8))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 8.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (8.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_Player_LiftBoxIdle", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/LiftBoxIdle/LiftBoxIdle%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);
#pragma endregion

	/*Texture_PlayTile*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static, L"Component_Texture_Tile_PlayTile"
		, CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Stage/CubeImages/Tile/PlayTile/PlayTile%d.dds", 82))))
	{
		PRINT_LOG(L"Error-MFCToolView-Ready_StaticResources", L"Failed to Add Component_Texture_PlayTile Prototype");
		return E_FAIL;
	}

	fFileCount += 80.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (80.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	/*Texture_DevTile*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static, L"Component_Texture_Tile_DevTile"
		, CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Stage/CubeImages/Tile/DevTile/DevTile%d.dds", 8))))
	{
		PRINT_LOG(L"Error-MFCToolView-Ready_StaticResources", L"Failed to Add Component_Texture_DevTile Prototype");
		return E_FAIL;
	}

	fFileCount += 8.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (8.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	if (FAILED(pManagement->ReadImagePath("../../Data/Client_PathInfo.txt")))
	{
		PRINT_LOG(L"Error", L"Failed to Load PathExtraction");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoadingScene::Ready_StageResources()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	list<CGameObject*>*	listLayer = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBar");
	CLoadingBar*		pLoadingBar = static_cast<CLoadingBar*>(listLayer->front());
	CTransform*			pTransform = static_cast<CTransform*>(pLoadingBar->Get_Component(L"Com_Transform"));
	TRANSFORM_DESC		tOldTempDesc = pTransform->Get_Desc();
	TRANSFORM_DESC		tTempDesc = pTransform->Get_Desc();
	m_iFileCount = 2;
	_float	fFileCount = 0.f;
	tTempDesc.vScale.x = 0.f;
	tTempDesc.vPos.x = tOldTempDesc.vPos.x * (tOldTempDesc.vScale.x + 2.f);
	pTransform->Set_Desc(tTempDesc);

#pragma region	GameObjects
	/*InterBox*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_InterBox", CInterBox::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
		return E_FAIL;
	}

	/*Monster*/  //진짜몬스터 아님...
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Monster", CMonster::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
		return E_FAIL;
	}

	/*Sky box*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}

	/*Portal*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Portal", CPortal::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}
	/*PortalBack*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_PortalBack", CPortalBack::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}
	/*Vine*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Vine", CVine::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add CVine ProtoType");
		return E_FAIL;
	}

	/*virtualCube*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_VirtualCube", CVirtualCube::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_VirtualCube", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Player/VirtualCube0%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_VirtualCube Prototype");
		return E_FAIL;
	}

	/*UICube*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_UIcube", CUIcube::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_UIcube", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/UIcube/Cube0%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}
	/*UICubepiece*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_UIcubepiece", CUIcubepiece::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_UIcubepiece", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/UIcubepiece/Cube0%d.png", 9))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}
	/*UIkey*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_UIkey", CUIkey::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_UIkey", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/UIkey/Key0%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	/*Item*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_Item", CItem::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_RealCube", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/YellowCube0%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	/*BlackMonster*/	//얘가 진짜 몬스터
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_BlackMonster", CBlackMonster::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BlackCube", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/BlackCube0%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	/*NPC*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_NPC", CNPC::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_NPC", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/NPC0%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_VirtualCube Prototype");
		return E_FAIL;
	}
	/*Bird*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_Bird", CBird::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Bird Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BirdWalk", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Bird/Walk/Walk0%d.png", 5))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BirdSFly", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Bird/SFly/SFly0%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BirdFly", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Bird/Fly/Fly0%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
	/*cat*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_Cat", CCat::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Bird Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_CatIdle", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Cat/Idle/Idle0%d.png", 5))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_CatJump", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Cat/Jump/Jump0%d.png", 14))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_CatStand", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Cat/Stand/Stand0%d.png", 7))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
	/*Speech_Bubble*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_SpeechBubble", CSpeechBubble::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Bird Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_SpeechBubble", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/SpeechBubble/Bubble0%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
	/*Effect*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_Effect", CEffect::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Bird Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Effect", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Effect/Effect0%d.png", 4))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
#pragma endregion

#pragma region	Components
	/*Texture_Vine*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Vine", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Vine/Vine%d.png", 4))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	/*Texture_Portal*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Portal", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Door/Door%d.png", 4))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	/*Texture_PortalBack*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_PortalBack", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Door/DoorBack%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	/*Texture_Monster*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Monster", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Koala%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);


	/*Texture_Sky box*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Skybox", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Sky/Morning%d.dds", 2))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Skybox ProtoType");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);
#pragma endregion

#pragma region Terrain

	/*wstring wsFilePath = L"";

	wsFilePath = L"../../Data/TileInfo.dat";

	if (FAILED(LoadTileData(wsFilePath)))
	{
	PRINT_LOG(L"Error", L"Terrain Files Load Failed");
	return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	wstring wsLayerName;
	wstring wsPrototypeName;

	for each (TILEINFO* pTile in m_vecTile)
	{
	if (pTile->byOption & TILE_COL) {
	wsPrototypeName = L"GameObject_Monster";
	wsLayerName = L"Layer_Collision_Terrain";
	}
	else if(pTile->byOption & TILE_INTER)
	{
	wsPrototypeName = L"GameObject_InterBox";
	wsLayerName = L"Layer_InterObj";
	}
	else if(pTile->byOption & TILE_VINE0)
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
	}*/

#pragma endregion

	return S_OK;
}

HRESULT CLoadingScene::Ready_Stage2Resources()
{
		CManagement*	pManagement = CManagement::Get_Instance();
		if (!pManagement)
		{
			PRINT_LOG(L"Error", L"pManagement is nullptr");
			return E_FAIL;
		}

		list<CGameObject*>*	listLayer = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBar");
		CLoadingBar*		pLoadingBar = static_cast<CLoadingBar*>(listLayer->front());
		CTransform*			pTransform = static_cast<CTransform*>(pLoadingBar->Get_Component(L"Com_Transform"));
		TRANSFORM_DESC		tOldTempDesc = pTransform->Get_Desc();
		TRANSFORM_DESC		tTempDesc = pTransform->Get_Desc();
		m_iFileCount = 2;
		_float	fFileCount = 0.f;
		tTempDesc.vScale.x = 0.f;
		tTempDesc.vPos.x = tOldTempDesc.vPos.x * (tOldTempDesc.vScale.x + 2.f);
		pTransform->Set_Desc(tTempDesc);

	#pragma region	GameObjects
		/*InterBox*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_InterBox", CInterBox::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
			return E_FAIL;
		}

		/*Monster*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_Monster", CMonster::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
			return E_FAIL;
		}

		/*Sky box*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
			return E_FAIL;
		}

		/*Portal*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_Portal", CPortal::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
			return E_FAIL;
		}
	#pragma endregion

	#pragma region	Components

		/*Texture_Monster*/
		if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
			, L"Component_Texture_Monster", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Koala%d.dds"))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
			return E_FAIL;
		}

		fFileCount += 1.f;
		tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
		tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
		pTransform->Set_Desc(tTempDesc);

		/*Texture_Sky box*/
		if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
			, L"Component_Texture_Skybox", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Player%d.dds"))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Skybox ProtoType");
			return E_FAIL;
		}

		fFileCount += 1.f;
		tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
		tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
		pTransform->Set_Desc(tTempDesc);
	#pragma endregion

		return S_OK;
	}

HRESULT CLoadingScene::Ready_Stage3Resources()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	list<CGameObject*>*	listLayer = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBar");
	CLoadingBar*		pLoadingBar = static_cast<CLoadingBar*>(listLayer->front());
	CTransform*			pTransform = static_cast<CTransform*>(pLoadingBar->Get_Component(L"Com_Transform"));
	TRANSFORM_DESC		tOldTempDesc = pTransform->Get_Desc();
	TRANSFORM_DESC		tTempDesc = pTransform->Get_Desc();
	m_iFileCount = 2;
	_float	fFileCount = 0.f;
	tTempDesc.vScale.x = 0.f;
	tTempDesc.vPos.x = tOldTempDesc.vPos.x * (tOldTempDesc.vScale.x + 2.f);
	pTransform->Set_Desc(tTempDesc);

#pragma region	GameObjects
	/*InterBox*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_InterBox", CInterBox::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
		return E_FAIL;
	}

	/*Monster*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Monster", CMonster::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
		return E_FAIL;
	}

	/*Sky box*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}

	/*Portal*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Portal", CPortal::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}
	/*PortalBack*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_PortalBack", CPortalBack::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}
	/*Vine*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Vine", CVine::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add CVine ProtoType");
		return E_FAIL;
	}

	/*virtualCube*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_VirtualCube", CVirtualCube::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_VirtualCube", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Player/VirtualCube0%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_VirtualCube Prototype");
		return E_FAIL;
	}

	/*UICube*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_UIcube", CUIcube::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_UIcube", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/UIcube/Cube0%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}
	/*UICubepiece*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_UIcubepiece", CUIcubepiece::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_UIcubepiece", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/UIcubepiece/Cube0%d.png", 9))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}
	/*UIkey*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_UIkey", CUIkey::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Texture_UIkey", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Player/UIkey/Key0%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	/*Item*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_Item", CItem::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VirtualCube Prototype");
		return E_FAIL;
	}

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_RealCube", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/YellowCube0%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	/*Effect*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_Effect", CEffect::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Bird Prototype");
		return E_FAIL;
	}

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Effect", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Effect/Effect0%d.png", 4))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Bird Prototype");
		return E_FAIL;
	}
#pragma endregion

#pragma region	Components
	/*Texture_Vine*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Vine", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Vine/Vine%d.png", 4))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	/*Texture_Portal*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Portal", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Door/Door%d.png", 4))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	/*Texture_PortalBack*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_PortalBack", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Door/DoorBack%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	/*Texture_Monster*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Monster", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Koala%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);


	/*Texture_Sky box*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Skybox", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Sky/Morning%d.dds", 2))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Skybox ProtoType");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	/* Helicopter */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Heli", CHeli::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Player Prototype");
		return E_FAIL;
	}

	/* Bullet_Normal */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Bullet", CBullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Player Prototype");
		return E_FAIL;
	}

	/* Bullet_Meteor */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Meteor", CMeteor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Player Prototype");
		return E_FAIL;
	}

	/* Explosion */ // 찬수 추가분
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Explosion", CExplosion::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_Explosion Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);
#pragma endregion

	return S_OK;
}

	HRESULT CLoadingScene::Ready_GomezHouseResources()
	{
		CManagement*	pManagement = CManagement::Get_Instance();
		if (!pManagement)
		{
			PRINT_LOG(L"LoadingScene", L"pManagement is nullptr");
			return E_FAIL;
		}

		list<CGameObject*>*	listLayer = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBar");
		CLoadingBar*		pLoadingBar = static_cast<CLoadingBar*>(listLayer->front());
		CTransform*			pTransform = static_cast<CTransform*>(pLoadingBar->Get_Component(L"Com_Transform"));
		TRANSFORM_DESC		tOldTempDesc = pTransform->Get_Desc();
		TRANSFORM_DESC		tTempDesc = pTransform->Get_Desc();
		m_iFileCount = 2;
		_float	fFileCount = 0.f;
		tTempDesc.vScale.x = 0.f;
		tTempDesc.vPos.x = tOldTempDesc.vPos.x * (tOldTempDesc.vScale.x + 2.f);
		pTransform->Set_Desc(tTempDesc);


	#pragma region	GameObjects
		/*InterBox*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_InterBox", CInterBox::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
			return E_FAIL;
		}

		/*Monster*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_Monster", CMonster::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
			return E_FAIL;
		}

		/*Sky box*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
			return E_FAIL;
		}

		/*Portal*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_Portal", CPortal::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
			return E_FAIL;
		}

		/*PortalBack*/
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_PortalBack", CPortalBack::Create(m_pDevice))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
			return E_FAIL;
		}

		/* GomezHouse */
		if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
			, L"GameObject_GomezHouse", CGomezRoom::Create(m_pDevice))))
		{
			PRINT_LOG(L"LoadingScene", L"Failed to Add GomezHouse ProtoType");
			return E_FAIL;
		}

	#pragma endregion

	#pragma region	Components

		/*Texture_Monster*/
		if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
			, L"Component_Texture_Monster", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Koala%d.dds"))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
			return E_FAIL;
		}

		fFileCount += 1.f;
		tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
		tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
		pTransform->Set_Desc(tTempDesc);

		/*Texture_Sky box*/
		if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
			, L"Component_Texture_Skybox", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Player%d.dds"))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Skybox ProtoType");
			return E_FAIL;
		}

		fFileCount += 1.f;
		tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
		tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
		pTransform->Set_Desc(tTempDesc);


		/* Texture_GomezHouse */
		if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
			, L"Component_Texture_GomezHouse", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Room/GomezHouse.dds"))))
		{
			PRINT_LOG(L"LoadingScene", L"Failed to Add Component_Texture_GomezHouse ProtoType");
			return E_FAIL;
		}

		/*Texture_Portal*/
		if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
			, L"Component_Texture_Portal", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Door/Door%d.png", 4))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
			return E_FAIL;
		}

		/*Texture_PortalBack*/
		if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
			, L"Component_Texture_PortalBack", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Door/DoorBack%d.png", 1))))
		{
			PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
			return E_FAIL;
		}
	#pragma endregion

		return S_OK;
}

HRESULT CLoadingScene::Ready_BonusStageResources()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"LoadingScene", L"pManagement is nullptr");
		return E_FAIL;
	}

	/* For.GameObject_Terrain */
	if (FAILED(pManagement->Add_GameObject_Prototype(
		ePrototypeType::NoneStatic,
		L"GameObject_BonusTerrain",
		CTerrain::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Terrain Prototype");
		return E_FAIL;
	}

	/* For.GameObject_BonusPlayer */
	if (FAILED(pManagement->Add_GameObject_Prototype(
		ePrototypeType::NoneStatic,
		L"GameObject_BonusPlayer",
		CBonusPlayer::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Terrain Prototype");
		return E_FAIL;
	}

	/* For.GameObject_GameOver */
	if (FAILED(pManagement->Add_GameObject_Prototype(
		ePrototypeType::NoneStatic,
		L"GameObject_GameOver",
		CUIFail::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Terrain Prototype");
		return E_FAIL;
	}

	/* For.GameObject_Success */
	if (FAILED(pManagement->Add_GameObject_Prototype(
		ePrototypeType::NoneStatic,
		L"GameObject_Success",
		CUISuccess::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Terrain Prototype");
		return E_FAIL;
	}

	/*BonusSky box*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_BonusSkybox", CBonusSkybox::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}

	/*BonusMonster*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_BonusMonster", CBonusMonster::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}

	/* For.GameObject_BonusCamera */
	if (FAILED(pManagement->Add_GameObject_Prototype(
		ePrototypeType::NoneStatic,
		L"GameObject_BonusCamera",
		CBonusCamera::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Terrain Prototype");
		return E_FAIL;
	}

	/*VIBuffer_BonusTerrainTexture*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_VIBuffer_BonusTerrainTexture", CVIBuffer_BonusTerrainTexture::Create(m_pDevice, 129, 129))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VIBuffer_RectTexture Prototype");
		return E_FAIL;
	}

#pragma region	Components

	/*Texture_GameOver*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_GameOver", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/GameOver%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	/*Texture_Success*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Success", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Success%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}


	if (FAILED(pManagement->Add_Component_Prototype(
		ePrototypeType::NoneStatic,
		L"Component_Texture_Terrain",
		CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Terrain/Terrain%d.png"))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player Prototype");
		return E_FAIL;
	}

	/*Texture_MonsterBigPlant*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BigPlant", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/BigPlant/sBigPlantWalk_%d.png", 10))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Monkey", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Monkey/sMonkeyWalk_%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusSky", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Sky/BonusSky%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Silk", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Silk/sSilkIdle_%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerIdle_Down", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/IDLE/DOWN/IDLE_DOWN%d.png", 10))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerIdle_Left", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/IDLE/LEFT/IDLE_LEFT%d.png", 10))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerIdle_Right", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/IDLE/RIGHT/IDLE_RIGHT%d.png", 10))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerIdle_Up", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/IDLE/UP/IDLE_UP%d.png", 10))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerWalk_Down", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/WALK/DOWN/WALK_DOWN%d.png", 8))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerWalk_Left", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/WALK/LEFT/WALK_LEFT%d.png", 8))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerWalk_Right", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/WALK/RIGHT/WALK_RIGHT%d.png", 8))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerWalk_Up", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/WALK/UP/WALK_UP%d.png", 8))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusPlayerWalk_Up", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/Player/WALK/UP/WALK_UP%d.png", 8))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_BonusGameOver", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Monster/GameOver%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}
#pragma endregion

	return S_OK;
}

HRESULT CLoadingScene::Ready_BonusStage2Resources()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	list<CGameObject*>*	listLayer = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBar");
	CLoadingBar*		pLoadingBar = static_cast<CLoadingBar*>(listLayer->front());
	CTransform*			pTransform = static_cast<CTransform*>(pLoadingBar->Get_Component(L"Com_Transform"));
	TRANSFORM_DESC		tOldTempDesc = pTransform->Get_Desc();
	TRANSFORM_DESC		tTempDesc = pTransform->Get_Desc();
	m_iFileCount = 2;
	_float	fFileCount = 0.f;
	tTempDesc.vScale.x = 0.f;
	tTempDesc.vPos.x = tOldTempDesc.vPos.x * (tOldTempDesc.vScale.x + 2.f);
	pTransform->Set_Desc(tTempDesc);

#pragma region	GameObjects
	/*InterBox*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_InterBox", CInterBox::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
		return E_FAIL;
	}

	/*Monster*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Monster", CMonster::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Monster ProtoType");
		return E_FAIL;
	}

	/*Sky box*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}

	/*Portal*/
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Portal", CPortal::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Skybox ProtoType");
		return E_FAIL;
	}
#pragma endregion

#pragma region	Components

	/*Texture_Monster*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Monster", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Koala%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Mosnter ProtoType");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);

	/*Texture_Sky box*/
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Texture_Skybox", CTexture::Create(m_pDevice, eTextureType::Cube, L"../Resources/Player%d.dds"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Texture_Skybox ProtoType");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.5f;
	pTransform->Set_Desc(tTempDesc);
#pragma endregion

	return S_OK;
}

HRESULT CLoadingScene::Ready_MenuResources()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}
	list<CGameObject*>*	listLayer = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_LoadingBar");
	CLoadingBar*		pLoadingBar = static_cast<CLoadingBar*>(listLayer->front());
	CTransform*			pTransform = static_cast<CTransform*>(pLoadingBar->Get_Component(L"Com_Transform"));
	TRANSFORM_DESC		tOldTempDesc = pTransform->Get_Desc();
	TRANSFORM_DESC		tTempDesc = pTransform->Get_Desc();
	m_iFileCount = 10;
	_float	fFileCount = 0.f;
	tTempDesc.vScale.x = 0.f;
	tTempDesc.vPos.x = tOldTempDesc.vPos.x * (tOldTempDesc.vScale.x + 2.f);
	pTransform->Set_Desc(tTempDesc);


#pragma region	GameObjects


	/* StarBack */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_StarBack", CStarBack::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_StarBack Prototype");
		return E_FAIL;
	}

	/* Font */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_MenuFont", CFont::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_MenuFont Prototype");
		return E_FAIL;
	}

	/* Font2 */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_MenuFont2", CFont2::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_MenuFont2 Prototype");
		return E_FAIL;
	}

	/* Mouse */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Mouse", CMouse::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_Mouse Prototype");
		return E_FAIL;
	}

	/* Rect */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_Rect", CRect::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_Rect Prototype");
		return E_FAIL;
	}

	/* RectCol */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_RectCol", CRectCol::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_RectCol Prototype");
		return E_FAIL;
	}

	/* RectRow */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_RectRow", CRectRow::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_RectRow Prototype");
		return E_FAIL;
	}

	/* Rect_ScaleFree */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_RectScaleFree", CRectScaleFree::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_RectScaleFree Prototype");
		return E_FAIL;
	}

	/* Rect_ScaleFree */
	if (FAILED(pManagement->Add_GameObject_Prototype(ePrototypeType::NoneStatic
		, L"GameObject_RectScaleFreeSmall", CRectScaleFreeSmall::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add GameObject_RectScaleFree Prototype");
		return E_FAIL;
	}

#pragma endregion




#pragma region	Components


	/* StarBack */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_StarBack", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/StarBack.png"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_StarBack Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

	/* Font_GameMenu */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_MenuFont", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/GameMenu/GameMenu0.png"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_GameMenu Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

	/* Font_GameMenu2 */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_MenuFont2", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/GameMenu/GameMenu1.png"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_GameMenu Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

	/* Mouse */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Mouse", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Mouse/Mouse%d.png", 2))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Mouse Prototype");
		return E_FAIL;
	}

	fFileCount += 2.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (2.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

	/* Rect */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_Rect", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Rect/AlphaRect.png"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Rect Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

	/* RectCol */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_RectCol", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Rect/RectCol.png"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_RectCol Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

	/* RectRow */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_RectRow", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Rect/RectRow.png"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_RectRow Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

	/* Rect_ScaleFree */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_RectScaleFree", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Rect/AlphaRect.png"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_RectScaleFree Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

	/* Rect_ScaleFree_Small */
	if (FAILED(pManagement->Add_Component_Prototype(ePrototypeType::NoneStatic
		, L"Component_RectScaleFreeSmall", CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Rect/AlphaRect_Small.png"))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_RectScaleFreeSmall Prototype");
		return E_FAIL;
	}

	fFileCount += 1.f;
	tTempDesc.vScale.x = tOldTempDesc.vScale.x * (fFileCount / m_iFileCount);
	tTempDesc.vPos.x += tOldTempDesc.vScale.x * (1.f / m_iFileCount) * 0.2f;
	pTransform->Set_Desc(tTempDesc);

#pragma endregion

	return S_OK;
}
