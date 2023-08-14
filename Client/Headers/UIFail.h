#pragma once

#ifndef __UIFAIL_H__
#define __UIFAIL_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;

class CUIFail final : public CGameObject
{
public:
	explicit CUIFail(LPDIRECT3DDEVICE9 pDevice);
	explicit CUIFail(const CUIFail& other);
	virtual ~CUIFail() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CUIFail*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;
	_vec3 change_view(POINT _pt, _uint _Wincx, _uint _Wincy, CCamera* pCameraconst, _vec4x4* pWorldMatrix);
	void Set_GameOver() { m_bGameOver = true; }

private:
	HRESULT	Movement(_float fDeltaTime);
	
private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	_uint		m_CubeNumber = 0;
	_float		m_StackDeltatime = 0.f;

	_bool		m_bGameOver;
};

#endif //!__UIFAIL_H__