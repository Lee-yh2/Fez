#include "stdafx.h"
#include "StarBack.h"
#include "Camera.h"


CStarBack::CStarBack(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
{
}

CStarBack::CStarBack(const CStarBack & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTextureCom(other.m_pTextureCom), m_pTransformCom(other.m_pTransformCom)
{
}

HRESULT CStarBack::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CStarBack::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);


	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_StarBack", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

_uint CStarBack::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed To Movement Skybox");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}




	return NO_EVENT;
}

_uint CStarBack::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::UI, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Skybox In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CStarBack::Render_GameObject()
{
	CGameObject::Render_GameObject();


	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed To SetTransform Skybox");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture()))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Texture Skybox");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Skybox");
		return E_FAIL;
	}


	return S_OK;
}

CStarBack * CStarBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStarBack*	pInstance = new CStarBack(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStarBack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStarBack::Clone(void * pArg)
{
	CStarBack*	pClone = new CStarBack(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CStarBack");
		Safe_Release(pClone);
	}

	return pClone;
}

void CStarBack::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CStarBack::Movement(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}



	if (GetAsyncKeyState(VK_RETURN))
{		m_bSel = true;	}

	/* 별 이동 */
	if (m_bSel)
	{
		if (m_fAccel < 0.25)
			m_fAccel += 0.00005f;
		else
			m_bSel = false;
		m_pTransformCom->SizeUp(fDeltaTime, m_fAccel);

	}
	else if (m_fAccel > 0.35) /* 제거*/
	{
		
		m_pTransformCom->Set_Pos({9999.f, 9999.f, 9999.f});
	}
	else /* 착시효과 */
	{
		m_pTransformCom->SizeUp(fDeltaTime, m_fAccel);
	}



	


	return S_OK;
}


