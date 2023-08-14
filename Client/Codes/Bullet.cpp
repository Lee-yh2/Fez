#include "stdafx.h"
#include "Bullet.h"
#include "Camera.h"
#include "Player.h"
#include "Heli.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)/*, m_pTerrainVIBufferCom(nullptr)*/
	, m_pTransformCom(nullptr), m_eCurState(STATE::MOVING), m_bIsRotate(false), m_fRotateAngle(0.f), m_fOldRotate(0.f), m_iRotDir(RIGHT_ROT)
	, m_iDirNum(4), m_eMapDir(eMapDir::SOUTH), m_iMovingDis(0), m_fBulletSpeed(5.f), m_vTargetPosViewport(_vec3(0.f,0.f,0.f)), m_bDead(false)
	, m_vBeginPos(_vec3(0.f,0.f,0.f)), m_iBulletDisCount(0) // 찬수 추가분
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
	ZeroMemory(&m_pTextureCom, sizeof(m_pTextureCom));
	m_tFrame.fFrameStart = 0.f;
	// m_tFrame.fFrameEnd = 7.f;
}

CBullet::CBullet(const CBullet & other)
	: CGameObject(other.m_pDevice), m_pVIBufferCom(other.m_pVIBufferCom)/*, m_pTerrainVIBufferCom(other.m_pTerrainVIBufferCom)*/
	, m_pTransformCom(other.m_pTransformCom), m_eCurState(other.m_eCurState), m_tFrame(other.m_tFrame), m_bIsRotate(other.m_bIsRotate)
	, m_fRotateAngle(other.m_fRotateAngle), m_fOldRotate(other.m_fOldRotate), m_iRotDir(other.m_iRotDir), m_iDirNum(other.m_iDirNum)
	, m_eMapDir(other.m_eMapDir), m_iMovingDis(other.m_iMovingDis), m_fBulletSpeed(other.m_fBulletSpeed), m_vTargetPosViewport(other.m_vTargetPosViewport)
	, m_bDead(other.m_bDead), m_vBeginPos(other.m_vBeginPos), m_iBulletDisCount(other.m_iBulletDisCount) // 찬수 추가
{
	memcpy(m_pTextureCom, other.m_pTextureCom, sizeof(m_pTextureCom));
}

HRESULT CBullet::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBullet::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Missile", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	m_pTargetHeli = static_cast<CHeli*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Monster_Heli")->front());
	m_eMapDir = static_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front())->Get_MapDir();
	m_pCamera = static_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());
	
	// 찬수 추가분
	_vec3 vTempViewport = { 0.f, 0.f, 0.f };
	_vec3 vTempWorld = static_cast<CTransform*>(m_pTargetHeli->Get_Component(L"Com_Transform"))->Get_Desc().vPos;
	_vec3 vTempFake = { 3.f, 12.5f, 15.f };
	m_pCamera->WorldToScreen(&vTempFake, &vTempViewport);

	m_vTargetPosViewport = vTempViewport;
	
	return S_OK;
}

_uint CBullet::Update_GameObject(_float fDeltaTime)
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

	// 찬수 추가분
	m_iBulletDisCount++;

	if (m_iBulletDisCount > 1000)
		m_bDead = true;


	return NO_EVENT;
}

_uint CBullet::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();

	Check_CollisionHeli();

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

HRESULT CBullet::Render_GameObject()
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

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBullet*	pInstance = new CBullet(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBullet::Clone(void * pArg)
{
	CBullet*	pClone = new CBullet(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMonster");
		Safe_Release(pClone);
	}

	return pClone;
}

void CBullet::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (auto _Dst : m_pTextureCom)
		Safe_Release(_Dst);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CBullet::Movement(_float fDeltaTime)
{
	_vec3 vBulletPos = m_pTransformCom->Get_Desc().vPos;

	switch (m_eMapDir)
	{
	case eMapDir::EAST:
		vBulletPos.z += fDeltaTime * 5.f;
		break;
	case eMapDir::WEST:
		vBulletPos.z -= fDeltaTime * 5.f;
		break;
	case eMapDir::SOUTH:
		vBulletPos.x += fDeltaTime * 5.f;
		break;
	case eMapDir::NORTH:
		vBulletPos.x -= fDeltaTime * 5.f;
		break;
	default:
		break;
	}

	m_pTransformCom->Set_Pos(vBulletPos);

	return S_OK;
}

void CBullet::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}

void CBullet::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CBullet::MOVING:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 0.f;
			break;
		case CBullet::EXPLOSION:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

_bool CBullet::Check_CollisionHeli()
{
	CManagement* pManagement = CManagement::Get_Instance();
	CTransform* pHeliTrans = static_cast<CTransform*>(pManagement->Get_Component(ePrototypeType::NoneStatic, L"Layer_Monster_Heli", L"Com_Transform", 0));

	_vec3	vCollisionRange = { 0.f, 0.f, 0.f };
	_vec3	vBulletPosFromViewport = { 0.f, 0.f, 0.f };
	// _vec3	vHeliPosFromViewport = { 0.f, 0.f, 0.f };
	_vec3	vBulletPosFromWorld = m_pTransformCom->Get_Desc().vPos;
	_vec3	vHeliPosFromWorld = pHeliTrans->Get_Desc().vPos;

	m_pCamera->WorldToScreen(&vBulletPosFromWorld, &vBulletPosFromViewport);
	// m_pCamera->WorldToScreen(&vHeliPosFromWorld, &vHeliPosFromViewport);

	if (Check_Rect(vBulletPosFromViewport, m_vTargetPosViewport, &vCollisionRange.x, &vCollisionRange.y)) {

		m_pTargetHeli->Set_UnderAttack();
		m_bDead = true;
		
		TRANSFORM_DESC tDesc;

		tDesc.vPos = vBulletPosFromWorld;
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

		CManagement* pManagement = CManagement::Get_Instance();

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Explosion"
			, ePrototypeType::NoneStatic, L"Layer_Explosion", &tDesc))) {
		}
	}
	return false;
}

_bool CBullet::Check_CollisionPlayer()
{
	return _bool();
}

_bool CBullet::Check_Rect(_vec3 _Dst, _vec3 _Src, _float * _x, _float * _y) // 찬수 추가분
{
	float	fCX = abs(_Dst.x - _Src.x);
	float	fCY = abs(_Dst.y - _Src.y);

	// 충돌 사이즈 여기서 넣으면 됨, 마법의 숫자를 넣어보세요 !!
	float	fDisX = (float)((100 + 100) / 2);
	float	fDisY = (float)((100 + 100) / 2);

	if (fCX < fDisX && fCY < fDisY)
	{
		*_x = fDisX - fCX;
		*_y = fDisY - fCY;
		return true;
	}

	return false;
}
