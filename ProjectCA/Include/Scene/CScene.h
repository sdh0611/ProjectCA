#pragma once

/*
	NOTE:
		게임 내 모든 Scene들의 기본 동작들을 구현해놓은 Class.
		Layer객체들을 List로 관리함.
		Scene의 구체적인 동작은 이 Class를 상속한 후 따로 구현해야함.
*/

//TODO(09.15) : Layer 개념 없애버리고 일단 빌드부터 되게끔 만들자.
#include "..\..\stdafx.h"

class CLayer;

class CScene {

public:
	CScene(Types::SceneType);
	virtual ~CScene();


public:
	virtual bool Init();
	virtual void Update(double deltaTime);
	virtual void Render(const HDC& hDC);


public:
	bool CreateLayer(const Types::tstring& tag, int order);	//Layer생성이 가능한 유일한 메소드
	bool DeleteLayer(const Types::tstring& tag);
	CLayer* FindLayer(const Types::tstring& tag);


private:
	static bool CompareLayer(CLayer* first, CLayer* second);
	void SortLayer() { m_LayerList.sort(CompareLayer); }


protected:
	Types::SceneType					m_SceneType;
	std::list<CLayer*>					m_LayerList;
	std::list<CLayer*>::iterator		m_it;


};
