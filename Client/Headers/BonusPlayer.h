#pragma once
#ifndef __BONUSPLAYER_H__

#include "KeyManager.h"
#include "GameObject.h"

USING(Engine)
class CBonusPlayer final : public CGameObject
{
public:
	explicit CBonusPlayer(LPDIRECT3DDEVICE9 pDevice);
	explicit CBonusPlayer(const CBonusPlayer& other);
	virtual ~CBonusPlayer() = default;

public:
	enum STATE {
		IDLE_DOWN, IDLE_LEFT, IDLE_RIGHT, IDLE_UP, WALK_DOWN, WALK_LEFT, WALK_RIGHT, WALK_UP, NUMOFSTATE
	};
	enum DIR {
		LEFT, RIGHT, UP, DOWN
	};

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;	

private:
	HRESULT Movement(_float fDeltaTime);

public:
	static CBonusPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	void FrameMove(_float fDeltaTime, float fMoveSpeed = 1.f);
	void Scene_Change();

private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom[NUMOFSTATE];

	CKeyManager	m_tKeyManager;
	eSceneType m_eNextScene;

	STATE		m_eCurState;
	STATE		m_ePreState;
	FRAME		m_tFrame;

	_float		m_fCurPlayTime;
	DIR			m_eCurDir;

	
};

#define __BONUSPLAYER_H__
#endif

