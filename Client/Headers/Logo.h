#pragma once



#ifndef __LOGO_H__
#define __LOGO_H__

#include "Scene.h"



USING(Engine)

class CLogo final : public CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogo() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float & fDeltaTime) override;
	virtual _uint Late_Update_Scene(_float & fDeltaTime) override;

public:
	static	CLogo*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual	void	Free()override;

private:
	DWORD	m_dwKey;


	IVideoWindow *pVidWin = NULL;



	bool	m_bPlay = true;
};

#endif //!__LOGO_H__