#include "stdafx.h"
#include "..\Headers\Vine.h"


CVine::CVine(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_fTextureNum(0.f), m_eVineDir(eMapDir::NOTHING)
{
}

CVine::CVine(const CVine & other)
	: CGameObject(other.m_pDevice), m_pVIBufferCom(other.m_pVIBufferCom), m_pTextureCom(other.m_pTextureCom), m_pTransformCom(other.m_pTransformCom)
	, m_fTextureNum(other.m_fTextureNum), m_eVineDir(other.m_eVineDir)

{
}

HRESULT CVine::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CVine::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectVineTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_Vine", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	m_fTextureNum = (_float)(rand() % 4);

	return S_OK;
}

_uint CVine::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (!m_bIsTurning)
	{
		if (FAILED(m_pTransformCom->Update_Transform()))
		{
			PRINT_LOG(L"Error", L"Failed To Update_Transform");
			return UPDATE_ERROR;
		}
	}

	return NO_EVENT;
}

_uint CVine::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::Alpha, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Monster In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CVine::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		PRINT_LOG(L"Error", L"Failed to Render Monster_Cullmode");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
	{
		PRINT_LOG(L"Error", L"Failed to Render Monster_Fillmode");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Monster matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, (_uint)m_fTextureNum)))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Texture Monster");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Monster");
		return E_FAIL;
	}

	return S_OK;
}

CVine * CVine::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVine*	pInstance = new CVine(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CVine::Clone(void * pArg)
{
	CVine*	pClone = new CVine(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVine");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVine::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
