#include "stdafx.h"
#include "Bird.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"
#include "SoundMgr.h"

CBird::CBird(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_fAccel(0.f)
{
	ZeroMemory(&m_pTextureCom, sizeof(m_pTextureCom));
}

CBird::CBird(const CBird & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom)
{
	memcpy(m_pTextureCom, other.m_pTextureCom, sizeof(m_pTextureCom));
}

HRESULT CBird::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBird::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BirdWalk", L"Com_Bird", (CComponent**)&m_pTextureCom[0])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BirdSFly", L"Com_Bird2", (CComponent**)&m_pTextureCom[1])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BirdFly", L"Com_Bird3", (CComponent**)&m_pTextureCom[2])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}
	m_eCurState = WALK;
	m_pTransformCom->Set_Scale(_vec3(2.2f, 2.2f, 1.f));
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = 5.f;
	return S_OK;
}

_uint CBird::Update_GameObject(_float fDeltaTime)
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

	//	m_StackDeltatime += fDeltaTime;

	return NO_EVENT;
}

_uint CBird::Late_Update_GameObject(_float fDeltaTime)
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

	//if (m_StackDeltatime > 1.f)
	//{

	Movement(fDeltaTime);



	//	m_StackDeltatime = 0.f;
	//}


	FrameMove(fDeltaTime, 1.0f);
	return NO_EVENT;
}

HRESULT CBird::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Player matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom[(_int)m_eCurState]->Set_Texture(0, (_uint)m_tFrame.fFrameStart)))
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

CBird * CBird::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBird*	pInstance = new CBird(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBird::Clone(void * pArg)
{
	CBird*	pClone = new CBird(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CBird::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (auto _Dst : m_pTextureCom)
		Safe_Release(_Dst);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CBird::Movement(_float fDeltaTime)
{
	srand((unsigned)time(nullptr));
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

	_vec3 vPlayerPos = pPlayer->Get_PlayerPos();
	_vec3 vBirdPos = m_pTransformCom->Get_Pos();

	HitCheck(vBirdPos, vPlayerPos, 0.5f);

	if (m_eCurState != WALK)
	{
		if (vBirdPos.x < vPlayerPos.x)
		{
			vBirdPos.x -= (fDeltaTime * 1.5f);
			vBirdPos.y += ((fDeltaTime) * 0.5f);
			m_pTransformCom->Set_RotY(D3DXToRadian(180.f));

		}
		else if (vBirdPos.x >= vPlayerPos.x)
		{
			vBirdPos.x += (fDeltaTime * 1.5f);
			vBirdPos.y += ((fDeltaTime) * 0.5f);

		}

		m_pTransformCom->Set_Pos(vBirdPos);
	}
	

	if (m_eCurState == WALK)
		m_tFrame.fFrameEnd = 5.f;
	else if (m_eCurState == SFLY)
		m_tFrame.fFrameEnd = 6.f;
	else
		m_tFrame.fFrameEnd = 6.f;



	return S_OK;
}

void CBird::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		if (m_eCurState == SFLY)
			m_eCurState = FLY;

		m_tFrame.fFrameStart = 0.f;
	}
}

_bool CBird::HitCheck(_vec3 vBirdPos, _vec3 vPlayerPos, _float fDis)
{


	if (vPlayerPos.x - fDis < vBirdPos.x && vBirdPos.x < vPlayerPos.x + fDis &&
		vPlayerPos.y - fDis < vBirdPos.y && vBirdPos.y < vPlayerPos.y + fDis)

	{
		if (WALK == m_eCurState)
		{

			CSoundMgr::Get_Instance()->PlaySound(L"BirdFly.wav", CSoundMgr::BOSS);
			m_tFrame.fFrameStart = 0.f;
			m_eCurState = SFLY;
		}
		//else if (STAND == m_eCurState && !m_bHit)
		//{
		//	m_bHit = true;
		//	m_eCurState = JUMP;

		//}

		return true;
	}

	return false;
}
