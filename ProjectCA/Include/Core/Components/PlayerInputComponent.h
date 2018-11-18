#pragma once

#include "..\..\..\stdafx.h"
#include "InputComponent.h"

//TODO :	 InputManager와 연동해서 조작키 변경할 수 있게끔 만들자.



class PlayerInputComponent : public InputComponent {

public:
	PlayerInputComponent() { }
	virtual ~PlayerInputComponent();

	
public:
	virtual bool PostInit(CObject* pOwner, const Types::tstring& strTag = TEXT("InputComponent")) override;
	virtual void Init();
	virtual void Update(double fDeltaTime) override;
	virtual void LateUpdate(double dDeltaTime) override;


private:
	//void UpdateKeyDown();
	void KeyProcess();


private:
	class CInputManager*		m_pInputManager;

};