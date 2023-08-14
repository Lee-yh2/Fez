#pragma once

#ifndef __SPEECHBUBBLE_H__
#define __SPEECHBUBBLE_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;

class CSpeechBubble final : public CGameObject
{
public:
	explicit CSpeechBubble(LPDIRECT3DDEVICE9 pDevice);
	explicit CSpeechBubble(const CSpeechBubble& other);
	virtual ~CSpeechBubble() = default;

public:
	enum STATE { IDLE, WALK, JUMP, DEAD, NUMOFSTATE };

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CSpeechBubble*		Create(LPDIRECT3DDEVICE9 pDevice);
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

protected:
	void FrameMove();

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom[NUMOFSTATE];
	CKeyManager m_tKeyManager;
	STATE		m_eCurState;
	FRAME		m_tFrame;
	_uint		m_Tuto = 0;
	_float		m_RotAngle = 0.f;
	_float		m_Onecheck = 9.7f;
	_float		m_Twocheck = 11.f;
	_float		m_StackDeltaTime = 0.f;
	_bool		m_StartUp = false;
	_bool		m_ShowCube = false;
	_bool		m_Blind = false;
};

#endif //!__SPEECHBUBBLE_H__