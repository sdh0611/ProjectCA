#pragma once

/*
	NOTE:
		게임 내 전반적인 입력처리를 담당하는 Manager Class

*/

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class CInputManager : public Singleton<CInputManager> {

	DECLARE_SINGLETON(CInputManager)

	struct InputInfo {
		InputInfo(const Types::tstring& strInputname, SHORT iInputCode, bool bPressed = false)
			:m_strInputName(strInputname), m_iInputCode(iInputCode), m_bPressed(bPressed)
		{
		}

		InputInfo(InputInfo&& other)
			:m_strInputName(std::move(other.m_strInputName)), m_iInputCode(other.m_iInputCode), m_bPressed(other.m_bPressed)
		{
		}

		bool operator ==(const InputInfo& other)
		{
			if (m_strInputName == other.m_strInputName && m_iInputCode == other.m_iInputCode
				&& m_bPressed == other.m_bPressed)
				return true;

			return false;
		}

		InputInfo& operator =(const InputInfo& other)
		{
			if (*this == other)
				return *this;

			this->m_strInputName = other.m_strInputName;
			this->m_iInputCode = other.m_iInputCode;
			this->m_bPressed = other.m_bPressed;

			return *this;
		}

		Types::tstring	m_strInputName;
		SHORT			m_iInputCode;
		bool				m_bPressed;
	};

public:
	bool Init();
	void Update();
	

public:
	void AddKey(const TSTRING& strKeyName, SHORT keyCode);
	bool DeleteKey(const TSTRING& strKeyName);
	bool ChangeKeyCode(const TSTRING& strKeyName, SHORT inputCode);
	SHORT FindKeyCode(const TSTRING& strKeyName);
	bool IsKeyDown(const TSTRING& strKeyName);
	bool IsKeyDown(const SHORT keyCode);


public:
	void ClearMouseInputState();
	void SetMouseInput(const TSTRING& strInputName, bool bPressed);
	void SetMouseInput(const SHORT mouseCode, bool bPressed);
	bool IsMouseDown(const TSTRING& strInputName);
	bool IsMouseDown(const SHORT& mouseCode);


private:
	void KeyProcess();
	void UpdateKeyDown();


private:
	std::vector<InputInfo> m_KeyInputList;
	std::vector<InputInfo> m_MouseInputList;

};

#define GET_KEY_DOWN(strName) CInputManager::GetInstance()->IsKeyDown(strName);