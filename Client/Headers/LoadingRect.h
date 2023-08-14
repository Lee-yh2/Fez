#pragma once

#ifndef __LOADINGRECT_H__
#define __LOADINGRECT_H__

#include "GameObject.h"

USING(Engine)

class CLoadingRect final : public CGameObject
{
public:
	explicit CLoadingRect(LPDIRECT3DDEVICE9 pDevice);
	explicit CLoadingRect(const CLoadingRect& other);
	virtual ~CLoadingRect() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CLoadingRect*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(const _float& fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;
};

#endif //!__LOADINGRECT_H__