#pragma once

/*
	NOTE:
		GameScene의 동작 기능들을 구현.
		CScene Class를 상속받음.

*/


#include "../../stdafx.h"
#include "CScene.h"

//NOTE(06.10) : 여러 개의 스테이지들에 대한 활동을 정의하는 Stage Class가 필요하다고 느껴짐.

//Stage들의 부모 클래스 전방 선언.
//추후 구현예정(06.10)
class CStage;

class CGameScene :public CScene {

public:
	CGameScene(Types::SceneType);
	virtual ~CGameScene();


public:
	virtual bool Init() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	

public:
	void CollisionDetect();
	void GameUpdate(float fDeltaTime);

private:
	void ResetScene();		


private:
	std::unique_ptr<class CollisionDetector>	m_pCollisionDetector;
	//std::unique_ptr<CStage>						m_pCurStage;
	//std::unique_ptr<CStage>						m_pNextStage;

};