#include "stdafx.h"
#include "Menu.h"
#include "LoadingScene.h"

#include "SoundMgr.h"

CMenu::CMenu(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

HRESULT CMenu::Ready_Scene()
{
	CSoundMgr::Get_Instance()->PlaySound(L"Menu_BorderStart.mp3", CSoundMgr::UI);
	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;




	if (FAILED(Ready_Layer_StarBack()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_StarBack");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_MenuFont()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_MenuFont");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_MenuFont2()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_MenuFont2");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Mouse()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_Mouse");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Rect()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_Rect");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_RectScaleFree()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_RectScaleFree");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_RectScaleFreeSmall()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_RectScaleFree");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_RectCol()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_RectCol");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_RectRow()))
	{
		PRINT_LOG(L"Menu", L"Failed to Ready_Layer_RectRow");
		return E_FAIL;
	}



	::SetWindowText(g_hWnd, L"Menu Scene");

	return S_OK;
}

_uint CMenu::Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;
	CSoundMgr::Get_Instance()->PlaySound(L"BGM_Menu.mp3", CSoundMgr::BGM);

	if (iEvent = CScene::Update_Scene(fDeltaTime))
		return iEvent;



	return iEvent;
}

_uint CMenu::Late_Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Late_Update_Scene(fDeltaTime))
		return iEvent;

	if (GetAsyncKeyState(VK_RETURN))
	{
		if (!m_bChange)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
			CSoundMgr::Get_Instance()->PlaySound(L"Menu_StartZoom.wav", CSoundMgr::BGM);
		}
		m_bChange = true;

	}


	if (m_bChange)
	{
		m_fCnt += 0.1f * fDeltaTime;

		if (m_fCnt > 0.5f)
			Scene_Change();
	}


	return iEvent;
}

CMenu * CMenu::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (!pDevice)
	{
		PRINT_LOG(L"Error", L"pDevice is nullptr");
		return nullptr;
	}

	CMenu* pInstance = new CMenu(pDevice);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMenu::Free()
{
	CScene::Free();

}

HRESULT CMenu::Ready_Layer_StarBack()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 0.f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale *= (2.f);
	//	tDesc.vScale.x *= (2.5f);

	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_StarBack"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CMenu::Ready_Layer_MenuFont()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}



	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 3.3f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale *= (0.15f);
	tDesc.vScale.x *= (2.2f);
	//	tDesc.vScale.x *= (2.f);


	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_MenuFont"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMenu::Ready_Layer_MenuFont2()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}



	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 3.3f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale *= (0.15f);
	tDesc.vScale.x *= (2.2f);
	//	tDesc.vScale.x *= (2.f);


	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_MenuFont2"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMenu::Ready_Layer_Mouse()
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}



	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 0.f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale *= (0.15f);
	//	tDesc.vScale.x *= (2.f);

	ShowCursor(FALSE);



	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Mouse"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CMenu::Ready_Layer_Rect()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}



	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 0.f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale *= (1.5f);
	tDesc.vScale.x *= (0.5f);
	tDesc.vScale.y *= (1.2f);

	//	pManagement->Set_Index(0);


	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Rect"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMenu::Ready_Layer_RectCol()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}



	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 0.5f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale *= (1.5f);
	//	tDesc.vScale.x *= (2.f);
	tDesc.vScale.x *= (0.5f);
	tDesc.vScale.y *= (1.2f);


	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_RectCol"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;

	tDesc.vPos = _vec3(0.f, -0.5f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };


	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_RectCol"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMenu::Ready_Layer_RectRow()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}



	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 0.f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale *= (1.5f);
	//	tDesc.vScale.x *= (2.f);
	tDesc.vScale.x *= (0.5f);
	tDesc.vScale.y *= (1.2f);


	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_RectRow"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMenu::Ready_Layer_RectScaleFree()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}



	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 0.f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale *= (1.5f);
	//	tDesc.vScale.x *= (2.f);
	tDesc.vScale.x *= (0.5f);
	tDesc.vScale.y *= (1.2f);

	//	pManagement->Set_Index(0);


	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_RectScaleFree"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMenu::Ready_Layer_RectScaleFreeSmall()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}



	TRANSFORM_DESC	tDesc;
	tDesc.vPos = _vec3(0.f, 3.3f, 0.f);
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.vScale = { 1.1f, 0.35f, 0.f };
	tDesc.vScale.x *= (0.4f);



	if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_RectScaleFreeSmall"
		, ePrototypeType::NoneStatic, L"Layer_UI", &tDesc)))
		return E_FAIL;


	return S_OK;
}


HRESULT CMenu::Scene_Change()
{

	CManagement*	pManagement = CManagement::Get_Instance();
	pManagement->Clear_NoneStatic();

	/* 동영상 켜면 제거 */
	//	pManagement->Set_CreateOn();




	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Setup_CurrentScene((_int)eSceneType::Loading, CLoadingScene::Create(m_pDevice, eSceneType::Stage))))
	{
		PRINT_LOG(L"Error", L"Failed To Setup_CurrentScene Logo");
		return UPDATE_ERROR;
	}

	return SCENE_CHANGE;
}
