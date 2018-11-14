#pragma once

#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CScene.h"



class CTitleScene : public CScene 
{

public:
	CTitleScene();
	virtual ~CTitleScene();


public:
	virtual bool Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


private:




private:



};