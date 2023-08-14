#include "stdafx.h"
#include "InterBox.h"
#include "Camera.h"
#include "Player.h"

CInterBox::CInterBox(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)/*, m_pTerrainVIBufferCom(nullptr)*/
	, m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_bInteraction(false), m_pTargetTransformCom(nullptr)
	, m_pCamera(nullptr), m_fPosYOnTile(false), m_fTargetPosY(0.f), m_bApproachToTarget(false), m_fFallPos(0.f), m_bIsThrowing(false)
	, m_iThrowDir(0), m_pInterObjList(nullptr), m_bPlayerUsing(false)
{
	ZeroMemory(&m_tJump, sizeof(m_tJump));
}

CInterBox::CInterBox(const CInterBox & other)
	: CGameObject(other.m_pDevice), m_pVIBufferCom(other.m_pVIBufferCom)/*, m_pTerrainVIBufferCom(other.m_pTerrainVIBufferCom)*/
	, m_pTextureCom(other.m_pTextureCom), m_pTransformCom(other.m_pTransformCom), m_bInteraction(other.m_bInteraction), m_pTargetTransformCom(other.m_pTargetTransformCom)
	, m_pCamera(other.m_pCamera), m_fPosYOnTile(other.m_fPosYOnTile), m_fTargetPosY(other.m_fTargetPosY), m_bApproachToTarget(other.m_bApproachToTarget)
	, m_tJump(other.m_tJump), m_bIsThrowing(other.m_bIsThrowing), m_iThrowDir(other.m_iThrowDir), m_eMapDir(other.m_eMapDir), m_fFallPos(other.m_fFallPos)
	, m_pInterObjList(other.m_pInterObjList), m_bPlayerUsing(other.m_bPlayerUsing)
{

}

HRESULT CInterBox::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CInterBox::Ready_GameObject(void * pArg)
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

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Tile_PlayTile", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

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

	Set_Camera();

	m_fFallPos = m_pTransformCom->Get_Desc().vPos.y;

	return S_OK;
}

_uint CInterBox::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (!m_bIsTurning)
	{
		if (FAILED(Movement(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed To Movement InterBox");
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

_uint CInterBox::Late_Update_GameObject(_float fDeltaTime)
{
	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::NoneAlpha, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Monster In Renderer");
		return UPDATE_ERROR;
	}

	return NO_EVENT;
}

HRESULT CInterBox::Render_GameObject()
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

	if (FAILED(m_pTextureCom->Set_Texture(0, m_tTileInfo.byPlayDrawID)))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Texture Monster");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	{
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Monster");
		return E_FAIL;
	}

	return S_OK;
}

CInterBox * CInterBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CInterBox*	pInstance = new CInterBox(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInterBox::Clone(void * pArg)
{
	CInterBox*	pClone = new CInterBox(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMonster");
		Safe_Release(pClone);
	}

	return pClone;
}

void CInterBox::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

void CInterBox::Set_Camera()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pCamera = dynamic_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());

	if (!m_pCamera)
	{
		PRINT_LOG(L"Error", L"m_pCamera is nullptr");
	}
}

void CInterBox::Set_Throwing(eMapDir _eMapDir, _int _iThrowDir)
{
	m_eMapDir = _eMapDir;
	m_iThrowDir = _iThrowDir;
	m_bIsThrowing = true;
}

void CInterBox::Begin_Throw(_float _fDeltaTime)
{
	if (m_iThrowDir == RIGHT_ROT)
	{
		switch (m_eMapDir)
		{
		case eMapDir::EAST: m_pTransformCom->Go_Straight(_fDeltaTime); break;
		case eMapDir::WEST: m_pTransformCom->Go_Straight(-_fDeltaTime); break;
		case eMapDir::SOUTH: m_pTransformCom->Go_Sideway(_fDeltaTime); break;
		case eMapDir::NORTH: m_pTransformCom->Go_Sideway(-_fDeltaTime); break;
		default: break;
		}
	}
	else
	{
		switch (m_eMapDir)
		{
		case eMapDir::EAST: m_pTransformCom->Go_Straight(-_fDeltaTime); break;
		case eMapDir::WEST: m_pTransformCom->Go_Straight(_fDeltaTime); break;
		case eMapDir::SOUTH: m_pTransformCom->Go_Sideway(-_fDeltaTime);  break;
		case eMapDir::NORTH: m_pTransformCom->Go_Sideway(_fDeltaTime); break;
		default: break;
		}
	}
}

HRESULT CInterBox::Movement(_float fDeltaTime)
{
	if (m_pTargetTransformCom) // 플레이어가 상자를 잡았을 때 타겟이 설정이됨
	{
		_vec3 vTargetPos = m_pTargetTransformCom->Get_Desc().vPos;
		m_fTargetPosY = vTargetPos.y + 0.5f;
		if (m_bApproachToTarget)
		{
			vTargetPos.y += 0.5f;
			m_pTransformCom->Set_Pos(vTargetPos);
			m_fFallPos = m_pTransformCom->Get_Desc().vPos.y;
		}
		else
		{
			_vec3 vCurBoxPos = m_pTransformCom->Get_Desc().vPos;

			if (vCurBoxPos.y >= m_fTargetPosY) {
				m_bApproachToTarget = true;
			}
			else
			{
				vCurBoxPos.y += 0.03f;
				m_pTransformCom->Set_Pos(vCurBoxPos);
			}
		}

		m_bPlayerUsing = true;
	}
	else // 플레이어가 상자를 잡지 않았을 때
	{
		_vec3 vCollisionTerrainPos = {0.f, 0.f, 0.f};
		_vec3 vCollisionInterObjPos = { 0.f, 0.f, 0.f };
		m_bApproachToTarget = false;

		if (m_bPlayerUsing && !Check_CollisionTerrain(&vCollisionTerrainPos, m_pTransformCom->Get_Desc().vPos) 
			&& !Check_CollisionInterobj(&vCollisionTerrainPos, m_pTransformCom->Get_Desc().vPos))
		{
			m_tJump.fJumpTime += 1.55f * 0.01f;
			float fTempY = m_fFallPos - (4.8f * m_tJump.fJumpTime * m_tJump.fJumpTime);
			m_pTransformCom->Set_PosY(fTempY);
			if (m_bIsThrowing) {
				Begin_Throw(0.01f);
			}
		}
		else
		{
			// 여기에 타일과 만났을 때의 코드를 추가해주면 됨.
			m_fFallPos = m_pTransformCom->Get_Desc().vPos.y;
			m_tJump.fJumpTime = 0.f;
			m_bIsThrowing = false;
		}
	}

	m_tTileInfo.vSize = m_pTransformCom->Get_Desc().vScale;
	m_tTileInfo.vRot = m_pTransformCom->Get_Desc().vRot;
	m_tTileInfo.vPos = m_pTransformCom->Get_Desc().vPos;

	return S_OK;
}

_bool CInterBox::Check_Rect(_vec3 _Dst, _vec3 _Src, _float * _x, _float * _y)
{

	float	fCX = abs(_Dst.x - _Src.x);
	float	fCY = abs(_Dst.y - _Src.y);

	float	fDisX = (float)((PLAYERCX + TILECX) / 2);
	float	fDisY = (float)((PLAYERCY + TILECY) / 2);

	if (fCX <= fDisX && fCY <= fDisY)
	{
		*_x = fDisX - fCX;
		*_y = fDisY - fCY;
		return true;
	}

	return false;
}

_bool CInterBox::Check_CollisionTerrain(_vec3 * _vCollisionPos, _vec3 _vPlayerPos)
{
	if (m_pCollisionTerrainList == nullptr)
		return false;

	for each (CGameObject* pCollisionTerrain in *m_pCollisionTerrainList)
	{
		CTransform* pTrans = dynamic_cast<CTransform*>(pCollisionTerrain->Get_Component(L"Com_Transform"));
		_vec3 vTerrainPosFromViewport = { 0.f,0.f, 0.f };
		_vec3 vPlayerPosFromViewport = { 0.f, 0.f, 0.f };
		_vec3 vTerrainPosFromWorld = pTrans->Get_Desc().vPos;
		_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;

		m_pCamera->WorldToScreen(&vTerrainPosFromWorld, &vTerrainPosFromViewport);
		m_pCamera->WorldToScreen(&vPlayerPosFromWorld, &vPlayerPosFromViewport);

		if (Check_Rect(vPlayerPosFromViewport, vTerrainPosFromViewport, &_vCollisionPos->x, &_vCollisionPos->y)) {
			m_pTransformCom->Set_PosYPlus((_vCollisionPos->y / CLIENTWINCY));
			m_fPosYOnTile = vTerrainPosFromWorld.y;

			return true;
		}
	}
	return false;
}

_bool CInterBox::Check_CollisionInterobj(_vec3 * _vCollisionPos, _vec3 _vPlayerPos)
{
	if (m_pInterObjList == nullptr)
		return false;

	for each (CGameObject* pInterObj in *m_pInterObjList)
	{
		CTransform* pTrans = dynamic_cast<CTransform*>(pInterObj->Get_Component(L"Com_Transform"));

		_vec3 vTerrainPosFromViewport = { 0.f,0.f, 0.f };
		_vec3 vPlayerPosFromViewport = { 0.f, 0.f, 0.f };
		_vec3 vTerrainPosFromWorld = pTrans->Get_Desc().vPos;
		_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;

		m_pCamera->WorldToScreen(&vTerrainPosFromWorld, &vTerrainPosFromViewport);
		m_pCamera->WorldToScreen(&vPlayerPosFromWorld, &vPlayerPosFromViewport);

		if (Check_Rect(vPlayerPosFromViewport, vTerrainPosFromViewport, &_vCollisionPos->x, &_vCollisionPos->y)) {

			_float	fPlusY = _vCollisionPos->y / CLIENTWINCY;

			if (m_pTransformCom->Get_Desc().vPos.y + fPlusY > vTerrainPosFromWorld.y + 0.95f)
			{
				fPlusY = vTerrainPosFromWorld.y + 0.95f - m_pTransformCom->Get_Desc().vPos.y;
				m_bPlayerUsing = false;
			}

			m_pTransformCom->Set_PosYPlus(fPlusY);
			
			m_fPosYOnTile = vTerrainPosFromWorld.y;
			if (this != pInterObj) {
				return true;
			}
		}
	}
	return false;
}
