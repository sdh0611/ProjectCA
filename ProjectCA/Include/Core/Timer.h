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
	double GetDeltaTime() const { return m_dDeltaTime; }
	double GetTimeElapsed() const{ return m_dTimeElapsed; }
	void CheckFrameCount();
	void DrawFPS(const HDC& hDC);

private:
	double				m_dDeltaTime;
	double				m_dFPSTime;
	UINT					m_iFrameMax;
	double				m_dFrameRate;
	DWORD				m_iFrame;
	DWORD											m_frameCount;
	double											m_dTimeElapsed;
	std::chrono::system_clock::time_point		m_curTime;
	std::chrono::system_clock::time_point		m_lastTime;


};