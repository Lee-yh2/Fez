#pragma once

#ifndef __BONUSSTAGE2_H__
#define __BONUSSTAGE2_H__

#include "Scene.h"

USING(Engine)

class CBonusStage2 final : public CScene
{
public:
	explicit CBonusStage2(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile);
	virtual ~CBonusStage2() = default;

public:
	virtual HRESULT	Ready_Scene() override;
	virtual _uint	Update_Scene(_float & fDeltaTime) override;
	virtual _uint	Late_Update_Scene(_float & fDeltaTime) override;

public:
	static	CBonusStage2*	Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile);
	virtual	void			Free()override;

private:
	HRESULT	Ready_Layer_Terrain();
	HRESULT	Ready_Layer_Camera();
	HRESULT	Ready_Layer_Skybox();
	HRESULT	Ready_Layer_VirtualCube();
	HRESULT	Ready_Layer_UIcube();
	HRESULT	Ready_Layer_UIcubepiece();
	HRESULT	Ready_Layer_UIkey();
	HRESULT	Ready_Layer_Portal();
	HRESULT Ready_Layer_Effect();

private:
	_uint	Check_Clear();

private:
	vector<TILEINFO*>	m_vecTile;
	_bool				m_bClear;
};

#endif //!__BONUSSTAGE2_H__