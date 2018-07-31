#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Object\CPlayer.h"
#include "..\..\..\Include\Core\Components\InputComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"



CPlayer::CPlayer(UINT iWidth, UINT iHeight)
	:CObject(Types::OT_PLAYER, Types::Point(0, 200), iWidth, iHeight)
{
	
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	//컴포넌트들 추가
	AddComponent(TEXT("InputComponent"), new InputComponent(this));
	AddComponent(TEXT("PhysicsComponent"), new PhysicsComponent(this));
	AddComponent(TEXT("Collider"), new ColliderBox(this, m_fObjectPoint.x, m_fObjectPoint.y,
		m_fObjectPoint.x + m_iObjectWidth, m_fObjectPoint.y + m_iObjectHeight));



	//컴포넌트들 초기화
	CObject::Init();

	return true;
}

void CPlayer::Update(float fDeltaTime)
{
	CObject::Update(fDeltaTime);

	//GetComponent(TEXT("Collider"))->Update(fDeltaTime);

}

//테스트용으로 일단 사각형 출력
void CPlayer::Render(const HDC& hDC)
{
	Rectangle(hDC, m_fObjectPoint.x, m_fObjectPoint.y, 
		m_fObjectPoint.x + 64, m_fObjectPoint.y + 64);

	TCHAR temp[1024];
	wsprintf(temp, TEXT("point : (%3ld, %3ld)"), (LONG)m_fObjectPoint.x, (LONG)m_fObjectPoint.y);

	TCHAR isTrue[10];
	ColliderBox* box = static_cast<ColliderBox*>(GetComponent(TEXT("Collider")));
	if (static_cast<Collider*>(GetComponent(TEXT("Collider")))->GetIsCollision())
		lstrcpy(isTrue, TEXT("true  "));
	else
		lstrcpy(isTrue, TEXT("false"));

	TextOut(hDC, 0, 0, temp, lstrlen(temp));
	//Debug::DrawingText(L"point : " + m_fObjectPoint.x, hDC, 0, 0);
	TextOut(hDC, 0, 20, isTrue, lstrlen(isTrue));

	Types::Rect rect = box->GetBoxSize();
	wsprintf(temp, TEXT("Box : (%3ld, %3ld, %3ld, %3ld)"), (LONG)rect.left, (LONG)rect.top, (LONG)rect.right, (LONG)rect.bottom);
	TextOut(hDC, 0, 40, temp, lstrlen(temp));

	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);


}

//void CPlayer::LateUpdate(float fDeltaTime)
//{
//	for (auto& component : m_ComponentTable) {
//		if (component.first == TEXT("InputComponent"))
//			continue;
//		
//		component.second->Update(fDeltaTime);
//	}
//	
//
//}
