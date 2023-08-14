#pragma once

#ifndef __FONT2_H__
#define __FONT2_H__

#include "GameObject.h"

USING(Engine)

class CFont2 final : public CGameObject
{
public:
	explicit CFont2(LPDIRECT3DDEVICE9 pDevice);
	explicit CFont2(const CFont2& other);
	virtual ~CFont2() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CFont2*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;


	bool		m_bClose = false;

private:
	/* ´ë±â */
	float		m_fCnt = 0.f;
	bool		m_bCnt = true;

};

#endif //!__SKYBOX_H__