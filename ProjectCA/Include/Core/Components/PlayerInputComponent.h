#pragma once

#include "..\..\..\stdafx.h"
#include "InputComponent.h"

//TODO :	 InputManager와 연동해서 조작키 변경할 수 있게끔 만들자.


class PlayerInputComponent : public InputComponent {

public:
	PlayerInputComponent() { }
	virtual ~PlayerInputComponent();

	
public:
	virtual bool Init(CActor* pOwner, const Types::tstring& strTag = TEXT("InputComponent")) override;
	virtual void Update(double fDeltaTime) override;


private:
	void KeyProcess();
	void JumpKeyProcess();


};