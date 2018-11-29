#pragma once

#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CInterface.h"

using Callback = std::function<void(void)>;


class CButton : public CInterface
{
public:
	CButton();
	virtual ~CButton();


public:
	virtual bool PostInit(const OBJECT_DATA& objectData, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


public:
	void SetOnClickCallback(Callback callback);


private:
	bool IsClickOnButton();


private:
	Callback	m_OnClick;


};