#pragma once

#include "..\..\..\stdafx.h"
#include "CProp.h"

//NOTE: Pipe를 구현한 Class
//			생성할 때 Width, Height값은 타일의 개수에 대한 값이어야 함.

class CPipe : public CProp
{
	const int PIPE_UNIT_WIDTH		= SPRITE_WIDTH * 2.5f;
	const int PIPE_UNIT_HEIGHT	= SPRITE_HEIGHT * 0.5f * 2.5f;

	//타일 값
	enum PipeInfo {
		PIPE_HEAD = 0, PIPE_BODY
	};

public:
	//방향은 Pipe Head가 나와있는 방향 기준
	enum PipeType {
		PIPE_RIGHT, PIPE_LEFT, PIPE_TOP, PIPE_BOT
	};


public:
	CPipe();
	virtual ~CPipe();


public:
	virtual bool PostInit(const OBJECT_DATA&, CScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void LateUpdate() override;


public:
	void SetPipeSize(int iSize);


private:
	void GeneratePipe(UINT iHeight);


private:
	PipeType			m_PipeType;
	typedef std::vector<std::weak_ptr<class CSprite>> PipeImageList;
	PipeImageList	m_PipeImageList;


};