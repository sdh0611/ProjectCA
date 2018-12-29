#pragma once

//현재는 1줄단위 입력만 지원.
//나중에 줄바꿈 출력까지 구현할 예정


#include "..\..\..\stdafx.h"
#include "CInterface.h"


class CFont: public CInterface
{

public:
	CFont();
	virtual ~CFont();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


public:
	void ResizeFontContainer();
	void SetFontInterval(UINT iInteval);
	void SetFontWidth(UINT iWidth);
	void SetFontHeight(UINT iHeight);
	void SetFontSize(UINT iWidth, UINT iHeight);
	void SetSentence(const TSTRING& strWord);


private:
	int										m_iFontInterval;
	UINT									m_iFontWidth;
	UINT									m_iFontHeight;
	TSTRING								m_strSentence;
	typedef std::unordered_map<char, std::weak_ptr<class CSprite>> FontTable;
	FontTable							m_FontTable;


};