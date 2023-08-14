#pragma once

#ifndef __RECTSCALEFREESMMALL_H__
#define __RECTSCALEFREESMMALL_H__

#include "GameObject.h"

USING(Engine)

class CRectScaleFreeSmall final : public CGameObject
{
public:
	explicit CRectScaleFreeSmall(LPDIRECT3DDEVICE9 pDevice);
	explicit CRectScaleFreeSmall(const CRectScaleFreeSmall& other);
	virtual ~CRectScaleFreeSmall() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CRectScaleFreeSmall*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *		Clone(void * pArg = nullptr) override;
	virtual	void				Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;


	bool		m_bClose = false;


private:
	/* 대기 */
	float		m_fCnt = 0.f;
	bool		m_bCnt = true;
	
	/* 이동 */
	bool		m_bScaling = false;
	float		m_fAccel = 0.f;
	bool		m_bMove = false;
	bool		m_bMoveUp = false;
	bool		m_bMoveDown = false;

};

#endif //!__SKYBOX_H__