//디버깅용 클래스

#pragma once

#include "..\..\..\stdafx.h"
#include "..\..\Singleton.hpp"


class Debug : Singleton<Debug>{

	DECLARE_SINGLETON(Debug)

public:
	bool Init(UINT iPoolSize);
	bool Resize(UINT iPoolSize);
	void DrawingText(const Types::tstring& text, const HDC& hDC, int x, int y);


public:
	static void MessageInfo(const Types::tstring& text);
	static void MeesageError(const Types::tstring& text);
	

private:
	std::shared_ptr<BYTE>			m_pMemPool;	//화면에 텍스트 출력할 떄 사용할 메모리풀.
	const UINT							m_iMaxPoolSize;	//메모리풀 할당크기 최대치 지정(byte단위)
	const UINT							m_iMaxLength;	//화면에 출력할 텍스트의 최대 길이 지정.(byte단위)


};