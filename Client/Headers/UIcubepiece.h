#pragma once

#ifndef __UICUBEPIECE_H__
#define __UICUBEPIECE_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;

class CUIcubepiece final : public CGameObject
{
public:
	explicit CUIcubepiece(LPDIRECT3DDEVICE9 pDevice);
	explicit CUIcubepiece(const CUIcubepiece& other);
	virtual ~CUIcubepiece() = default;

public:
	enum STATE { IDLE, WALK, JUMP, DEAD, NUMOFSTATE };

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CUIcubepiece*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;
	_vec3 change_view(POINT _pt, _uint _Wincx, _uint _Wincy, CCamera* pCameraconst, _vec4x4* pWorldMatrix);
	void Set_CubeNumber(_uint a) { m_CubeNumber += a; }
private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	CKeyManager	m_tKeyManager;
	_uint		m_CubeNumber = 0;
	_bool		m_EatItem = false;
};

#endif //!__UICUBEPIECE_H__