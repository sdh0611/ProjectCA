#pragma once

#include "..\..\..\stdafx.h"
#include "CInterface.h"


class CNumberInterface : public CInterface
{
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

private:
	typedef std::vector<std::weak_ptr<class CSprite>> NumberFontList;
	NumberFontList	m_NumberFontList;
	int						m_iFontWidth;
	int						m_iDigit;
	int*					m_pValue;

};