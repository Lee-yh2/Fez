#pragma once

#ifndef __LOADINGBAR_H__
#define __LOADINGBAR_H__

#include "GameObject.h"

USING(Engine)

class CLoadingBar final : public CGameObject
{
public:
	explicit CLoadingBar(LPDIRECT3DDEVICE9 pDevice);
	explicit CLoadingBar(const CLoadingBar& other);
	virtual ~CLoadingBar() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CLoadingBar*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;
};

#endif //!__LOADINGBAR_H__