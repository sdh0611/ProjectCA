#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CFlower.h"

CFlower::CFlower()
{
}

CFlower::~CFlower()
{
}

bool CFlower::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	return true;
}

bool CFlower::Init()
{
	return false;
}

void CFlower::Update(double dDeltaTime)
{
}

void CFlower::Render(const HDC & hDC)
{
}