#pragma once

#ifndef __GRASS_H__
#define __GRASS_H__

#include "GameObject.h"

USING(Engine)

class CGrass final : public CGameObject
{
public:
	explicit CGrass(LPDIRECT3DDEVICE9 pDevice);
	explicit CGrass(const CGrass& other);
	virtual ~CGrass() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CGrass*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);
	HRESULT	BillBoard();

private:
	CVIBuffer*	m_pVIBufferCom;
	CVIBuffer*	m_pTerrainVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;
};

#endif //!__GRASS_H__