#include "stdafx.h"
#include "VirtualCube.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"

CVirtualCube::CVirtualCube(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CVirtualCube::CVirtualCube(const CVirtualCube & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom)
	, m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CVirtualCube::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CVirtualCube::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_CubeTexture"
		, L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_VirtualCube"
		, L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	m_pTransformCom->Set_Scale(_vec3(1.4f, 1.4f, 1.4f));
	m_pTransformCom->RotateX(23.f);
	return S_OK;
}

_uint CVirtualCube::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (GetAsyncKeyState('K') & 0x8000)
		m_bIsFullCube = true;

	if (!m_bIsFullCube)
	{
		if (FAILED(Movement(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed to Movement Player");
			return UPDATE_ERROR;
		}
	}
	else
	{
		if (FAILED(Movement2(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed to Movement Player");
			return UPDATE_ERROR;
		}
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}
	return NO_EVENT;
}

_uint CVirtualCube::Late_Update_GameObject(_float fDeltaTime)
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
	if (!m_bIsFullCube)
	{
		if (m_ShowCube)
			m_StackDeltatime += fDeltaTime;
		if (m_StackDeltatime > 6.5f)
		{
			m_StackDeltatime = 0.f;
			m_ShowCube = false;
		}
	}
	return NO_EVENT;
}

HRESULT CVirtualCube::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Player matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, 0)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture VirtualCube");
		return E_FAIL;
	}
	if (m_ShowCube)
	{
		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		{
			PRINT_LOG(L"Error", L"Failed to Render_VIBuffer Player");
			return E_FAIL;
		}
	}
	return S_OK;
}

CVirtualCube * CVirtualCube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVirtualCube*	pInstance = new CVirtualCube(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CVirtualCube::Clone(void * pArg)
{
	CVirtualCube*	pClone = new CVirtualCube(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVirtualCube::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CVirtualCube::Movement(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	m_CubeTurnAngle += fDeltaTime * 0.4f;
	if (m_CubeTurnAngle > 360.f)
		m_CubeTurnAngle = 0.f;

	_float a, b, c;
	a = pPlayer->Return_Transform()->Get_Pos().x;
	b = pPlayer->Return_Transform()->Get_Pos().y + 1.75f;
	c = pPlayer->Return_Transform()->Get_Pos().z;
	m_pTransformCom->Set_Pos({ a, b, c });
	m_pTransformCom->Set_RotY(m_CubeTurnAngle);

	return S_OK;
}

HRESULT CVirtualCube::Movement2(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	m_StackDeltatime += fDeltaTime;
	m_CubeTurnAngle += fDeltaTime * 1.f;
	if (m_CubeTurnAngle > 360.f)
		m_CubeTurnAngle = 0.f;

	_float a, b, c;
	a = pPlayer->Return_Transform()->Get_Pos().x;
	b = pPlayer->Return_Transform()->Get_Pos().y + 1.75f;
	c = pPlayer->Return_Transform()->Get_Pos().z;
	m_pTransformCom->Set_Pos({ a, b, c });
	m_pTransformCom->Set_RotY(-m_CubeTurnAngle);
	m_ShowCube = true;
	if (m_StackDeltatime > 15.f)
	{
		m_bIsFullCube = false;
		m_ShowCube = true;
	}
	return S_OK;
}