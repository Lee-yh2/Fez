#include "stdafx.h"
#include "Player.h"
#include "Camera.h"
#include "Texture.h"
#include "InterBox.h"
#include "LoadingScene.h"
#include "Item.h"
#include "VirtualCube.h"
#include "Portal.h"
#include "Monster.h"
#include "Effect.h"
#include "UIcubepiece.h"
#include "SoundMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr)
	, m_pTransformCom(nullptr), m_bCameraMode(false), m_bIsMoving(false), m_vGoalPos(_vec3(0.f, 0.f, 0.f))
	, m_bIsRotate(false), m_fRotateAngle(0.f), m_fOldRotate(0.f), m_iRotDir(RIGHT_ROT), m_iNumOfMonsters(0)
	, m_bCameraDisCalculateByZ(true), m_iDirNum(4), m_eMapDir(eMapDir::SOUTH), m_bPlayerOnTerrain(false)
	, m_bJump(false), m_pInterObjList(nullptr), m_pVineTerrainList(nullptr), m_pCollisionTerrainList(nullptr)
	, m_fPosYOnTile(0.f), m_bGrabSomething(false), m_pCamera(nullptr), m_ePreState(NUMOFSTATE), m_eCurState(BEGINFALL)
	, m_vFallPos(_vec3(0.f, 0.f, 0.f)), m_iCurDir(RIGHT_ROT), m_bIsFall(false), m_pGrabbedInterObj(nullptr), m_bTryFindVine(false), m_bIsVineNearPlayer(false)
	, m_fCurTopOnTileY(0.f), m_bJumpOnTopFalling(false), m_bCanGrab(false), m_bGraping(false), m_bLiftingBox(false), m_vTempPos(_vec3(0.f, 0.f, 0.f))
	, m_fColDis(0.f), m_fCurRotate(0.f), m_fSetCurRotate(0.f), m_vTargetCollisionPos(_vec3(0.f, 0.f, 0.f)), m_vTargetCollisionOldPos(_vec3(0.f, 0.f, 0.f))
	, m_pAllTerrainVector(nullptr), m_vFrontPos(_vec3(0.f, 0.f, 0.f)), m_bCanGoThrough(true), m_bFrontTileCollisionOk(false), m_bSetPositionOrNot(true)
	, m_bIsAlpha(false), m_pCollisionTerrain(nullptr), m_bPlayerOnBonusStage(false), m_bBonusStageClear(false), m_bOnNewBonusStage(false), m_bNewBonusStageClear(false)
{
	ZeroMemory(&m_tJump, sizeof(m_tJump));
	ZeroMemory(&m_pTextureCom, sizeof(m_pTextureCom));
	m_tFrame.fFrameStart = 0.f;
	m_tFrame.fFrameEnd = 7.f;
}

CPlayer::CPlayer(const CPlayer & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom)
	, m_pTransformCom(other.m_pTransformCom), m_bCameraMode(other.m_bCameraMode), m_tJump(other.m_tJump),
	m_bIsMoving(other.m_bIsMoving), m_vGoalPos(other.m_vGoalPos), m_bIsRotate(other.m_bIsRotate), m_fRotateAngle(other.m_fRotateAngle)
	, m_fOldRotate(0.f), m_iRotDir(other.m_iRotDir), m_iNumOfMonsters(other.m_iNumOfMonsters), m_bCameraDisCalculateByZ(other.m_bCameraDisCalculateByZ)
	, m_iDirNum(other.m_iDirNum), m_eMapDir(other.m_eMapDir), m_bPlayerOnTerrain(other.m_bPlayerOnTerrain), m_tFrame(other.m_tFrame), m_bJump(other.m_bJump)
	, m_pCollisionTerrainList(other.m_pCollisionTerrainList), m_pInterObjList(other.m_pInterObjList), m_pVineTerrainList(other.m_pVineTerrainList)
	, m_fPosYOnTile(other.m_fPosYOnTile), m_bGrabSomething(other.m_bGrabSomething), m_pCamera(other.m_pCamera)
	, m_vFallPos(other.m_vFallPos), m_iCurDir(other.m_iCurDir), m_bIsFall(other.m_bIsFall), m_pGrabbedInterObj(other.m_pGrabbedInterObj), m_bTryFindVine(other.m_bTryFindVine)
	, m_bIsVineNearPlayer(true), m_NearVineTerrainList(other.m_NearVineTerrainList), m_fCurTopOnTileY(other.m_fCurTopOnTileY), m_bJumpOnTopFalling(other.m_bJumpOnTopFalling)
	, m_bCanGrab(other.m_bCanGrab), m_bGraping(other.m_bGraping), m_bLiftingBox(other.m_bLiftingBox), m_vTempPos(other.m_vTempPos), m_fColDis(other.m_fColDis)
	, m_fCurRotate(other.m_fCurRotate), m_fSetCurRotate(other.m_fSetCurRotate)
	, m_vTargetCollisionPos(other.m_vTargetCollisionPos), m_vTargetCollisionOldPos(other.m_vTargetCollisionOldPos)
	, m_pAllTerrainVector(other.m_pAllTerrainVector), m_vFrontPos(other.m_vFrontPos), m_bCanGoThrough(other.m_bCanGoThrough), m_bFrontTileCollisionOk(other.m_bFrontTileCollisionOk)
	, m_bSetPositionOrNot(other.m_bSetPositionOrNot), m_bIsAlpha(other.m_bIsAlpha), m_pCollisionTerrain(other.m_pCollisionTerrain), m_bPlayerOnBonusStage(other.m_bPlayerOnBonusStage)
	, m_bBonusStageClear(other.m_bBonusStageClear), m_bOnNewBonusStage(other.m_bOnNewBonusStage), m_bNewBonusStageClear(other.m_bNewBonusStageClear)
{
	memcpy(m_pTextureCom, other.m_pTextureCom, sizeof(m_pTextureCom));
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CPlayer::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	//// 그랩처리 할 오브젝트 받아오기
	//m_pInterObjList = pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_InterObj");


	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_Idle", L"Com_Player_Idle", (CComponent**)&m_pTextureCom[0])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_Walk", L"Com_Player_Walk", (CComponent**)&m_pTextureCom[1])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_Jump", L"Com_Player_Jump", (CComponent**)&m_pTextureCom[2])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_BeginFall", L"Com_Player_BeginFall", (CComponent**)&m_pTextureCom[3])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_EndFall", L"Com_Player_EndFall", (CComponent**)&m_pTextureCom[4])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_FallDie", L"Com_Player_FallDie", (CComponent**)&m_pTextureCom[5])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_Climb", L"Com_Player_Climb", (CComponent**)&m_pTextureCom[6])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_GetIn", L"Com_Player_GetIn", (CComponent**)&m_pTextureCom[7])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_GetOut", L"Com_Player_GetOut", (CComponent**)&m_pTextureCom[8])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_GrapIdle", L"Com_Player_GrapIdle", (CComponent**)&m_pTextureCom[9])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_GrapMove", L"Com_Player_GrapMove", (CComponent**)&m_pTextureCom[10])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_GrapUp", L"Com_Player_GrapUp", (CComponent**)&m_pTextureCom[11])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_LiftBox", L"Com_Player_LiftBox", (CComponent**)&m_pTextureCom[12])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_LiftBoxMove", L"Com_Player_LiftBoxMove", (CComponent**)&m_pTextureCom[13])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_PutDownBox", L"Com_Player_PutDownBox", (CComponent**)&m_pTextureCom[14])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}
	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Texture_Player_LiftBoxIdle", L"Com_Player_LiftBoxIdle", (CComponent**)&m_pTextureCom[15])))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}

	m_vFallPos = m_pTransformCom->Get_Desc().vPos;


	return S_OK;
}

_uint CPlayer::Update_GameObject(_float fDeltaTime)
{
	if (m_bChangeScene)
		return NO_EVENT;

	CGameObject::Update_GameObject(fDeltaTime);

	if (!m_bOnNewBonusStage)
	{
		Find_TerrainForCollision();
		Check_CollisionGrapList();
		Check_Item(fDeltaTime);

		if (FAILED(Movement(fDeltaTime)))
		{
			PRINT_LOG(L"Error", L"Failed to Movement Player");
			return UPDATE_ERROR;
		}

		if (FAILED(m_pTransformCom->Update_Transform()))
		{
			PRINT_LOG(L"Error", L"Failed to Update_Transform");
			return UPDATE_ERROR;
		}

		if (!m_bIsTurning)
		{

			if (!m_sbCameraMode)
			{
				if (FALLDIE != m_eCurState && !m_bChangeScene && GETIN != m_eCurState && GETOUT != m_eCurState
					&& GRAPUP != m_eCurState && LIFTBOX != m_eCurState && PUTDOWNBOX != m_eCurState)
					Key_Check(fDeltaTime);
			}
			if (m_eCurState != CLIMB && GRAPIDLE != m_eCurState && GRAPMOVE != m_eCurState)
			{
				if (JUMP == m_eCurState)
					FrameMove(fDeltaTime, 1.5f);
				else if (BEGINFALL == m_eCurState || ENDFALL == m_eCurState)
					FrameMove(fDeltaTime, 2.2f);
				else if (GRAPUP == m_eCurState)
					FrameMove(fDeltaTime, 1.5f);
				else if (LIFTBOX == m_eCurState)
					FrameMove(fDeltaTime, 1.5f);
				else
					FrameMove(fDeltaTime, 0.9f);
			}
			Scene_Change();



			m_tKeyManager.KeyCheck();
		}
	}
	return NO_EVENT;
}

_uint CPlayer::Late_Update_GameObject(_float fDeltaTime)
{

	if (m_bChangeScene)
		return NO_EVENT;

	CGameObject::Late_Update_GameObject(fDeltaTime);
	if (!m_bOnNewBonusStage)
	{
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

		_float fTempA = floor(m_fTemp * 100) / 100;

		if (fTempA && fTempA < m_pTransformCom->Get_Desc().vPos.y)
		{
			m_bJumpOnTopFalling = true;
		}

		// _vec3 vPlayerPos = m_pTransformCom->Get_Pos();

		// cout << "Player Pos   x:" << vPlayerPos.x << " y:" << vPlayerPos.y << " z:" << vPlayerPos.z << endl;
	}
	return NO_EVENT;
}

HRESULT CPlayer::Render_GameObject()
{
	if (m_bChangeScene)
		return NO_EVENT;
	if (!m_bIsAlpha)
	{
		CGameObject::Render_GameObject();
		if (!m_bOnNewBonusStage)
		{
			if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
			{
				PRINT_LOG(L"Error", L"Failed to Render Player_Cullmode");
				return E_FAIL;
			}

			if (FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
			{
				PRINT_LOG(L"Error", L"Failed to Render Player_Fillmode");
				return E_FAIL;
			}

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

			if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			{
				PRINT_LOG(L"Error", L"Failed to Render_VIBuffer Player");
				return E_FAIL;
			}
		}
	}
	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer*	pInstance = new CPlayer(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed to Create Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pClone = new CPlayer(*this);
	if (FAILED(pClone->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Clone Player");
		Safe_Release(pClone);
	}

	return pClone;
}

void CPlayer::Free()
{
	Safe_Release(m_pVIBufferCom);
	for (auto _Dst : m_pTextureCom)
		Safe_Release(_Dst);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

void CPlayer::Set_Camera()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pCamera = dynamic_cast<CCamera*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Camera")->front());

	if (!m_pCamera)
	{
		PRINT_LOG(L"Error", L"m_pCamera is nullptr");
	}
}

_bool CPlayer::Check_CollisionTerrain(_vec3* _vCollisionPos, _vec3 _vPlayerPos)
{
	if (m_pCollisionTerrainList == nullptr)
		return false;

	/*if (!m_bFrontTileCollisionOk)
	return false;*/

	_vec3 vTerrainPosFromViewport = { 0.f,0.f, 0.f };
	_vec3 vPlayerPosFromViewport = { 0.f, 0.f, 0.f };
	_vec3 vTerrainPosFromViewport2 = { 0.f,0.f, 0.f };

	_vec3 vTerrainPosFromWorld = m_vTargetCollisionPos;
	_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;
	_vec3 vTerrainPosFromWorld2 = { m_vTargetCollisionPos.x,  m_vTargetCollisionPos.y + 1.f, m_vTargetCollisionPos.z };

	m_pCamera->WorldToScreen(&vTerrainPosFromWorld2, &vTerrainPosFromViewport2);
	m_pCamera->WorldToScreen(&vTerrainPosFromWorld, &vTerrainPosFromViewport);
	m_pCamera->WorldToScreen(&vPlayerPosFromWorld, &vPlayerPosFromViewport);

	if (Check_Rect(vPlayerPosFromViewport, vTerrainPosFromViewport, &_vCollisionPos->x, &_vCollisionPos->y)) {
		if (vTerrainPosFromWorld.y < m_fCurTopOnTileY && !m_bOnVine)
		{
			m_pTransformCom->Set_PosY(vPlayerPosFromWorld.y + (_vCollisionPos->y / CLIENTWINCY));
			// m_pTransformCom->Set_PosY(vPlayerPosFromWorld.y);
			m_vOnTilePosFromViewport = vTerrainPosFromViewport;
			m_vOnTilePosFromWorld = vTerrainPosFromWorld;
			m_fPosYOnTile = vTerrainPosFromWorld.y;
			if (m_bPlayerOnBonusStage)
				static_cast<CMonster*>(m_pCollisionTerrain)->Change_TileByPlayDrawID(80); // 찬수 추가분
			// m_vTempPos = vTerrainPosFromWorld;
			//m_fCurTopOnTileY = vTerrainPosFromViewport.y - 5.f;
			return true;
		}
	}

	return false;
}

_bool CPlayer::Check_VineTerrain(_vec3 * _vCollisionPos, _vec3 _vPlayerPos)
{
	CTransform* pTransformCom;

	if (m_pVineTerrainList == nullptr)
		return false;

	for each (CGameObject* pVineTerrain in *m_pVineTerrainList)
	{
		pTransformCom = dynamic_cast<CTransform*>(pVineTerrain->Get_Component(L"Com_Transform"));
		if (pTransformCom->Get_Desc().iVineDir == (_uint)m_eMapDir)
		{
			_vec3 vTerrainPosFromViewport = { 0.f,0.f, 0.f };
			_vec3 vPlayerPosFromViewport = { 0.f, 0.f, 0.f };
			_vec3 vTerrainPosFromWorld = pTransformCom->Get_Desc().vPos;
			_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;

			m_pCamera->WorldToScreen(&vTerrainPosFromWorld, &vTerrainPosFromViewport);
			m_pCamera->WorldToScreen(&vPlayerPosFromWorld, &vPlayerPosFromViewport);

			if (Check_Rect(vPlayerPosFromViewport, vTerrainPosFromViewport, &_vCollisionPos->x, &_vCollisionPos->y)) {
				switch (m_eMapDir)
				{	// 0.6은 임의의값, 해줘야 앞으로 붙음
				case eMapDir::EAST:	vPlayerPosFromWorld.x = vTerrainPosFromWorld.x += 0.1f; break;
				case eMapDir::WEST:	vPlayerPosFromWorld.x = vTerrainPosFromWorld.x -= 0.1f; break;
				case eMapDir::SOUTH: vPlayerPosFromWorld.z = vTerrainPosFromWorld.z -= 0.1f; break;
				case eMapDir::NORTH: vPlayerPosFromWorld.z = vTerrainPosFromWorld.z += 0.1f; break;
				}
				m_pTransformCom->Set_Pos(vPlayerPosFromWorld);
				m_bOnVine = true;
				return true;
			}
		}
	}

	m_bOnVine = false;
	return false;
}

_bool CPlayer::Check_NearVineTerrain()
{
	_bool bDidFindForwardTerrain = false;
	_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;
	_float fTempPos = 0.f;

	for each (CGameObject* pVineTerrain in *m_pVineTerrainList)
	{
		CTransform* pTrans = dynamic_cast<CTransform*>(pVineTerrain->Get_Component(L"Com_Transform"));
		_vec3 vTerrainPosFromWorld = pTrans->Get_Desc().vPos;


		if ((_uint)vPlayerPosFromWorld.y == (_uint)pTrans->Get_Desc().vPos.y) {
			switch (m_eMapDir)
			{
			case eMapDir::EAST:
				if (m_iCurDir == RIGHT_ROT) {
					if (vPlayerPosFromWorld.z < pTrans->Get_Desc().vPos.z) {
						bDidFindForwardTerrain = true;
					}
				}
				else {
					if (vPlayerPosFromWorld.z > pTrans->Get_Desc().vPos.z) {
						bDidFindForwardTerrain = true;
					}
				}
				break;
			case eMapDir::WEST:
				if (m_iCurDir == RIGHT_ROT) {
					if (vPlayerPosFromWorld.z > pTrans->Get_Desc().vPos.z) {
						bDidFindForwardTerrain = true;
					}
				}
				else {
					if (vPlayerPosFromWorld.z < pTrans->Get_Desc().vPos.z) {
						bDidFindForwardTerrain = true;
					}
				}
				break;
			case eMapDir::SOUTH:
				if (m_iCurDir == RIGHT_ROT) {
					if (vPlayerPosFromWorld.x < pTrans->Get_Desc().vPos.x) {
						bDidFindForwardTerrain = true;
					}
				}
				else {
					if (vPlayerPosFromWorld.x > pTrans->Get_Desc().vPos.x) {
						bDidFindForwardTerrain = true;
					}
				}
				break;
			case eMapDir::NORTH:
				if (m_iCurDir == RIGHT_ROT) {
					if (vPlayerPosFromWorld.x > pTrans->Get_Desc().vPos.x) {
						bDidFindForwardTerrain = true;
					}
				}
				else {
					if (vPlayerPosFromWorld.x < pTrans->Get_Desc().vPos.x) {
						bDidFindForwardTerrain = true;
					}
				}
				break;
			default: break;
			}
		}
	}
	return  bDidFindForwardTerrain;
}

_bool CPlayer::Check_InterObj(_vec3 * _vCollisionPos, _vec3 _vPlayerPos)
{
	if (m_pInterObjList == nullptr)
		return false;

	for each (CGameObject* pInterObj in *m_pInterObjList)
	{
		CTransform* pTrans = dynamic_cast<CTransform*>(pInterObj->Get_Component(L"Com_Transform"));
		_vec3 vInterObjPosFromViewport = { 0.f,0.f, 0.f };
		_vec3 vPlayerPosFromViewport = { 0.f, 0.f, 0.f };
		_vec3 vInterObjPosFromWorld = pTrans->Get_Desc().vPos;
		_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;

		m_pCamera->WorldToScreen(&vInterObjPosFromWorld, &vInterObjPosFromViewport);
		m_pCamera->WorldToScreen(&vPlayerPosFromWorld, &vPlayerPosFromViewport);

		if (Check_Rect(vPlayerPosFromViewport, vInterObjPosFromViewport, &_vCollisionPos->x, &_vCollisionPos->y)) {
			// 오브젝트 잡았을 때 처리
			if (!m_bGrabSomething)
			{

				dynamic_cast<CInterBox*>(pInterObj)->Set_Target(m_pTransformCom);
				m_eCurState = LIFTBOX;
				m_tFrame.fFrameStart = 0;
				m_pGrabbedInterObj = pInterObj;
				m_bGrabSomething = true;
			}
			else
			{
				dynamic_cast<CInterBox*>(pInterObj)->SetOff_Target();
				m_bGrabSomething = false;
				m_eCurState = PUTDOWNBOX;
				m_tFrame.fFrameStart = 0;
			}
			return true;
		}
	}
	return false;
}

_bool CPlayer::Check_Portal(_vec3 * _vCollisionPos, _vec3 _vPlayerPos)
{
	if (m_pPortalList == nullptr)
		return false;

	for each (CGameObject* pPortalObj in *m_pPortalList)
	{
		CTransform* pTrans = dynamic_cast<CTransform*>(pPortalObj->Get_Component(L"Com_Transform"));
		if (pTrans->Get_Desc().iVineDir == (_uint)m_eMapDir)
		{
			_vec3 vInterObjPosFromViewport = { 0.f,0.f, 0.f };
			_vec3 vPlayerPosFromViewport = { 0.f, 0.f, 0.f };
			_vec3 vInterObjPosFromWorld = pTrans->Get_Desc().vPos;
			_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;
			if ((_uint)eSceneType::Stage2 == pTrans->Get_Desc().iNum)
				vInterObjPosFromWorld.y -= 2.f;
			m_pCamera->WorldToScreen(&vInterObjPosFromWorld, &vInterObjPosFromViewport);
			m_pCamera->WorldToScreen(&vPlayerPosFromWorld, &vPlayerPosFromViewport);

			if (Check_Rect(vPlayerPosFromViewport, vInterObjPosFromViewport, &_vCollisionPos->x, &_vCollisionPos->y)) {
				// 오브젝트 잡았을 때 처리



				if ((_uint)eSceneType::Stage == pTrans->Get_Desc().iNum)
				{
					m_eNextScene = eSceneType::Stage;
					dynamic_cast<CPortal*>(pPortalObj)->Set_LightOn(true);
				}
				else if ((_uint)eSceneType::Stage2 == pTrans->Get_Desc().iNum)
				{
					m_eNextScene = eSceneType::Stage2;
					dynamic_cast<CPortal*>(pPortalObj)->Set_Open(true);
				}
				else if ((_uint)eSceneType::GomezHouse == pTrans->Get_Desc().iNum)
				{
					m_eNextScene = eSceneType::GomezHouse;
					dynamic_cast<CPortal*>(pPortalObj)->Set_LightOn(true);
				}

				if (true == dynamic_cast<CPortal*>(pPortalObj)->Get_LighOn())
				{
					m_eCurState = GETIN;
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
					CSoundMgr::Get_Instance()->PlaySound(L"Player_Enter.wav", CSoundMgr::PLAYER);
					m_tFrame.fFrameStart = 0;
				}
				return true;
			}
		}
	}
	return false;
}

_bool CPlayer::Check_Item(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	CVirtualCube* pVirtualCube = dynamic_cast<CVirtualCube*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_VirtualCube")->front());
	CEffect* pEffect = dynamic_cast<CEffect*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_Effect")->front());
	if (m_pItemList == nullptr)
		return false;
	_vec3 * _vCollisionPos = new _vec3;

	if (m_CheckCollisionCube)
	{
		m_bTimeGo = true;
		pVirtualCube->ShowCube();
		for each (CItem* pItmeObj in *m_pItemList)
		{
			pItmeObj->ShowCube();
		}
	}

	if (m_CheckCollisionCube)
	{
		m_StackDeltaTIme += _fDeltaTime;
		if (m_StackDeltaTIme > 0.4f)
		{
			m_CubeCount += 1;
			m_StackDeltaTIme = 0.f;
			m_CheckCollisionCube = false;
		}
	}
	if (m_bTimeGo)
		m_WaitTIme += _fDeltaTime;

	if (m_WaitTIme > 4.85)
	{
		if (m_CubeCount == 9)
			pEffect->Set_condition(2);
		else
			pEffect->Set_condition(1);
		m_WaitTIme = 0.f;
		m_bTimeGo = false;
	}

	for each (CItem* pItmeObj in *m_pItemList)
	{
		CTransform* pTrans = dynamic_cast<CTransform*>(pItmeObj->Get_Component(L"Com_Transform"));
		_vec3 vInterObjPosFromViewport = { 0.f,0.f, 0.f };
		_vec3 vPlayerPosFromViewport = { 0.f, 0.f, 0.f };
		_vec3 vInterObjPosFromWorld = pTrans->Get_Desc().vPos;
		_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;

		m_pCamera->WorldToScreen(&vInterObjPosFromWorld, &vInterObjPosFromViewport);
		m_pCamera->WorldToScreen(&vPlayerPosFromWorld, &vPlayerPosFromViewport);

		if (Check_Rect2(vPlayerPosFromViewport, vInterObjPosFromViewport, &_vCollisionPos->x, &_vCollisionPos->y)) {
			CUIcubepiece* pUIcube = dynamic_cast<CUIcubepiece*>(pManagement->Get_GameObjectsByLayer(ePrototypeType::NoneStatic, L"Layer_UIcubepiece")->front());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CUBE);

				if (m_CubeCount == 8)
				{
					pItmeObj->Set_CubeNum(m_CubeCount);
					pVirtualCube->Say_FullCube(true);
					m_CheckCollisionCube = true;
					CSoundMgr::Get_Instance()->PlaySound(L"Player_CubeMax.wav", CSoundMgr::CUBE);
					pUIcube->Set_CubeNumber(-7);
				}
				else
				{
					CSoundMgr::Get_Instance()->PlaySound(L"Player_CubeEat.wav", CSoundMgr::CUBE);
					pItmeObj->Set_CubeNum(m_CubeCount);
					pUIcube->Set_CubeNumber(1);
					m_CheckCollisionCube = true;
				}
			return true;
		}
	}
	return false;
}

_bool CPlayer::Check_Rect(_vec3 _Dst, _vec3 _Src, _float * _x, _float * _y)
{
	float	fCX = abs(_Dst.x - _Src.x);
	float	fCY = abs(_Dst.y - _Src.y);

	float	fDisX = (float)((PLAYERCX + TILECX) / 2);
	float	fDisY = (float)((PLAYERCY + TILECY) / 2);

	if (fCX < fDisX && fCY < fDisY)
	{
		*_x = fDisX - fCX;
		*_y = fDisY - fCY;
		return true;
	}

	return false;
}

_bool CPlayer::Check_Rect2(_vec3 _Dst, _vec3 _Src, _float * _x, _float * _y)
{
	float	fCX = abs(_Dst.x - _Src.x);
	float	fCY = abs(_Dst.y - _Src.y);
	float	fDisX = (float)((PLAYERCX + 30) / 2);
	float	fDisY = (float)((PLAYERCY + 30) / 2);

	if (fCX < fDisX && fCY < fDisY)
	{
		*_x = fDisX - fCX;
		*_y = fDisY - fCY;
		return true;
	}

	return false;
}

_bool CPlayer::Check_CollisionTerrainOnVine(_vec3 * _vCollisionPos, _vec3 _vPlayerPos, _bool * _bUp)
{
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
			*_bUp = (vPlayerPosFromViewport.y < vTerrainPosFromViewport.y) ? true : false;
			return true;
		}
	}
	return false;
}

_bool CPlayer::Check_CollisionGrapList()
{
	_vec3* _vCollisionPos = new _vec3;

	_vec3 vTerrainPosFromViewport = { 0.f,0.f, 0.f };
	_vec3 vPlayerPosFromViewport = { 0.f, 0.f, 0.f };

	_vec3 vTerrainPosFromWorld = m_vGrabCollisionPos;;
	_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;
	// vPlayerPosFromWorld.y += 0.3f;

	m_pCamera->WorldToScreen(&vTerrainPosFromWorld, &vTerrainPosFromViewport);
	m_pCamera->WorldToScreen(&vPlayerPosFromWorld, &vPlayerPosFromViewport);


	if (Check_Rect(vPlayerPosFromViewport, vTerrainPosFromViewport, &_vCollisionPos->x, &_vCollisionPos->y)) {

		if (JUMP == m_eCurState && !m_bGraping)
		{
			switch (m_eMapDir)
			{    // 0.51은 임의의값, 해줘야 앞으로 붙음, 추후 수정
			case eMapDir::EAST: m_pTransformCom->Set_Pos(_vec3(m_vGrabCollisionPos.x += 100.51f, vPlayerPosFromWorld.y, vPlayerPosFromWorld.z)); break;
			case eMapDir::WEST: m_pTransformCom->Set_Pos(_vec3(m_vGrabCollisionPos.x -= 100.51f, vPlayerPosFromWorld.y, vPlayerPosFromWorld.z)); break;
			case eMapDir::SOUTH: m_pTransformCom->Set_Pos(_vec3(vPlayerPosFromWorld.x, vPlayerPosFromWorld.y, m_vGrabCollisionPos.z -= 100.51f)); break;
			case eMapDir::NORTH: m_pTransformCom->Set_Pos(_vec3(vPlayerPosFromWorld.x, vPlayerPosFromWorld.y, m_vGrabCollisionPos.z += 100.51f)); break;
			}

			//switch (m_eMapDir)
			//{	// 0.6은 임의의값, 해줘야 앞으로 붙음, 추후 수정
			//case eMapDir::EAST:	vPlayerPosFromWorld.x = vTerrainPosFromWorld.x += 0.51f; break;
			//case eMapDir::WEST:	vPlayerPosFromWorld.x = vTerrainPosFromWorld.x -= 0.51f; break;
			//case eMapDir::SOUTH: vPlayerPosFromWorld.z = vTerrainPosFromWorld.z -= 0.51f; break;
			//case eMapDir::NORTH: vPlayerPosFromWorld.z = vTerrainPosFromWorld.z += 0.51f; break;
			//}

			m_bCanGrab = true;
			m_fPosYOnTile = vTerrainPosFromWorld.y;
			//m_vTempPos = vTerrainPosFromWorld;
			return true;
		}
	}

	m_bCanGrab = false;

	return false;
}

_bool CPlayer::Find_TerrainForCollision()
{
	_float	fNearDis = 1000.f;
	_float	fNearDisFromCamera = 0.f;
	_vec3	vNearPos = { 0.f, 0.f, 0.f };
	_vec3	vGrabPos = { 0.f, 0.f, 0.f };
	TILEINFO* pTileInfo = nullptr;
	BYTE	bTemp = 0;

	switch (m_eMapDir)
	{
	case eMapDir::WEST:
		fNearDisFromCamera = 1000.f;
		break;
	case eMapDir::SOUTH:
		fNearDisFromCamera = 1000.f;
		break;
	default:
		break;
	}

	for each(CGameObject* pCollisionTerrain in *m_pCollisionTerrainList) {
		CTransform* pTrans = dynamic_cast<CTransform*>(pCollisionTerrain->Get_Component(L"Com_Transform"));
		_vec3 vTerrainPosFromWorld = pTrans->Get_Desc().vPos;
		_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;
		pTileInfo = static_cast<CMonster*>(pCollisionTerrain)->Get_TileInfo();

		if (!m_bJumpOnTopFalling) //추가
		{
			if (vPlayerPosFromWorld.y >= vTerrainPosFromWorld.y + 1.8f)
			{
				if (m_fCurTopOnTileY < vTerrainPosFromWorld.y + 1.8f)
					m_fCurTopOnTileY = vTerrainPosFromWorld.y + 1.8f;
			}
		}

		switch (m_eMapDir)
		{
		case eMapDir::EAST:
			if (vPlayerPosFromWorld.z >= vTerrainPosFromWorld.z - 0.5f
				&& vPlayerPosFromWorld.z <= vTerrainPosFromWorld.z + 0.5f)
			{
				_float fDis = vPlayerPosFromWorld.y - vTerrainPosFromWorld.y;
				if (fDis >= 0.3f && fDis <= 1.8f) {
					if (fNearDisFromCamera < vTerrainPosFromWorld.x) {
						fNearDisFromCamera = vTerrainPosFromWorld.x;
						vNearPos = vTerrainPosFromWorld;
						bTemp = 0;
						bTemp |= pTileInfo->btCollisionDir;
						m_pCollisionTerrain = pCollisionTerrain;
					}
				}

				if (fDis >= 0.f && fDis <= 0.7f) {
					if (fNearDisFromCamera < vTerrainPosFromWorld.x) {
						fNearDisFromCamera = vTerrainPosFromWorld.x;
						vGrabPos = vTerrainPosFromWorld;
					}
				}

			}
			break;
		case eMapDir::WEST:
			if (vPlayerPosFromWorld.z >= vTerrainPosFromWorld.z - 0.5f
				&& vPlayerPosFromWorld.z <= vTerrainPosFromWorld.z + 0.5f)
			{
				_float fDis = vPlayerPosFromWorld.y - vTerrainPosFromWorld.y;
				if (fDis >= 0.3f && fDis <= 1.8f) {
					if (fNearDisFromCamera > vTerrainPosFromWorld.x) {
						fNearDisFromCamera = vTerrainPosFromWorld.x;
						vNearPos = vTerrainPosFromWorld;
						bTemp = 0;
						bTemp |= pTileInfo->btCollisionDir;
						m_pCollisionTerrain = pCollisionTerrain;
					}
				}
				if (fDis >= 0.f && fDis <= 0.7f) {
					if (fNearDisFromCamera < vTerrainPosFromWorld.x) {
						fNearDisFromCamera = vTerrainPosFromWorld.x;
						vGrabPos = vTerrainPosFromWorld;
					}
				}
			}
			break;
		case eMapDir::SOUTH: // 점프뛸때는 모든놈보다 앞에 그리고, 내려올땐 아랫놈을 찾아서 보정하는건 어떨까? 
			if (vPlayerPosFromWorld.x >= vTerrainPosFromWorld.x - 0.5f
				&& vPlayerPosFromWorld.x <= vTerrainPosFromWorld.x + 0.5f)
			{

				_float fDis = vPlayerPosFromWorld.y - vTerrainPosFromWorld.y;
				if (fDis >= 0.3f && fDis <= 1.8f) {
					if (fNearDisFromCamera > vTerrainPosFromWorld.z) {
						fNearDisFromCamera = vTerrainPosFromWorld.z;
						vNearPos = vTerrainPosFromWorld;
						bTemp = 0;
						bTemp |= pTileInfo->btCollisionDir;
						m_pCollisionTerrain = pCollisionTerrain;
					}
				}
				if (fDis >= 0.f && fDis <= 0.7f) {
					if (fNearDisFromCamera < vTerrainPosFromWorld.x) {
						fNearDisFromCamera = vTerrainPosFromWorld.x;
						vGrabPos = vTerrainPosFromWorld;
					}
				}
			}
			break;
		case eMapDir::NORTH:
			if (vPlayerPosFromWorld.x >= vTerrainPosFromWorld.x - 0.5f &&
				vPlayerPosFromWorld.x <= vTerrainPosFromWorld.x + 0.5f)
			{
				_float fDis = vPlayerPosFromWorld.y - vTerrainPosFromWorld.y;
				if (fDis >= 0.3f && fDis <= 1.8f) {
					if (fNearDisFromCamera < vTerrainPosFromWorld.z) {
						fNearDisFromCamera = vTerrainPosFromWorld.z;
						vNearPos = vTerrainPosFromWorld;
						bTemp = 0;
						bTemp |= pTileInfo->btCollisionDir;
						m_pCollisionTerrain = pCollisionTerrain;
					}
				}
				if (fDis >= 0.f && fDis <= 0.7f) {
					if (fNearDisFromCamera < vTerrainPosFromWorld.x) {
						fNearDisFromCamera = vTerrainPosFromWorld.x;
						vGrabPos = vTerrainPosFromWorld;
					}
				}
			}
			break;
		default:
			break;
		}
	}

	if (bTemp & TILECOL_DIR_EAST && m_eMapDir == eMapDir::EAST) {
		m_bFrontTileCollisionOk = true;
	}
	else if (bTemp & TILECOL_DIR_WEST && m_eMapDir == eMapDir::WEST) {
		m_bFrontTileCollisionOk = true;
	}
	else if (bTemp & TILECOL_DIR_SOUTH && m_eMapDir == eMapDir::SOUTH) {
		m_bFrontTileCollisionOk = true;
	}
	else if (bTemp & TILECOL_DIR_NORTH && m_eMapDir == eMapDir::NORTH) {
		m_bFrontTileCollisionOk = true;
	}
	else {
		m_bFrontTileCollisionOk = false;
		// cout << "충돌해야할 타일 감지 못함!!!!" << endl;
	}

	switch (m_eMapDir)
	{
	case eMapDir::EAST:
		m_fColDis = vNearPos.x;
		break;
	case eMapDir::WEST:
		m_fColDis = vNearPos.x;
		break;
	case eMapDir::SOUTH:
		m_fColDis = vNearPos.z;
		break;
	case eMapDir::NORTH:
		m_fColDis = vNearPos.z;
		break;
	}

	m_vGrabCollisionPos = vGrabPos;
	m_vTargetCollisionPos = vNearPos;

	return S_OK;
}

_bool CPlayer::Find_TerrainForAdjustPos()
{
	_float	fNearDis = 1000.f;
	_float	fNearDisFromCamera = 0.f;
	_vec3	vFrontPos = { 0.f, 0.f, 0.f };

	switch (m_eMapDir)
	{
	case eMapDir::WEST:
		fNearDisFromCamera = 1000.f;
		break;
	case eMapDir::SOUTH:
		fNearDisFromCamera = 1000.f;
		break;
	default:
		break;
	}

	for each(TILEINFO* pTile in *m_pAllTerrainVector) {

		_vec3 vTerrainPosFromWorld = pTile->vPos;
		_vec3 vPlayerPosFromWorld = m_pTransformCom->Get_Desc().vPos;

		switch (m_eMapDir)
		{
		case eMapDir::EAST:
			if (vPlayerPosFromWorld.x >= vTerrainPosFromWorld.x - 0.5f
				&& vPlayerPosFromWorld.x <= vTerrainPosFromWorld.x + 0.5f
				&& vPlayerPosFromWorld.y >= vTerrainPosFromWorld.y - 0.5f
				&& vPlayerPosFromWorld.y <= vTerrainPosFromWorld.y + 0.5f)
			{
				if (fNearDisFromCamera < vTerrainPosFromWorld.x) {
					fNearDisFromCamera = vTerrainPosFromWorld.x;
					vFrontPos = vTerrainPosFromWorld;
				}
			}
			break;
		case eMapDir::WEST:
			if (vPlayerPosFromWorld.x >= vTerrainPosFromWorld.x - 0.5f
				&& vPlayerPosFromWorld.x <= vTerrainPosFromWorld.x + 0.5f
				&& vPlayerPosFromWorld.y >= vTerrainPosFromWorld.y - 0.5f
				&& vPlayerPosFromWorld.y <= vTerrainPosFromWorld.y + 0.5f)
			{
				if (fNearDisFromCamera > vTerrainPosFromWorld.x) {
					fNearDisFromCamera = vTerrainPosFromWorld.x;
					vFrontPos = vTerrainPosFromWorld;
				}
			}
			break;
		case eMapDir::SOUTH:
			if (vPlayerPosFromWorld.x >= vTerrainPosFromWorld.x - 0.5f
				&& vPlayerPosFromWorld.x <= vTerrainPosFromWorld.x + 0.5f
				&& vPlayerPosFromWorld.y >= vTerrainPosFromWorld.y - 0.5f
				&& vPlayerPosFromWorld.y <= vTerrainPosFromWorld.y + 0.5f)
			{
				if (fNearDisFromCamera > vTerrainPosFromWorld.z) {
					fNearDisFromCamera = vTerrainPosFromWorld.z;
					vFrontPos = vTerrainPosFromWorld;
				}
			}
			break;
		case eMapDir::NORTH:
			if (vPlayerPosFromWorld.x >= vTerrainPosFromWorld.x - 0.5f
				&& vPlayerPosFromWorld.x <= vTerrainPosFromWorld.x + 0.5f
				&& vPlayerPosFromWorld.y >= vTerrainPosFromWorld.y - 0.5f
				&& vPlayerPosFromWorld.y <= vTerrainPosFromWorld.y + 0.5f)
			{
				if (fNearDisFromCamera < vTerrainPosFromWorld.z) {
					fNearDisFromCamera = vTerrainPosFromWorld.z;
					vFrontPos = vTerrainPosFromWorld;
				}
			}
			break;
		default:
			break;
		}
	}

	switch (m_eMapDir)
	{
	case eMapDir::EAST:
		m_fColDis = vFrontPos.x + 0.5001f;
		break;
	case eMapDir::WEST:
		m_fColDis = vFrontPos.x - 0.5001f;
		break;
	case eMapDir::SOUTH:
		m_fColDis = vFrontPos.z - 0.5001f;
		break;
	case eMapDir::NORTH:
		m_fColDis = vFrontPos.z + 0.5001f;
		break;
	}

	m_vFrontPos = vFrontPos;

	return S_OK;
}

_bool CPlayer::Check_CanGoFrontOrNot(_vec3 _vTilePos)
{
	_vec3 vPlayerPos = m_pTransformCom->Get_Desc().vPos;

	switch (m_eMapDir)
	{
	case eMapDir::EAST:
		if (vPlayerPos.y < _vTilePos.y + 0.5f && vPlayerPos.y > _vTilePos.y - 0.5f
			&& vPlayerPos.z < _vTilePos.z + 0.5f && vPlayerPos.z >_vTilePos.z - 0.5f) {
			return false;
		}
		break;
	case eMapDir::WEST:
		if (vPlayerPos.y < _vTilePos.y + 0.5f && vPlayerPos.y > _vTilePos.y - 0.5f
&& vPlayerPos.z < _vTilePos.z + 0.5f && vPlayerPos.z >_vTilePos.z - 0.5f) {
return false;
		}
		break;
	case eMapDir::SOUTH:
		if (vPlayerPos.y < _vTilePos.y + 0.5f && vPlayerPos.y > _vTilePos.y - 0.5f
			&& vPlayerPos.x < _vTilePos.y + 0.5f && vPlayerPos.x >_vTilePos.x - 0.5f) {
			return false;
		}
		break;
	case eMapDir::NORTH:
		if (vPlayerPos.y < _vTilePos.y + 0.5f && vPlayerPos.y > _vTilePos.y - 0.5f
			&& vPlayerPos.x < _vTilePos.y + 0.5f && vPlayerPos.x >_vTilePos.x - 0.5f) {
			return false;
		}
		break;
	}

	return true;
}

void CPlayer::FrameMove(_float fDeltaTime, float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fDeltaTime * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		if (JUMP == m_eCurState)
		{
			m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
		}
		else if (BEGINFALL == m_eCurState)
		{
			m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
		}
		else if (FALLDIE == m_eCurState)
		{
			m_bJumpOnTopFalling = false;				//추가
			m_pTransformCom->Set_Pos(m_vFallPos);
			m_eCurState = IDLE;
			m_tFrame.fFrameStart = 0.f;
			m_bIsFall = false;
		}
		else if (GETIN == m_eCurState)
		{
			CManagement*	pManagement = CManagement::Get_Instance();

			if (FAILED(pManagement->Clear_NoneStatic()))
			{
				PRINT_LOG(L"Error", L"Failed to Clear_NoneStatic");
			}
			if (nullptr != m_pItemList || nullptr != m_pVineTerrainList || nullptr != m_pInterObjList
				|| nullptr != m_pPortalList || nullptr != m_pItemList)
			{
				m_pItemList = nullptr;
				m_pVineTerrainList = nullptr;
				m_pInterObjList = nullptr;
				m_pPortalList = nullptr;
				m_pItemList = nullptr;
				m_CubeCount = 1;
			}

			m_bChangeScene = true;
			m_eCurState = GETOUT;
			m_tFrame.fFrameStart = 0.f;
			if (eSceneType::Stage == m_eNextScene)
				pManagement->Setup_CurrentScene((_int)eSceneType::Loading, CLoadingScene::Create(m_pDevice, m_eNextScene, false));
			else
				pManagement->Setup_CurrentScene((_int)eSceneType::Loading, CLoadingScene::Create(m_pDevice, m_eNextScene));
		}
		else if (GETOUT == m_eCurState)
		{
			m_eCurState = IDLE;
			//m_pTransformCom->Set_Pos(_vec3(12.f, 7.f, 14.f));
			m_tFrame.fFrameStart = 0.f;
		}
		else if (GRAPUP == m_eCurState)
		{
			//m_pTransformCom->Set_PosYPlus(1.5f);
			m_tJump.fJumpTime = 0.f;
			m_vFallPos = m_pTransformCom->Get_Desc().vPos;
			m_fCurTopOnTileY = m_pTransformCom->Get_Desc().vPos.y;
			m_eCurState = IDLE;
			m_tFrame.fFrameStart = 0.f;
			m_bGraping = false;
		}
		else if (LIFTBOX == m_eCurState)
		{
			m_eCurState = LIFTBOXIDLE;
			m_tFrame.fFrameStart = 0.f;
			m_bLiftingBox = true;
		}
		else if (PUTDOWNBOX == m_eCurState)
		{
			m_eCurState = IDLE;
			m_tFrame.fFrameStart = 0.f;
			m_bLiftingBox = false;
		}
		else
			m_tFrame.fFrameStart = 0.f;
	}
}

void CPlayer::Key_Check(_float fDeltaTime)
{
	if (m_tKeyManager.KeyDown(KEY_1_CLIENT))
	{
		m_eNextScene = eSceneType::Stage;
		m_eCurState = GETIN;
		m_tFrame.fFrameStart = 0;
	}
	if (m_tKeyManager.KeyDown(KEY_2_CLIENT))
	{
		m_eNextScene = eSceneType::Stage2;
		m_eCurState = GETIN;
		m_tFrame.fFrameStart = 0;
	}
	if (m_tKeyManager.KeyDown(KEY_3_CLIENT))
	{
		m_eNextScene = eSceneType::Stage3;
		m_eCurState = GETIN;
		m_tFrame.fFrameStart = 0;
	}
	if (m_tKeyManager.KeyDown(KEY_4_CLIENT))
	{
		m_eNextScene = eSceneType::GomezHouse;
		m_eCurState = GETIN;
		m_tFrame.fFrameStart = 0;
	}
	if (m_tKeyManager.KeyDown(KEY_5_CLIENT))
	{
		m_eNextScene = eSceneType::BonusStage;
		m_eCurState = GETIN;
		m_tFrame.fFrameStart = 0;
	}
	if (m_tKeyManager.KeyDown(KEY_B2_CLIENT))
	{
		m_eNextScene = eSceneType::BonusStage2;
		m_eCurState = GETIN;
		m_tFrame.fFrameStart = 0;
	}

	if (m_tKeyManager.KeyDown(KEY_Z_CLIENT)){
		// 보스 소환하자
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
		CSoundMgr::Get_Instance()->PlaySound(L"BGM_Boss.mp3", CSoundMgr::BGM);

		TRANSFORM_DESC tDesc;
		// 찬수 추가분
		_float fPlayerZ = m_pTransformCom->Get_Pos().z;
		tDesc.vPos = _vec3(-10.f, 0.f, fPlayerZ);
		tDesc.vLook = { 0.f, 0.f, 1.f };
		tDesc.fMoveSpdPerSec = 5.f;
		tDesc.fRotSpdPerSec = D3DXToRadian(135.f);

		CManagement* pManagement = CManagement::Get_Instance();

		if (FAILED(pManagement->Add_GameObjectInLayer(ePrototypeType::NoneStatic, L"GameObject_Heli"
			, ePrototypeType::NoneStatic, L"Layer_Monster_Heli", &tDesc))) {

		}
	}

	if (m_tKeyManager.KeyDown(KEY_F_CLIENT)) // 덩쿨 잡기
	{
		if (!m_NearVineTerrainList.empty()) {
			m_NearVineTerrainList.clear();
			for each (CGameObject* pTerrain in m_NearVineTerrainList)
			{
				Safe_Delete(pTerrain);
			}
		}

		if (!m_bOnVine)
			if (Check_VineTerrain(&m_vCollisionPos, m_pTransformCom->Get_Desc().vPos))
			{
				m_tFrame.fFrameStart = 0.f;
				m_bJumpOnTopFalling = false;		//추가
				m_eCurState = CLIMB;
			}
	}

	if (m_tKeyManager.KeyDown(KEY_R_CLIENT)) // 박스 잡기
	{
		Check_InterObj(&m_vCollisionPos, m_pTransformCom->Get_Desc().vPos);
	}

	if (m_tKeyManager.KeyDown(KEY_G_CLIENT)) // 박스 던지기
	{
		if (m_bGrabSomething) {
			dynamic_cast<CInterBox*>(m_pGrabbedInterObj)->SetOff_Target();
			dynamic_cast<CInterBox*>(m_pGrabbedInterObj)->Set_Throwing(m_eMapDir, m_iCurDir);
			m_eCurState = PUTDOWNBOX;
			m_tFrame.fFrameStart = 0;
			//m_bLiftingBox = false;
			m_bGrabSomething = false;
		}
	}

	if (m_tKeyManager.KeyPressing(KEY_LEFT_CLIENT))
	{
		if (m_eCurState == CLIMB || m_bGraping)
			FrameMove(fDeltaTime, 1.0f);

		if (LEFT_ROT != m_iCurDir)
		{
			m_iCurDir = LEFT_ROT;
			m_pTransformCom->Set_ScaleReverse();
		}
		if (IDLE == m_eCurState)
			m_eCurState = WALK;
		else if (m_bGraping)
			m_eCurState = GRAPMOVE;
		else if (m_bLiftingBox)
			m_eCurState = LIFTBOXMOVE;

		if (m_bOnVine)
		{
			if (Check_NearVineTerrain())
				m_pTransformCom->Go_Sideway(fDeltaTime);
		}
		else if (m_bLiftingBox)
			m_pTransformCom->Go_Sideway(fDeltaTime / 2.f);
		else
			m_pTransformCom->Go_Sideway(fDeltaTime);
	}
	else if (m_tKeyManager.KeyPressing(KEY_RIGHT_CLIENT))
	{
		if (m_eCurState == CLIMB || m_bGraping)
			FrameMove(fDeltaTime, 1.0f);
		/*m_pTextureCom = (CTexture*)Get_Component(L"Com_Player_Walk");*/
		if (RIGHT_ROT != m_iCurDir)
		{
			m_iCurDir = RIGHT_ROT;
			m_pTransformCom->Set_ScaleReverse();
		}
		if (IDLE == m_eCurState)
			m_eCurState = WALK;
		else if (m_bGraping)
			m_eCurState = GRAPMOVE;
		else if (m_bLiftingBox)
			m_eCurState = LIFTBOXMOVE;

		if (m_bOnVine)
		{
			if (Check_NearVineTerrain())
				m_pTransformCom->Go_Sideway(fDeltaTime);
		}
		else if (m_bLiftingBox)
			m_pTransformCom->Go_Sideway(fDeltaTime / 2.f);
		else
			m_pTransformCom->Go_Sideway(fDeltaTime);
	}
	else if (m_tKeyManager.KeyPressing(KEY_UP_CLIENT))
	{
		if (m_bGraping)
		{
			/*m_eCurState = GRAPUP;
			m_tFrame.fFrameStart = 0.f;*/
		}
		else
		{
			if (m_bOnVine)
			{
				FrameMove(fDeltaTime, 1.2f);
				m_pTransformCom->Go_Up(fDeltaTime);
			}
			else if (!m_bOnVine && !m_bIsFall && 0 == m_tJump.iJumpCount && !m_bCanGrab && !m_bGraping)
			{
				Check_Portal(&m_vCollisionPos, m_pTransformCom->Get_Desc().vPos);
			}
			else if (m_bCanGrab)
			{
				m_tJump.iJumpCount = 0;
				m_eCurState = GRAPIDLE;
				m_tFrame.fFrameStart = 0.f;
				m_bGraping = true;
				m_pTransformCom->Set_PosY(m_fPosYOnTile + 0.6f);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"Player_Grab.wav", CSoundMgr::PLAYER);
			}
		}
	}
	else if (m_tKeyManager.KeyPressing(KEY_DOWN_CLIENT) && m_bOnVine)
	{
		FrameMove(fDeltaTime, 1.2f);
		m_pTransformCom->Go_Up(-fDeltaTime);
	}
	else if (WALK == m_eCurState)
	{
		m_eCurState = IDLE;
	}
	else if (GRAPMOVE == m_eCurState)
	{
		m_eCurState = GRAPIDLE;
		m_tFrame.fFrameStart = 0.f;
	}
	else if (LIFTBOXMOVE == m_eCurState)
	{
		m_eCurState = LIFTBOXIDLE;
		m_tFrame.fFrameStart = 0.f;
	}

	// 점프
	if (m_tKeyManager.KeyDown(KEY_SPACE_CLIENT) && 0 == m_tJump.iJumpCount)
	{

			
		if (m_bGraping)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Player_GrabUp.wav", CSoundMgr::PLAYER);
			m_eCurState = GRAPUP;
			m_tFrame.fFrameStart = 0.f;
		}
		else
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Player_Jump.wav", CSoundMgr::PLAYER);

			if (!m_bIsFall)
			{
				if (m_bLiftingBox)
				{
					m_tJump = { 4.f, 0.f, m_pTransformCom->Get_Desc().vPos.y, ++m_tJump.iJumpCount };

					m_fTemp = m_tJump.fJumpY + (m_tJump.fJumpPower * m_tJump.fJumpPower) / (GRAVITY * 2.f);

					m_tFrame.fFrameStart = 0.f;
					m_eCurState = JUMP;
				}
				else
				{
					if (m_bOnVine)
						m_bOnVine = false;

					m_tJump = { 7.f, 0.f, m_pTransformCom->Get_Desc().vPos.y, ++m_tJump.iJumpCount };

					m_fTemp = m_tJump.fJumpY + (m_tJump.fJumpPower * m_tJump.fJumpPower) / (GRAVITY * 2.f);

					m_tFrame.fFrameStart = 0.f;
					m_eCurState = JUMP;
				}
			}
		}
	}

	if (m_tKeyManager.KeyDown(KEY_A_CLIENT) && !m_bGraping)
	{
		if (!m_bIsRotate)
		{
			m_iRotDir = LEFT_ROT;
			m_fOldRotate = m_pTransformCom->Get_Desc().vRot.y;
			m_bIsRotate = true;
			m_bIsTurning = true;
			m_fRotateAngle = 0.f;
			if (m_iDirNum == 1)
				m_iDirNum = 4;
			else
				--m_iDirNum;
		}
	}
	else if (m_tKeyManager.KeyDown(KEY_D_CLIENT) && !m_bGraping)
	{
		if (!m_bIsRotate)
		{
			m_iRotDir = RIGHT_ROT;
			m_fOldRotate = m_pTransformCom->Get_Desc().vRot.y;
			m_bIsRotate = true;
			m_bIsTurning = true;
			m_fRotateAngle = 0.f;
			if (m_iDirNum == 4)
				m_iDirNum = 1;
			else
				++m_iDirNum;
		}
	}
}

void CPlayer::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 9.f;
			break;
		case CPlayer::WALK:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		case CPlayer::JUMP:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		case CPlayer::BEGINFALL:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 2.f;
			break;
		case CPlayer::ENDFALL:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 3.f;
			break;
		case CPlayer::FALLDIE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 8.f;
			break;
		case CPlayer::CLIMB:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		case CPlayer::GETIN:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 12.f;
			break;
		case CPlayer::GETOUT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 13.f;
			break;
		case CPlayer::GRAPIDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 0.f;
			break;
		case CPlayer::GRAPMOVE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			break;
		case CPlayer::GRAPUP:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 6.f;
			break;
		case CPlayer::LIFTBOX:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 9.f;
			break;
		case CPlayer::LIFTBOXMOVE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 7.f;
			break;
		case CPlayer::PUTDOWNBOX:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 7.f;
			break;
		case CPlayer::LIFTBOXIDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 0.f;
			break;
		default:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CPlayer::Set_RightRot()
{
	m_iRotDir = RIGHT_ROT;
	m_fOldRotate = m_pTransformCom->Get_Desc().vRot.y;
	m_bIsRotate = true;
	m_bIsTurning = true;
	m_fRotateAngle = 0.f;
	if (m_iDirNum == 4)
		m_iDirNum = 1;
	else
		++m_iDirNum;
}

void CPlayer::Set_LeftRot()
{
	m_iRotDir = LEFT_ROT;
	m_fOldRotate = m_pTransformCom->Get_Desc().vRot.y;
	m_bIsRotate = true;
	m_bIsTurning = true;
	m_fRotateAngle = 0.f;
	if (m_iDirNum == 1)
		m_iDirNum = 4;
	else
		--m_iDirNum;
}

HRESULT CPlayer::Movement(_float fDeltaTime)
{
	Sound(fDeltaTime);

	if (m_bIsRotate)
	{
		if (D3DXToRadian(90.f)/*1.57f*/ <= m_fRotateAngle)
		{
			if (RIGHT_ROT == m_iRotDir)
			{
				m_pTransformCom->Set_RotY(m_fOldRotate - D3DXToRadian(90.f));
				m_fSetCurRotate +=  -90.f;
				m_fCurRotate = m_fSetCurRotate;
			}
			else if (LEFT_ROT == m_iRotDir)
			{
				m_pTransformCom->Set_RotY(m_fOldRotate + D3DXToRadian(90.f));
				m_fSetCurRotate += 90.f;
				m_fCurRotate = m_fSetCurRotate;
			}
			m_bIsTurning = false;
			m_bIsRotate = false;

			m_bCameraDisCalculateByZ = !m_bCameraDisCalculateByZ;

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
			{
				m_pTransformCom->RotateY(-fDeltaTime);
				m_fCurRotate += (_float)D3DXToDegree(m_pTransformCom->Get_Desc().fRotSpdPerSec * -fDeltaTime);
			}
			else if (LEFT_ROT == m_iRotDir)
			{
				m_pTransformCom->RotateY(fDeltaTime);
				m_fCurRotate += (_float)D3DXToDegree(m_pTransformCom->Get_Desc().fRotSpdPerSec * fDeltaTime);
			}

			m_fRotateAngle += fDeltaTime * m_pTransformCom->Get_Desc().fRotSpdPerSec;
		}
	}
	if (!m_bIsTurning)
	{
		if (0 < m_tJump.iJumpCount)
		{
			m_tJump.fJumpTime += 1.55f * fDeltaTime;
			float fTempY = m_tJump.fJumpY + (m_tJump.fJumpPower * m_tJump.fJumpTime - (0.5f * GRAVITY * m_tJump.fJumpTime * m_tJump.fJumpTime));
			m_pTransformCom->Set_PosY(fTempY);
		}

		if (GRAPUP == m_eCurState)
		{
			m_pTransformCom->Set_PosYPlus(0.03f);
		}

		if (m_bOnVine) { // 덩쿨 온 상태이면 멈춰!!
			m_tJump.iJumpCount = 0;
			_vec3 vTemp;
			_bool bTemp;
			if (Check_CollisionTerrainOnVine(&vTemp, m_pTransformCom->Get_Desc().vPos, &bTemp)) {
				_vec3 vPlayerPos = m_pTransformCom->Get_Desc().vPos;
				if (!bTemp)
					vPlayerPos.y -= (vTemp.y / CLIENTWINCY);
				else
					vPlayerPos.y += (vTemp.y / CLIENTWINCY);
				//vPlayerPos.x += (vTemp.x / CLIENTWINCX);
				m_pTransformCom->Set_Pos(vPlayerPos);
			};
			return S_OK;
		}

		if (m_pCollisionTerrainList->size()) {
			if (!Check_CollisionTerrain(&m_vCollisionPos, m_pTransformCom->Get_Desc().vPos))
			{
				m_bPlayerOnTerrain = false;

				_vec3 _vPlayerPos = m_pTransformCom->Get_Desc().vPos;

				if (0.f != m_vFrontPos.x) { // 동일선상에 카메라와 더 가까운 타일 찾으면 거기에 붙이고

					// cout << "m_vFrontPos " << m_vFrontPos.x << " " << m_vFrontPos.y << " " << m_vFrontPos.z << endl;

					switch (m_eMapDir)
					{
					case eMapDir::EAST: _vPlayerPos.x = m_vFrontPos.x + 0.5001f; break;
					case eMapDir::WEST: _vPlayerPos.x = m_vFrontPos.x - 0.5001f; break;
					case eMapDir::SOUTH: _vPlayerPos.z = m_vFrontPos.z - 0.5001f; break;
					case eMapDir::NORTH: _vPlayerPos.z = m_vFrontPos.z + 0.5001f; break;
					default: break;
					}
				}
				else if (0.f != m_vTargetCollisionPos.x && m_bFrontTileCollisionOk) { // 타일 없을땐 아래쪽에 있는 타일 위치에 붙이고

					 // cout << "m_vTargetCollisionPos " << m_vTargetCollisionPos.x << " " << m_vTargetCollisionPos.y << " " << m_vTargetCollisionPos.z << endl;

					switch (m_eMapDir)
					{
					case eMapDir::EAST: _vPlayerPos.x = m_vTargetCollisionPos.x + 0.5001f; break;
					case eMapDir::WEST: _vPlayerPos.x = m_vTargetCollisionPos.x - 0.5001f; break;
					case eMapDir::SOUTH: _vPlayerPos.z = m_vTargetCollisionPos.z - 0.5001f; break;
					case eMapDir::NORTH: _vPlayerPos.z = m_vTargetCollisionPos.z + 0.5001f; break;
					default: break;
					}
				}
				else {

				}

				m_pTransformCom->Set_Pos(_vPlayerPos);


				if (m_vFallPos.y - m_pTransformCom->Get_Desc().vPos.y > 13.f && !m_bIsFall)
				{
					m_eCurState = ENDFALL;
					m_tFrame.fFrameStart = 0.f;
					m_bIsFall = true;
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
					CSoundMgr::Get_Instance()->PlaySound(L"Player_AirPanic.wav", CSoundMgr::PLAYER);
				}

				if (0 == m_tJump.iJumpCount && !m_bGraping)							//공중에서 정프상태가 아니면 중력값 받도록
				{
					if (BEGINFALL != m_eCurState && !m_bIsFall)
					{
						m_eCurState = BEGINFALL;
						m_bJumpOnTopFalling = true;					//추가
						m_tFrame.fFrameStart = 0.f;
					}
					m_tJump.fJumpTime += 1.55f * fDeltaTime;
					float fTempY = m_vFallPos.y - (GRAVITY * m_tJump.fJumpTime * m_tJump.fJumpTime);
					m_pTransformCom->Set_PosY(fTempY);
				}

				if (m_pTransformCom->Get_Desc().vPos.y < -30.f)
				{
					if (0 != m_tJump.iJumpCount)
						m_tJump.iJumpCount = 0;
					m_eCurState = IDLE;
					m_tFrame.fFrameStart = 0.f;
					m_bIsFall = false;
					m_pTransformCom->Set_Pos(m_vFallPos);
				}
			}
			else
			{
				// 여기에 타일과 만났을 때의 코드를 추가해주면 됨.

				m_bPlayerOnTerrain = true;

				_vec3 _vTemp = m_pTransformCom->Get_Desc().vPos;

				if (m_bCameraDisCalculateByZ)
					_vTemp.z = m_fColDis;
				else
					_vTemp.x = m_fColDis;

				m_pTransformCom->Set_Pos(_vTemp);

				if ((0 != m_tJump.iJumpCount && m_pTransformCom->Get_Desc().vPos.y < m_vOnTilePosFromWorld.y)
					|| (BEGINFALL == m_eCurState && m_pTransformCom->Get_Desc().vPos.y < m_vOnTilePosFromWorld.y)
					|| (m_bOnVine && m_pTransformCom->Get_Desc().vPos.y < m_vOnTilePosFromWorld.y))
				{
					if (BEGINFALL == m_eCurState)
					{
						m_tJump.fJumpTime += 1.55f * fDeltaTime;
						float fTempY = m_vFallPos.y - (GRAVITY * m_tJump.fJumpTime * m_tJump.fJumpTime);
						m_pTransformCom->Set_PosY(fTempY);
					}
					else if (0 != m_tJump.iJumpCount)
					{
						m_tJump.fJumpTime += 1.55f * fDeltaTime;
						float fTempY = m_tJump.fJumpY + (m_tJump.fJumpPower * m_tJump.fJumpTime - (0.5f * GRAVITY * m_tJump.fJumpTime * m_tJump.fJumpTime));
						m_pTransformCom->Set_PosY(fTempY);
					}
				}
				else if ((0 != m_tJump.iJumpCount || BEGINFALL == m_eCurState) && !m_bIsFall)					//점프상태일때 땅에 닿으면 Idle상태로 바뀌도록
				{
					/*m_pTransformCom->Set_PosYPlus(m_vCollisionPos.y / (_float)CLIENTWINCY);*/
					m_tJump.iJumpCount = 0;
					m_eCurState = IDLE;
					m_tFrame.fFrameStart = 0;
					m_fCurTopOnTileY = m_pTransformCom->Get_Desc().vPos.y;		// 추가
					m_bJumpOnTopFalling = false;								// 추가
				}
				else if (ENDFALL == m_eCurState)
				{
					m_tJump.iJumpCount = 0;
					m_eCurState = FALLDIE;
					m_tFrame.fFrameStart = 0;
				}

				if (0 == m_tJump.iJumpCount && BEGINFALL != m_eCurState
					&& !m_bIsFall) //점프상태아니고 떨어지는 상태 아닐때 땅에 닿아있으면 현재 위치 확인
				{
					m_tJump.fJumpTime = 0.f;
					m_vFallPos = m_pTransformCom->Get_Desc().vPos;
				}
			}
		}
	}


	return S_OK;
}

void CPlayer::Sound(_float fDeltaTime)
{
	if (GetAsyncKeyState('A'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"Player_A.wav", CSoundMgr::PLAYER);
	}
	if (GetAsyncKeyState('D'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"Player_D.wav", CSoundMgr::PLAYER);
	}


	if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN))
		&& (m_eCurState == CLIMB))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Player_ClimbVine.wav", CSoundMgr::PLAYER);
	}

	if (GetAsyncKeyState(VK_LEFT) || (GetAsyncKeyState(VK_RIGHT)))
	{
		if (m_eCurState == WALK)
			CSoundMgr::Get_Instance()->PlaySound(L"Player_Walk.wav", CSoundMgr::PLAYER);
		else if (m_eCurState == GRAPMOVE)
			CSoundMgr::Get_Instance()->PlaySound(L"Player_GrabMove.wav", CSoundMgr::PLAYER);
		else if (m_eCurState == CLIMB)
			CSoundMgr::Get_Instance()->PlaySound(L"Player_ClimbVine.wav", CSoundMgr::PLAYER);

	}
}
