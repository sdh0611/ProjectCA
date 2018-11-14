#pragma once


#include "..\..\..\stdafx.h"
#include "ComponentBase.h"


class RenderComponent : public ComponentBase {

public:
	RenderComponent();
	virtual ~RenderComponent();


public:
	virtual bool	PostInit(CObject* pOwner, const Types::tstring& strTag = TEXT("RenderComponent"));
	virtual void	Init() override;
	virtual void	Update(double dDeltaTime) override;
	virtual void	LateUpdate(double dDeltaTime) override;
	virtual void	Draw(const HDC& hDC) = 0;


public:
	void SetVisible(bool bVisible);


public:
	bool IsVisible() const;


protected:
	HDC											m_hRenderDC;		//Backbuffer의 m_hMemDC에 대한 Compatible DC


private:
	bool											m_bVisible;

};