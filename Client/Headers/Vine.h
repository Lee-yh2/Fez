#pragma once

#ifndef __VINE_H__
#define __VINE_H__

#include "GameObject.h"

USING(Engine)

class CVine final : public CGameObject
{
public:
	explicit CVine(LPDIRECT3DDEVICE9 pDevice);
	explicit CVine(const CVine& other);
	virtual ~CVine() = default;
	

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	void Set_VineDir(eMapDir _VineDir) { m_eVineDir = _VineDir; }

public:
	static	CVine*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;

private:
	_float	m_fTextureNum;
	eMapDir m_eVineDir;
};

#endif