#include "stdafx.h"
#include "Player.h"
#include "Camera.h"
#include "Texture.h"
#include "BlackMonster.h"

CBlackMonster::CBlackMonster(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCamera(nullptr)
{
}

CBlackMonster::CBlackMonster(const CBlackMonster & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom), m_pCamera(other.m_pCamera)
{
}

HRESULT CBlackMonster::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBlackMonster::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_CubeTexture"
		, L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_BlackCube"
		, L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}


	m_pTransformCom->Set_Pos(_vec3(12.f, 20.8f, 18.f));
	m_pTransformCom->Set_Scale(_vec3(0.7f, 0.7f, 0.7f));
	return S_OK;
}

_uint CBlackMonster::Update_GameObject(_float fDeltaTime)
{
	srand(unsigned(time(NULL)));
	CManagement* pManagement = CManagement::Get_Instance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

	m_vPlayerPos = pPlayer->Return_Transform()->Get_Desc().vPos;
	m_vMyPos = m_pTransformCom->Get_Desc().vPos;
	_vec3 vDistan = m_vPlayerPos - m_vMyPos;
	_float fDistan = sqrt((vDistan.x * vDistan.x) + (vDistan.y * vDistan.y) + (vDistan.z * vDistan.z));
	////////////////////////////////////////
	if (fDistan < 0.75f && m_iMode == 4 && !m_mujok)	//충돌했을때
		pPlayer->Set_CollisionMonster();
	if (GetAsyncKeyState('M') & 0x8000)
	{
		m_mujok = false;
	}
	////////////////////////////////////////
	D3DXVECTOR3 d;
	d = pPlayer->Return_Transform()->Get_Rot();
	m_pTransformCom->Set_Rot(d);

	if (m_iMode == 0)
	{
		_vec3 Real_Pos = (vDistan*0.009f) + m_vMyPos;
		m_pTransformCom->Set_Pos(Real_Pos);

		if (FAILED(m_pTransformCom->Update_Transform()))
		{
			PRINT_LOG(L"Error", L"Failed to Update_Transform");
			return UPDATE_ERROR;
		}

		if (fDistan < 6.f)
			m_iMode = 1;

	}
	if (m_iMode == 1)
	{
		_float fPointX = pPlayer->Return_Transform()->Get_Pos().x;
		_float fPointY = pPlayer->Return_Transform()->Get_Pos().y;
		_float fPointZ = pPlayer->Return_Transform()->Get_Pos().z;

		if (pPlayer->Return_eMapDir() == eMapDir::SOUTH)
			fPointX = pPlayer->Return_Transform()->Get_Pos().x + 6.f;
		else if (pPlayer->Return_eMapDir() == eMapDir::EAST)
			fPointZ = pPlayer->Return_Transform()->Get_Pos().z + 6.f;
		else if (pPlayer->Return_eMapDir() == eMapDir::NORTH)
			fPointX = pPlayer->Return_Transform()->Get_Pos().x - 6.f;
		else if (pPlayer->Return_eMapDir() == eMapDir::WEST)
			fPointZ = pPlayer->Return_Transform()->Get_Pos().z - 6.f;

		_vec3 vPoint = { fPointX,fPointY,fPointZ };
		_vec3 Real_Dir = vPoint - m_pTransformCom->Get_Pos();
		_vec3 Real_Pos = (Real_Dir*0.08f) + m_vMyPos;
		m_pTransformCom->Set_Pos(Real_Pos);
		if (FAILED(m_pTransformCom->Update_Transform()))
		{
			PRINT_LOG(L"Error", L"Failed to Update_Transform");
			return UPDATE_ERROR;
		}
		m_StackDeltatime += fDeltaTime;
		if (m_StackDeltatime > 1.5f)
		{
			m_StackDeltatime = 0.f;
			m_iMode = 2;
			m_RotAngel = 0.f;
		}
	}

	if (m_iMode == 2)
	{
		m_pTransformCom->Set_Pos({ 6.f, 0.f, 0.f });
		_vec4x4 matAxis, matParents;

		_vec4x4	matRotX, matRotY, matRotZ, matTrans;
		_float target_PosX = pPlayer->Return_Transform()->Get_Pos().x;
		_float target_PosY = pPlayer->Return_Transform()->Get_Pos().y;
		_float target_PosZ = pPlayer->Return_Transform()->Get_Pos().z;

		_float target_rotX = pPlayer->Return_Transform()->Get_Rot().x;
		_float target_rotY = pPlayer->Return_Transform()->Get_Rot().y;
		_float target_rotZ = pPlayer->Return_Transform()->Get_Rot().z;

		D3DXMatrixRotationX(&matRotX, target_rotX);
		D3DXMatrixRotationY(&matRotY, target_rotY);
		D3DXMatrixRotationZ(&matRotZ, target_rotZ);
		D3DXMatrixTranslation(&matTrans, target_PosX, target_PosY, target_PosZ);
		matParents = matRotX * matRotY * matRotZ * matTrans;
		_vec3 vAxis = { 0.f,0.f,1.f };
		m_RotAngel += fDeltaTime*2.0f;
		D3DXMatrixRotationAxis(&matAxis, &vAxis, m_RotAngel);

		if (FAILED(m_pTransformCom->Update_Transform3(matAxis, matParents)))
		{
			PRINT_LOG(L"Error", L"Failed to Update_Transform");
			return UPDATE_ERROR;
		}
		_vec3 Scale, Pos;
		D3DXQUATERNION Rot;
		D3DXMATRIX World = m_pTransformCom->Get_WorldMatrix();
		D3DXMatrixDecompose(&Scale, &Rot, &Pos, &World);
		m_pTransformCom->Set_Pos(Pos);
		m_StackDeltatime += fDeltaTime;

		_uint iStopPos = rand() % 5 + 1;

		if (m_StackDeltatime > _float(iStopPos))
		{
			m_iMode = 3;
			m_StackDeltatime = 0.f;
		}
	}

	if (m_iMode == 3)
	{
		m_StackDeltatime += fDeltaTime;
		if (m_StackDeltatime > 1.5f)
		{
			m_StackDeltatime = 0.f;
			m_iMode = 4;
			m_bFixDir = false;
		}
	}

	if (m_iMode == 4)
	{
		if (!m_bFixDir)
		{
			m_vAttackDir = m_vPlayerPos - m_vMyPos;
			m_bFixDir = true;
		}
		_vec3 Real_Pos = (m_vAttackDir*0.08f) + m_vMyPos;
		m_pTransformCom->Set_Pos(Real_Pos);
		m_StackDeltatime += fDeltaTime;
		if (FAILED(m_pTransformCom->Update_Transform()))
		{
			PRINT_LOG(L"Error", L"Failed to Update_Transform");
			return UPDATE_ERROR;
		}
		if (fDistan > 9.5f && m_StackDeltatime > 1.7f)
		{
			m_iMode = 0;
			m_StackDeltatime = 0.f;
		}
	}

	return NO_EVENT;
}

_uint CBlackMonster::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement*	pManagement = CManagement::Get_Instance();

	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}


	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::UI, this)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Player in Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CBlackMonster::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Player matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, 0)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture ITem");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed to Render_VIBuffer ITem");
		return E_FAIL;
	}

	return S_OK;
}

CBlackMonster * CBlackMonster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBlackMonster*	pInstance = new CBlackMonster(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlackMonster::Clone(void * pArg)
{
	CBlackMonster*	pClone = new CBlackMonster(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CBlackMonster::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

void CBlackMonster::Set_Camera()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pCamera = dynamic_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());

	if (!m_pCamera)
	{
		PRINT_LOG(L"Error", L"m_pCamera is nullptr");
	}
}