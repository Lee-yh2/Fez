#pragma once
#ifndef __BONUSCAMERA_H__

#include "Camera.h"

USING(Engine)
class CBonusCamera final : public CCamera
{
public:
	explicit CBonusCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CBonusCamera(const CBonusCamera& other);
	virtual ~CBonusCamera() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;	

private:
	HRESULT Movement(_float fDeltaTime);
	HRESULT KeyInput(_float fDeltaTime);

public:
	static CBonusCamera* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	CTransform* m_pTargetTransform = nullptr;
	_float m_fDistanceToTarget = 15.f;
	_float m_fCameraAngle = D3DXToRadian(50.f);

	_vec3	m_vCameraAngle;
};

#define __BONUSCAMERA_H__
#endif

