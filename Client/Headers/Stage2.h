#pragma once

#ifndef __STAGE2_H__
#define __STAGE2_H_

#include "Scene.h"

USING(Engine)
class CStage2 final : public CScene
{
private:
	explicit CStage2(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile);
	virtual ~CStage2() = default;

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float & fDeltaTime) override;
	virtual _uint Late_Update_Scene(_float & fDeltaTime) override;

public:
	static	CStage2*	Create(LPDIRECT3DDEVICE9 pDevice, vector<TILEINFO*> vecTile);
	virtual	void		Free()override;

private:
	HRESULT	Ready_Layer_Portal();
	HRESULT	Ready_Layer_Terrain();
	HRESULT	Ready_Layer_Monster();
	HRESULT	Ready_Layer_Skybox();
	HRESULT Ready_Layer_Obj();
	HRESULT	Ready_Layer_Camera(); 
	HRESULT	Ready_Layer_VirtualCube();
	HRESULT	Ready_Layer_UIcube();
	HRESULT	Ready_Layer_UIcubepiece();
	HRESULT	Ready_Layer_UIkey();
	HRESULT Ready_Layer_Item();
	HRESULT Ready_Layer_BlackMonster();


	HRESULT Ready_Layer_Effect();
private:
	vector<TILEINFO*> m_vecTile;
};

#endif // !__STAGE2_H__
