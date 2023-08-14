#pragma once

#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "GameObject.h"

USING(Engine)

class CPortal final : public CGameObject
{
public:
	explicit CPortal(LPDIRECT3DDEVICE9 pDevice);
	explicit CPortal(const CPortal& other);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CPortal*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

public:
	void Set_Open(_bool _bOpen) { m_bOpen = _bOpen; }
	void Set_LightOn(_bool _bLight) { m_bLightOn = _bLight; }
	_bool Get_LighOn() { return m_bLightOn; }
private:
	void FrameMove(_float fDeltaTime, float fMoveSpeed = 1.f);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;

	FRAME		m_tFrame;
	_bool		m_bOpen;
	_bool		m_bLightOn;
};
#endif