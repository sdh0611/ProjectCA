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


private:
	typedef std::vector<std::weak_ptr<class CSprite>> NumberFontList;
	std::vector<NumberFontList	>	m_NumberFontList;
	int										m_iFontWidth;
	int										m_iDigit;
	const int* 							m_pValue;
	FontType								m_FontType;

};