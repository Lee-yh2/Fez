#include "stdafx.h"
#include "RectCol.h"
#include "Camera.h"

CRectCol::CRectCol(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CRectCol::CRectCol(const CRectCol & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTransformCom(other.m_pTransformCom), m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CRectCol::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();
	return S_OK;
}

HRESULT CRectCol::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_RectCol", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

_uint CRectCol::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);
	m_fY = m_pTransformCom->Get_Desc().vPos.y;

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

_uint CRectCol::Late_Update_GameObject(_float fDeltaTime)
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
		PRINT_LOG(L"Error", L"Failed To Add Mouse In Renderer");
		return UPDATE_ERROR;
	}



	return S_OK;
}

HRESULT CRectCol::Render_GameObject()
{
	CGameObject::Render_GameObject();


	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed To SetTransform Mouse");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture()))
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

CRectCol * CRectCol::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRectCol*	pInstance = new CRectCol(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Mouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRectCol::Clone(void * pArg)
{
	CRectCol*	pClone = new CRectCol(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Mouse");
		Safe_Release(pClone);
	}

	return pClone;
}

void CRectCol::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CRectCol::Movement(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	/* 첫 생성 */
	if (m_fY >= 0.5) {
		m_pTransformCom->Set_PosY(m_fY + (m_fY * fDeltaTime));
	}
	else if (m_fY <= -0.5) {
		m_pTransformCom->Set_PosY(m_fY + (m_fY * fDeltaTime));
	}



	/* 종료시 */
	if (GetAsyncKeyState(VK_RETURN)) {
		m_bCnt = true;
	}

	if (m_bCnt){
		m_fCnt += fDeltaTime;
	}

	if ((m_fCnt > 0.6f)) {


		if (m_fY >= 0.5) {
			m_pTransformCom->Set_PosY(1.5f);
		}
		else if (m_fY <= -0.5) {
			m_pTransformCom->Set_PosY(-1.5f);
		}
		m_bCnt = false;
		m_bClose = true;

		m_fCnt = 0.f;
		m_fY = m_pTransformCom->Get_Desc().vPos.y;
	}

	if (m_bClose) {
		if (m_fY >= 0.1){
			m_pTransformCom->Set_PosY(m_fY - ((m_fY) * fDeltaTime));
		}
		if (m_fY <= -0.1){
			m_pTransformCom->Set_PosY(m_fY - ((m_fY) * fDeltaTime));
		}
	}


	return S_OK;
}


