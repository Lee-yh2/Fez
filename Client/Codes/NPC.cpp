#include "stdafx.h"
#include "NPC.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"
#include "SpeechBubble.h"
CNPC::CNPC(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CNPC::CNPC(const CNPC & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom)
	, m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CNPC::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CNPC::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_NPC", L"Com_NPC", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	m_pTransformCom->Set_Scale(_vec3(2.f, 2.f, 1.f));
	m_Blind = true;
	return S_OK;
}

_uint CNPC::Update_GameObject(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (GetAsyncKeyState('I') & 0x8000)
	{
		m_Tuto = 2;
		m_StartUp = true;
	}

	if (m_StartUp) //&& m_StackDeltaTime < 10.f)
		m_StackDeltaTime += fDeltaTime;

	static _bool Check = false;
	CGameObject::Update_GameObject(fDeltaTime);

	if (m_Tuto == 2 && m_StackDeltaTime < 3.8f)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());

		m_pTransformCom->Set_Pos({ m_Onecheck, m_Twocheck, 0.f });
		m_pTransformCom->RotateZ(m_StackDeltaTime*2.f);
		//m_pTransformCom->Set_RotY(fDeltaTime);
		_vec4x4 matworld, matAxis;
		matworld = pPlayer->Return_Transform()->Get_WorldMatrix();
		m_RotAngle -= fDeltaTime * 1.f;
		m_Onecheck -= fDeltaTime * 3.f;
		m_Twocheck -= fDeltaTime * 3.f;
		_vec3 Axis = _vec3(0.f, 0.f, 1.f);
		D3DXMatrixRotationAxis(&matAxis, &Axis, m_RotAngle);
		if (FAILED(m_pTransformCom->Update_Transform3(matAxis, matworld)))
		{
			PRINT_LOG(L"Error", L"Failed to Update_Transform");
			return UPDATE_ERROR;
		}
	}
	else if(m_StackDeltaTime >= 3.8)
	{	//여기에 이제 말풍선을 랜더하는 코드를 치면됨. 스위치 온시키는 코드
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
		m_pTransformCom->RotateZ(m_StackDeltaTime);
		_vec4x4 matworld;
		matworld = pPlayer->Return_Transform()->Get_WorldMatrix();
		if (!Check)
		{
			_float a = m_pTransformCom->Get_Pos().x;
			_float b = m_pTransformCom->Get_Pos().y;
			_float c = m_pTransformCom->Get_Pos().z;
			m_pTransformCom->Set_Pos({ -a,b,c });
			Check = true;
		}
		if (FAILED(m_pTransformCom->Update_Transform2(matworld)))
		{
			PRINT_LOG(L"Error", L"Failed to Update_Transform");
			return UPDATE_ERROR;
		}
	}
	if (m_StackDeltaTime > 4.5f && m_StackDeltaTime < 4.75f)
	{
		CSpeechBubble* pBubble = dynamic_cast<CSpeechBubble*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_SpeechBubble")->front());
		pBubble->Set_Blind(true);
	}
	return NO_EVENT;
}

_uint CNPC::Late_Update_GameObject(_float fDeltaTime)
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

	return NO_EVENT;
}

HRESULT CNPC::Render_GameObject()
{
	CGameObject::Render_GameObject();

	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld)))
	{
		PRINT_LOG(L"Error", L"Failed to SetTransform Player matWorld");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture(0, 0)))
	{
		PRINT_LOG(L"Error", L"Failed to Set_Texture VirtualCube");
		return E_FAIL;
	}

	if (m_Blind)
	{
		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		{
			PRINT_LOG(L"Error", L"Failed to Render_VIBuffer Player");
			return E_FAIL;
		}
	}

	return S_OK;
}

CNPC * CNPC::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CNPC*	pInstance = new CNPC(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNPC::Clone(void * pArg)
{
	CNPC*	pClone = new CNPC(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CNPC::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CNPC::Movement(_float fDeltaTime)
{

	return S_OK;
}