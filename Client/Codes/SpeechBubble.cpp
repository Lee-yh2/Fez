#include "stdafx.h"
#include "SpeechBubble.h"
#include "Camera.h"
#include "Texture.h"
#include "Player.h"
#include "npc.h"
CSpeechBubble::CSpeechBubble(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr)
{
	ZeroMemory(&m_pTextureCom, sizeof(m_pTextureCom));
}

CSpeechBubble::CSpeechBubble(const CSpeechBubble & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom)
{
	memcpy(m_pTextureCom, other.m_pTextureCom, sizeof(m_pTextureCom));
}


HRESULT CSpeechBubble::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CSpeechBubble::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_Texture_SpeechBubble", L"Com_Bubble", (CComponent**)&m_pTextureCom[0])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	m_pTransformCom->Set_Scale(_vec3(13.f, 2.6f, 1.f));
	
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = 5.f;
	return S_OK;
}

_uint CSpeechBubble::Update_GameObject(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	//CNPC* pNPC = dynamic_cast<CNPC*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_NPC")->front());
	CPlayer* pNPC  = dynamic_cast<CPlayer*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::Static, L"Layer_Player")->front());
	_float a = pNPC->Return_Transform()->Get_Pos().x;
	_float b = pNPC->Return_Transform()->Get_Pos().y;
	_float c = pNPC->Return_Transform()->Get_Pos().z;
	m_pTransformCom->Set_Pos({ a+2.5f,b+3.5f,c-20.f });

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed to Update_Transform");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

_uint CSpeechBubble::Late_Update_GameObject(_float fDeltaTime)
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
	FrameMove();
	return NO_EVENT;
}

HRESULT CSpeechBubble::Render_GameObject()
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

CSpeechBubble * CSpeechBubble::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpeechBubble*	pInstance = new CSpeechBubble(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpeechBubble::Clone(void * pArg)
{
	CSpeechBubble*	pClone = new CSpeechBubble(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CSpeechBubble::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (auto _Dst : m_pTextureCom)
		Safe_Release(_Dst);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CSpeechBubble::Movement(_float fDeltaTime)
{

	return S_OK;
}

void CSpeechBubble::FrameMove()
{
	if (m_tKeyManager.KeyDown(KEY_RETURN_CLIENT))
		m_tFrame.fFrameStart += 1.f;
	m_tKeyManager.KeyCheck();
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_Blind = false;
		CManagement* pManagement = CManagement::Get_Instance();
		CNPC* pNPC = dynamic_cast<CNPC*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_NPC")->front());
		pNPC->Set_Blind(false);
		m_tFrame.fFrameStart = 0.f;
	}
}