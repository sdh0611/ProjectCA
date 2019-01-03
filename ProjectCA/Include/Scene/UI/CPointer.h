#pragma once

#include "..\..\..\stdafx.h"
#include "CUI.h"

class CButton;

class CPointer : public CUI
{
public:
	CPointer();
	virtual ~CPointer();


public:
	virtual void Update(double dDeltaTime) override;


public:
	void SetCurFocusButton(std::shared_ptr<CButton> pButton);


private:
	void Behavior(double dDelatTime);


private:
	std::weak_ptr<CButton> m_pCurFocusButton;


};