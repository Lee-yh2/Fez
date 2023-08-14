#include "stdafx.h"
#include "Explosion.h"
#include "Camera.h"
#include "Player.h"
#include "Heli.h"

CExplosion::CExplosion(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)/*, m_pTerrainVIBufferCom(nullptr)*/
	, m_pTransformCom(nullptr), m_eCurState(STATE::MOVING), m_bIsRotate(false), m_fRotateAngle(0.f), m_fOldRotate(0.f), m_iRotDir(RIGHT_ROT)
	, m_iDirNum(4), m_eMapDir(eMapDir::SOUTH), m_iMovingDis(0), m_fBulletSpeed(5.f), m_vTargetPosViewport(_vec3(0.f,0.f,0.f)), m_bDead(false)
	, m_vBeginPos(_vec3(0.f,0.f,0.f))
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
	ZeroMemory(&m_pTextureCom, sizeof(m_pTextureCom));
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = 5.f;
}

CExplosion::CExplosion(const CExplosion & other)
	: CGameObject(other.m_pDevice), m_pVIBufferCom(other.m_pVIBufferCom)/*, m_pTerrainVIBufferCom(other.m_pTerrainVIBufferCom)*/
	, m_pTransformCom(other.m_pTransformCom), m_eCurState(other.m_eCurState), m_tFrame(other.m_tFrame), m_bIsRotate(other.m_bIsRotate)
	, m_fRotateAngle(other.m_fRotateAngle), m_fOldRotate(other.m_fOldRotate), m_iRotDir(other.m_iRotDir), m_iDirNum(other.m_iDirNum)
	, m_eMapDir(other.m_eMapDir), m_iMovingDis(other.m_iMovingDis), m_fBulletSpeed(other.m_fBulletSpeed), m_vTargetPosViewport(other.m_vTargetPosViewport)
	, m_bDead(other.m_bDead), m_vBeginPos(other.m_vBeginPos)
{
	memcpy(m_pTextureCom, other.m_pTextureCom, sizeof(m_pTextureCom));
}

HRESULT CExplosion::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CExplosion::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Explosion", L"Com_Texture", (CComponent**)&m_pTextureCom[0])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	//m_pTargetHeli = static_cast<CHeli*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Monster_Heli")->front());
	m_eMapDir = static_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front())->Get_MapDir();
	m_pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());


	_vec3 vExPos = m_pTransformCom->Get_Pos();
	vExPos.z += 10.f;
	m_pTransformCom->Set_Pos(vExPos);
	m_pTransformCom->Set_Scale(_vec3(2.f, 2.f, 2.f));

	return S_OK;
}

_uint CExplosion::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (m_bDead) {
		return OBJ_DEAD;
	}

	if (FAILED(Movement(fDeltaTime)))
	{
	PRINT_LOG(L"Error", L"Failed To Movement Monster");
	return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}

	FrameMove(fDeltaTime);

	Scene_Change();

	return NO_EVENT;
}

_uint CExplosion::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();

	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::Alpha, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Monster In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CExplosion::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		PRINT_LOG(L"Error", L"Failed to Render Monster_Cullmode");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
	{
		PRINT_LOG(L"Error", L"Failed to Render Monster_Fillmode");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Monster matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom[(_int)m_eCurState]->Set_Texture(0, (_uint)m_tFrame.fFrameStart)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture Player");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Monster");
		return E_FAIL;
	}

	return S_OK;
}

CExplosion * CExplosion::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CExplosion*	pInstance = new CExplosion(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CExplosion::Clone(void * pArg)
{
	CExplosion*	pClone = new CExplosion(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMonster");
		Safe_Release(pClone);
	}

	return pClone;
}

void CExplosion::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (auto _Dst : m_pTextureCom)
		Safe_Release(_Dst);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CExplosion::Movement(_float fDeltaTime)
{

	return S_OK;
}

void CExplosion::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed;

	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_bDead = true;
	}
}

void CExplosion::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CExplosion::MOVING:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		case CExplosion::EXPLOSION:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}