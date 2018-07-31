#pragma once

#include "ColliderBox.h"



class TriggerBox : public ColliderBox {

public:
	TriggerBox(CObject* pOwner, float fLeft, float fTop, float fRight, float fBottom);
	TriggerBox(CObject* pOwner, const Types::Rect& rect);
	virtual ~TriggerBox();


public:




private:



private:
	


};