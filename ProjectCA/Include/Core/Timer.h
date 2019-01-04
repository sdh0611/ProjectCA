#pragma once

/*
	NOTE:
		게임 내의 시간, Frame등의 관리를 위한 Manager Class
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
	double											m_dDeltaTime;
	double											m_dFPSTime;
	UINT												m_iFrameMax;
	double											m_dFrameRate;
	DWORD											m_iFrame;
	DWORD											m_frameCount;
	double											m_dTimeElapsed;
	std::chrono::system_clock::time_point		m_curTime;
	std::chrono::system_clock::time_point		m_lastTime;


};