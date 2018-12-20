#pragma once


#include "..\..\..\stdafx.h"
#include "ComponentBase.h"


class RenderComponent : public ComponentBase {

public:
	RenderComponent();
	virtual ~RenderComponent();


public:
	virtual bool	PostInit(CEntity* pOwner, const TSTRING& strTag = TEXT("RenderComponent"));
	virtual void	Init() override;
	virtual void	Update(double dDeltaTime) override;
	virtual void	LateUpdate() override;
	virtual void	Draw(const HDC& hDC) = 0;


public:
	void SetDrawWidth(UINT iWidth);
	void SetDrawHeight(UINT iHeight);
	void SetDrawWidthRatio(float fRatio);
	void SetDrawHeightRatio(float fRatio);
	void SetDrawSize(UINT iWidth, UINT iHeight);
	void SetExpansionRatio(float fRatio);
	void SetWidthPivotRatio(float fRatio);
	void SetHeightPivotRatio(float fRatio);
	void SetPivotRatio(float fWidthRatio, float fHeightRatio);
	void MovePivot(float fx, float fy);


public:
	const POSITION& GetDrawPivot() const;


protected:
	HDC			m_hRenderDC;		//Backbuffer의 m_hMemDC에 대한 Compatible DC
	UINT			m_iDrawWidth;
	UINT			m_iDrawHeight;
	float			m_fWidthExpansionRatio;
	float			m_fHeightExpansionRatio;
	float			m_fWidthPivotRatio;
	float			m_fHeightPivotRatio;
	POSITION	m_DrawPivot;		


private:


};