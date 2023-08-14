#include "stdafx.h"
#include "BonusMonster.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"

CBonusMonster::CBonusMonster(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr), m_pTargetTransformCom(nullptr)
{
	//ZeroMemory(&m_pTextureCom, sizeof(m_pTextureCom));
}

CBonusMonster::CBonusMonster(const CBonusMonster & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTransformCom(other.m_pTransformCom)
	, m_pTextureCom(other.m_pTextureCom), m_pTargetTransformCom(other.m_pTargetTransformCom)
{
	
}

HRESULT CBonusMonster::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBonusMonster::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	_int iMonster = rand() % 3;
	if (0 == iMonster)
	{
		if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_Monkey", L"Com_Monkey", (CComponent**)&m_pTextureCom)))
		{
			PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
			return E_FAIL;
		}
		m_tFrame.fFrameEnd = 5.f;
		m_pTransformCom->Set_MoveSpeed(2.f);
	}
	else if (1 == iMonster)
	{
		if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BigPlant", L"Com_BigPlant", (CComponent**)&m_pTextureCom)))
		{
			PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
			return E_FAIL;
		}
		m_tFrame.fFrameEnd = 9.f;
		m_pTransformCom->Set_Scale(_vec3{ 4.f, 4.f, 1.f });
		m_pTransformCom->Set_PosY(3.f);
		m_pTransformCom->Set_MoveSpeed(1.f);
	}
	else if (2 == iMonster)
	{
		if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_Silk", L"Com_Silk", (CComponent**)&m_pTextureCom)))
		{
			PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
			return E_FAIL;
		}
		m_tFrame.fFrameEnd = 5.f;
		m_pTransformCom->Set_MoveSpeed(3.f);
	}
	m_eCurState = WALK;
	m_tFrame.fFrameStart = 0.f;
	m_pTargetTransformCom = (CTransform*)pManagement->Get_Component(ePrototypeType::NoneStatic, L"Layer_BonusPlayer", L"Com_Transform");

	return S_OK;
}

_uint CBonusMonster::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed to Movement Player");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}
	VillBoard();
	//m_StackDeltatime += fDeltaTime;

	return NO_EVENT;
}

_uint CBonusMonster::Late_Update_GameObject(_float fDeltaTime)
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

	FrameMove(fDeltaTime,1.0f);
	return NO_EVENT;
}

HRESULT CBonusMonster::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Player matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, (_uint)m_tFrame.fFrameStart)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture Player");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed to Render_VIBuffer Player");
		return E_FAIL;
	}

	return S_OK;
}

CBonusMonster * CBonusMonster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBonusMonster*	pInstance = new CBonusMonster(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBonusMonster::Clone(void * pArg)
{
	CBonusMonster*	pClone = new CBonusMonster(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CBonusMonster::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CBonusMonster::Movement(_float fDeltaTime)
{
	//srand((unsigned)time(nullptr));
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_BonusPlayer")->front());

	_vec3 _vPos = { m_pTransformCom->Get_Desc().vPos.x, 0.5f, m_pTransformCom->Get_Desc().vPos.z };
	_vec3 _vDir = m_pTargetTransformCom->Get_Desc().vPos - _vPos;
	//D3DXVec3Normalize(&_vDir, &_vDir);

	m_pTransformCom->MoveToDirection(_vDir, fDeltaTime);

	//m_pTransformCom->Set_Rot(pPlayer->Return_Transform()->Get_Rot());

	return S_OK;
}

HRESULT CBonusMonster::VillBoard()
{
	_vec4x4 matView;
	if (FAILED(m_pDevice->GetTransform(D3DTS_VIEW, &matView)))
	{
		PRINT_LOG(L"Error", L"Failed to GetTransform D3DTS_VIEW");
		return E_FAIL;
	}

	_vec4x4 matBillY;
	D3DXMatrixIdentity(&matBillY);

	matBillY._11 = matView._11;
	matBillY._13 = matView._13;
	matBillY._31 = matView._31;
	matBillY._33 = matView._33;
	D3DXMatrixInverse(&matBillY, 0, &matBillY);

	_vec4x4 matWorld = m_pTransformCom->Get_Desc().matWorld;

	m_pTransformCom->Set_WorldMatrix(matBillY * matWorld);

	return S_OK;
}

void CBonusMonster::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}