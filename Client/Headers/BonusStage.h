#pragma once
#ifndef __BONUSSTAGE_H__

#include "Scene.h"

USING(Engine)
class CBonusStage final : public CScene
{
private:
	explicit CBonusStage(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile);
	virtual ~CBonusStage() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float & fDeltaTime) override;
	virtual _uint Late_Update_Scene(_float & fDeltaTime) override;

private:
	HRESULT Ready_Layer_Player();
	HRESULT Ready_Layer_Terrain();
	HRESULT Ready_Layer_Camera();
	HRESULT Ready_Layer_Monster();
	HRESULT Ready_Layer_Grass();
	HRESULT Ready_Layer_Skybox();
	HRESULT Ready_Layer_Fail();
	HRESULT Ready_Layer_Success();


private:
	HRESULT Wave_Set();

public:
	static CBonusStage* Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile);
	virtual void Free() override;

private:
	_float		m_fCurPlayTime;

	_bool		m_bWave1, m_bWave2, m_bWave3;
	_bool		m_bClearStage;
	_bool		m_bFailStage;
};

#define __BONUSSTAGE_H__
#endif