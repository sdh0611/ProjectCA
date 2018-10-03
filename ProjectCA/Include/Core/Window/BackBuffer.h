#pragma once

#include "..\..\..\stdafx.h"


class BackBuffer {

public:
	BackBuffer(const HDC& hDC);
	BackBuffer();
	~BackBuffer();


public:
	bool DrawSet(const HDC& hDC);
	bool DrawEnd();


public:
	//bool SetLoadBit();
	const HDC& GetMemDC() const { return m_hMemDC; }
	const HDC& GetDrawDC() const { return m_hDrawDC; }


private:
	HDC		m_hMemDC;		//Main DC에 그릴 내용을 담고있는 DC
	HDC		m_hDrawDC;		//Mem DC에 내용을 담기위한 DC
	HBITMAP	m_hBit;			//Mem DC와 연결할 Bitmap 핸들
	HBITMAP	m_hLoadBit;		
	HBITMAP	m_hOldBit;

};