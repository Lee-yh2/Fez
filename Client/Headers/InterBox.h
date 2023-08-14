#pragma once

#ifndef __INTERBOX_H__
#define __INTERBOX_H__

#include "GameObject.h"

USING(Engine)

class CInterBox final : public CGameObject
{
public:
	explicit CInterBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CInterBox(const CInterBox& other);
	virtual ~CInterBox() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CInterBox*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

public:
	const	TILEINFO&	Get_TileInfo()const { return m_tTileInfo; }

public:
	void Set_Target(CTransform*& _pTargetTrans) {
		m_pTargetTransformCom = _pTargetTrans;
		Safe_AddRef(m_pTargetTransformCom);
		m_pTransformCom->Set_Pos(_pTargetTrans->Get_Desc().vPos);
	}
	void Set_Camera();
	void SetOff_Target() {
		m_fFallPos = m_pTransformCom->Get_Desc().vPos.y;
		m_pTargetTransformCom = nullptr;
	}
	void Set_Throwing(eMapDir _eMapDir, _int _iThrowDir);
	void Begin_Throw(_float _fDeltaTime);
	void Set_CollisionTerrainList(list<CGameObject*>* _CollisionTerrainList) {m_pCollisionTerrainList = _CollisionTerrainList;}
	void Set_InterObjList(list<CGameObject*>* _InterObjList) { m_pInterObjList = _InterObjList; }

private:
	HRESULT	Movement(_float fDeltaTime);
	_bool Check_Rect(_vec3 _Dst, _vec3 _Src, _float* _x, _float* _y);
	_bool Check_CollisionTerrain(_vec3* _vCollisionPos, _vec3 _vPlayerPos);
	_bool Check_CollisionInterobj(_vec3* _vCollisionPos, _vec3 _vPlayerPos);

CVIBuffer*	m_pVIBufferCom;
	//CVIBuffer*	m_pTerrainVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;
	CTransform* m_pTargetTransformCom;

	list<CGameObject*>* m_pCollisionTerrainList;
	list<CGameObject*>* m_pInterObjList;

	CCamera*	m_pCamera;

	_bool		m_bInteraction;
	_float		m_fPosYOnTile;
	_float		m_fTargetPosY;
	_bool		m_bApproachToTarget;
	JUMPINFO	m_tJump;
	_float		m_fFallPos;
	_bool		m_bIsThrowing;
	_int		m_iThrowDir;
	eMapDir		m_eMapDir;

	TILEINFO	m_tTileInfo;
	_bool		m_bPlayerUsing;
};

#endif //!__INTERBOX_H__