#pragma once

#ifndef __MAINCAMERA_H__
#define __MAINCAMERA_H__

#include "Camera.h"
#include "KeyManager.h"

USING(Engine)

class CMainCamera final : public CCamera
{
public:
	explicit CMainCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CMainCamera(const CMainCamera& other);
	virtual	~CMainCamera() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CMainCamera*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

public:
	const	_vec3&	Get_CameraAngle()const { return m_vCameraAngle; }

public:
	void	Set_Start(const _bool& bStart) { m_bStart = bStart; }
	void	Set_ParentPos(const _vec3& vParentPos) { m_vParentPos = vParentPos; }
	void	Set_CameraAngle(const _vec3& vCameraAngle) { m_vCameraAngle = vCameraAngle; }
	// 찬수 추가분
	void	Set_BeginCameraMovingBonusStage() { m_bCameraMovingBonusStage = true; }
	void	Begin_CameraMovingBonusStage(_float fDeltaTime);

private:
	HRESULT	KeyInput(_float fDeltaTime);
	HRESULT	Movement(_float fDeltaTime);

private:
	CTransform*	m_pCameraTransform;
	CTransform*	m_pTarget_RoomTransform;
	CTransform*	m_pTarget_PlayerTransform;
	_float		m_fDistanceToTarget;

	CKeyManager	m_tKeyManager;

	_vec3		m_vCameraAngle;
	_float		m_fOldCameraZAngle;
	_int		m_iZRotCount;

	_bool		m_bCameraMode;
	DWORD		m_dwKey;

	_bool		m_bStart;
	_vec3		m_vParentPos;

	_bool		m_bFinish;
	//찬수 추가분
	_bool		m_bCameraMovingBonusStage;
};

#endif //!__MAINCAMERA_H__