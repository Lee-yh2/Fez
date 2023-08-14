#pragma once

#ifndef __ITEM_H__
#define __ITEM_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;
class CVirtualCube;
class CItem final : public CGameObject
{
public:
	explicit CItem(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem(const CItem& other);
	virtual ~CItem() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CItem*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free() override;

public:
	void Set_CubeNum(_uint _num)
	{
		m_CubeNum = _num;
	}
	void ShowCube()
	{
		m_ShowCube = true;
	}
private:
	HRESULT	Movement(_float fDeltaTime);
	HRESULT	Movement2(_float fDeltaTime);
	HRESULT	Movement3(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	CKeyManager	m_tKeyManager;
	_float		m_StackDeltatime = 0.f;
	_float		m_StackDeltatime2 = 0.f;
	_uint		m_EatItem = 0;
	_float		m_Onecheck = 0.f;
	_bool		m_Twocheck = true;
	_float		m_Threecheck = 0.f;
	_float		m_FourCheck = false;
	_float		m_RotAngel = 0.f;
	_float		m_RotAngel2 = 0.f;
	_float		m_PlusAxisAngle = 0.f;
	_float		m_moveX = 0.f;
	_float		m_moveY = 0.f;
	_float		m_moveZ = 0.f;
	_int		m_CubeNum = 0;
	_bool		m_ShowCube = false;
};

#endif //!__ITEM_H__