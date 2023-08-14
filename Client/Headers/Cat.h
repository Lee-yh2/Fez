#pragma once

#ifndef __CAT_H__
#define __CAT_H__

#include "KeyManager.h"
#include "GameObject.h"
#include "Camera.h"

USING(Engine)

class CPlayer;

class CCat final : public CGameObject
{
public:
	explicit CCat(LPDIRECT3DDEVICE9 pDevice);
	explicit CCat(const CCat& other);
	virtual ~CCat() = default;

public:
	enum STATE { IDLE, JUMP, STAND, NUMOFSTATE };

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CCat*		Create(LPDIRECT3DDEVICE9 pDevice);
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
private:
	HRESULT	Movement(_float fDeltaTime);

protected:
	void FrameMove(_float fDeltaTime, float fMoveSpeed = 1.f);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom[NUMOFSTATE];
	STATE		m_eCurState;
	FRAME		m_tFrame;
	eMapDir		m_eMapDir;
	_uint		m_CubeNumber = 0;
	_float		m_StackDeltatime = 0.f;
	_bool		m_EatItem = false;
	_float		m_CubeTurnAngle = 0.f;
	_bool		m_ShowCube = false;
	_bool		m_OneCheck = false;
	_bool		m_TwoCheck = false;



private:
	_bool		HitCheck(_vec3 vCatPos, _vec3 vPlayerPos, _float fDis);

};

#endif //!__CAT_H__