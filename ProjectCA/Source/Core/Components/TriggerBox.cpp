#include "..\..\..\Include\Core\Components\TriggerBox.h"

//이러니까 ColliderType 초기화할 때 문제생김. ColliderBox쪽을 손봐야 할 듯.(18.07.05)
TriggerBox::TriggerBox(CObject* pOwner, float fLeft, float fTop, float fRight, float fBottom)
	:ColliderBox(pOwner, fLeft, fTop, fRight, fBottom)
{
	


}

TriggerBox::TriggerBox(CObject * pOwner, const Types::Rect & rect)
	:ColliderBox(pOwner, rect)
{


}

TriggerBox::~TriggerBox()
{


}
