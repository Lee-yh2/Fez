#pragma once

#ifndef __PORTALBACK_H__
#define __PORTALBACK_H__

#include "GameObject.h"

USING(Engine)

class CPortalBack final : public CGameObject
{
public:
	explicit CPortalBack(LPDIRECT3DDEVICE9 CPortalBack);
	explicit CPortalBack(const CPortalBack& other);
	virtual ~CPortalBack() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CPortalBack*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

public:
	void Set_Open(_bool _bOpen) { m_bOpen = _bOpen; }

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;

	_bool		m_bOpen;
};
#endif

