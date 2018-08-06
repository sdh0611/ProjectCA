#pragma once

#include "CObject.h"
#include "..\..\..\stdafx.h"


class CProbs :public CObject {
public:
	CProbs();
	virtual ~CProbs();


public:
	virtual bool Init(Types::Point point, UINT iWidth, UINT iHeight,
		Types::ObjectState state = Types::OS_IDLE, Types::Direction dir = Types::DIR_IDLE) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(const HDC& hDC) override;
	//virtual void LateUpdate(float fDeltaTime) override; 


public:
	UINT GetProbsWidth() const { return m_iObjectWidth; }
	void SetProbsWidth(UINT width) { if(width > 0) m_iObjectWidth = width; }
	UINT GetProbsHeight() const { return m_iObjectHeight; }
	void SetProbsHeight(UINT height) { if(height > 0) m_iObjectHeight = height; }
	//void SetProbsSize(UINT width, UINT height){ }


private:



private:
	//Types::Rect	m_ProbSize;


};