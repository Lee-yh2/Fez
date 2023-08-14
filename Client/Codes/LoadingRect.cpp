#include "stdafx.h"
#include "LoadingRect.h"


CLoadingRect::CLoadingRect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
{
}

CLoadingRect::CLoadingRect(const CLoadingRect & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTextureCom(other.m_pTextureCom), m_pTransformCom(other.m_pTransformCom)
{
}

HRESULT CLoadingRect::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CLoadingRect::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Loading_LoadingRect", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

_uint CLoadingRect::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed to Move LoadingRect");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

_uint CLoadingRect::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::NoneAlpha, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add LoadingRect In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CLoadingRect::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		PRINT_LOG(L"Error", L"Failed to Render LoadingRect_Cullmode");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
	{
		PRINT_LOG(L"Error", L"Failed to Render LoadingRect_Fillmode");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform LoadingRect matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture()))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Texture LoadingRect");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer LoadingRect");
		return E_FAIL;
	}

	return S_OK;
}

CLoadingRect * CLoadingRect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLoadingRect*	pInstance = new CLoadingRect(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error-LoadingRect-Create", L"Failed to Create LoadingRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoadingRect::Clone(void * pArg)
{
	CLoadingRect*	pClone = new CLoadingRect(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error-LoadingRect-Clone", L"Failed to Clone LoadingRect");
		Safe_Release(pClone);
	}

	return pClone;
}

void CLoadingRect::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CLoadingRect::Movement(const _float & fDeltaTime)
{
	m_pTransformCom->RotateZ(-fDeltaTime);

	return S_OK;
}
