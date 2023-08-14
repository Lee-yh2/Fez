
#include "stdafx.h"
#include "Logo.h"
#include "LoadingScene.h"



CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice), m_dwKey(0)
{





}

HRESULT CLogo::Ready_Scene()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
		PRINT_LOG(L"Error", L"pManagement is nullptr");

	/* 동영상 */
	pManagement->Set_CreateOff();

	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;

	::SetWindowText(g_hWnd, L"Logo Scene");

	return S_OK;
}

_uint CLogo::Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Update_Scene(fDeltaTime))
		return iEvent;



	if (m_bPlay)
	{
		IGraphBuilder *pGraph = NULL;
		IMediaControl *pMediaControl = NULL;
		IMediaEvent *pEvent = NULL;
		HWND hWnd = g_hWnd;  // 뷰에 대한 윈도우 핸들
		LPCWSTR fileName = L"../Resources/Start.avi";

		CoInitialize(NULL);
		CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
			IID_IGraphBuilder, (void**)&pGraph);
		pGraph->QueryInterface(IID_IMediaControl, (void**)&pMediaControl);
		pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin);

		pGraph->RenderFile(fileName, NULL);


		// 비디오 윈도우 설정

		pVidWin->put_Owner((OAHWND)hWnd);
		pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);


		RECT grc;

		GetClientRect(hWnd, &grc);

		pVidWin->SetWindowPosition(0, 0, grc.right, grc.bottom);

		// 그래프 실행

		pMediaControl->Run();

		CoUninitialize();


		m_bPlay = false;
	}

	if (GetAsyncKeyState(VK_RETURN))
	{
		pVidWin->SetWindowPosition(0, 0, 0, 0);
	}

	return iEvent;
}

_uint CLogo::Late_Update_Scene(_float & fDeltaTime)
{
	_uint	iEvent = NO_EVENT;

	if (iEvent = CScene::Late_Update_Scene(fDeltaTime))
		return iEvent;

	if (::GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (0 == m_dwKey)
		{
			CManagement*	pManagement = CManagement::Get_Instance();

			/* 동영상 */
			pManagement->Set_CreateOn();

			if (!pManagement)
			{
				PRINT_LOG(L"Error", L"pManagement is nullptr");
				return UPDATE_ERROR;
			}

			if (FAILED(pManagement->Setup_CurrentScene((_int)eSceneType::Loading, CLoadingScene::Create(m_pDevice, eSceneType::Menu))))
			{
				PRINT_LOG(L"Error", L"Failed To Setup_CurrentScene Logo");
				return UPDATE_ERROR;
			}

			m_dwKey |= KEY_RETURN_CLIENT;
			return SCENE_CHANGE;
		}
	}
	else
	{
		m_dwKey = 0;

	}
	return iEvent;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (!pDevice)
	{
		PRINT_LOG(L"Error", L"pDevice is Error");
		return nullptr;
	}

	CLogo*	pInstance = new CLogo(pDevice);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogo::Free()
{
	CScene::Free();
}
