#include "stdafx.h"
#include "UIFail.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"

CUIFail::CUIFail(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr), m_pTextureCom(nullptr), m_bGameOver(false)
{
}

CUIFail::CUIFail(const CUIFail & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom)
	, m_pTextureCom(other.m_pTextureCom), m_bGameOver(other.m_bGameOver)
{
}

HRESULT CUIFail::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CUIFail::Ready_GameObject(void * pArg)
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


	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_GameOver", L"Com_Cube", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}


	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	m_pTransformCom->Set_Scale({ 0.6f,0.6f,0.6f });
	return S_OK;
}

_uint CUIFail::Update_GameObject(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	POINT Std = { (LONG)(CLIENTWINCX/2.f), (LONG)(CLIENTWINCY/2.f) };

	_vec4x4 matTerrainWorld;
	D3DXMatrixIdentity(&matTerrainWorld);

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObject(ePrototypeType::NoneStatic, L"Layer_BonusCamera"));
	_vec3 Pos = change_view(Std, CLIENTWINCX, CLIENTWINCY, pCamera, &matTerrainWorld);
	m_pTransformCom->Set_Pos(_vec3{ Pos.x, Pos.y, Pos.z});

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

_uint CUIFail::Late_Update_GameObject(_float fDeltaTime)
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

HRESULT CUIFail::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (m_bGameOver)
	{
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

		if (FAILED(m_pTextureCom->Set_Texture(0, 0)))
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

	}
	return S_OK;
}

CUIFail * CUIFail::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUIFail*	pInstance = new CUIFail(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUIFail::Clone(void * pArg)
{
	CUIFail*	pClone = new CUIFail(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CUIFail::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CUIFail::Movement(_float fDeltaTime)
{
	return S_OK;
}

_vec3 CUIFail::change_view(POINT _pt, _uint _Wincx, _uint _Wincy, CCamera* pCamera, _vec4x4* pWorldMatrix)
{
	_vec3 vRaser = _vec3(0.f, 0.f, 0.f);
	vRaser.x = _pt.x / (_Wincx * 0.5f) - 1.f;
	vRaser.y = 1.f - _pt.y / (_Wincy * 0.5f);

	return vRaser;
}