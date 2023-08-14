#include "stdafx.h"
#include "RectRow.h"
#include "Camera.h"

#include "SoundMgr.h"

CRectRow::CRectRow(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CRectRow::CRectRow(const CRectRow & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTransformCom(other.m_pTransformCom), m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CRectRow::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();
	return S_OK;
}

HRESULT CRectRow::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_RectRow", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

_uint CRectRow::Update_GameObject(_float fDeltaTime)
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

_uint CRectRow::Late_Update_GameObject(_float fDeltaTime)
{



	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"RectRow", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}


	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::UI, this)))
	{
		PRINT_LOG(L"RectRow", L"Failed To Add Mouse In Renderer");
		return UPDATE_ERROR;
	}

	return S_OK;
}

HRESULT CRectRow::Render_GameObject()
{
	CGameObject::Render_GameObject();


	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"RectRow", L"Failed To SetTransform RectRow");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture()))
	{
		PRINT_LOG(L"RectRow", L"Failed To Set_Texture RectRow");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"RectRow", L"Failed To Render_VIBuffer RectRow");
		return E_FAIL;
	}



	return S_OK;
}

CRectRow * CRectRow::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRectRow*	pInstance = new CRectRow(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"RectRow", L"Failed To Create RectRow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRectRow::Clone(void * pArg)
{
	CRectRow*	pClone = new CRectRow(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"RectRow", L"Failed To Clone RectRow");
		Safe_Release(pClone);
	}

	return pClone;
}

void CRectRow::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CRectRow::Movement(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	if (m_bStart)
	{

		m_bStart = false;
	}

	/* 카운트 */
	m_fCnt += fDeltaTime;

	/* 첫 생성 */
	if (!m_bEnd)
	{
		if (m_fCnt >= 1.8f)
		{
			m_pTransformCom->ScaleXDown(fDeltaTime, m_fAccel);
		}

		if (m_pTransformCom->Get_Desc().vScale.x < 0.05)
		{
			m_pTransformCom->Set_PosY(2.f);
			m_bEnd = true;
		}
		else if (m_pTransformCom->Get_Desc().vScale.x < 0.5)
		{
			m_fAccel += 0.15f;
		}
	}

	/* 종료시 */
	if (GetAsyncKeyState(VK_RETURN))
	{
		TRANSFORM_DESC temp = m_pTransformCom->Get_Desc();
		temp.vScale.x = 0.1f;
		temp.vPos.y = 0.f;

		m_pTransformCom->Set_Desc(temp);
		m_fAccel = 4.5f;
		m_bClose = true;
	}

	if (m_bClose) {
		TRANSFORM_DESC temp = m_pTransformCom->Get_Desc();
		temp.vScale.x += (fDeltaTime * m_fAccel);
		m_pTransformCom->Set_Desc(temp);}




	return S_OK;
}


