#pragma once

#include "..\..\..\stdafx.h"
#include "CUI.h"

class CButton;
class CInterface;

class CMenu : public CUI
{
public:
	CMenu();
	virtual ~CMenu();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene);
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);
	virtual void LateUpdate();


public:
	void AddButton(std::shared_ptr<CButton> pButton);
	void DeleteButton(std::shared_ptr<CButton> pButton);
	void SetFocusPointer(std::shared_ptr<CInterface> pFocus);
	void SetInterval(float fInterval);


public:
	void SetFocusPointerImage(const TSTRING& strImageName);


private:
	void MenuBehavior();
	void LiningButton();


private:
	typedef std::vector<std::shared_ptr<CButton>> ButtonList;
	ButtonList							m_ButtonList;
	std::weak_ptr<CButton>			m_FocusButton;
	std::shared_ptr<CInterface>		m_pFocusPointer;
	int										m_iCurFocusButtonIndex;
	int										m_iNumberOfButton;
	float									m_fInterval;

};