#include "stdafx.h"
#include "VirtualCube.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"
#include "Effect.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CEffect::CEffect(const CEffect & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom)
	, m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CEffect::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CEffect::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_Effect", L"Com_Effect", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	m_pTransformCom->Set_Scale(_vec3(3.4f, 3.4f, 1.4f));
	//m_pTransformCom->RotateX(23.f);
	return S_OK;
}

_uint CEffect::Update_GameObject(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	m_pTransformCom->Set_Rot(pPlayer->Return_Transform()->Get_Rot());

	CGameObject::Update_GameObject(fDeltaTime);

	if (m_ICondition == BLIND)
	{
		if (FAILED(Movement(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed to Movement Player");
			return UPDATE_ERROR;
		}
	}
	else if (m_ICondition == LIGHT)
	{
		if (FAILED(Movement2(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed to Movement Player");
			return UPDATE_ERROR;
		}
	}
	else if (m_ICondition == ROTATE)
	{
		if (FAILED(Movement3(fDeltaTime)))
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

_uint CEffect::Late_Update_GameObject(_float fDeltaTime)
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

HRESULT CEffect::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Player matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, m_ICondition)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture VirtualCube");
		return E_FAIL;
	}
	if (m_ICondition != BLIND)
	{
		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		{
			PRINT_LOG(L"Error", L"Failed to Render_VIBuffer Player");
			return E_FAIL;
		}
	}
	return S_OK;
}

CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffect*	pInstance = new CEffect(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect::Clone(void * pArg)
{
	CEffect*	pClone = new CEffect(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CEffect::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CEffect::Movement(_float fDeltaTime)
{
	/*CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

	_float a, b, c;
	a = pPlayer->Return_Transform()->Get_Pos().x;
	b = pPlayer->Return_Transform()->Get_Pos().y + 1.75f;
	c = pPlayer->Return_Transform()->Get_Pos().z;
	m_pTransformCom->Set_Pos({ a, b, c });
	m_pTransformCom->RotateZ(fDeltaTime);*/

	return S_OK;
}

HRESULT CEffect::Movement2(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

	_float a, b, c;
	a = pPlayer->Return_Transform()->Get_Pos().x;
	b = pPlayer->Return_Transform()->Get_Pos().y + 1.75f;
	c = pPlayer->Return_Transform()->Get_Pos().z;
	m_pTransformCom->Set_Pos({ a, b, c });
	//m_pTransformCom->RotateZ(fDeltaTime);
	m_StackDeltatime += fDeltaTime;
	m_fSizeUpX += fDeltaTime*50.f;
	m_fSizeUpY += fDeltaTime*50.f;
	m_pTransformCom->Set_Scale({m_fSizeUpX,m_fSizeUpY, 1.f});
	if (m_StackDeltatime > 0.25f)
	{
		m_ICondition = BLIND;
		m_StackDeltatime = 0.f;
		m_fSizeUpX = 0.f;
		m_fSizeUpY = 0.f;
	}
	return S_OK;
}

HRESULT CEffect::Movement3(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	//m_pTransformCom->Set_Scale({ 7.4f,7.4f,1.f });
	_float a, b, c;
	a = pPlayer->Return_Transform()->Get_Pos().x;
	b = pPlayer->Return_Transform()->Get_Pos().y + 1.75f;
	c = pPlayer->Return_Transform()->Get_Pos().z;
	m_pTransformCom->Set_Pos({ a, b, c });
	m_pTransformCom->RotateZ(fDeltaTime);
	m_StackDeltatime += fDeltaTime;
	m_fSizeUpX += fDeltaTime*0.9f;
	m_fSizeUpY += fDeltaTime*0.8f;
	m_pTransformCom->Set_Scale({ m_fSizeUpX,m_fSizeUpY,1.f });

	if (m_StackDeltatime > 6.5f)
	{
		m_ICondition = BLIND;
		m_StackDeltatime = 0.f;
		m_fSizeUpX = 0.f;
		m_fSizeUpY = 0.f;
	}
	return S_OK;
}