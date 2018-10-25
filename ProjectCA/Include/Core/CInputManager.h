#pragma once

/*
	NOTE:
		게임 내 전반적인 입력처리를 담당하는 Manager Class

*/

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class CInputManager : public Singleton<CInputManager> {

	DECLARE_SINGLETON(CInputManager)

public:
	bool Init();
	void Update();
	

public:
	void AddKey(const Types::tstring& strKeyName, SHORT keyCode);
	bool DeleteKey(const Types::tstring& strKeyName);
	bool ChangeKeyCode(const Types::tstring& strKeyName, SHORT keyCode);
	SHORT FindKeyCode(const Types::tstring& strKeyName);
	bool IsKeyDown(const Types::tstring& strKeyName);


private:
	void KeyProcess();
	void UpdateKeyDown();



private:
	std::vector<Types::KeyInfo> m_keyInfoList;


};

#define GET_KEY_DOWN(strName) CInputManager::GetInstance()->IsKeyDown(strName);