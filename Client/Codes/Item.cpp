#include "stdafx.h"
#include "Player.h"
#include "Camera.h"
#include "Texture.h"
#include "Item.h"
#include "VirtualCube.h"
CItem::CItem(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CItem::CItem(const CItem & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom), m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CItem::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CItem::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_RealCube"
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

	m_Onecheck = 5;
	m_pTransformCom->Set_Scale(_vec3(0.471f, 0.471f, 0.471f));
	return S_OK;
}

_uint CItem::Update_GameObject(_float fDeltaTime)
{
	if (m_CubeNum > 0 && !m_FourCheck)
	{
		m_EatItem = 1;
		m_FourCheck = true;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	if (GetAsyncKeyState('I'))
	{
	m_pTransformCom->Go_Up(fDeltaTime);
	}
	if (GetAsyncKeyState('J'))
	{
	m_pTransformCom->Go_Sideway(-fDeltaTime);
	}
	if (GetAsyncKeyState('K'))
	{
	m_pTransformCom->Go_Up(-fDeltaTime);
	}
	if (GetAsyncKeyState('L'))
	{
	m_pTransformCom->Go_Sideway(fDeltaTime);
	}
	if (GetAsyncKeyState('U'))
	{
	m_pTransformCom->Go_Straight(-fDeltaTime);
	}
	if (GetAsyncKeyState('O'))
	{
	m_pTransformCom->Go_Straight(fDeltaTime);
	}
	////////////////////////////////////////////////////////////////////////////////////////
	if (m_EatItem > 0)
	{
		m_StackDeltatime += fDeltaTime;
	}
	if (m_EatItem == 0)
	{
		if (FAILED(m_pTransformCom->Update_Transform()))
		{
			PRINT_LOG(L"Error", L"Failed to Update_Transform");
			return UPDATE_ERROR;
		}
	}
	else if (m_EatItem == 1)
	{
		if (FAILED(Movement(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed to Movement");
			return UPDATE_ERROR;
		}
	}
	else if (m_EatItem == 2)
	{
		if (FAILED(Movement2(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed to Movement");
			return UPDATE_ERROR;
		}
	}
	if (m_EatItem == 3)
	{
		if (FAILED(Movement3(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed to Movement");
			return UPDATE_ERROR;
		}
	}

	m_tKeyManager.KeyCheck();

	return NO_EVENT;
}

_uint CItem::Late_Update_GameObject(_float fDeltaTime)
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

	if (m_StackDeltatime > 4.85f)
		m_EatItem = 3;

	if (m_ShowCube)
		m_StackDeltatime2 += fDeltaTime;

	if (m_StackDeltatime2 > 6.5f)
	{
		m_ShowCube = false;
		m_StackDeltatime2 = 0.f;
	}
	return NO_EVENT;
}

HRESULT CItem::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Player matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, 0)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture ITem");
		return E_FAIL;
	}

	if (m_EatItem == 0 || m_ShowCube)
	{
		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		{
			PRINT_LOG(L"Error", L"Failed to Render_VIBuffer ITem");
			return E_FAIL;
		}
	}
	return S_OK;
}

CItem * CItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem*	pInstance = new CItem(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone(void * pArg)
{
	CItem*	pClone = new CItem(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CItem::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CItem::Movement(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

	m_pTransformCom->Set_Pos(_vec3(m_Threecheck, 0.f, -8.f));
	m_Threecheck += fDeltaTime * 5.f;
	_vec4x4 matParents;
	_vec4x4	matRotX, matRotY, matRotZ, matTrans;
	_float target_PosX = pPlayer->Return_Transform()->Get_Pos().x;
	_float target_PosY = pPlayer->Return_Transform()->Get_Pos().y;
	_float target_PosZ = pPlayer->Return_Transform()->Get_Pos().z;

	_float target_rotX = pPlayer->Return_Transform()->Get_Rot().x;
	_float target_rotY = pPlayer->Return_Transform()->Get_Rot().y;
	_float target_rotZ = pPlayer->Return_Transform()->Get_Rot().z;

	D3DXMatrixRotationX(&matRotX, target_rotX);
	D3DXMatrixRotationY(&matRotY, target_rotY);
	D3DXMatrixRotationZ(&matRotZ, target_rotZ);
	D3DXMatrixTranslation(&matTrans, target_PosX, target_PosY, target_PosZ);
	matParents = matRotX * matRotY * matRotZ * matTrans;

	if (FAILED(m_pTransformCom->Update_Transform2(matParents)))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}
	if (m_StackDeltatime > 1.f)
		m_EatItem = 2;

	return S_OK;
}

HRESULT CItem::Movement2(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

	m_pTransformCom->Set_Pos(_vec3(m_Onecheck, 0.f, -8.f));
	m_Onecheck -= fDeltaTime;
	D3DXVECTOR3 d;
	d = pPlayer->Return_Transform()->Get_Rot();
	m_pTransformCom->Set_Rot(d);
	m_pTransformCom->RotateX(m_StackDeltatime);
	m_pTransformCom->RotateY(m_StackDeltatime);
	m_pTransformCom->RotateZ(m_StackDeltatime);

	_vec4x4 matAxis, matParents;

	_vec4x4	matRotX, matRotY, matRotZ, matTrans;
	_float target_PosX = pPlayer->Return_Transform()->Get_Pos().x;
	_float target_PosY = pPlayer->Return_Transform()->Get_Pos().y;
	_float target_PosZ = pPlayer->Return_Transform()->Get_Pos().z;

	_float target_rotX = pPlayer->Return_Transform()->Get_Rot().x;
	_float target_rotY = pPlayer->Return_Transform()->Get_Rot().y;
	_float target_rotZ = pPlayer->Return_Transform()->Get_Rot().z;

	D3DXMatrixRotationX(&matRotX, target_rotX);
	D3DXMatrixRotationY(&matRotY, target_rotY);
	D3DXMatrixRotationZ(&matRotZ, target_rotZ);
	D3DXMatrixTranslation(&matTrans, target_PosX, target_PosY, target_PosZ);
	matParents = matRotX * matRotY * matRotZ * matTrans;
	_vec3 vAxis = { 0.f,0.f,1.f };
	m_RotAngel += fDeltaTime*2.0f;
	D3DXMatrixRotationAxis(&matAxis, &vAxis, m_RotAngel);

	if (FAILED(m_pTransformCom->Update_Transform3(matAxis, matParents)))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}

	return S_OK;
}

HRESULT CItem::Movement3(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CVirtualCube* pVirtualCube = dynamic_cast<CVirtualCube*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_VirtualCube")->front());

	if (m_Twocheck)
	{
		m_pTransformCom->Set_Rot(_vec3(76.94f, 0.f, 0.f));
		m_Twocheck = false;
	}

	_vec4x4 matParents;
	matParents = pVirtualCube->Return_Transform()->Get_Desc().matWorld;

	switch (m_CubeNum)
	{
	case 1: m_pTransformCom->Set_Pos({ -0.2374f, 0.2449f, -0.2471f });
		break;
	case 2: m_pTransformCom->Set_Pos({ 0.2419f, 0.2419f, -0.2470f });
		break;
	case 3: m_pTransformCom->Set_Pos({ 0.2451f, -0.2396f, -0.2356f });
		break;
	case 4: m_pTransformCom->Set_Pos({ -0.2431f, -0.2396f, -0.2356f });
		break;
	case 5: m_pTransformCom->Set_Pos({ -0.2382f, 0.2281f, 0.2388f });
		break;
	case 6: m_pTransformCom->Set_Pos({ 0.2403f, 0.2281f, 0.2388f });
		break;
	case 7: m_pTransformCom->Set_Pos({ 0.2403f, -0.2489f, 0.2517f });
		break;
	case 8: m_pTransformCom->Set_Pos({ -0.2396f, -0.2356f, 0.2517f });
		break;
	}

	if (FAILED(m_pTransformCom->Update_Transform2(matParents)))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}

	return S_OK;
}