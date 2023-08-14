#pragma once

#ifndef __LOADINGBACKGROUND_H__
#define __LOADINGBACKGROUND_H__

#include "GameObject.h"

USING(Engine)

class CLoadingBackground final : public CGameObject
{
public:
	explicit CLoadingBackground(LPDIRECT3DDEVICE9 pDevice);
	explicit CLoadingBackground(const CLoadingBackground& other);
	virtual ~CLoadingBackground() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CLoadingBackground*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *		Clone(void * pArg = nullptr) override;
	virtual	void				Free()override;

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;
};

#endif //!__LOADINGBACKGROUND_H__