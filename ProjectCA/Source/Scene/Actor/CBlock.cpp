#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\CObject.h"
#include "..\..\..\Include\Scene\Actor\CBlock.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"


CBlock::CBlock()
{
}

CBlock::~CBlock()
{
}

bool CBlock::PostInit(const OBJECT_DATA & objectData, CScene * pScene)
{
	if (!CObject::PostInit(objectData, pScene))
		return false;

	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	//pRender->AddAnimation(0.2f, TEXT("Default"),  );


	return false;
}

void CBlock::Init()
{
	CObject::Init();

}

void CBlock::Update(double dDeltaTime)
{
	CObject::Update(dDeltaTime);
}

void CBlock::Render(const HDC & hDC)
{

}

void CBlock::LateUpdate()
{
}
