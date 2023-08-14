#pragma once

#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include "GameObject.h"
#include "KeyManager.h"
#include "Camera.h"
#include "Heli.h"

USING(Engine)

class CExplosion final : public CGameObject
{
public:
	enum STATE { MOVING, EXPLOSION, NUMOFSTATE };

public:
	explicit CExplosion(LPDIRECT3DDEVICE9 pDevice);
	explicit CExplosion(const CExplosion& other);
	virtual ~CExplosion() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CExplosion*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);
	void FrameMove(_float fDeltaTime, float fMoveSpeed = 1.f);
	void Scene_Change();

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom[NUMOFSTATE];
	CTransform*	m_pTransformCom;
	CCamera*	m_pCamera;
	CHeli*		m_pTargetHeli;

	FRAME		m_tFrame;
	STATE		m_eCurState;
	STATE		m_ePreState;

	_bool		m_bIsRotate;
	CKeyManager	m_tKeyManager;

	_float      m_fRotateAngle;
	_float		m_fOldRotate;
	_int		m_iRotDir;
	_int		m_iDirNum;
	eMapDir		m_eMapDir;

	_int		m_iMovingDis;
	_float		m_fBulletSpeed;
	_vec3		m_vTargetPosViewport;
	_bool		m_bDead;
	_vec3		m_vBeginPos;
};

#endif //!__EXPLOSION_H__