#pragma once

/*
	NOTE:
		Scene들의 생성, 교체, 파괴를 담당하는 Manager Class.
		Rumtime에서 Instance는 2개 이상은 필요 없으므로 Singleton으로 작성 
*/


#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class CScene;

class CSceneManager : public Singleton<CSceneManager> {

	DECLARE_SINGLETON(CSceneManager)

public:
	bool Init(Types::SceneType type);
	void Update(float fDeltaTime);
	void Render(const HDC& hDC);


public:
	bool CreateScene(Types::SceneType type);
	bool CreateNextScene(Types::SceneType type);
	bool ChangeScene();


private:



private:
	CScene *		m_pScene;
	CScene*		m_pNextScene;



};