#pragma once

/*
	NOTE:
		게임 내 전반적인 입력처리를 담당하는 Manager Class

*/

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class InputManager : public Singleton<InputManager> {

	DECLARE_SINGLETON(InputManager)

public:
	bool Init();
	void Update();
	

public:



private:
	void KeyProcess();



private:





};