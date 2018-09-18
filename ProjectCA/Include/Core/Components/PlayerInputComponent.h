#pragma once

#include "..\..\..\stdafx.h"
#include "InputComponent.h"


class PlayerInputComponent : public InputComponent {

public:
	PlayerInputComponent() { }
	virtual ~PlayerInputComponent();

	
public:
	virtual bool Init(std::shared_ptr<CActor> pOwner, const Types::tstring& strTag = TEXT("InputComponent")) override;
	virtual void Update(float fDeltaTime) override;


private:
	void KeyProcess();


};