#include "stdafx.h"
#include "..\Headers\BonusCamera.h"

CBonusCamera::CBonusCamera(LPDIRECT3DDEVICE9 pDevice)
	: CCamera(pDevice), m_vCameraAngle(_vec3(0.f, 50.f, 45.f))
{
}

CBonusCamera::CBonusCamera(const CBonusCamera & other)
	: CCamera(other), m_vCameraAngle(other.m_vCameraAngle)
{
}

HRESULT CBonusCamera::Ready_GameObject_Prototype()
{
	CCamera::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBonusCamera::Ready_GameObject(void * pArg/* = nullptr*/)
{
	CCamera::Ready_GameObject(pArg);	

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	m_pTargetTransform = (CTransform*)pManagement->Get_Component(
		ePrototypeType::NoneStatic, 
		L"Layer_BonusPlayer", 
		L"Com_Transform");
	if (nullptr == m_pTargetTransform)
	{
		PRINT_LOG(L"Error", L"m_pTargetTransform is nullptr");
		return E_FAIL;
	}

	Safe_AddRef(m_pTargetTransform);

	return S_OK;
}

_uint CBonusCamera::Update_GameObject(_float fDeltaTime)
{
	CCamera::Update_GameObject(fDeltaTime);		

	if (FAILED(KeyInput(fDeltaTime)))
	{
		return UPDATE_ERROR;
	}

	if (FAILED(Movement(fDeltaTime)))
	{
		return UPDATE_ERROR;
	}	

	return _uint();
}

_uint CBonusCamera::Late_Update_GameObject(_float fDeltaTime)
{
	return CCamera::Late_Update_GameObject(fDeltaTime);;
}

HRESULT CBonusCamera::Render_GameObject()
{
	CCamera::Render_GameObject();	

	return S_OK;
}

HRESULT CBonusCamera::Movement(_float fDeltaTime)
{
	const TRANSFORM_DESC& TransformDesc = m_pTargetTransform->Get_Desc();
	m_tDesc.vAt = TransformDesc.vPos;

	_vec3 vLook;
	memcpy(&vLook, &TransformDesc.matWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vReverseDir = (vLook * m_fDistanceToTarget) * -1.f;
	

	_vec3 vRight;
	memcpy(&vRight, &TransformDesc.matWorld.m[0][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);

	_vec4x4 matRotY;
	D3DXMatrixRotationAxis(&matRotY, &vRight, m_fCameraAngle);
	D3DXVec3TransformNormal(&vReverseDir, &vReverseDir, &matRotY);
	m_tDesc.vEye = m_tDesc.vAt + vReverseDir;

	/*_vec3	vUp;
	memcpy(&vUp, &TransformDesc.matWorld.m[1][0], sizeof(_vec3));
	D3DXVec3Normalize(&vUp, &vUp);
	
	_vec4x4	matRotZ;
	D3DXMatrixRotationAxis(&matRotZ, &vUp, D3DXToRadian(m_vCameraAngle.z));
	D3DXVec3TransformNormal(&vReverseDir, &vReverseDir, &matRotZ);

	m_tDesc.vEye = vReverseDir + m_tDesc.vAt;*/

	return S_OK;
}

HRESULT CBonusCamera::KeyInput(_float fDeltaTime)
{
	if (GetAsyncKeyState(VK_ADD) & 0x8000)
	{
		m_fDistanceToTarget -= 10.f * fDeltaTime;
		//m_tDesc.fFovY -= D3DXToRadian(45.f) * fDeltaTime;
	}

	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
	{
		m_fDistanceToTarget += 10.f * fDeltaTime;
		//m_tDesc.fFovY += D3DXToRadian(45.f) * fDeltaTime;
	}

	if (GetAsyncKeyState('P') & 0x8000)
	{
		m_fCameraAngle += D3DXToRadian(45.f) * fDeltaTime;
	}

	if (GetAsyncKeyState('O') & 0x8000)
	{
		m_fCameraAngle -= D3DXToRadian(45.f) * fDeltaTime;
	}

	return S_OK;
}

CBonusCamera * CBonusCamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBonusCamera* pInstance = new CBonusCamera(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMainCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBonusCamera::Clone(void * pArg/* = nullptr*/)
{
	CBonusCamera* pClone = new CBonusCamera(*this); /* 복사 생성자 호출 */
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMainCamera");
		Safe_Release(pClone);
	}

	return pClone;
}

void CBonusCamera::Free()
{	
	Safe_Release(m_pTargetTransform);

	CCamera::Free();
}
