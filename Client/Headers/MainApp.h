#pragma once

#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#include "Base.h"

USING(Engine)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual	~CMainApp() = default;

public:
	HRESULT	Ready_MainApp();
	_uint	Update_MainApp();

public:
	static	CMainApp*	Create();
	virtual	void		Free()override;

private:
	HRESULT	Ready_StaticResources();
	HRESULT	Ready_DefaultSetting();

private:
	CManagement*		m_pManagement;
	LPDIRECT3DDEVICE9	m_pDevice;

	/* µ¿¿µ»ó*/
private:
	bool				m_bCreate;
};

#endif //!__MAINAPP_H__
