#pragma once

#include "..\..\..\stdafx.h"
#include "InputComponent.h"

//TODO :	 InputManager와 연동해서 조작키 변경할 수 있게끔 만들자.



class PlayerInputComponent : public InputComponent {

public:
	PlayerInputComponent() { }
	virtual ~PlayerInputComponent();

	
public:
	virtual bool PostInit(CActor* pOwner, const Types::tstring& strTag = TEXT("InputComponent")) override;
	virtual void Init(){ }
	virtual void Update(double fDeltaTime) override;


public:
	bool GetKeyDown(const Types::tstring& strKeyName);


private:
	void UpdateKeyDown();
	void KeyProcess();


private:
	std::vector<Types::KeyInfo>	m_keyInfoList;

};