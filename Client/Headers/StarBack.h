#pragma once

#ifndef __STARBACK_H__
#define __STARBACK_H__

#include "GameObject.h"

USING(Engine)

class CStarBack final : public CGameObject
{
public:
	explicit CStarBack(LPDIRECT3DDEVICE9 pDevice);
	explicit CStarBack(const CStarBack& other);
	virtual ~CStarBack() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CStarBack*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;

private:
	float	m_fScaleAdd = 0.f;
	float	m_fAccel = 0.f;
	bool	m_bSel = false;
};

#endif //!__SKYBOX_H__