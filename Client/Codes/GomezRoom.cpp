#include "stdafx.h"
#include "GomezRoom.h"
#include "Camera.h"


CGomezRoom::CGomezRoom(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
{
}

CGomezRoom::CGomezRoom(const CGomezRoom & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTextureCom(other.m_pTextureCom), m_pTransformCom(other.m_pTransformCom)
{
}

HRESULT CGomezRoom::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CGomezRoom::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_CubeTexture"
		, L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"GomezRoom", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_GomezHouse"
		, L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"GomezRoom", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform"
		, L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"GomezRoom", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	return S_OK;
}

_uint CGomezRoom::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"GomezRoom", L"Failed To Movement GomezRoom");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"GomezRoom", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

_uint CGomezRoom::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"GomezRoom", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::Priority, this)))
	{
		PRINT_LOG(L"GomezRoom", L"Failed To Add Skybox In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CGomezRoom::Render_GameObject()
{
	CGameObject::Render_GameObject();

	//m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE)	z-read
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//z-write

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"GomezRoom", L"Failed To SetTransform Skybox");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture()))
	{
		PRINT_LOG(L"GomezRoom", L"Failed To Set_Texture GomezRoom");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"GomezRoom", L"Failed To Render_VIBuffer GomezRoom");
		return E_FAIL;
	}

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CGomezRoom * CGomezRoom::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGomezRoom*	pInstance = new CGomezRoom(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"GomezRoom", L"Failed To Create CGomezRoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGomezRoom::Clone(void * pArg)
{
	CGomezRoom*	pClone = new CGomezRoom(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"GomezRoom", L"Failed To Clone CGomezRoom");
		Safe_Release(pClone);
	}

	return pClone;
}

void CGomezRoom::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CGomezRoom::Movement(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"GomezRoom", L"pManagement is nullptr");
		return E_FAIL;
	}

	CCamera*	pCamera = static_cast<CCamera*>(pManagement->Get_GameObject(ePrototypeType::NoneStatic, L"Layer_Camera"));
	if (!pCamera)
	{
		PRINT_LOG(L"GomezRoom", L"pCamera is nullptr");
		return E_FAIL;
	}


	if ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('D') & 0x8000))
	{
		m_bRotate = true;
	}

	if (m_bRotate)
	{
		(m_bRectangle) ? m_bRectangle = false : m_bRectangle = true;

		m_bRotate = false;
	}

	if (m_bRectangle)
	{
		//float fX =  m_pTransformCom->Get_Desc().vPos.x;
		//TRANSFORM_DESC	tDesc;

		//if (fX <= 9.1f)
		//	tDesc.vPos = _vec3(9.f, 9.f, 11.5f);
		//else
		//	tDesc.vPos = _vec3(9.f, 9.f, 11.5f);


		/* 수정해봅시당 */

		//m_pTransformCom->Set_Desc(tDesc);


		m_bRectangle = false;
	}
	
//	m_pTransformCom->Set_Pos(pCamera->Get_Desc().vEye);

	return S_OK;
}
