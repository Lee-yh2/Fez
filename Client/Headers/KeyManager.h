#pragma once

#ifndef __KEYMANAGER_H__
#define __KEYMANAGER_H__

class CKeyManager
{
public:
	CKeyManager();
	~CKeyManager();

public:
	void KeyCheck();
	bool KeyUp(DWORD dwkey);
	bool KeyDown(DWORD dwkey);
	bool KeyPressing(DWORD dwkey);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyDown;
	DWORD m_dwKeyUp;
};

#endif // !__KEYMANAGER_H__

