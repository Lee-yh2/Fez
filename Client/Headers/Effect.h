#pragma once

#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CEffect;

class CEffect final : public CGameObject
{
public:
	explicit CEffect(LPDIRECT3DDEVICE9 pDevice);
	explicit CEffect(const CEffect& other);
	virtual ~CEffect() = default;
	enum STATE { BLIND, LIGHT, ROTATE, NUMOFSTATE };
public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CEffect*		Create(LPDIRECT3DDEVICE9 pDevice);
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
	_uint return_condition()
	{
		return m_ICondition;
	}
	void Set_condition(_uint _num)
	{
		m_ICondition = _num;
	}
private:
	HRESULT	Movement(_float fDeltaTime);
	HRESULT	Movement2(_float fDeltaTime);
	HRESULT	Movement3(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	_uint		m_ICondition = BLIND;
	_float		m_StackDeltatime = 0.f;
	_bool		m_ShowCube = false;
	_float		m_fSizeUpX = 0.f;
	_float		m_fSizeUpY = 0.f;
	//_float		m_ItemEatTime = 0.f;
};

#endif //!__EFFECT_H__