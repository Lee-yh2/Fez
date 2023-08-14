#pragma once

#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "GameObject.h"

USING(Engine)

class CMouse final : public CGameObject
{
public:
	explicit CMouse(LPDIRECT3DDEVICE9 pDevice);
	explicit CMouse(const CMouse& other);
	virtual ~CMouse() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CMouse*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;


	bool		m_bClose = false;
	int			m_iCursor = 0;


};

#endif //!__SKYBOX_H__