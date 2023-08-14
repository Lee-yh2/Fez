#pragma once

#ifndef __NPC_H__
#define __NPC_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;

class CNPC final : public CGameObject
{
public:
	explicit CNPC(LPDIRECT3DDEVICE9 pDevice);
	explicit CNPC(const CNPC& other);
	virtual ~CNPC() = default;

public:
	enum STATE { IDLE, WALK, JUMP, DEAD, NUMOFSTATE };

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CNPC*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;

public:
	CTransform* Return_Transform()
	{
		return m_pTransformCom;
	}
	void Set_Blind(_bool _blind)
	{
		m_Blind = _blind;
	}
private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	_uint		m_Tuto = 0;
	_float		m_RotAngle = 0.f;
	_float		m_Onecheck = 9.7f;
	_float		m_Twocheck = 11.f;
	_float		m_StackDeltaTime = 0.f;
	_bool		m_StartUp = false;
	_bool		m_ShowCube = false;
	_bool		m_Blind = true;
};

#endif //!__NPC_H__