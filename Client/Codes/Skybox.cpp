#include "stdafx.h"
#include "Skybox.h"
#include "Camera.h"


CSkybox::CSkybox(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_iStageNum(0)
{
}

CSkybox::CSkybox(const CSkybox & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTextureCom(other.m_pTextureCom), m_pTransformCom(other.m_pTransformCom)
	, m_iStageNum(other.m_iStageNum)
{
}

HRESULT CSkybox::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CSkybox::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_CubeTexture"
		, L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_Skybox"
		, L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform"
		, L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	if (1 == m_pTransformCom->Get_Desc().iNum)
		m_iStageNum = m_pTransformCom->Get_Desc().iNum;
	else
		m_iStageNum = 0;

	return S_OK;
}

_uint CSkybox::Update_GameObject(_float fDeltaTime)
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

_uint CSkybox::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::Priority, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Skybox In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CSkybox::Render_GameObject()
{
	CGameObject::Render_GameObject();

	//m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE)	z-read
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//z-write

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed To SetTransform Skybox");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, m_iStageNum)))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Texture Skybox");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Skybox");
		return E_FAIL;
	}

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CSkybox * CSkybox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkybox*	pInstance = new CSkybox(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CSkybox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkybox::Clone(void * pArg)
{
	CSkybox*	pClone = new CSkybox(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CSkybox");
		Safe_Release(pClone);
	}

	return pClone;
}

void CSkybox::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CSkybox::Movement(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObject(ePrototypeType::NoneStatic, L"Layer_Camera"));
	if (!pCamera)
	{
		PRINT_LOG(L"Error", L"pCamera is nullptr");
		return E_FAIL;
	}

	m_pTransformCom->Set_Pos(pCamera->Get_Desc().vEye);

	return S_OK;
}
