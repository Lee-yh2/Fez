#include "stdafx.h"
#include "..\Headers\KeyManager.h"


CKeyManager::CKeyManager()
	:m_dwKey(0), m_dwKeyUp(0), m_dwKeyDown(0)
{
}


CKeyManager::~CKeyManager()
{
}

void CKeyManager::KeyCheck()
{
	m_dwKey = 0;
	if (GetAsyncKeyState(KEY_ADD) & 0x8000)
		m_dwKey |= KEY_ADD;
	if (GetAsyncKeyState(KEY_SUBTRACT) & 0x8000)
		m_dwKey |= KEY_SUBTRACT;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT_CLIENT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT_CLIENT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP_CLIENT;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN_CLIENT;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A_CLIENT;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D_CLIENT;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q_CLIENT;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E_CLIENT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE_CLIENT;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R_CLIENT;
	if (GetAsyncKeyState('F') & 0x8000)
		m_dwKey |= KEY_F_CLIENT;
	if (GetAsyncKeyState('G') & 0x8000)
		m_dwKey |= KEY_G_CLIENT;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwKey |= KEY_P_CLIENT;
	if (GetAsyncKeyState('T') & 0x8000)
		m_dwKey |= KEY_T_CLIENT;
	if (GetAsyncKeyState('Z') & 0x8000)
		m_dwKey |= KEY_Z_CLIENT;
	if (GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_1_CLIENT;
	if (GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_2_CLIENT;
	if (GetAsyncKeyState('3') & 0x8000)
		m_dwKey |= KEY_3_CLIENT;
	if (GetAsyncKeyState('4') & 0x8000)
		m_dwKey |= KEY_4_CLIENT;
	if (GetAsyncKeyState('5') & 0x8000)
		m_dwKey |= KEY_5_CLIENT;
	if (GetAsyncKeyState('6') & 0x8000)
		m_dwKey |= KEY_B2_CLIENT;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN_CLIENT;
	if (GetAsyncKeyState('K') & 0x8000) // 찬수 추가분
		m_dwKey |= KEY_K_CLIENT;
}

bool CKeyManager::KeyUp(DWORD dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	return false;
}

bool CKeyManager::KeyDown(DWORD dwKey)
{
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}
	return false;
}

bool CKeyManager::KeyPressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;
	return false;
}
