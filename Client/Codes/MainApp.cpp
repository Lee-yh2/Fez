#include "stdafx.h"
#include "MainApp.h"
#include "Logo.h"
#include "Player.h"
#include "StarBack.h"
#include "LoadingScene.h"
#include "MainCamera.h"
#include "LoadingBackground.h"
#include "LoadingBar.h"
#include "LoadingRect.h"


/* 사운드 */
#include "SoundMgr.h"

CMainApp::CMainApp()
	: m_pManagement(CManagement::Get_Instance()), m_pDevice(nullptr)
{
}

HRESULT CMainApp::Ready_MainApp()
{
	if (!m_pManagement)
	{
		PRINT_LOG(L"Error", L"m_pManagement is nullptr");
		return E_FAIL;
	}

	if (FAILED(m_pManagement->Ready_Game(g_hWnd, CLIENTWINCX, CLIENTWINCY, eDisPlay::Win)))
	{
		PRINT_LOG(L"Error", L"Failed to Ready Game");
		return E_FAIL;
	}

	m_pDevice = m_pManagement->Get_Device();
	if (!m_pDevice)
	{
		PRINT_LOG(L"Error", L"m_pDevice is nullptr");
		return E_FAIL;
	}

	Safe_AddRef(m_pDevice);


	if (FAILED(Ready_StaticResources()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_StaticResources");
		return E_FAIL;
	}

	if (FAILED(Ready_DefaultSetting()))
	{
		PRINT_LOG(L"Error", L"Failed to Ready_DefaultSetting");
		return E_FAIL;
	}
	CSoundMgr::Get_Instance()->Initialize();

	if (FAILED(m_pManagement->Setup_CurrentScene((_int)eSceneType::Loading, CLoadingScene::Create(m_pDevice, eSceneType::Logo))))
	{
		PRINT_LOG(L"Error", L"Failed to Setup_CurrentScene Logo");
		return E_FAIL;
	}


	return S_OK;
}

_uint CMainApp::Update_MainApp()
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = m_pManagement->Update_Game())
		return iEvent;

	/* 동영상 체크 */
	m_bCreate = m_pManagement->Get_CreateBackBuffer();

	/* 동영상 */
	if (m_bCreate)
	{
		if (FAILED(m_pManagement->Render_Game()))
			return 0;
	}
	return iEvent;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp;
	if (FAILED(pInstance->Ready_MainApp()))
	{
		PRINT_LOG(L"Error", L"Failed to Create MainApp");
		delete	pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDevice);
	CSoundMgr::Destroy_Instance();

	if (CManagement::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed to Destroy_Instance CManagement");
}

HRESULT CMainApp::Ready_StaticResources()
{
#pragma	region	GameObjects
	/*MainCamera*/
	if (FAILED(m_pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_MainCamera", CMainCamera::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add MainCamera Prototype");
		return E_FAIL;
	}
	/*LoadingBackground*/
	if (FAILED(m_pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_LoadingBackground", CLoadingBackground::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add LoadingBackground Prototype");
		return E_FAIL;
	}

	/*LoadingBar*/
	if (FAILED(m_pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_LoadingBar", CLoadingBar::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add LoadingBar Prototype");
		return E_FAIL;
	}

	/*LoadingRect*/
	if (FAILED(m_pManagement->Add_GameObject_Prototype(ePrototypeType::Static
		, L"GameObject_LoadingRect", CLoadingRect::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add LoadingRect Prototype");
		return E_FAIL;
	}


#pragma	endregion

#pragma region	Components
	/*VIBuffer_TriColor*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_VIBuffer_TriColor", CVIBuffer_TriColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VIBuffer_TriColor Prototype");
		return E_FAIL;
	}

	/*VIBuffer_RectColor*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_VIBuffer_RectColor", CVIBuffer_RectColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VIBuffer_RectColor Prototype");
		return E_FAIL;
	}

	/*VIBuffer_RectTexture*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_VIBuffer_RectTexture", CVIBuffer_RectTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VIBuffer_RectTexture Prototype");
		return E_FAIL;
	}

	/*VIBuffer_RectVineTexture*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_VIBuffer_RectVineTexture", CVIBuffer_RectVineTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VIBuffer_RectVineTexture Prototype");
		return E_FAIL;
	}

	/*VIBuffer_CubeTexture*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_VIBuffer_CubeTexture", CVIBuffer_CubeTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add VIBuffer_CubeTexture Prototype");
		return E_FAIL;
	}

	/*VIBuffer_Transform*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static
		, L"Component_Transform", CTransform::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed to Add Component_Transform Prototype");
		return E_FAIL;
	}

	/*Texture_LoadingBackground*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static, L"Component_Texture_Loading_LoadingBackground"
		, CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Loading/LoadingBackground/LoadingBackground%d.png", 1))))
	{
		PRINT_LOG(L"Error-MFCToolView-Ready_StaticResources", L"Failed to Add Component_Texture_Loading_LoadingBackground Prototype");
		return E_FAIL;
	}

	/*Texture_LoadingBar*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static, L"Component_Texture_Loading_LoadingBar"
		, CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Loading/LoadingBar/LoadingBar%d.png", 1))))
	{
		PRINT_LOG(L"Error-MFCToolView-Ready_StaticResources", L"Failed to Add Component_Texture_Loading_LoadingBar Prototype");
		return E_FAIL;
	}

	/*Texture_LoadingRect*/
	if (FAILED(m_pManagement->Add_Component_Prototype(ePrototypeType::Static, L"Component_Texture_Loading_LoadingRect"
		, CTexture::Create(m_pDevice, eTextureType::Normal, L"../Resources/Loading/LoadingRect/LoadingRect%d.png", 1))))
	{
		PRINT_LOG(L"Error-MFCToolView-Ready_StaticResources", L"Failed to Add Component_Texture_Loading_LoadingRect Prototype");
		return E_FAIL;
	}
#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Ready_DefaultSetting()
{
	if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
		return E_FAIL;

	return S_OK;
}
