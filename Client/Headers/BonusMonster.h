#pragma once

#ifndef __BONUSMONSTER_H__
#define __BONUSMONSTER_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;

class CBonusMonster final : public CGameObject
{
public:
	explicit CBonusMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CBonusMonster(const CBonusMonster& other);
	virtual ~CBonusMonster() = default;

public:
	enum STATE { WALK ,NUMOFSTATE };

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CBonusMonster*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;

private:
	HRESULT	Movement(_float fDeltaTime);
	HRESULT VillBoard();

protected:
	void FrameMove(_float fDeltaTime, float fMoveSpeed = 1.f);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTransform*	m_pTargetTransformCom;
	CTexture*	m_pTextureCom;
	STATE		m_eCurState;
	FRAME		m_tFrame;
};

#endif //!__BONUSMONSTER_H__