#pragma once

#include "..\..\stdafx.h"
#include "CScene.h"



class CGameOverScene : public CScene
{
public:
	CGameOverScene();
	virtual ~CGameOverScene();


public:
	virtual bool Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);





};