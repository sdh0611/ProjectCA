#pragma once

#include "..\..\..\stdafx.h"
#include "CUI.h"


class CInterface : public CUI
{
public:
	CInterface();
	virtual ~CInterface();


public:
	virtual void LateUpdate();


public:
	bool SetImage(const TSTRING& strImageName);

	
};