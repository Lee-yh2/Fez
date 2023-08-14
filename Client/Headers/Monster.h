#pragma once

#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "GameObject.h"

USING(Engine)

class CMonster final : public CGameObject
{
public:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonster(const CMonster& other);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint Late_Update_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static	CMonster*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual	void			Free()override;

public:
	void	Set_DevMode(const _bool& bDevMode) { m_bDevMode = bDevMode; }
	void	Set_StartPos(const _vec3& vStartPos, const _float& fTotalTime);
	void	Set_Start(const _bool& bStart) { m_bStart = bStart; }
	void	Change_TileByPlayDrawID(_uint iNum) { m_tTileInfo.byPlayDrawID = iNum; } // 찬수 추가분

public:
	TILEINFO* Get_TileInfo() { return &m_tTileInfo; }

private:
	HRESULT	Movement(_float fDeltaTime);

private:
	CVIBuffer*	m_pVIBufferCom;
	//CVIBuffer*	m_pTerrainVIBufferCom;
	CTexture*	m_pPlayTextureCom;
	CTexture*	m_pDevTextureCom;
	CTransform*	m_pTransformCom;

	TILEINFO	m_tTileInfo;
	_bool		m_bDevMode;

	_bool		m_bStart;
	_vec3		m_vStartDir;
	_float		m_fTotalTime;
	_float		m_fStartTime;
};

#endif //!__MONSTER_H__