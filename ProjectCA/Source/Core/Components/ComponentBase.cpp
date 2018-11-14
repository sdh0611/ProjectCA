#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"

void ComponentBase::Destroy()
{
	//Bug!!
	m_pOwner.reset();
}
