#pragma once

#include "..\..\..\stdafx.h"
#include "CInterface.h"


class CNumberInterface : public CInterface
{
public:
	enum FontType {
		FONT_WHITE = 0, FONT_YELLOW
	};

public:
	CNumberInterface();
	virtual ~CNumberInterface();


public:
	virtual bool PostInit(const OBJECT_DATA& data, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


public:
	void LinkValuePtr(int* pValue);
	void SetDigit(int iDigit);
	void SetFontType(FontType type);
	void SetFontInterval(UINT iInteval);
	void SetFontWidth(UINT iWidth);
	void SetFontHeight(UINT iHeight);
	void SetFontSize(UINT iWidth, UINT iHeight);


private:
	int										m_iDigit;
	UINT									m_iFontInterval;
	UINT									m_iFontWidth;
	UINT									m_iFontHeight;
	const int* 							m_pValue;
	FontType								m_FontType;
	typedef std::vector<std::weak_ptr<class CSprite>> NumberFontList;
	std::vector<NumberFontList	>	m_NumberFontList;


};