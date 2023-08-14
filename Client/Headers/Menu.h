#pragma once

#ifndef __MENU_H__
#define __MENU_H__

#include "Scene.h"

USING(Engine)

class CMenu final : public CScene
{
private:
	explicit CMenu(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMenu() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float & fDeltaTime) override;
	virtual _uint Late_Update_Scene(_float & fDeltaTime) override;

public:
	static	CMenu*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual	void	Free()override;

private:
	HRESULT	Ready_Layer_StarBack();
	HRESULT	Ready_Layer_MenuFont();
	HRESULT	Ready_Layer_MenuFont2();
	HRESULT	Ready_Layer_Mouse();
	HRESULT	Ready_Layer_Rect();
	HRESULT	Ready_Layer_RectCol();
	HRESULT	Ready_Layer_RectRow();
	HRESULT	Ready_Layer_RectScaleFree();
	HRESULT	Ready_Layer_RectScaleFreeSmall();




private:
	HRESULT		Scene_Change();
	float		m_fCnt = 0.f;
	bool		m_bChange = false;

};

#endif //!__STAGE_H__