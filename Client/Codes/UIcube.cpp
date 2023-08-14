#include "stdafx.h"
#include "UIcube.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"

CUIcube::CUIcube(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CUIcube::CUIcube(const CUIcube & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom)
	, m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CUIcube::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CUIcube::Ready_GameObject(void * pArg)
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


	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_UIcube", L"Com_Cube", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}
	m_pTransformCom->Set_Scale({ 0.15f,0.1f,0.2f });
	return S_OK;
}

_uint CUIcube::Update_GameObject(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	POINT Std = { (LONG)30.f, (LONG)30.f };

	_vec4x4 matTerrainWorld;
	D3DXMatrixIdentity(&matTerrainWorld);

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObject(ePrototypeType::NoneStatic, L"Layer_Camera"));
	_vec3 Pos = change_view(Std, WINCX, WINCY, pCamera, &matTerrainWorld);
	m_pTransformCom->Set_Pos(Pos);

	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed to Movement Player");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

_uint CUIcube::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement*	pManagement = CManagement::Get_Instance();

	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}


	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::Alpha, this)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Player in Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CUIcube::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Player matWorld");
		return E_FAIL;
	}

	_vec4x4 matView, matProj, matIdentity;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixIdentity(&matIdentity);
	m_pDevice->SetTransform(D3DTS_VIEW, &matIdentity);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matIdentity);

	if (m_StackDeltatime > 5.f)
		m_CubeNumber = 1;

	if (FAILED(m_pTextureCom->Set_Texture(0, m_CubeNumber)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture Player");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed to Render_VIBuffer Player");
		return E_FAIL;
	}

	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	return S_OK;
}

CUIcube * CUIcube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUIcube*	pInstance = new CUIcube(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUIcube::Clone(void * pArg)
{
	CUIcube*	pClone = new CUIcube(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CUIcube::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CUIcube::Movement(_float fDeltaTime)
{
	return S_OK;
}

_vec3 CUIcube::change_view(POINT _pt, _uint _Wincx, _uint _Wincy, CCamera* pCamera, _vec4x4* pWorldMatrix)
{
	_vec3 vRaser = _vec3(0.f, 0.f, 0.f);
	vRaser.x = _pt.x / (_Wincx * 0.5f) - 1.f;
	vRaser.y = 1.f - _pt.y / (_Wincy * 0.5f);
	return vRaser;
}