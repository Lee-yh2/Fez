#include "stdafx.h"
#include "Grass.h"
#include "Camera.h"

CGrass::CGrass(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTerrainVIBufferCom(nullptr)
	, m_pTextureCom(nullptr), m_pTransformCom(nullptr)
{
}

CGrass::CGrass(const CGrass & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTerrainVIBufferCom(other.m_pTerrainVIBufferCom)
	, m_pTextureCom(other.m_pTextureCom), m_pTransformCom(other.m_pTransformCom)
{
}

HRESULT CGrass::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CGrass::Ready_GameObject(void * pArg)
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

	m_pTerrainVIBufferCom = static_cast<CVIBuffer*>(pManagement->Get_Component(ePrototypeType::NoneStatic, L"Layer_Terrain", L"Com_VIBuffer"));
	if (!m_pTerrainVIBufferCom)
	{
		PRINT_LOG(L"Error", L"m_pTerrainVIBufferCom is nullptr");
		return E_FAIL;
	}

	Safe_AddRef(m_pTerrainVIBufferCom);

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_Grass", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

_uint CGrass::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed To Movement Grass");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}

	if (FAILED(BillBoard()))
	{
		PRINT_LOG(L"Error", L"Failed to BillBoard Grass");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

_uint CGrass::Late_Update_GameObject(_float fDeltaTime)
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
		PRINT_LOG(L"Error", L"Failed To Add Grass In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CGrass::Render_GameObject()
{
	CGameObject::Render_GameObject();

	/*CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}*/

	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		PRINT_LOG(L"Error", L"Failed to Render Grass_Cullmode");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
	{
		PRINT_LOG(L"Error", L"Failed to Render Grass_Fillmode");
		return E_FAIL;
	}

	/*CCamera* pCamera = static_cast<CCamera*>(pManagement->Get_GameObject(eProtoTypeType::NoneStatic, L"Layer_Camera"));
	if (!pCamera)
	{
		PRINT_LOG(L"Error", L"pCamera is nullptr");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Transform Grass");
		return E_FAIL;
	}*/

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Grass matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture()))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Texture Grass");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Grass");
		return E_FAIL;
	}

	return S_OK;
}

CGrass * CGrass::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGrass*	pInstance = new CGrass(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create CGrass");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGrass::Clone(void * pArg)
{
	CGrass*	pClone = new CGrass(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone CGrass");
		Safe_Release(pClone);
	}

	return pClone;
}

void CGrass::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTerrainVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CGrass::Movement(_float fDeltaTime)
{
	_float	fY = 0.f;
	if (static_cast<CVIBuffer_TerrainTexture*>(m_pTerrainVIBufferCom)->Check_Terrain(&fY, &m_pTransformCom->Get_Desc().vPos))
		m_pTransformCom->Set_PosY(fY + 0.5f);

	return S_OK;
}

HRESULT CGrass::BillBoard()
{
	/*_vec4x4	matView;
	if (FAILED(m_pDevice->GetTransform(D3DTS_VIEW, &matView)))
	{
		PRINT_LOG(L"Error", L"Failed to GetTransform D3DTS_VIEW");
		return E_FAIL;
	}*/

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(ePrototypeType::NoneStatic, L"Layer_Camera");
	if (!pCamera)
	{
		PRINT_LOG(L"Error", L"pCamera is nullptr");
		return E_FAIL;
	}

	_vec4x4	matView = pCamera->Get_Desc().matView;
	_vec4x4	matBillY;
	D3DXMatrixIdentity(&matBillY);

	/*Scale*/
	matBillY._11 = matView._11;
	matBillY._22 = matView._22;
	matBillY._33 = matView._33;
	matBillY._44 = matView._44;

	/*RotationY*/
	matBillY._11 = matView._11;
	matBillY._13 = matView._13;
	matBillY._31 = matView._31;
	matBillY._33 = matView._33;

	D3DXMatrixInverse(&matBillY, 0, &matBillY);
	_vec4x4	matWorld = m_pTransformCom->Get_Desc().matWorld;

	m_pTransformCom->Set_WorldMatrix(matBillY * matWorld);

	return S_OK;
}
