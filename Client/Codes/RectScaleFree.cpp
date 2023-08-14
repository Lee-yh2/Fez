#include "stdafx.h"
#include "RectScaleFree.h"
#include "Camera.h"


#include "SoundMgr.h"

CRectScaleFree::CRectScaleFree(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CRectScaleFree::CRectScaleFree(const CRectScaleFree & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTransformCom(other.m_pTransformCom), m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CRectScaleFree::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();
	return S_OK;
}

HRESULT CRectScaleFree::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_RectScaleFree", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

_uint CRectScaleFree::Update_GameObject(_float fDeltaTime)
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

_uint CRectScaleFree::Late_Update_GameObject(_float fDeltaTime)
{



	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

//	m_iIndex = pManagement->Get_Index();

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::UI, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Mouse In Renderer");
		return UPDATE_ERROR;
	}

	return S_OK;
}

HRESULT CRectScaleFree::Render_GameObject()
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

CRectScaleFree * CRectScaleFree::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRectScaleFree*	pInstance = new CRectScaleFree(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Mouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRectScaleFree::Clone(void * pArg)
{
	CRectScaleFree*	pClone = new CRectScaleFree(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Mouse");
		Safe_Release(pClone);
	}

	return pClone;
}

void CRectScaleFree::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CRectScaleFree::Movement(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}


	TRANSFORM_DESC temp = m_pTransformCom->Get_Desc();



	/* 대기 */
	if (m_bCnt)
	{

		/* 카운트 */
		m_fCnt += (fDeltaTime * 2);

		if (m_fCnt > 6.f){
			m_bCnt = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
			CSoundMgr::Get_Instance()->PlaySound(L"Menu_RectBorder.wav", CSoundMgr::UI);
		}
	}
	
	/* 크기 변형 */
	else if (!m_bScaling)
	{
		m_fAccel += fDeltaTime;



		float fScaleX = fDeltaTime * m_fAccel * 2.f;
		float fScaleY = fDeltaTime * m_fAccel * 8.f;

		if (temp.vScale.x > 0.5f)
			temp.vScale.x -= fScaleX;

		if (temp.vScale.y > 0.35f)
			temp.vScale.y -= fScaleY;



		if (temp.vPos.y < 0.3f && temp.vScale.y < 0.7f)
			temp.vPos.y += (fDeltaTime);
		else if (temp.vPos.y > 0.3f)
		{
			m_bScaling = true;
		}
	}
	else
	{
		temp.vPos.y = 5.f;
	}


	
	m_pTransformCom->Set_Desc(temp);

	return S_OK;
}


