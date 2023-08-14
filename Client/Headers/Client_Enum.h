#pragma once

#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

enum class eSceneType
{
	Logo, Menu, Loading,
	Stage,
	GomezHouse,
	Stage2, Stage3,
	BonusStage, BonusStage2,
	NumOfScene
};

enum class eMapDir
{
	EAST, WEST, SOUTH, NORTH, NOTHING
};


#endif //!__CLIENT_ENUM_H__