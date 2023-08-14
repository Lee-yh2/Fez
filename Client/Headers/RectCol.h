#pragma once

#ifndef __RECTCOL_H__
#define __RECTCOL_H__

#include "GameObject.h"

USING(Engine)

class CRectCol final : public CGameObject
{
public:
	explicit CRectCol(LPDIRECT3DDEVICE9 pDevice);
	explicit CRectCol(const CRectCol& other);
	virtual ~CRectCol() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CRectCol*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;


	bool		m_bClose	= false;
	bool		m_bCnt		= false;

	float		m_fY		= 0.f;
	float		m_fCnt		= 0.f;


};

#endif //!__SKYBOX_H__