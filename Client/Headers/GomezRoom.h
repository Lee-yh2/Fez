#pragma once

#ifndef __GOMEZROOM_H__
#define __GOMEZROOM_H__

#include "GameObject.h"

USING(Engine)

class CGomezRoom final : public CGameObject
{
public:
	explicit CGomezRoom(LPDIRECT3DDEVICE9 pDevice);
	explicit CGomezRoom(const CGomezRoom& other);
	virtual ~CGomezRoom() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CGomezRoom*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;


	/* 직사각형~정사각형 위치 변경 */
private:
	bool	m_bRotate = false;
	bool	m_bRectangle = false;		// 직사각형
};

#endif //!__SKYBOX_H__