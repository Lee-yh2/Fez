#pragma once

#ifndef __RECTROW_H__
#define __RECTROW_H__

#include "GameObject.h"

USING(Engine)

class CRectRow final : public CGameObject
{
public:
	explicit CRectRow(LPDIRECT3DDEVICE9 pDevice);
	explicit CRectRow(const CRectRow& other);
	virtual ~CRectRow() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CRectRow*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;

private:
	bool		m_bClose = false;
	bool		m_bEnd = false;

	float		m_fCnt = 0.5f;
	float		m_fX = 0.f;
	float		m_fAccel = 1.f;

private:
	bool		m_bStart = true;
};

#endif //!__SKYBOX_H__