#include "stdafx.h"
#include "Heli.h"
#include "Camera.h"

#include "SoundMgr.h"

CHeli::CHeli(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)/*, m_pTerrainVIBufferCom(nullptr)*/
	, m_pTransformCom(nullptr), m_eCurState(STATE::IDLE), m_bIsRotate(false), m_fRotateAngle(0.f), m_fOldRotate(0.f), m_iRotDir(RIGHT_ROT)
	, m_iDirNum(4), m_eMapDir(eMapDir::SOUTH), m_pCamera(nullptr), m_vStayPos(_vec3(0.f, 0.f, 0.f)), m_iHp(10), m_iBulletCount(0), m_iMovingCount(0)
	, m_bUpAndDown(false), m_fSpeed(0.5f), m_bHeliDead(false), m_iDeadCount(0) // 찬수 추가분
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
	ZeroMemory(&m_pTextureCom, sizeof(m_pTextureCom));
	m_tFrame.fFrameStart = 0.f;
	// m_tFrame.fFrameEnd = 7.f;
}

CHeli::CHeli(const CHeli & other)
	: CGameObject(other.m_pDevice), m_pVIBufferCom(other.m_pVIBufferCom) /*, m_pTerrainVIBufferCom(other.m_pTerrainVIBufferCom)*/
	, m_pTransformCom(other.m_pTransformCom), m_eCurState(other.m_eCurState), m_tFrame(other.m_tFrame), m_bIsRotate(other.m_bIsRotate)
	, m_fRotateAngle(other.m_fRotateAngle), m_fOldRotate(other.m_fOldRotate), m_iRotDir(other.m_iRotDir), m_iDirNum(other.m_iDirNum)
	, m_eMapDir(other.m_eMapDir), m_pCamera(other.m_pCamera), m_vStayPos(other.m_vStayPos), m_iHp(other.m_iHp), m_iBulletCount(other.m_iBulletCount), m_bUpAndDown(other.m_bUpAndDown)
	, m_iMovingCount(other.m_iMovingCount), m_fSpeed(other.m_fSpeed), m_bHeliDead(other.m_bHeliDead), m_iDeadCount(other.m_iDeadCount) // 찬수 추가분
{
	memcpy(m_pTextureCom, other.m_pTextureCom, sizeof(m_pTextureCom));
}

HRESULT CHeli::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CHeli::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Hell", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	CTransform* pTrans = static_cast<CTransform*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front()->Get_Component(L"Com_Transform"));

	m_pTransformCom->Set_MatOrbitTarget(pTrans->Get_Desc().matWorld);
	m_pTransformCom->Set_Scale(_vec3(5.f, 5.f, 5.f));

	return S_OK;
}

_uint CHeli::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
	PRINT_LOG(L"Error", L"Failed To Movement Monster");
	return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform_ForHeli()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}

	Key_Check(fDeltaTime);

	// Set_Billboard();

	// FrameMove(fDeltaTime); 

	// Scene_Change();

	return NO_EVENT;
}

_uint CHeli::Late_Update_GameObject(_float fDeltaTime)
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

HRESULT CHeli::Render_GameObject()
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

CHeli * CHeli::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHeli*	pInstance = new CHeli(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHeli::Clone(void * pArg)
{
	CHeli*	pClone = new CHeli(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMonster");
		Safe_Release(pClone);
	}

	return pClone;
}

void CHeli::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (auto _Dst : m_pTextureCom)
		Safe_Release(_Dst);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CHeli::Movement(_float fDeltaTime)
{

	if (m_bIsRotate)
	{
		if (D3DXToRadian(90.f)/*1.57f*/ <= m_fRotateAngle)
		{
			if (RIGHT_ROT == m_iRotDir) {
				//m_pTransformCom->Set_OrbitRotY(m_fOldRotate - D3DXToRadian(90.f));
			}
			else if (LEFT_ROT == m_iRotDir) {
				//m_pTransformCom->Set_OrbitRotY(m_fOldRotate + D3DXToRadian(90.f));
			}

			m_bIsRotate = false;

			if (m_iDirNum == 1)
				m_eMapDir = eMapDir::EAST;
			if (m_iDirNum == 2)
				m_eMapDir = eMapDir::NORTH;
			if (m_iDirNum == 3)
				m_eMapDir = eMapDir::WEST;
			if (m_iDirNum == 4)
				m_eMapDir = eMapDir::SOUTH;
		}
		else
		{
			if (RIGHT_ROT == m_iRotDir)
				m_pTransformCom->OrbitRotateY(-fDeltaTime);
			else if (LEFT_ROT == m_iRotDir)
				m_pTransformCom->OrbitRotateY(fDeltaTime);

			m_fRotateAngle += fDeltaTime * m_pTransformCom->Get_Desc().fRotSpdPerSec;
		}
	}



	// 찬수 추가
	_vec3 vHeliPos = m_pTransformCom->Get_Pos();

	if (!m_bHeliDead) {

		vHeliPos.y += (m_fSpeed * fDeltaTime);

		m_iMovingCount++;

		if (m_iMovingCount > 300) {
			m_fSpeed *= -1;
			m_iMovingCount = 0;
		}
	}
	else {
		vHeliPos.y -= (3.f * fDeltaTime);
		m_iBulletCount++;
		if (m_iBulletCount > 500)
			m_bDead = true;
	}

	m_pTransformCom->Set_Pos(vHeliPos);

	return S_OK;
}

void CHeli::Key_Check(_float fDeltaTime)
{
	if (m_tKeyManager.KeyDown(KEY_A_CLIENT))
	{
		if (!m_bIsRotate)
		{
			m_iRotDir = LEFT_ROT;
			m_fOldRotate = m_pTransformCom->Get_Desc().vRot.y;
			m_bIsRotate = true;
			m_fRotateAngle = 0.f;
			if (m_iDirNum == 1)
				m_iDirNum = 4;
			else
				--m_iDirNum;
		}
	}
	else if (m_tKeyManager.KeyDown(KEY_D_CLIENT))
	{
		if (!m_bIsRotate)
		{
			m_iRotDir = RIGHT_ROT;
			m_fOldRotate = m_pTransformCom->Get_Desc().vRot.y;
			m_bIsRotate = true;
			m_fRotateAngle = 0.f;
			if (m_iDirNum == 4)
				m_iDirNum = 1;
			else
				++m_iDirNum;
		}
	}

	/* Normal Bullet */
	if (m_tKeyManager.KeyDown(KEY_R_CLIENT)) 
	{
		CManagement* pManagement = CManagement::Get_Instance();


		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BULLET);
		CSoundMgr::Get_Instance()->PlaySound(L"Boss_RocketFire.wav", CSoundMgr::BULLET);

		_vec3 vBulletPos = m_pTransformCom->Get_Desc().vPos;

		TRANSFORM_DESC	tDesc;

		// 찬수 추가분
		// 0.1 숫자는 미사일 출발지점 설정해주는곳.
		//tDesc.vPos = _vec3(vBulletPos.x + 4.f, vBulletPos.y, vBulletPos.z);
		_float fTempY = (_float)(rand() % 3);
		fTempY += 12.f;
		tDesc.vPos = _vec3(5.f, fTempY, 15.f);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

		wstring wsBulletLayerName = L"Layer_Bullet";
		wsBulletLayerName += to_wstring(m_iBulletCount);
		m_iBulletCount++;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Bullet"
			, ePrototypeType::NoneStatic, wsBulletLayerName, &tDesc)))
		{
			PRINT_LOG(L"Error", L"Failed to Add_GameObject In a Layer_GameObject Bullet to Layer_Bullet");
		}
	}

	/* Meteor Bullet */
	if (m_tKeyManager.KeyDown(KEY_T_CLIENT))
	{
		CManagement* pManagement = CManagement::Get_Instance();
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CANNON);
		CSoundMgr::Get_Instance()->PlaySound(L"Boss_CannonFire.wav", CSoundMgr::CANNON);
		_vec3 vBulletPos = m_pTransformCom->Get_Desc().vPos;

		TRANSFORM_DESC	tDesc;

		tDesc.vPos = _vec3(vBulletPos.x + 1.f, vBulletPos.y + 10.f, vBulletPos.z);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

		wstring wsBulletLayerName = L"Layer_Bullet";
		wsBulletLayerName += to_wstring(m_iBulletCount);
		m_iBulletCount++;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Meteor"
			, ePrototypeType::NoneStatic, wsBulletLayerName, &tDesc)))
		{
			PRINT_LOG(L"Error", L"Failed to Add_GameObject In a Layer_GameObject Bullet to Layer_Bullet");
		}

		tDesc.vPos = _vec3(vBulletPos.x + 3.f, vBulletPos.y +11.f, vBulletPos.z);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

		wsBulletLayerName = L"Layer_Bullet";
		wsBulletLayerName += to_wstring(m_iBulletCount);
		m_iBulletCount++;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Meteor"
			, ePrototypeType::NoneStatic, wsBulletLayerName, &tDesc)))
		{
			PRINT_LOG(L"Error", L"Failed to Add_GameObject In a Layer_GameObject Bullet to Layer_Bullet");
		}

		tDesc.vPos = _vec3(vBulletPos.x + 5.f, vBulletPos.y + 12.f, vBulletPos.z);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

		wsBulletLayerName = L"Layer_Bullet";
		wsBulletLayerName += to_wstring(m_iBulletCount);
		m_iBulletCount++;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Meteor"
			, ePrototypeType::NoneStatic, wsBulletLayerName, &tDesc)))
		{
			PRINT_LOG(L"Error", L"Failed to Add_GameObject In a Layer_GameObject Bullet to Layer_Bullet");
		}

		tDesc.vPos = _vec3(vBulletPos.x + 7.f, vBulletPos.y + 13.f, vBulletPos.z);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

		wsBulletLayerName = L"Layer_Bullet";
		wsBulletLayerName += to_wstring(m_iBulletCount);
		m_iBulletCount++;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Meteor"
			, ePrototypeType::NoneStatic, wsBulletLayerName, &tDesc)))
		{
			PRINT_LOG(L"Error", L"Failed to Add_GameObject In a Layer_GameObject Bullet to Layer_Bullet");
		}

		tDesc.vPos = _vec3(vBulletPos.x + 10.f, vBulletPos.y + 14.f, vBulletPos.z);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

		wsBulletLayerName = L"Layer_Bullet";
		wsBulletLayerName += to_wstring(m_iBulletCount);
		m_iBulletCount++;

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Meteor"
			, ePrototypeType::NoneStatic, wsBulletLayerName, &tDesc)))
		{
			PRINT_LOG(L"Error", L"Failed to Add_GameObject In a Layer_GameObject Bullet to Layer_Bullet");
		}
	}
	
	m_tKeyManager.KeyCheck();
}

void CHeli::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}

void CHeli::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CHeli::IDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 0.f;
			break;
		case CHeli::WALK:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		case CHeli::JUMP:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		case CHeli::BEGINFALL:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 2.f;
			break;
		case CHeli::ENDFALL:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 3.f;
			break;
		case CHeli::FALLDIE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 8.f;
			break;
		case CHeli::CLIMB:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		case CHeli::GETIN:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 12.f;
			break;
		case CHeli::GETOUT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 13.f;
			break;
		case CHeli::GRAPIDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 0.f;
			break;
		case CHeli::GRAPMOVE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		default:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CHeli::Set_UnderAttack()
{
	if (m_iHp > 0) {
		--m_iHp;
		cout << "현재 체력 : " << m_iHp << endl;
	}
	else
	{
		if (!m_bDead)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOSS);
			CSoundMgr::Get_Instance()->PlaySound(L"Boss_BoomBeforeDie.mp3", CSoundMgr::BOSS);
			m_bDead = true;
		}


		// 찬수 추가분
		cout << "헬기 죽음" << endl;
		m_bHeliDead = true;
	}
}

HRESULT CHeli::Set_Billboard()
{
	_vec4x4 matView;
	if (FAILED(m_pDevice->GetTransform(D3DTS_VIEW, &matView)))
	{
		PRINT_LOG(L"Error", L"Failed GetTransform D3DTS_VIEW");
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
