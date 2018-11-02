#pragma once

#include "..\..\..\stdafx.h"
#include "Collider.h"


class ColliderBox : public Collider {
	
public:
	ColliderBox();
	virtual ~ColliderBox();


public:
	virtual bool PostInit(CActor* owner, const Types::tstring& strTag = TEXT("Collider")) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void LateUpdate(double dDeltaTime) override;


public:
	virtual void DrawCollider(const HDC& hDC, const POSITION& position) override;


public:
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);
	void SetSize(float fWidth, float fHeight);
	void SetRect(float left, float top, float right, float bottom);



public:
	float GetWidth() const;
	float GetHeight() const;
	const Types::Rect& GetRect() const;

private:


	
private:
	float			m_fWidth;
	float			m_fHeight;
	float			m_fCurWidth;
	float			m_fCurHeight;
	Types::Rect	m_ColliderRect;



};