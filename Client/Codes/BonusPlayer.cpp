#include "stdafx.h"
#include "..\Headers\BonusPlayer.h"

CBonusPlayer::CBonusPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTransformCom(nullptr), m_ePreState(NUMOFSTATE), m_eCurState(IDLE_DOWN)
	, m_eCurDir(DOWN), m_fCurPlayTime(0.f)
{
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = 9.f;
}

CBonusPlayer::CBonusPlayer(const CBonusPlayer & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTransformCom(other.m_pTransformCom), m_ePreState(other.m_ePreState), m_eCurState(other.m_eCurState)
	, m_eCurDir(other.m_eCurDir), m_fCurPlayTime(other.m_fCurPlayTime)
{
}

HRESULT CBonusPlayer::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();
	return S_OK;
}

HRESULT CBonusPlayer::Ready_GameObject(void * pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component(
		ePrototypeType::Static,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BonusPlayerIdle_Down", L"Com_Player_Idle_Down", (CComponent**)&m_pTextureCom[0])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BonusPlayerIdle_Left", L"Com_Player_Idle_Left", (CComponent**)&m_pTextureCom[1])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BonusPlayerIdle_Right", L"Com_Player_Idle_Right", (CComponent**)&m_pTextureCom[2])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BonusPlayerIdle_Up", L"Com_Player_Idle_Up", (CComponent**)&m_pTextureCom[3])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BonusPlayerWalk_Down", L"Com_Player_Walk_Down", (CComponent**)&m_pTextureCom[4])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BonusPlayerWalk_Left", L"Com_Player_Walk_Left", (CComponent**)&m_pTextureCom[5])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BonusPlayerWalk_Right", L"Com_Player_Walk_Right", (CComponent**)&m_pTextureCom[6])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BonusPlayerWalk_Up", L"Com_Player_Walk_Up", (CComponent**)&m_pTextureCom[7])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	TRANSFORM_DESC m_TransformDesc;	
	m_TransformDesc.fMoveSpdPerSec = 6.f;
	m_TransformDesc.fRotSpdPerSec = D3DXToRadian(90.f);
	m_TransformDesc.vScale = { 1.f, 1.f, 1.f };
	m_TransformDesc.vPos = { 60.f, 0.f, 60.f };
	m_TransformDesc.vPos.y += 0.5f;

	if (FAILED(Add_Component(
		ePrototypeType::Static,
		L"Component_Transform",
		L"Com_Transform",
		(CComponent**)&m_pTransformCom,
		&m_TransformDesc)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Com_Transform");
		return E_FAIL;
	}


	return S_OK;
}

_uint CBonusPlayer::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed To Movement Player");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}

	m_tKeyManager.KeyCheck();

	FrameMove(fDeltaTime, 1.2f);

	m_fCurPlayTime += fDeltaTime;
	Scene_Change();

	return NO_EVENT;
}

_uint CBonusPlayer::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::Alpha, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Player In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CBonusPlayer::Render_GameObject()
{
	CGameObject::Render_GameObject();
	
	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed To SetTransform Player");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom[(_int)m_eCurState]->Set_Texture(0, (_uint)m_tFrame.fFrameStart)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture Player");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Player");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBonusPlayer::Movement(_float fDeltaTime)
{
	if (m_tKeyManager.KeyPressing(KEY_UP_CLIENT))
	{
		m_pTransformCom->Go_Straight(fDeltaTime);
		m_eCurState = WALK_UP;
		m_eCurDir = UP;
	}
	else if (m_tKeyManager.KeyPressing(KEY_DOWN_CLIENT))
	{
		m_pTransformCom->Go_Straight(-fDeltaTime);
		m_eCurState = WALK_DOWN;
		m_eCurDir = DOWN;
	}
	else if (m_tKeyManager.KeyPressing(KEY_LEFT_CLIENT))
	{
		m_pTransformCom->Go_Sideway(-fDeltaTime);
		m_eCurState = WALK_LEFT;
		m_eCurDir = LEFT;
	}
	else if (m_tKeyManager.KeyPressing(KEY_RIGHT_CLIENT))
	{
		m_pTransformCom->Go_Sideway(fDeltaTime);
		m_eCurState = WALK_RIGHT;
		m_eCurDir = RIGHT;
	}
	else if (m_eCurState == WALK_UP)
	{
		m_eCurState = IDLE_UP;
		m_tFrame.fFrameStart = 0.f;
	}
	else if (m_eCurState == WALK_DOWN)
	{
		m_eCurState = IDLE_DOWN;
		m_tFrame.fFrameStart = 0.f;
	}
	else if (m_eCurState == WALK_LEFT)
	{
		m_eCurState = IDLE_LEFT;
		m_tFrame.fFrameStart = 0.f;
	}
	else if (m_eCurState == WALK_RIGHT)
	{
		m_eCurState = IDLE_RIGHT;
		m_tFrame.fFrameStart = 0.f;
	}

	if (m_tKeyManager.KeyPressing(KEY_A_CLIENT))
	{
		m_pTransformCom->RotateY(-fDeltaTime);
	}

	if (m_tKeyManager.KeyPressing(KEY_D_CLIENT))
	{
		m_pTransformCom->RotateY(fDeltaTime);
	}

	return S_OK;
}

CBonusPlayer * CBonusPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBonusPlayer* pInstance = new CBonusPlayer(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBonusPlayer::Clone(void * pArg/* = nullptr*/)
{
	CBonusPlayer* pClone = new CBonusPlayer(*this); /* 복사 생성자 호출 */
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CBonusPlayer::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (auto _Dst : m_pTextureCom)
		Safe_Release(_Dst);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

void CBonusPlayer::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
			m_tFrame.fFrameStart = 0.f;
	}
}

void CBonusPlayer::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBonusPlayer::IDLE_DOWN:
		case CBonusPlayer::IDLE_UP:
		case CBonusPlayer::IDLE_LEFT:
		case CBonusPlayer::IDLE_RIGHT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 9.f;
			break;
		case CBonusPlayer::WALK_DOWN:
		case CBonusPlayer::WALK_UP:
		case CBonusPlayer::WALK_LEFT:
		case CBonusPlayer::WALK_RIGHT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 7.f;
			break;
		default:
			break;
		}

		m_ePreState = m_eCurState;
	}
}
