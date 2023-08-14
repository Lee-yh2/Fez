#pragma once

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "GameObject.h"

USING(Engine)

class CSkybox final : public CGameObject
{
public:
	explicit CSkybox(LPDIRECT3DDEVICE9 pDevice);
	explicit CSkybox(const CSkybox& other);
	virtual ~CSkybox() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CSkybox*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

public:
	void Set_StageNum(_uint iStageNum) { m_iStageNum = iStageNum; }

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	CTexture*	m_pTextureCom;
	CTransform*	m_pTransformCom;

	_uint		m_iStageNum;
};

#endif //!__SKYBOX_H__