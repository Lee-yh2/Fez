#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer final : public CGameObject
{
public:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayer(const CPlayer& other);
	virtual ~CPlayer() = default;

public:
	enum STATE { IDLE, WALK, JUMP, BEGINFALL, ENDFALL, FALLDIE, CLIMB, GETIN, GETOUT, GRAPIDLE, GRAPMOVE, GRAPUP, LIFTBOX, LIFTBOXMOVE, PUTDOWNBOX, LIFTBOXIDLE, NUMOFSTATE };

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CPlayer*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;
	CTransform* Return_Transform()
	{
		return m_pTransformCom;
	}
public:
	void Set_Camera();
	void Set_InterObjList(list<CGameObject*>* _InterObjList) { m_pInterObjList = _InterObjList; }
	void Set_PortalList(list<CGameObject*>* _PortaList) { m_pPortalList = _PortaList; }
	void Set_ItemList(list<CGameObject*>* _ItemList) { m_pItemList = _ItemList; }
	void Set_CollisionTerrainList(list<CGameObject*>* _CollisionTerrainList) { m_pCollisionTerrainList = _CollisionTerrainList; }
	void Set_VineTerrainList(list<CGameObject*>* _VineTerrainList) { m_pVineTerrainList = _VineTerrainList; }
	void Set_PlayerPos(_vec3 _vPos) { m_pTransformCom->Set_Pos(_vPos); }
	void Set_AllTerrainList(vector<TILEINFO*>* _pAllTerrain) { m_pAllTerrainVector = _pAllTerrain; } // 추가

	_vec3 Get_PlayerPos() { return m_pTransformCom->Get_Desc().vPos; }


	list<CGameObject*>*	Get_CollisionTerrainList() { return m_pCollisionTerrainList; }
	CCamera* Get_Camera() { return m_pCamera; }
	_bool Check_CollisionTerrain(_vec3* _vCollisionPos, _vec3 _vPlayerPos);
	_bool Check_VineTerrain(_vec3* _vCollisionPos, _vec3 _vPlayerPos);
	_bool Check_NearVineTerrain();
	_bool Check_InterObj(_vec3* _vCollisionPos, _vec3 _vPlayerPos);
	_bool Check_Portal(_vec3* _vCollisionPos, _vec3 _vPlayerPos);
	_bool Check_Item(_float _DeltaTime);
	_bool Check_Rect(_vec3 _Dst, _vec3 _Src, _float* _x, _float* _y);
	_bool Check_Rect2(_vec3 _Dst, _vec3 _Src, _float* _x, _float* _y);
	_bool Check_CollisionTerrainOnVine(_vec3* _vCollisionPos, _vec3 _vPlayerPos, _bool * _bUp);
	_bool Check_CollisionGrapList();
	_bool Find_TerrainForCollision();
	_bool Find_TerrainForAdjustPos();
	_bool Check_CanGoFrontOrNot(_vec3 _vTilePos);
	eMapDir Get_MapDir() { return m_eMapDir; }

protected:
	void FrameMove(_float fDeltaTime, float fMoveSpeed = 1.f);
	void Key_Check(_float fDeltaTime);
	void Scene_Change();

public:
	void Set_CurrentState(STATE _CurState) { m_eCurState = _CurState; }
	void Set_Frame(_float _fFrameStart) { m_tFrame.fFrameStart = _fFrameStart; }
	void Set_SceneChange(_bool _bChangeScene) { m_bChangeScene = _bChangeScene; }
	void Set_Alpha(_bool _bAlpha) { m_bIsAlpha = _bAlpha; }
	void Set_FallPos(_vec3 vPos) { m_vFallPos = vPos; }
	void Set_RightRot();
	void Set_LeftRot();
	void Set_PlayerOnBonusStage() { m_bPlayerOnBonusStage = true; } // 찬수 추가분 04.21
	void Set_OnNewBonusStage(_bool _bOnNewBonusStage) { m_bOnNewBonusStage = _bOnNewBonusStage; }
	void Set_ClearNewBonusStage(_bool _bClearNewBonusStage) { m_bNewBonusStageClear = _bClearNewBonusStage; }
	eMapDir Return_eMapDir() { return m_eMapDir; }
	void Set_CollisionMonster() { m_eCurState = FALLDIE;}
private:
	HRESULT	Movement(_float fDeltaTime);

private:
	_bool m_bChangeScene = false;

private:
	void Sound(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom[NUMOFSTATE];
	CCamera* m_pCamera;
	CGameObject* m_pGrabbedInterObj;
	_bool				m_bIsAlpha;
	CGameObject*	m_pCollisionTerrain;
	_bool			m_bPlayerOnBonusStage;
	_bool			m_bBonusStageClear;
	_vec3				m_vFrontPos;
	vector<TILEINFO*>*	m_pAllTerrainVector;

	list<CGameObject*>* m_pCollisionTerrainList;
	list<CGameObject*>* m_pVineTerrainList;
	list<CGameObject*> m_NearVineTerrainList;
	list<CGameObject*>* m_pInterObjList;
	list<CGameObject*>* m_pPortalList;
	list<CGameObject*>* m_pItemList;

	CKeyManager	m_tKeyManager;

	eSceneType m_eNextScene;

	_bool		m_bOnNewBonusStage;
	_bool		m_bNewBonusStageClear;

	STATE		m_eCurState;
	STATE		m_ePreState;
	_int		m_iCurDir;

	_float		m_fCurRotate;
	_float		m_fSetCurRotate;

	_float		m_fTemp;
	_float		m_fCurTopOnTileY;
	_bool		m_bJumpOnTopFalling;

	_bool		m_bCanGrab;
	_bool		m_bGraping;
	_bool		m_bLiftingBox;

	_vec3		m_vOnTilePosFromViewport;
	_vec3		m_vOnTilePosFromWorld;

	_bool		m_bCameraMode;
	FRAME		m_tFrame;

	JUMPINFO	m_tJump;
	_bool		m_bJump;
	_bool		m_bIsMoving;
	_vec3		m_vGoalPos;
	_bool		m_bOnVine;
	_bool		m_bGrabSomething;
	_float		m_fPosYOnTile;
	_vec3		m_vFallPos;
	_bool		m_bIsFall;

	_bool		m_bPlayerOnTerrain;
	_bool		m_bIsRotate;
	_float		m_fRotateAngle;
	_float		m_fOldRotate;
	_int		m_iRotDir;
	_int		m_iNumOfMonsters;
	_bool		m_bCameraDisCalculateByZ;
	_int		m_iDirNum;
	eMapDir		m_eMapDir;
	_bool		m_bTryFindVine;
	_bool		m_bIsVineNearPlayer;

	_vec3		m_vCollisionPos; // 나중에 쓰게될수도 !? 뷰포트에서 충돌하고 난 뒤 얼마나 겹치는지 반환해서 받을 값

	_vec3		m_vTempPos;
	_float		m_fColDis;
	_vec3		m_vTargetCollisionPos;
	_vec3		m_vTargetCollisionOldPos;
	_vec3		m_vGrabCollisionPos;
	_uint		m_CubeCount = 1;
	_float		m_StackDeltaTIme = 0.f;
	_bool		m_CheckCollisionCube = false;
	_float		m_WaitTIme = 0.f;
	_bool		m_bTimeGo = false;

	_bool		m_bCanGoThrough;
	_bool		m_bFrontTileCollisionOk;
	_bool		m_bSetPositionOrNot;


	/* 추가 */
private:
	bool		m_bCheckGetItem = false;
	float		m_fCnt = 0.f;
	float		m_fWalk = 0.f;


	/* 찬수 추가 04.26 */

};

#endif //!__PLAYER_H__