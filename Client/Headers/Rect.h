#pragma once

#ifndef __RECT_H__
#define __RECT_H__

#include "GameObject.h"

USING(Engine)

class CRect final : public CGameObject
{
public:
	explicit CRect(LPDIRECT3DDEVICE9 pDevice);
	explicit CRect(const CRect& other);
	virtual ~CRect() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CRect*			Create(LPDIRECT3DDEVICE9 pDevice);
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
	int			m_iIndex = 0;

};

#endif //!__SKYBOX_H__