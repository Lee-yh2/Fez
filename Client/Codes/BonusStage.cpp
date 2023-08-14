#include "stdafx.h"
#include "..\Headers\BonusStage.h"
#include "LoadingScene.h"
#include "Camera.h"
#include "Player.h"
#include "UIFail.h"
#include "UISuccess.h"
#include "SoundMgr.h"

CBonusStage::CBonusStage(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
	: CScene(pDevice), m_fCurPlayTime(0.f), m_bWave1(false), m_bWave2(false), m_bWave3(false), m_bClearStage(false), m_bFailStage(false)
{
}

HRESULT CBonusStage::Ready_Scene()
{
	if (FAILED(CScene::Ready_Scene()))
	{
		return E_FAIL;
	}	

	if (FAILED(Ready_Layer_Terrain()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Terrain");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Player()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Player");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Monster()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Monster");
		return E_FAIL;
	}	

	if (FAILED(Ready_Layer_Camera()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Camera");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Skybox()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Skybox");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Fail()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Skybox");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Success()))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Layer_Skybox");
		return E_FAIL;
	}
	CManagement* pManagement = CManagement::Get_Instance();

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	pPlayer->Set_OnNewBonusStage(true);

	::SetWindowText(g_hWnd, L"BonusStage Scene");

	return S_OK;
}
HRESULT CBonusStage::Ready_Layer_Fail()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_GameOver", ePrototypeType::NoneStatic, L"Layer_GameOver");
}

HRESULT CBonusStage::Ready_Layer_Success()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Success", ePrototypeType::NoneStatic, L"Layer_Success");
}

_uint CBonusStage::Update_Scene(_float & fDeltaTime)
{
	_uint iEvent = NO_EVENT;

	if (iEvent = CScene::Update_Scene(fDeltaTime))
	{
		return iEvent;
	}
	CSoundMgr::Get_Instance()->PlaySound(L"BGM_Bonus1.mp3", CSoundMgr::BGM);
	return iEvent;
}

_uint CBonusStage::Late_Update_Scene(_float & fDeltaTime)
{
	_uint iEvent = NO_EVENT;

	if (iEvent = CScene::Late_Update_Scene(fDeltaTime))
	{
		return iEvent;
	}
	if (m_fCurPlayTime >= 23.f && !m_bClearStage)
	{
		CManagement* pManagement = CManagement::Get_Instance();

		CUISuccess* pUISuccess = dynamic_cast<CUISuccess*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Success")->front());
		if (pUISuccess)
		{
			pUISuccess->Set_Success();
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
			CSoundMgr::Get_Instance()->PlaySound(L"Win.mp3", CSoundMgr::BGM);

			m_bClearStage = true;
		}
	}

	if (!m_bClearStage)
		m_fCurPlayTime += fDeltaTime;

	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && m_bClearStage)
	{
		/*CManagement* pManagement = CManagement::Get_Instance();
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
		pPlayer->Set_ClearNewBonusStage(true);*/
		CManagement*	pManagement = CManagement::Get_Instance();
		if (FAILED(pManagement->Clear_NoneStatic()))
		{
			PRINT_LOG(L"Error", L"Failed to Clear_NoneStatic");
		}

		pManagement->Setup_CurrentScene((_int)eSceneType::Loading, CLoadingScene::Create(m_pDevice, eSceneType::BonusStage2));
	}
	else if ((GetAsyncKeyState('Q') & 0x8000) && !m_bClearStage)
	{
		CManagement* pManagement = CManagement::Get_Instance();

		CUIFail* pUIFail = dynamic_cast<CUIFail*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_GameOver")->front());
		if (pUIFail)
		{
			pUIFail->Set_GameOver();
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
			CSoundMgr::Get_Instance()->PlaySound(L"GameOver.mp3", CSoundMgr::BGM);

			m_bFailStage = true;
		}
	}
	else if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && m_bFailStage)
	{
		CManagement*	pManagement = CManagement::Get_Instance();
		if (FAILED(pManagement->Clear_NoneStatic()))
		{
			PRINT_LOG(L"Error", L"Failed to Clear_NoneStatic");
		}

		pManagement->Setup_CurrentScene((_int)eSceneType::Loading, CLoadingScene::Create(m_pDevice, eSceneType::BonusStage));
	}

	Wave_Set();
	return iEvent;
}

HRESULT CBonusStage::Ready_Layer_Player()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_BonusPlayer", ePrototypeType::NoneStatic, L"Layer_BonusPlayer");
}

HRESULT CBonusStage::Ready_Layer_Terrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_BonusTerrain", ePrototypeType::NoneStatic, L"Layer_Terrain");
}

HRESULT CBonusStage::Ready_Layer_Camera()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	CAMERA_DESC CameraDesc;
	CameraDesc.vUp = _vec3(0.f, 1.f, 0.f);
	CameraDesc.fFovY = D3DXToRadian(90.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fNear = 1.f;
	CameraDesc.fFar = 1000.f;

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_BonusCamera", ePrototypeType::NoneStatic, L"Layer_BonusCamera", &CameraDesc);
}

HRESULT CBonusStage::Ready_Layer_Monster()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC tDesc;
	CTransform* pTargetTransform = (CTransform*)pManagement->Get_Component(
		ePrototypeType::NoneStatic,
		L"Layer_BonusPlayer",
		L"Com_Transform");
	for (_uint i = 0; i < 30; ++i)
	{
		_float x, z;
		while (true)
		{
			x = _float(rand() % 129);
			z = _float(rand() % 129);

			if((x < pTargetTransform->Get_Desc().vPos.x + 5.f && x > pTargetTransform->Get_Desc().vPos.x - 5.f)
				|| (z < pTargetTransform->Get_Desc().vPos.z + 5.f && z > pTargetTransform->Get_Desc().vPos.z - 5.f))
				continue;
			else
				break;
		}

		tDesc.vPos = _vec3(x, 1.f, z);
		tDesc.vScale = { 2.f, 2.f, 1.f };

		if (FAILED(pManagement->Add_GameObjectInLayer(
			ePrototypeType::NoneStatic, 
			L"GameObject_BonusMonster", 
			ePrototypeType::NoneStatic, 
			L"Layer_BonusMonster",
			&tDesc)))
		{
			return E_FAIL;
		}
	}	

	return S_OK;
}

HRESULT CBonusStage::Ready_Layer_Grass()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC TransformDesc;

	for (_uint i = 0; i < 200; ++i)
	{
		_float x = _float(rand() % 129);
		_float z = _float(rand() % 129);

		TransformDesc.vPos = _vec3(x, 0.f, z);

		if (FAILED(pManagement->Add_GameObjectInLayer(
			ePrototypeType::NoneStatic,
			L"GameObject_Grass",
			ePrototypeType::NoneStatic,
			L"Layer_Grass",
			&TransformDesc)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CBonusStage::Ready_Layer_Skybox()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vScale *= 40.f;

	return pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_BonusSkybox", ePrototypeType::NoneStatic, L"Layer_Skybox");
}

HRESULT CBonusStage::Wave_Set()
{

	if (m_fCurPlayTime > 4.f && !m_bWave1)
	{
		Ready_Layer_Monster();
		m_bWave1 = true;
	}
	if (m_fCurPlayTime > 8.f && !m_bWave2)
	{
		Ready_Layer_Monster();
		m_bWave2 = true;
	}
	if (m_fCurPlayTime > 12.f && !m_bWave3)
	{
		Ready_Layer_Monster();
		m_bWave3 = true;
	}
	return S_OK;
}

CBonusStage * CBonusStage::Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile)
{
	if (nullptr == pDevice)
	{
		PRINT_LOG(L"Error", L"pDevice is nullptr");
		return nullptr;
	}

	CBonusStage* pInstance = new CBonusStage(pDevice, vecTile);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBonusStage::Free()
{

	CScene::Free();
}
