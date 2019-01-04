#pragma once

/*
	NOTE: 게임 내 전반적인 입력처리를 담당하는 Manager Class

*/

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class CInputManager : public Singleton<CInputManager> {

	DECLARE_SINGLETON(CInputManager)

	//InputManager 내에서 입력들의 관리를 위해 사용하게 될 내부 구조체.
	struct InputInfo {
		InputInfo(const TSTRING& strInputname, SHORT iInputCode, bool bCheckOnce = false, bool bPressed = false)
			:m_strInputName(strInputname), m_iInputCode(iInputCode), 
			m_bCheckOnce(bCheckOnce), m_bPressed(bPressed)
		{
		}

		InputInfo(InputInfo&& other)
			:m_strInputName(std::move(other.m_strInputName)), m_iInputCode(other.m_iInputCode), 
			m_bCheckOnce(other.m_bCheckOnce), m_bPressed(other.m_bPressed)
		{
		}

		bool operator ==(const InputInfo& other)
		{
			if (m_strInputName == other.m_strInputName && m_iInputCode == other.m_iInputCode
				&& m_bCheckOnce == other.m_bCheckOnce && m_bPressed == other.m_bPressed)
				return true;

			return false;
		}

		InputInfo& operator =(const InputInfo& other)
		{
			if (*this == other)
				return *this;

			this->m_strInputName	= other.m_strInputName;
			this->m_iInputCode		= other.m_iInputCode;
			this->m_bCheckOnce		= other.m_bCheckOnce;
			this->m_bPressed			= other.m_bPressed;

			return *this;
		}

		TSTRING		m_strInputName;
		SHORT		m_iInputCode;
		bool			m_bCheckOnce;
		bool			m_bPressed;
	};

public:
	bool Init();
	void Update();
	

public:
	void AddKey(const TSTRING& strKeyName, SHORT keyCode);
	void SetKeyCheckMode(const TSTRING& strKeyName, bool bCheckOnce);
	void SetKeyCheckMode(SHORT keyCode, bool bCheckOnce);
	bool DeleteKey(const TSTRING& strKeyName);
	bool ChangeKeyCode(const TSTRING& strKeyName, SHORT inputCode);
	SHORT FindKeyCode(const TSTRING& strKeyName);
	bool IsKeyDown(const TSTRING& strKeyName);
	bool IsKeyDown(const SHORT keyCode);


public:
	void SetMouseInput(const TSTRING& strInputName, bool bPressed);
	void SetMouseInput(const SHORT mouseCode, bool bPressed);
	bool IsMouseDown(const TSTRING& strInputName);
	bool IsMouseDown(const SHORT& mouseCode);
	void ClearMouseInputState();
	

private:
	void KeyProcess();
	void UpdateKeyDown();
	InputInfo* GetKeyInfo(const TSTRING& strKeyName);
	InputInfo* GetKeyInfo(SHORT keyCode);


private:
	//마우스에 대한 입력과 키보드에 대한 입력을 따로 관리한다.
	std::vector<InputInfo> m_KeyInputList;
	std::vector<InputInfo> m_MouseInputList;

};

#define GET_KEY_DOWN(strName) CInputManager::GetInstance()->IsKeyDown(strName);