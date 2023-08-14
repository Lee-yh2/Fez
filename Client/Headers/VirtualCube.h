#pragma once

#ifndef __VIRTUALCUBE_H__
#define __VIRTUALCUBE_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;

class CVirtualCube final : public CGameObject
{
public:
	explicit CVirtualCube(LPDIRECT3DDEVICE9 pDevice);
	explicit CVirtualCube(const CVirtualCube& other);
	virtual ~CVirtualCube() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CVirtualCube*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;

public:
	CTransform* Return_Transform()
	{
		return m_pTransformCom;
	}
	void ShowCube()
	{
		m_ShowCube = true;
	}
	_bool return_Cube()
	{
		return m_ShowCube;
	}
	void Say_FullCube(_bool _Say)
	{
		m_bIsFullCube = _Say;
	}
private:
	HRESULT	Movement(_float fDeltaTime);
	HRESULT	Movement2(_float fDeltaTime);
private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	_uint		m_CubeNumber = 0;
	_float		m_StackDeltatime = 0.f;
	_bool		m_EatItem = false;
	_float		m_CubeTurnAngle = 0.f;
	_bool		m_ShowCube = false;
	_bool		m_bIsFullCube = false;
};

#endif //!__VIRTUALCUBE_H__