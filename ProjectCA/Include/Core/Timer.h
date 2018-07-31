#pragma once

/*
	NOTE:
		게임 내의 Frame 과 관련된 제어를 실시하는 Class
		두 개 이상 존재할 필요가 없으므로 Singleton객체로 작성
*/


#include "..\..\stdafx.h"
#include	"..\Singleton.hpp"


class Timer : public Singleton<Timer>{

	DECLARE_SINGLETON(Timer)

public:
	bool Init();
	void Update();


public:
	float GetDeltaTime() const { return m_fDeltaTime; }
	float GetFPS()	const { return m_fFPS; }


private:
	LARGE_INTEGER	m_tSecond;
	LARGE_INTEGER	m_tTime;
	float					m_fDeltaTime;
	float					m_fFPS;
	float					m_fFPSTime;
	float					m_iFrameMax;
	float					m_iFrame;



};