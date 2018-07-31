#pragma once

/*
NOTE:
	게임 내에서 생성하게 될 Window들에 대해 정의한 기본Class
	Init() 메소드를 이용해서만 초기화 가능하게 만듦.
	MainWindow를 제외한 게임 내 Window관련 Class들은 반드시 이 클래스를 상속받아서 작성.
*/

#include "..\..\..\stdafx.h"


class MyWindow {

public:
	MyWindow() { };
	virtual ~MyWindow();


public:
	virtual bool Init(HINSTANCE hInstance, UINT iWidth, UINT iHeight);
	virtual void Update(const float fDeltaTime);
	virtual void Render(const float fDeltaTime);


public:
	void SetWndSize(UINT iWidth, UINT iHeight);
	bool SetVisible(HWND hWnd, BOOL bIsVisible);
	

protected:
	void DeadtimeLogic();
	bool CreateMyWindow(const Types::tstring& wndClassName,
		const Types::tstring& wndName, BOOL bIsVisible);
	UINT MessageLoop();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //정적 메소드이기 때문에 virtual키워드는 따로 안붙임.



protected:
	WNDCLASS			m_WndClass;
	HWND				m_hWnd;
	HINSTANCE			m_hInstance;
	HDC					m_hDC;
	UINT					m_iWidth;
	UINT					m_iHeight;


protected:
	//class CSceneManager*			m_pSceneManager;



};