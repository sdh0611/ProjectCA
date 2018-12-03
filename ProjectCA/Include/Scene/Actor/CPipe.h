#pragma once


#include "..\..\..\stdafx.h"
#include "CProb.h"


class CPipe : public CProb
{
	enum PipeInfo {
		PIPE_HEAD = 0, PIPE_BODY
	};


public:
	CPipe();
	virtual ~CPipe();


public:
	virtual bool PostInit(const OBJECT_DATA&, CScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


public:
	void SetPipeSize(int iSize);



private:
	typedef std::vector<std::weak_ptr<class CSprite>> PipeImageList;
	PipeImageList m_PipeImageList;

};