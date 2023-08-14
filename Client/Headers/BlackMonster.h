#pragma once

#ifndef __BLACKMONSTER_H__
#define __BLACKMONSTER_H__

#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CBlackMonster final : public CGameObject
{
public:
	explicit CBlackMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CBlackMonster(const CBlackMonster& other);
	virtual ~CBlackMonster() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CBlackMonster*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;

public:
	void Set_Camera();

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	CCamera* m_pCamera;

	_float		m_StackDeltatime = 0.f;
	_float		m_RotAngel = 0.f;
	_bool		m_bFixDir = false;
	_bool		m_mujok = false;
	_uint		m_iMode = 0;
	_vec3		m_vPlayerPos;
	_vec3		m_vMyPos;
	_vec3		m_vAttackDir;
};

#endif //!__BLACKMONSTER_H__