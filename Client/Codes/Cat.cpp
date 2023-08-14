#include "stdafx.h"
#include "Cat.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"
#include "SoundMgr.h"

CCat::CCat(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr)
{
	ZeroMemory(&m_pTextureCom, sizeof(m_pTextureCom));
}

CCat::CCat(const CCat & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom)
{
	memcpy(m_pTextureCom, other.m_pTextureCom, sizeof(m_pTextureCom));
}

HRESULT CCat::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CCat::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_CatIdle", L"Com_Cat", (CComponent**)&m_pTextureCom[0])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_CatJump", L"Com_Cat2", (CComponent**)&m_pTextureCom[1])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_CatStand", L"Com_Cat3", (CComponent**)&m_pTextureCom[2])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}
	m_eCurState = IDLE;
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = 5.f;
	return S_OK;
}

_uint CCat::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);



	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}

	m_StackDeltatime += fDeltaTime;

	return NO_EVENT;
}

_uint CCat::Late_Update_GameObject(_float fDeltaTime)
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


	//if (m_StackDeltatime > 2.f)
	//{

	Movement(fDeltaTime);



	//		m_StackDeltatime = 0.f;
	//	}


	FrameMove(fDeltaTime, 1.0f);
	return NO_EVENT;
}

HRESULT CCat::Render_GameObject()
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

CCat * CCat::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCat*	pInstance = new CCat(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCat::Clone(void * pArg)
{
	CCat*	pClone = new CCat(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CCat::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (auto _Dst : m_pTextureCom)
		Safe_Release(_Dst);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CCat::Movement(_float fDeltaTime)
{
	srand((unsigned)time(nullptr));
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());


	_vec3 vPlayerPos = pPlayer->Get_PlayerPos();
	_vec3 vCatPos = m_pTransformCom->Get_Pos();

	HitCheck(vCatPos, vPlayerPos, 2.f);

	if (m_eCurState != IDLE)
	{
		if (vCatPos.x < vPlayerPos.x)
		{
			m_pTransformCom->Set_RotY(D3DXToRadian(0.f));

		}
		else if (vCatPos.x >= vPlayerPos.x)
		{
			m_pTransformCom->Set_RotY(D3DXToRadian(180.f));


		}

	}


	if (m_eCurState == IDLE)
		m_tFrame.fFrameEnd = 5.f;
	else if (m_eCurState == JUMP)
		m_tFrame.fFrameEnd = 14.f;
	else
		m_tFrame.fFrameEnd = 7.f;







	return S_OK;
}

void CCat::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed*0.4f;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{

		if (STAND == m_eCurState)
		{
			m_eCurState = IDLE;
		}


		m_tFrame.fFrameStart = 0.f;
	}
}

_bool CCat::HitCheck(_vec3 vCatPos, _vec3 vPlayerPos, _float fDis)
{


	if (vPlayerPos.x - fDis < vCatPos.x && vCatPos.x < vPlayerPos.x + fDis &&
		vPlayerPos.y - fDis * 0.5f < vCatPos.y && vCatPos.y < vPlayerPos.y + fDis * 0.5f &&
		vPlayerPos.z - 3.f < vCatPos.z && vCatPos.z < vPlayerPos.z + 3.f)
	{
		if (IDLE == m_eCurState)
		{
			m_eCurState = JUMP;


			int iRand = rand() % 3;

			if (0 == iRand)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"cat1.mp3", CSoundMgr::MONSTER);

			}
			else if (1 == iRand)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"cat2.mp3", CSoundMgr::MONSTER);

			}
			else
			{
				CSoundMgr::Get_Instance()->PlaySound(L"cat3.mp3", CSoundMgr::MONSTER);

			}


			//			m_eCurState = STAND;
		}
		//else if (STAND == m_eCurState && !m_bHit)
		//{
		//	m_bHit = true;
		//	m_eCurState = JUMP;

		//}

		return true;
	}
	else if (JUMP == m_eCurState)
	{
		m_eCurState = STAND;
		m_tFrame.fFrameStart = 0.f;

	}


	return false;
}
