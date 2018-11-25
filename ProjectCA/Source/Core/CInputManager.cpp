#include "..\..\stdafx.h"
#include "..\..\Include\Core\CInputManager.h"


CInputManager::CInputManager() 
{
}

CInputManager::~CInputManager()
{

}

bool CInputManager::Init() {
	
	m_KeyInputList.emplace_back(TEXT("LEFT"), VK_LEFT);
	m_KeyInputList.emplace_back(TEXT("RIGHT"), VK_RIGHT);
	m_KeyInputList.emplace_back(TEXT("UP"), VK_UP);
	m_KeyInputList.emplace_back(TEXT("DOWN"), VK_DOWN);
	m_KeyInputList.emplace_back(TEXT("JUMP"), 'X');
	m_KeyInputList.emplace_back(TEXT("ACCEL"), 'A');
	m_KeyInputList.emplace_back(TEXT("ATTACK"), 'C', true);
	m_KeyInputList.emplace_back(TEXT("FUNC1"), VK_SHIFT, true);
	m_KeyInputList.emplace_back(TEXT("RESET"), VK_ESCAPE, true);

	m_MouseInputList.emplace_back(TEXT("LBUTTON_DBCLK"), WM_LBUTTONDBLCLK);
	m_MouseInputList.emplace_back(TEXT("LBUTTON"), WM_LBUTTONDOWN);
	m_MouseInputList.emplace_back(TEXT("RBUTTON_DBCLK"), WM_RBUTTONDBLCLK);
	m_MouseInputList.emplace_back(TEXT("RBUTTON"), WM_RBUTTONDOWN);

	return true;
}

void CInputManager::Update()
{
	UpdateKeyDown();
}

void CInputManager::AddKey(const TSTRING& strKeyName, SHORT keyCode)
{
	if (FindKeyCode(strKeyName) != -1)
		return;

	m_KeyInputList.emplace_back(InputInfo(strKeyName, keyCode));

}

bool CInputManager::DeleteKey(const TSTRING& strKeyName)
{
	for(auto it = m_KeyInputList.begin(); it != m_KeyInputList.end(); ++it)
	{
		if ((*it).m_strInputName == strKeyName)
		{
			m_KeyInputList.erase(it);
			return true;
		}
	}	

	return false;
}

bool CInputManager::ChangeKeyCode(const TSTRING& strKeyName, SHORT inputCode)
{
	for (auto& it : m_KeyInputList)
	{
		if (it.m_strInputName == strKeyName)
		{
			it.m_iInputCode = inputCode;
			return true;
		}
	}

	

	return true;
}

SHORT CInputManager::FindKeyCode(const TSTRING & strKeyName)
{
	for (const auto& it : m_KeyInputList)
	{
		if (it.m_strInputName == strKeyName)
			return it.m_iInputCode;
	}


	return -1;
}

bool CInputManager::IsKeyDown(const TSTRING & strKeyName)
{
	for (const auto& it : m_KeyInputList)
	{
		if (strKeyName == it.m_strInputName)
		{
			return it.m_bPressed;
		}

	}

	return false;
}

bool CInputManager::IsKeyDown(const SHORT keyCode)
{
	for (auto& it : m_KeyInputList)
	{
		if (keyCode== it.m_iInputCode)
		{
			return it.m_bPressed;
		}

	}

	return false;
}

void CInputManager::ClearMouseInputState()
{
	for (auto& inputInfo : m_MouseInputList)
	{
		inputInfo.m_bPressed = false;
	}

}

void CInputManager::SetMouseInput(const TSTRING & strInputName, bool bPressed)
{
	for (auto& inputInfo : m_MouseInputList)
	{
		if (inputInfo.m_strInputName == strInputName)
		{
			inputInfo.m_bPressed = bPressed;
		}
	}

}

void CInputManager::SetMouseInput(const SHORT mouseCode, bool bPressed)
{
	for (auto& inputInfo : m_MouseInputList)
	{
		if (inputInfo.m_iInputCode== mouseCode)
		{
			inputInfo.m_bPressed = bPressed;
		}
	}
}

bool CInputManager::IsMouseDown(const TSTRING & strInputName)
{
	for (const auto& inputInfo : m_MouseInputList)
	{
		if (inputInfo.m_strInputName == strInputName)
		{
			return inputInfo.m_bPressed;
		}
	}

	return false;
}

bool CInputManager::IsMouseDown(const SHORT & mouseCode)
{
	for (const auto& inputInfo : m_MouseInputList)
	{
		if (inputInfo.m_iInputCode== mouseCode)
		{
			return inputInfo.m_bPressed;
		}
	}

	return false;
}

void CInputManager::KeyProcess()
{

}

void CInputManager::UpdateKeyDown()
{
	for (auto& key : m_KeyInputList)
	{
		if (key.m_bCheckOnce)
		{
			if (KEY_ONCE_PRESS(key.m_iInputCode))
			{
				key.m_bPressed = true;
			}
			else
			{
				key.m_bPressed = false;
			}
		}
		else
		{
			if (KEY_DOWN(key.m_iInputCode))
			{
				key.m_bPressed = true;
			}
			else
			{
				key.m_bPressed = false;
			}
		}
	}

}
