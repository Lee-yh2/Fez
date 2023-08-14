#include "stdafx.h"
#include "Font.h"
#include "Camera.h"


#include "SoundMgr.h"

CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{


	
}

CFont::CFont(const CFont & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTransformCom(other.m_pTransformCom), m_pTextureCom(other.m_pTextureCom)
{

	
}

HRESULT CFont::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CFont::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);


	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_MenuFont", L"Com_Texture", (CComponent**)&m_pTextureCom)))
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

_uint CFont::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed To Movement Font");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}




	return NO_EVENT;
}

_uint CFont::Late_Update_GameObject(_float fDeltaTime)
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

HRESULT CFont::Render_GameObject()
{
	CGameObject::Render_GameObject();


		if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
		{
			PRINT_LOG(L"Error", L"Failed To SetTransform Skybox");
			return E_FAIL;
		}

		if (FAILED(m_pTextureCom->Set_Texture()))
		{
			PRINT_LOG(L"Error", L"Failed To Set_Texture Font");
			return E_FAIL;
		}

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		{
			PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Font");
			return E_FAIL;
		}


	return S_OK;
}

CFont * CFont::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFont*	pInstance = new CFont(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Font");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFont::Clone(void * pArg)
{
	CFont*	pClone = new CFont(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Font");
		Safe_Release(pClone);
	}

	return pClone;
}

void CFont::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CFont::Movement(_float fDeltaTime)
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



		if (m_fCnt > 6.f) {
			m_bCnt = false;
			temp.vPos.y = 0.3f;
		}


	}
	m_pTransformCom->Set_Desc(temp);

	if (GetAsyncKeyState(VK_RETURN))
	{
		m_bClose = true;
		CSoundMgr::Get_Instance()->PlaySound(L"Menu_Start.wav", CSoundMgr::UI);
	}
	else if (m_bClose)
	{
		m_pTransformCom->SizeDown(fDeltaTime, 10.f);
	}



//	m_pTransformCom->SizeUp(fDeltaTime, m_fAccel);
	


	return S_OK;
}


