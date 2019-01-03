#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CInterface.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"


CInterface::CInterface()
{
}

CInterface::~CInterface()
{
}

void CInterface::LateUpdate()
{
}

bool CInterface::SetImage(const TSTRING & strImageName)
{
	return GetComponent<ImageRender>().lock()->SetSprite(strImageName);
}
