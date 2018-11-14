#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CTitleScene.h"
#include "..\..\Include\Core\CInputManager.h"

CTitleScene::CTitleScene()
	:CScene(Types::ST_TITLE)
{
}

CTitleScene::~CTitleScene()
{
}

bool CTitleScene::Init()
{
	return false;
}

void CTitleScene::Update(double dDeltaTime)
{
	CScene::Update(dDeltaTime);
}

void CTitleScene::Render(const HDC & hDC)
{
	CScene::Render(hDC);
}
