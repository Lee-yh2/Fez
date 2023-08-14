#pragma once

#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__

#include "Scene.h"

USING(Engine)

class CLoadingScene final : public CScene
{
public:
	explicit CLoadingScene(LPDIRECT3DDEVICE9 pDevice, eSceneType eNextScene);
	virtual ~CLoadingScene() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float & fDeltaTime) override;
	virtual _uint Late_Update_Scene(_float & fDeltaTime) override;

public:
	HRESULT	Ready_Layer_LoadingBackground();
	HRESULT	Ready_Layer_LoadingBar();
	HRESULT	Ready_Layer_LoadingRect();
	HRESULT LoadTileData(const wstring& wstrFilePath);

public:
	static	CLoadingScene*	Create(LPDIRECT3DDEVICE9 pDevice, eSceneType eNextScene, _bool _bFirst = true);
	virtual	void			Free()override;

private:
	static	_uint	__stdcall	ThreadMain(void* pParam);
	HRESULT						Ready_LogoResources();
	HRESULT						Ready_StageResources();
	HRESULT						Ready_Stage2Resources();
	HRESULT						Ready_Stage3Resources();
	HRESULT						Ready_GomezHouseResources();
	HRESULT						Ready_BonusStageResources();
	HRESULT						Ready_BonusStage2Resources();


private:
	vector<TILEINFO*>	m_vecTile;
	eSceneType			m_eNextScene;
	_bool				m_bIsFinished;
	HANDLE				m_hLoadingThread;
	CRITICAL_SECTION	m_tCriticalSection;
	_uint				m_iFileCount;

	_bool				m_bFirst;
private:
	HRESULT						Ready_MenuResources();

	/* 방 / 스테이지 구분 */
	bool						bStage = false;
};

#endif //!__LOADINGSCENE_H__