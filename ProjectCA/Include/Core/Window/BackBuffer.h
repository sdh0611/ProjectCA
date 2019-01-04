#pragma once

#include "..\..\..\stdafx.h"
#include "..\..\Singleton.hpp"

//NOTE: 더블버퍼링을 위해 구현한 Manager클래스.
//			MainWindow의 DC를 받아 BackDC를 생성

class BackBuffer : public Singleton<BackBuffer>{

	DECLARE_SINGLETON(BackBuffer)

public:
	bool Init(const HDC& hDC);
	void Update();
	bool DrawSet(const HDC& hDC);
	bool DrawEnd();


public:
	//bool SetLoadBit();
	HDC&&			AllocationCompatibleDC();
	const HDC&	GetMemDC() const;
	const HDC&	GetBlendDC() const;


private:
	HDC				m_hMemDC;		//Main DC에 그릴 내용을 담고있는 DC
	HDC				m_hBlendDC;		//AlphaBlending용 DC
	HBITMAP		m_hBit;			//Mem DC와 연결할 Bitmap 핸들
	HBITMAP		m_hOldBit;


};