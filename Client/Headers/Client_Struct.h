#pragma once

#ifndef __CLINET_STRUCT_H__
#define __CLINET_STRUCT_H__

typedef	struct tagJumpInformation
{
	_float	fJumpPower;
	_float	fJumpTime;
	_float	fJumpY;
	_int	iJumpCount;
}JUMPINFO;

typedef struct tagPathInfo
{
	wstring	wstrPath;
	wstring	wstrObjectKey;
	wstring	wstrStateKey;
	_uint	iCount;
	_uint	iTextureType;
}PATHINFO;

typedef	struct tagStageInfo
{
	_uint	iStageNum;
	_uint	iWinCX;
	_uint	iWinCY;
	_uint	IWinCZ;
}STAGEINFO;

typedef	struct tagTileInfo
{
	_vec3	vSize;
	_vec3	vColSize;
	_vec3	vRot;
	_vec3	vPos;
	BYTE	byOption;
	BYTE	byPlayDrawID;
	BYTE	byDevDrawID;
	_uint	iStageNum;
	_uint	iIndex;
	BYTE	btCollisionDir;
}TILEINFO;

typedef	struct tagObjectInfo 
{
	_vec3	vSize;
	_vec3	vColSize;
	_vec3	vRot;
	_vec3	vPos;
	_float	fPatrolRange;
	BYTE	byOption;
	BYTE	byPlayDrawID;
	_bool	bCubeTexture;
	_uint	iStageNum;
	_uint	iIndex;
}OBJECTINFO;

typedef struct
{
	float fFrameStart;
	float fFrameEnd;
}FRAME;

#endif //!__CLINET_STRUCT_H__