#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CPipe.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\CTile.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\CGameScene.h"
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
	UINT& iWidth = const_cast<UINT&>(data.m_iEntityWidth);
	UINT& iHeight = const_cast<UINT&>(data.m_iEntityHeight);

	iWidth = data.m_iEntityWidth * PIPE_UNIT_WIDTH;
	iHeight = data.m_iEntityHeight * PIPE_UNIT_HEIGHT;

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
		m_PipeType = PIPE_TOP;
		GeneratePipe(data.m_iEntityHeight);

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
		auto pRender			= GetComponent<ImageRender>().lock();
		POSITION position	= GetTransform().lock()->GetScreenPivot();

		//Rectangle(hDC, position.x, position.y, position.x + m_iEntityWidth, position.y + m_iEntityHeight);

		size_t size = m_PipeImageList.size();
		position.y -= PIPE_UNIT_HEIGHT;
		for (auto it = m_PipeImageList.cbegin(); it != m_PipeImageList.cend(); ++it)
		{
			pRender->Draw(hDC, position, it->lock());
			position.y += PIPE_UNIT_HEIGHT;
		}
	}

}

void CPipe::LateUpdate()
{
	CObject::LateUpdate();

	UINT cameraWidth = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraWidth();
	UINT cameraHeight = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraHeight();
	POSITION cameraPosition = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraPosition();
	POSITION position = GetObjectPosition();

	if (IsActive())
	{
		if (position.x <  cameraPosition.x - cameraWidth
			|| position.x > cameraPosition.x + 2 * cameraWidth)
		{
			puts("InActive");
			SetActive(false);
			return;
		}
		else if (position.y < cameraPosition.y - cameraHeight
			|| position.y > cameraPosition.y + 2 * cameraHeight)
		{
			puts("InActive");
			SetActive(false);
			SetObjectState(Types::OS_DEAD);
			return;
		}
	}
	else
	{
		auto pPlayer = static_cast<CGameScene*>(m_pOwnerScene)->GetPlayerPtr().lock();
		POSITION playerPosition = pPlayer->GetObjectPosition();

		if (position.x > cameraPosition.x - cameraWidth
			&& position.x < cameraPosition.x + 2 * cameraWidth)
		{
			puts("Active");
			Init();
			SetActive(true);
		}
	}

}

void CPipe::SetPipeSize(int iSize)
{
	if (iSize < 1)
		return;

	m_iEntityHeight = SPRITE_HEIGHT * 0.5f * iSize;
}

void CPipe::GeneratePipe(UINT iHeight)
{
	auto pObjMgr = CResourceManager::GetInstance();
	m_PipeImageList.emplace_back(pObjMgr->GetWeakSprtiePtr(TEXT("PipeHead")));

	for (int i = 0; i < (iHeight / PIPE_UNIT_HEIGHT)-1; ++i)
	{
		m_PipeImageList.emplace_back(pObjMgr->GetWeakSprtiePtr(TEXT("PipeBody")));
	}

}
