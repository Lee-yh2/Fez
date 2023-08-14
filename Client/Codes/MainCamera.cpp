#include "stdafx.h"
#include "MainCamera.h"


CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 pDevice)
	: CCamera(pDevice), m_pCameraTransform(nullptr), m_pTarget_RoomTransform(nullptr), m_pTarget_PlayerTransform(nullptr)
	, m_fDistanceToTarget(30.f), m_vCameraAngle(_vec3(0.f, 50.f, 45.f)), m_fOldCameraZAngle(0.f)
	, m_iZRotCount(0), m_bCameraMode(false), m_dwKey(0), m_bStart(true), m_bFinish(false), m_bCameraMovingBonusStage(false)
{
	ZeroMemory(&m_vParentPos, sizeof(_vec3));
}

CMainCamera::CMainCamera(const CMainCamera & other)
	: CCamera(other), m_pCameraTransform(other.m_pCameraTransform)
	, m_pTarget_RoomTransform(other.m_pTarget_RoomTransform), m_pTarget_PlayerTransform(other.m_pTarget_PlayerTransform)
	, m_fDistanceToTarget(other.m_fDistanceToTarget), m_vCameraAngle(other.m_vCameraAngle), m_fOldCameraZAngle(other.m_fOldCameraZAngle)
	, m_iZRotCount(other.m_iZRotCount), m_bCameraMode(other.m_bCameraMode), m_dwKey(other.m_dwKey), m_bStart(other.m_bStart)
	, m_vParentPos(other.m_vParentPos), m_bFinish(false), m_bCameraMovingBonusStage(other.m_bCameraMovingBonusStage)
{
}

HRESULT CMainCamera::Ready_GameObject_Prototype()
{
	CCamera::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CMainCamera::Ready_GameObject(void * pArg)
{
	CCamera::Ready_GameObject(pArg);

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC	tTransformDesc;
	tTransformDesc.fMoveSpdPerSec = 10.f;
	tTransformDesc.fRotSpdPerSec = D3DXToRadian(90.f);
	tTransformDesc.vPos = _vec3(35.f, 55.f, 35.f);

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pCameraTransform, &tTransformDesc)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}


	/* 수정! */
	if (!pManagement->Get_Stage())
	{
		m_pTarget_RoomTransform = static_cast<CTransform*>(pManagement->Get_Component(ePrototypeType::NoneStatic, L"Layer_Room", L"Com_Transform"));

		if (!m_pTarget_RoomTransform)
		{
			PRINT_LOG(L"Error", L"m_pTarget_RoomTransform is nullptr");
			return E_FAIL;
		}

		Safe_AddRef(m_pTarget_RoomTransform);

	}

	m_pTarget_PlayerTransform = static_cast<CTransform*>(pManagement->Get_Component(ePrototypeType::Static, L"Layer_Player", L"Com_Transform"));


	Safe_AddRef(m_pTarget_PlayerTransform);

	return S_OK;
}

_uint CMainCamera::Update_GameObject(_float fDeltaTime)
{
	CCamera::Update_GameObject(fDeltaTime);

	if (FAILED(KeyInput(fDeltaTime)))
		return UPDATE_ERROR;
	m_tKeyManager.KeyCheck();

	/*if (m_bStart)
	{*/
		
	/*}*/

	if (m_bFinish)
	{
		m_vCameraAngle.y += 45.f * fDeltaTime;

		if (89.f < m_vCameraAngle.y)
		{
			m_vCameraAngle.y = 89.f;
			m_bFinish = false;
		}
	}

	if (FAILED(m_pCameraTransform->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}

	if (FAILED(Movement(fDeltaTime)))
		return UPDATE_ERROR;

	return NO_EVENT;
}

_uint CMainCamera::Late_Update_GameObject(_float fDeltaTime)
{
	return CCamera::Late_Update_GameObject(fDeltaTime);
}

HRESULT CMainCamera::Render_GameObject()
{
	CCamera::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pCameraTransform->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform MainCamera matWorld");
		return E_FAIL;
	}

	return S_OK;
}

CMainCamera * CMainCamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMainCamera*	pInstance = new CMainCamera(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create CMainCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMainCamera::Clone(void * pArg)
{
	CMainCamera*	pClone = new CMainCamera(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone CMainCamera");
		Safe_Release(pClone);
	}

	return pClone;
}

void CMainCamera::Free()
{
	Safe_Release(m_pCameraTransform);
	Safe_Release(m_pTarget_PlayerTransform);
	Safe_Release(m_pTarget_RoomTransform);

	CCamera::Free();
}

void CMainCamera::Begin_CameraMovingBonusStage(_float fDeltaTime)
{
	// 카메라 무빙 여기서 구현
}

HRESULT CMainCamera::KeyInput(_float fDeltaTime)
{
	if (m_tKeyManager.KeyDown(KEY_K_CLIENT))
	{
		m_bFinish = true;
	}

	if (m_tKeyManager.KeyDown(KEY_P_CLIENT))
	{
		if (m_bCameraMode)
			m_pCameraTransform->Set_Desc(m_pTarget_PlayerTransform->Get_Desc());
		else
		{
			TRANSFORM_DESC	tTransformDesc;
			tTransformDesc.vPos = m_pCameraTransform->Get_Desc().vPos;
			tTransformDesc.fMoveSpdPerSec = 10.f;
			tTransformDesc.fRotSpdPerSec = D3DXToRadian(90.f);
			_vec3	vLook;
			memcpy(&vLook, &m_pCameraTransform->Get_Desc().matWorld.m[2][0], sizeof(_vec3));
			D3DXVec3Normalize(&vLook, &vLook);
			m_tDesc.vAt = vLook * 3.f;
			m_pCameraTransform->Set_Desc(tTransformDesc);
		}

		m_tDesc.bToolmode = !m_tDesc.bToolmode;
		m_sbCameraMode = !m_sbCameraMode;
		m_bCameraMode = !m_bCameraMode;
	}

	if (m_sbCameraMode)
	{
		if (m_tKeyManager.KeyPressing(KEY_RIGHT_CLIENT))
		{
			//m_fDistanceToTarget += -10.f * fDeltaTime;
			////m_tDesc.fFovY += -D3DXToRadian(45.f) * fDeltaTime;

			//if (1.f > m_fDistanceToTarget)
			//	m_fDistanceToTarget = 1.f;
			m_pCameraTransform->RotateY(1.f * fDeltaTime);
		}

		if (m_tKeyManager.KeyPressing(KEY_LEFT_CLIENT))
		{
			//m_fDistanceToTarget += 10.f * fDeltaTime;
			////m_tDesc.fFovY += D3DXToRadian(45.f) * fDeltaTime;

			//if (10.f < m_fDistanceToTarget)
			//	m_fDistanceToTarget = 10.f;
			m_pCameraTransform->RotateY(-1.f * fDeltaTime);
		}

		/*if (m_tKeyManager.KeyPressing(KEY_UP_CLIENT))
		{
			if (!m_bCameraMode)
			{
				m_vCameraAngle.y += 90.f * fDeltaTime;

				if (89.f < m_vCameraAngle.y)
					m_vCameraAngle.y = 89.f;
			}
			else if (89.f > D3DXToDegree(m_pCameraTransform->Get_Desc().vRot.x) + m_vCameraAngle.y)
				m_pCameraTransform->RotateX(1.f * fDeltaTime);
		}*/

		if (m_tKeyManager.KeyPressing(KEY_DOWN_CLIENT))
		{
			if (!m_bCameraMode)
			{
				m_vCameraAngle.y += (-90.f) * fDeltaTime;

				if (15.f > m_vCameraAngle.y)
					m_vCameraAngle.y = 15.f;
			}
			else if (-89.f < D3DXToDegree(m_pCameraTransform->Get_Desc().vRot.x) + m_vCameraAngle.y)
				m_pCameraTransform->RotateX(-1.f * fDeltaTime);
		}

		if (m_bCameraMode)
		{
			if (GetAsyncKeyState('W') & 0x8000)
				m_pCameraTransform->Go_Straight(2.f * fDeltaTime);
			if (GetAsyncKeyState('A') & 0x8000) {
				m_pCameraTransform->Go_Sideway(-2.f * fDeltaTime);
			}
			if (GetAsyncKeyState('S') & 0x8000)
				m_pCameraTransform->Go_Straight(-2.f * fDeltaTime);
			if (GetAsyncKeyState('D') & 0x8000)
				m_pCameraTransform->Go_Sideway(2.f * fDeltaTime);
			/*if (GetAsyncKeyState('Q') & 0x8000)
			m_pCameraTransform->RotateY(-2.f * fDeltaTime);
			if (GetAsyncKeyState('E') & 0x8000)
			m_pCameraTransform->RotateY(2.f * fDeltaTime);*/
		}
	}

	return S_OK;
}

HRESULT CMainCamera::Movement(_float fDeltaTime)
{
	const	TRANSFORM_DESC*	pTransformRoomDesc = &m_pTarget_RoomTransform->Get_Desc();
	const	TRANSFORM_DESC* pTransformPlayerDesc = &m_pCameraTransform->Get_Desc();
	if (!m_bStart && !m_bCameraMode)
		pTransformPlayerDesc = &m_pTarget_PlayerTransform->Get_Desc();
	/* 스테이지 구분용 */
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	m_tDesc.vAt = pTransformPlayerDesc->vPos;

	if (m_bStart)
		m_tDesc.vAt += m_vParentPos;

	_vec3	vLook;
	memcpy(&vLook, &pTransformPlayerDesc->matWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3	vReverseDir = -1.f * (vLook * m_fDistanceToTarget);

	_vec3	vRight;
	memcpy(&vRight, &pTransformPlayerDesc->matWorld.m[0][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);

	_vec4x4	matRotY;
	D3DXMatrixRotationAxis(&matRotY, &vRight, D3DXToRadian(m_vCameraAngle.y));
	D3DXVec3TransformNormal(&vReverseDir, &vReverseDir, &matRotY);

	_vec3	vUp;

	/* 스테이지 구분용 */
	if (!pManagement->Get_Stage())
	{
		m_tDesc.vAt = pTransformRoomDesc->vPos;
		pTransformRoomDesc = &m_pTarget_RoomTransform->Get_Desc();
		memcpy(&vUp, &pTransformRoomDesc->matWorld.m[1][0], sizeof(_vec3));
		D3DXVec3Normalize(&vUp, &vUp);
	}
	else
	{
		m_tDesc.vAt = pTransformPlayerDesc->vPos;
		memcpy(&vUp, &pTransformPlayerDesc->matWorld.m[1][0], sizeof(_vec3));
		D3DXVec3Normalize(&vUp, &vUp);
	}
	/* 끝 */

	_vec4x4	matRotZ;
	D3DXMatrixRotationAxis(&matRotZ, &vUp, D3DXToRadian(m_vCameraAngle.z));
	D3DXVec3TransformNormal(&vReverseDir, &vReverseDir, &matRotZ);

	m_tDesc.vEye = vReverseDir + m_tDesc.vAt;

	return S_OK;
}