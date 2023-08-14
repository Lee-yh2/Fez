#pragma once

#ifndef __STAGE_H__
#define __STAGE_H__

#include "Scene.h"

USING(Engine)

class CStage final : public CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile, _bool _bFirst);
	virtual ~CStage() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float & fDeltaTime) override;
	virtual _uint Late_Update_Scene(_float & fDeltaTime) override;

public:
	static	CStage*	Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile, _bool _bFirst = true);
	virtual	void	Free()override;

private:
	HRESULT	Ready_Layer_Player();
	HRESULT	Ready_Layer_VirtualCube();
	HRESULT	Ready_Layer_Portal();
	HRESULT	Ready_Layer_Camera();
	HRESULT	Ready_Layer_Monster();
	HRESULT	Ready_Layer_Skybox();
	HRESULT Ready_Layer_Obj();
	HRESULT Ready_Layer_Terrain();
	HRESULT Ready_Layer_Vine();
	HRESULT	Ready_Layer_UIcube();
	HRESULT	Ready_Layer_UIcubepiece();
	HRESULT	Ready_Layer_UIkey();
	HRESULT Ready_Layer_Item();

	HRESULT Ready_Layer_NPC();
	HRESULT Ready_Layer_Bird();
	HRESULT Ready_Layer_Cat();
	HRESULT Ready_Layer_SpeechBubble();
	HRESULT Ready_Layer_Effect();

private:
	vector<TILEINFO*> m_vecTile;

	_bool	m_bStart;
	_bool	m_bStartPosFinished;
	_bool	m_bIntroFinished;
	_float	m_fStartTime;

	_bool	m_bFirst;
};

#endif //!__STAGE_H__