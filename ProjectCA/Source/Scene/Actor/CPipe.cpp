#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPipe.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\CTile.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"





CPipe::CPipe()
{
}

CPipe::~CPipe()
{
}

bool CPipe::PostInit(const OBJECT_DATA & data, CScene * pScene)
{
	if (!CProb::PostInit(data, pScene))
		return false;

	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;

	pRender->SetExpansionRatio(2.5f);
	//pRender->SetPivotRatio(0.5f, 1.f);
	if(!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	//Pipe image 정보 초기화
	//기본은 3단계 높이의 파이프
	{
		auto pObjMgr = CResourceManager::GetInstance();
		m_PipeImageList.emplace_back(pObjMgr->GetWeakSprtiePtr(TEXT("PipeHead")));

		for (int i = 0; i < 2; ++i)
		{
			m_PipeImageList.emplace_back(pObjMgr->GetWeakSprtiePtr(TEXT("PipeBody")));
		}

	}

	return true;
}

void CPipe::Init()
{
	CProb::Init();
}

void CPipe::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CProb::Update(dDeltaTime);
	}
}

void CPipe::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		auto pRender = GetComponent<ImageRender>().lock();
		auto position = GetTransform().lock()->GetScreenPivot();

		//Rectangle(hDC, position.x, position.y, position.x + m_iObjectWidth, position.y + m_iObjectHeight);

		size_t size = m_PipeImageList.size();
		position.y -= (m_iObjectHeight / size);
		for (auto it = m_PipeImageList.cbegin(); it != m_PipeImageList.cend(); ++it)
		{
			pRender->Draw(hDC, position, it->lock());
			position.y += (m_iObjectHeight / size);
		}
	}

}

void CPipe::SetPipeSize(int iSize)
{
	if (iSize < 1)
		return;

	m_iObjectHeight = SPRITE_HEIGHT * 0.5f * iSize;
}
