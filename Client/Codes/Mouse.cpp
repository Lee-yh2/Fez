#include "stdafx.h"
#include "Mouse.h"
#include "Camera.h"

#include "SoundMgr.h"

CMouse::CMouse(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CMouse::CMouse(const CMouse & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTransformCom(other.m_pTransformCom), m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CMouse::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();
	return S_OK;
}

HRESULT CMouse::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Mouse", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	return S_OK;
}

_uint CMouse::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed To Movement Mouse");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}
	return S_OK;
}

_uint CMouse::Late_Update_GameObject(_float fDeltaTime)
{
	//if (GetAsyncKeyState(VK_LBUTTON))
	//{ m_iCursor = 1; }
	//else
	//{ m_iCursor = 0; }

	(GetAsyncKeyState(VK_LBUTTON)) ? m_iCursor = 1 : m_iCursor = 0;


	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::UI, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Mouse In Renderer");
		return UPDATE_ERROR;
	}

	return S_OK;
}

HRESULT CMouse::Render_GameObject()
{
	CGameObject::Render_GameObject();


	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed To SetTransform Mouse");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, m_iCursor)))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Texture Mouse");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Mouse");
		return E_FAIL;
	}


	return S_OK;
}

CMouse * CMouse::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMouse*	pInstance = new CMouse(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Mouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMouse::Clone(void * pArg)
{
	CMouse*	pClone = new CMouse(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Mouse");
		Safe_Release(pClone);
	}

	return pClone;
}

void CMouse::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CMouse::Movement(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	if (GetAsyncKeyState(VK_RETURN))
	{
		m_bClose = true;
	}
	else if (m_bClose)
	{
		m_pTransformCom->SizeDown(fDeltaTime, 10.f);
	}



	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
//	ShowCursor(FALSE);

	_vec3 vMousePos = { (2 * (float)pt.x / WINCX) - 1, (-2 * (float)pt.y / WINCY) + 1, 0.f};
	m_pTransformCom->Set_Pos(vMousePos);
	
	


	return S_OK;
}


