#pragma once

#include "..\..\..\stdafx.h"
#include "CInterface.h"

class CButton;

class CMenu : public CInterface
{
public:
	CMenu();
	virtual ~CMenu();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);
	virtual void LateUpdate();


public:
	void AddButton(std::shared_ptr<CButton> pButton);
	void DeleteButton(std::shared_ptr<CButton> pButton);
	void SetFocusPointer(std::shared_ptr<CInterface> pFocus);


public:
	void SetFocusPointerImage(const TSTRING& strImageName);


private:
	void MenuBehavior();



private:
	typedef std::vector<std::shared_ptr<CButton>> ButtonList;
	ButtonList							m_ButtonList;
	std::weak_ptr<CButton>			m_FocusButton;
	std::shared_ptr<CInterface>		m_pFocusPointer;

};