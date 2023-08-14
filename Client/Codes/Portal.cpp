#include "stdafx.h"
#include "Player.h"
#include "..\Headers\Portal.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_bOpen(false), m_bLightOn(false)
{
}

CPortal::CPortal(const CPortal & other)
	: CGameObject(other.m_pDevice), m_pVIBufferCom(other.m_pVIBufferCom), m_pTextureCom(other.m_pTextureCom), m_pTransformCom(other.m_pTransformCom)
	,m_bOpen(other.m_bOpen), m_bLightOn(other.m_bLightOn)
{
}

HRESULT CPortal::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CPortal::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_Portal", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}
	if ((_uint)eSceneType::Stage2 == m_pTransformCom->Get_Desc().iNum)
	{
		m_tFrame.fFrameStart = 0.f;
	}
	else if ((_uint)eSceneType::GomezHouse == m_pTransformCom->Get_Desc().iNum || (_uint)eSceneType::Stage == m_pTransformCom->Get_Desc().iNum)
	{
		m_tFrame.fFrameStart = 3.f;
	}
	m_tFrame.fFrameEnd = 2.f;
	return S_OK;
}

_uint CPortal::Update_GameObject(_float fDeltaTime)
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
	if ((_uint)eSceneType::Stage2 == m_pTransformCom->Get_Desc().iNum)
	{
		if (m_bOpen)
			FrameMove(fDeltaTime, 1.f);

		if (m_bLightOn)
		{
			/*if(m_pTransformCom->Get_Desc().vPos.y < 17.f)
			m_pTransformCom->Set_PosYPlus(0.07f);*/
		}
	}

	return NO_EVENT;
}

_uint CPortal::Late_Update_GameObject(_float fDeltaTime)
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

HRESULT CPortal::Render_GameObject()
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

	if (FAILED(m_pTextureCom->Set_Texture(0, (_uint)m_tFrame.fFrameStart)))
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

CPortal * CPortal::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPortal*	pInstance = new CPortal(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CPortal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPortal::Clone(void * pArg)
{
	CPortal*	pClone = new CPortal(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CPortal");
		Safe_Release(pClone);
	}

	return pClone;
}

void CPortal::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

void CPortal::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
		m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed;
		if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
		{
			m_bLightOn = true;
			m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
		}
}
