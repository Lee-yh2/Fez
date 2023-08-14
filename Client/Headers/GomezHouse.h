#pragma once

#ifndef __GOMEZHOUSE_H__
#define __GOMEZHOUSE_H__

#include "Scene.h"

USING(Engine)

class CGomezHouse final : public CScene
{
private:
	explicit CGomezHouse(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile);
	virtual ~CGomezHouse() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float & fDeltaTime) override;
	virtual _uint Late_Update_Scene(_float & fDeltaTime) override;

public:
	static	CGomezHouse*	Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile);
	virtual	void	Free()override;

private:
	HRESULT	Ready_Layer_Player();
	HRESULT	Ready_Layer_VirtualCube();
	HRESULT	Ready_Layer_Portal();
	HRESULT	Ready_Layer_Camera();
	HRESULT	Ready_Layer_GomezHouse();
	HRESULT Ready_Layer_Obj();
	HRESULT Ready_Layer_Terrain();



//private:
//	HRESULT	Ready_Layer_Portal();
//	HRESULT	Ready_Layer_Monster();
//	HRESULT	Ready_Layer_Skybox();
//	HRESULT Ready_Layer_Obj();
//	HRESULT	Ready_Layer_Camera();
//	HRESULT	Ready_Layer_VirtualCube();

private:
	vector<TILEINFO*> m_vecTile;


};

#endif //!__STAGE_H__