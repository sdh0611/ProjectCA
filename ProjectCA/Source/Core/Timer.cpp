#include "..\..\stdafx.h"
#include "..\..\Include\Core\Timer.h"

Timer::Timer() {


}

Timer::~Timer() {

}


bool Timer::Init()
{
	//QueryPerformanceFrequency(&m_tSecond);
	//QueryPerformanceCounter(&m_tTime);

	m_dDeltaTime = 0.f;
	m_dFPSTime = 0.f;
	m_iFrameMax = 60;
	m_dFrameRate = 1 / m_iFrameMax;
	m_iFrame = 0;

	m_frameCount = 0;
	m_lastTime = std::chrono::system_clock::now();
	m_dTimeElapsed = 0.f;

	return true;
}

void Timer::Update()
{
	m_curTime = std::chrono::system_clock::now();
	m_dDeltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(m_curTime - m_lastTime).count();
	
	m_dTimeElapsed += m_dDeltaTime;

	m_lastTime = m_curTime;

}

void Timer::CheckFrameCount()
{
	if (m_dTimeElapsed >= 1.f) 
	{
		m_iFrame = m_frameCount / m_dTimeElapsed;
		m_frameCount = 0;
		m_dTimeElapsed = 0.f;
	}
	else 
	{
		++m_frameCount;
	}

}

void Timer::DrawFPS(const HDC& hDC) {

	static TCHAR str[256];

	wsprintf(str, TEXT("FPS : %d"), m_iFrame);
	TextOut(hDC, 0, 0, str, wcslen(str));

}