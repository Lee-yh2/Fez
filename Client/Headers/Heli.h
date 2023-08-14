#pragma once

#ifndef __HELI_H__
#define __HELI_H__

#include "GameObject.h"
#include "KeyManager.h"

USING(Engine)

class CHeli final : public CGameObject
{
public:
	enum STATE { IDLE, WALK, JUMP, BEGINFALL, ENDFALL, FALLDIE, CLIMB, GETIN, GETOUT, GRAPIDLE, GRAPMOVE, NUMOFSTATE };

public:
	explicit CHeli(LPDIRECT3DDEVICE9 pDevice);
	explicit CHeli(const CHeli& other);
	virtual ~CHeli() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CHeli*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;
	void Set_UnderAttack();

private:
	HRESULT	Movement(_float fDeltaTime);
	void Key_Check(_float fDeltaTime);
	void FrameMove(_float fDeltaTime, float fMoveSpeed = 1.f);
	void Scene_Change();
	HRESULT Set_Billboard(); // 현재 사용 X 혹시모르니 두겠음

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom[NUMOFSTATE];
	CTransform*	m_pTransformCom;
	CCamera*	m_pCamera;

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
	_vec3		m_vStayPos;

	_int		m_iHp;
	_int		m_iBulletCount;

	_bool		m_bUpAndDown;
	_int		m_iMovingCount;

	// 죽을떄 소리 내기용
private:
	bool			m_bDead = false;

private: // 찬수 추가분
	_float		m_fSpeed;
	_bool		m_bHeliDead;
	_uint		m_iDeadCount;
};

#endif //!__HELI_H__