#pragma once

/*
	NOTE:
		게임 내 MainWindow의 동작을 정의한 Class.
		이 Class를 제외한 다른 Window들은 MyWindow Class를 상속받아서 작성해야함.
		프로그램이 동작하는 동안 Instance가 1개임을 보장하기 위해 Singleton Template Class를 상속
*/

#include "..\..\..\stdafx.h"
#include "..\..\Singleton.hpp"


class MainWindow : public Singleton<MainWindow> {

	DECLARE_SINGLETON(MainWindow)
	
public:
	bool Init(HINSTANCE hInstance, UINT iWidth, UINT iHeight);
	void Update();
	void Render();


public:
	void SetWndSize(UINT iWidth, UINT iHeight);
	bool SetVisible(HWND hWnd, BOOL bIsVisible);
	UINT MessageLoop();
	const HINSTANCE& GetWindowInstance() const { return m_hInstance; }


private:
	bool CreateMyWindow(const Types::tstring& wndClassName,
		const Types::tstring& wndName, BOOL bIsVisible);


private:
	void DeadtimeLogic();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 
	

private:
	WNDCLASS			m_WndClass;
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;
	HDC					m_hDC;
	//HDC					m_hMemDC;
	UINT					m_iWidth;
	UINT					m_iHeight;
	HBITMAP			m_hBackground;
	HBRUSH				m_hBrush;
	HBRUSH				m_hOldBrush;


private:
	class CSceneManager*					m_pSceneManager;
	class Timer*									m_pTimer;
	class CResourceManager*				m_pResourceManager;
	std::unique_ptr<class BackBuffer>			m_pBackBuffer;

};