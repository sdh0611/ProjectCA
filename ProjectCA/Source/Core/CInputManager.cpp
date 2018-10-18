#include "..\..\stdafx.h"
#include "..\..\Include\Core\CInputManager.h"


bool CInputManager::Init() {
	m_keyInfoList.reserve(10);
	
	m_keyInfoList.emplace_back(Types::KeyInfo(TEXT("LEFT"), VK_LEFT));
	m_keyInfoList.emplace_back(Types::KeyInfo(TEXT("RIGHT"), VK_RIGHT));
	m_keyInfoList.emplace_back(Types::KeyInfo(TEXT("UP"), VK_UP));
	m_keyInfoList.emplace_back(Types::KeyInfo(TEXT("DOWN"), VK_DOWN));
	m_keyInfoList.emplace_back(Types::KeyInfo(TEXT("JUMP"), 'X'));
	m_keyInfoList.emplace_back(Types::KeyInfo(TEXT("ACCEL"), 'A'));

	return true;
}

void CInputManager::Update()
{
	UpdateKeyDown();
}

void CInputManager::AddKey(const Types::tstring& strKeyName, SHORT keyCode)
{
	if (FindKeyCode(strKeyName) != -1)
		return;

	m_keyInfoList.emplace_back(Types::KeyInfo(strKeyName, keyCode));

}

bool CInputManager::DeleteKey(const Types::tstring& strKeyName)
{
	for(auto it = m_keyInfoList.begin(); it != m_keyInfoList.end(); ++it)
	{
		if ((*it).m_strKeyName == strKeyName)
		{
			m_keyInfoList.erase(it);
			return true;
		}
	}	

	return false;
}

bool CInputManager::ChangeKeyCode(const Types::tstring& strKeyName, SHORT keyCode)
{
	for (auto& it : m_keyInfoList)
	{
		if (it.m_strKeyName == strKeyName)
		{
			it.m_iKeyCode = keyCode;
			return true;
		}
	}


	return true;
}

SHORT CInputManager::FindKeyCode(const Types::tstring & strKeyName)
{
	for (auto& it : m_keyInfoList)
	{
		if (it.m_strKeyName == strKeyName)
			return it.m_iKeyCode;
	}

	return -1;
}

bool CInputManager::IsKeyDown(const Types::tstring & strKeyName)
{
	for (auto& it : m_keyInfoList)
	{
		if (strKeyName == it.m_strKeyName)
		{
			return it.m_bPressed;
		}

	}

	return false;
}

void CInputManager::KeyProcess()
{




}

void CInputManager::UpdateKeyDown()
{
	for (auto& it : m_keyInfoList)
	{
		if (KEY_DOWN(it.m_iKeyCode))
			it.m_bPressed = true;
		else
			it.m_bPressed = false;

	}

}
