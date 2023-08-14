#pragma once

#ifndef __UIKEY_H__
#define __UIKEY_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;

class CUIkey final : public CGameObject
{
public:
	explicit CUIkey(LPDIRECT3DDEVICE9 pDevice);
	explicit CUIkey(const CUIkey& other);
	virtual ~CUIkey() = default;

public:
	enum STATE { IDLE, WALK, JUMP, DEAD, NUMOFSTATE };

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CUIkey*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;
	_vec3 change_view(POINT _pt, _uint _Wincx, _uint _Wincy, CCamera* pCameraconst, _vec4x4* pWorldMatrix);

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	_uint		m_CubeNumber = 0;
	_float		m_StackDeltatime = 0.f;
	_bool		m_EatItem = false;
};

#endif //!__UIKEY_H__