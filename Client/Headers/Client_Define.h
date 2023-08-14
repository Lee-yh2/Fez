#pragma once

#ifndef __CLINET_DEFINE_H__
#define __CLINET_DEFINE_H__

#define CLIENTWINCX	1920
#define CLIENTWINCY	1080
#define WINCX 800
#define WINCY 600
#define PLAYERCX 50
#define PLAYERCY 50
#define TILECX 40
#define TILECY 40

#define GRAVITY 6.f

#pragma region	Player_Rotate_Dir
#define RIGHT_ROT	0
#define LEFT_ROT	1
#pragma  endregion

#pragma region	Client_Define
#define KEY_RETURN_CLIENT		0x00000001
#define KEY_SPACE_CLIENT		0x00000002
#define	KEY_LEFT_CLIENT			0x00000004
#define KEY_RIGHT_CLIENT		0x00000008
#define KEY_UP_CLIENT			0x00000010
#define KEY_DOWN_CLIENT			0x00000020
#define KEY_A_CLIENT			0x00000040
#define KEY_D_CLIENT			0x00000080
#define KEY_E_CLIENT			0x00000100
#define KEY_Q_CLIENT			0x00000200
#define KEY_R_CLIENT			0x00000400 // Normal Bullet 持失
#define KEY_F_CLIENT			0x00000800
#define KEY_G_CLIENT			0x00001000
#define KEY_P_CLIENT			0x00002000
#define KEY_T_CLIENT			0x00004000 // Meteor Bullet 持失
#define KEY_Z_CLIENT			0x00008000 // 左什 社発
#define KEY_1_CLIENT			0x00010000
#define KEY_2_CLIENT			0x00020000
#define KEY_3_CLIENT			0x00040000
#define KEY_4_CLIENT			0x00080000
#define KEY_5_CLIENT			0x00100000
#define KEY_B2_CLIENT			0x00200000
#define KEY_K_CLIENT			0x00400000
#pragma endregion

#pragma region	Tool_Define
#define KEY_RETURN		0x00000001
#define KEY_SPACE		0x00000002
#define KEY_LBUTTON		0x00000004
#define	KEY_LEFT		0x00000008
#define KEY_RIGHT		0x00000010
#define KEY_ADD			0x00000020
#define KEY_SUBTRACT	0x00000040
#define KEY_W			0x00000080
#define KEY_A			0x00000100
#define KEY_S			0x00000200
#define KEY_D			0x00000400
#define KEY_UP			0x00000800
#define KEY_DOWN		0x00001000
#pragma endregion

#pragma region	TileOption
#define TILE_COL		0x01
#define TILE_INTER		0x02
#define TILE_VINE0		0x04
#define	TILE_VINE90		0x08
#define TILE_VINE180	0x10
#define TILE_VINE270	0x20
#pragma endregion

#pragma region	ObjectOption
#define OBJECT_COL		0x01
#define OBJECT_INTER	0x02
#define OBJECT_PATROLX	0x04
#define OBJECT_PATROLY	0x08
#define OBJECT_PATROLZ	0x10
#pragma endregion

#pragma region TileCollisionDir
#define TILECOL_DIR_SOUTH	0x01
#define TILECOL_DIR_NORTH   0x02
#define TILECOL_DIR_EAST	0x04
#define TILECOL_DIR_WEST	0x08
#define TILECOL_INTER		0x10
#pragma endregion


#endif //!__CLINET_DEFINE_H__