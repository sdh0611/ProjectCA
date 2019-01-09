#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CObjectManager.hpp"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CScene.h"
#include "..\..\..\Include\Scene\CLayer.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\PlayerInputComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"



CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager() 
{	
}

bool CObjectManager::Init() 
{		
	m_LastEntityID = 0;

	return true;
}

void CObjectManager::Clear()
{
	m_LastEntityID = 0;
}
