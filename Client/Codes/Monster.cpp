#include "stdafx.h"
#include "Monster.h"
#include "Camera.h"


CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)/*, m_pTerrainVIBufferCom(nullptr)*/
	, m_pPlayTextureCom(nullptr), m_pDevTextureCom(nullptr), m_pTransformCom(nullptr), m_bDevMode(false), m_bStart(false)
	, m_fStartTime(0.f), m_fTotalTime(0.f)
{
	ZeroMemory(&m_vStartDir, sizeof(_vec3));
}

CMonster::CMonster(const CMonster & other)
	: CGameObject(other.m_pDevice), m_pVIBufferCom(other.m_pVIBufferCom)/*, m_pTerrainVIBufferCom(other.m_pTerrainVIBufferCom)*/
	, m_pPlayTextureCom(other.m_pPlayTextureCom), m_pDevTextureCom(other.m_pDevTextureCom), m_pTransformCom(other.m_pTransformCom)
	, m_bDevMode(other.m_bDevMode), m_bStart(other.m_bStart), m_vStartDir(other.m_vStartDir)
	, m_fStartTime(other.m_fStartTime), m_fTotalTime(other.m_fTotalTime)
{
}

HRESULT CMonster::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CMonster::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	/*m_pTerrainVIBufferCom = static_cast<CVIBuffer*>(pManagement->Get_Component(ePrototypeType::NoneStatic, L"Layer_Terrain", L"Com_VIBuffer"));
	if (!m_pTerrainVIBufferCom)
	{
	PRINT_LOG(L"Error", L"m_pTerrainVIBufferCom is nullptr");
	return E_FAIL;
	}

	Safe_AddRef(m_pTerrainVIBufferCom);*/

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Tile_PlayTile", L"Com_Texture", (CComponent**)&m_pPlayTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Tile_DevTile", L"Com_Texture2", (CComponent**)&m_pDevTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	/*if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}*/

	TILEINFO*	pTile = (TILEINFO*)pArg;
	m_tTileInfo = *pTile;

	TRANSFORM_DESC	tDesc;
	tDesc.vScale = m_tTileInfo.vSize;
	tDesc.vRot = m_tTileInfo.vRot;
	tDesc.vPos = m_tTileInfo.vPos;
	tDesc.vLook = { 0.f, 0.f, 1.f };
	tDesc.fMoveSpdPerSec = 5.f;
	tDesc.fRotSpdPerSec = D3DXToRadian(135.f);
	tDesc.iRendererID = 0;

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tDesc)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	return S_OK;
}

_uint CMonster::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);
	
	if (!m_bIsTurning)
	{
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
	}

	return NO_EVENT;
}

_uint CMonster::Late_Update_GameObject(_float fDeltaTime)
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

HRESULT CMonster::Render_GameObject()
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

	if (!m_bDevMode)
	{
		if (FAILED(m_pPlayTextureCom->Set_Texture(0, m_tTileInfo.byPlayDrawID)))
		{
			PRINT_LOG(L"Error", L"Failed To Set_Texture ToolTile");
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(m_pDevTextureCom->Set_Texture(0, m_tTileInfo.byDevDrawID)))
		{
			PRINT_LOG(L"Error", L"Failed To Set_Texture ToolTile");
			return E_FAIL;
		}
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Monster");
		return E_FAIL;
	}

	return S_OK;
}

CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonster*	pInstance = new CMonster(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster::Clone(void * pArg)
{
	CMonster*	pClone = new CMonster(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMonster");
		Safe_Release(pClone);
	}

	return pClone;
}

void CMonster::Free()
{
	Safe_Release(m_pVIBufferCom);
	//Safe_Release(m_pTerrainVIBufferCom);
	Safe_Release(m_pPlayTextureCom);
	Safe_Release(m_pDevTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

void CMonster::Set_StartPos(const _vec3 & vStartPos, const _float & fTotalTime)
{
	TRANSFORM_DESC	tDesc = m_pTransformCom->Get_Desc();

	m_vStartDir = vStartPos - tDesc.vPos;
	m_vStartDir.y /= 4.f;
	D3DXVec3Normalize(&m_vStartDir, &m_vStartDir);
	tDesc.vPos += -m_vStartDir * fTotalTime * fTotalTime * 2.f;
	m_pTransformCom->Set_Desc(tDesc);

	m_fTotalTime = 0.f;
	m_fStartTime = fTotalTime;
	m_bStart = true;
}

HRESULT CMonster::Movement(_float fDeltaTime)
{
	if (m_bStart)
	{
		TRANSFORM_DESC	tDesc = m_pTransformCom->Get_Desc();

		tDesc.vPos += m_vStartDir * fDeltaTime * m_fStartTime * 2.f;
		m_pTransformCom->Set_Desc(tDesc);
		m_fTotalTime += fDeltaTime;

		if (m_fTotalTime >= m_fStartTime)
		{
			tDesc.vPos.x = m_tTileInfo.vPos.x;
			tDesc.vPos.y = m_tTileInfo.vPos.y;
			tDesc.vPos.z = m_tTileInfo.vPos.z;
			m_pTransformCom->Set_Desc(tDesc);
			m_bStart = false;
		}
	}

	return S_OK;
}
