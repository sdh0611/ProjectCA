#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"



ColliderBox::ColliderBox()
	:Collider(CT_BOX), m_ColliderRect(0, 0, 0, 0)
{
}

ColliderBox::~ColliderBox()
{
}

bool ColliderBox::PostInit(CActor* pOwner, const Types::tstring& strTag)
{
	auto pActor = std::shared_ptr<CActor>(pOwner);

	//m_ColliderPoint = point;
	m_pOwner					= pActor;
	
	//처음 Init할 때 기본값으로 Actor의 너비, 높이를 따라가도록 함.
	m_fWidth					= m_fCurWidth = m_pOwner->GetActorWidth();
	m_fHeight					= m_fCurHeight =m_pOwner->GetActorHeight();

	m_ColliderPoint			= m_CurColliderPoint = m_pOwner->GetActorPoint();

	m_ColliderRect.left			= m_CurColliderPoint.x - m_fWidth / 2;
	m_ColliderRect.top		= m_CurColliderPoint.y - m_fHeight;
	m_ColliderRect.right		= m_CurColliderPoint.x + m_fWidth / 2;
	m_ColliderRect.bottom	= m_CurColliderPoint.y;

	m_bCollision				= false;
	m_bUseTrigger				= false;
	m_strComponentTag		= strTag;

	return true;
}

void ColliderBox::Init()
{
	m_CurColliderPoint = m_ColliderPoint;
	m_fCurWidth = m_fWidth;
	m_fCurHeight = m_fCurHeight;
	
	m_ColliderRect.left = m_ColliderPoint.x - m_fWidth / 2;
	m_ColliderRect.top = m_ColliderPoint.y - m_fHeight;
	m_ColliderRect.right = m_ColliderPoint.x + m_fWidth / 2;
	m_ColliderRect.bottom = m_ColliderPoint.y;

}


//물체가 움직임에 따라 CollisionBox의 좌표도 같이 이동해야함.
void ColliderBox::Update(double dDeltaTime)
{

	float fPivotWidthRatio	= m_pOwner->GetComponent<TransformComponent>()->GetPivotWidthRatio();
	float fPivotHeightRatio	= m_pOwner->GetComponent<TransformComponent>()->GetPivotHeightRatio();

	m_CurColliderPoint		= m_pOwner->GetActorPoint();

	m_fCurHeight = m_fHeight;
	if (m_pOwner->GetActorState() == Types::AS_SITDOWN)
	{
		m_fCurHeight /= 2.f;
	}
	m_ColliderRect.left			= m_CurColliderPoint.x - m_fCurWidth * fPivotWidthRatio;
	m_ColliderRect.top		= m_CurColliderPoint.y - m_fCurHeight * fPivotHeightRatio;
	m_ColliderRect.right		= m_CurColliderPoint.x + m_fCurWidth * fPivotWidthRatio;
	m_ColliderRect.bottom	= m_CurColliderPoint.y;
		
}

void ColliderBox::LateUpdate(double dDeltaTime)
{
}

void ColliderBox::DrawCollider(const HDC & hDC, const POSITION& position)
{
	Rectangle(hDC, position.x - m_fCurWidth/2.f, position.y - m_fCurHeight, position.x + m_fCurWidth/2.f, position.y );

}

void ColliderBox::SetWidth(float fWidth)
{
	if (fWidth < 0.f)
		return;

	m_fWidth = m_fCurWidth = fWidth;

}

void ColliderBox::SetHeight(float fHeight)
{
	if (fHeight < 0.f)
		return;

	m_fHeight = m_fCurHeight = fHeight;

}

void ColliderBox::SetSize(float fWidth, float fHeight)
{
	if (fWidth < 0.f || fHeight < 0.f)
		return;

	m_fWidth	= m_fCurWidth =fWidth;
	m_fHeight	= m_fCurHeight =fHeight;
}

void ColliderBox::SetRect(float left, float top, float right, float bottom)
{
	m_ColliderRect.left			= left;
	m_ColliderRect.top		= top;
	m_ColliderRect.right		= right;
	m_ColliderRect.bottom	= bottom;
}

float ColliderBox::GetWidth() const
{
	return m_fWidth;
}

float ColliderBox::GetHeight() const
{
	return m_fHeight;
}

const Types::Rect & ColliderBox::GetRect() const
{
	return m_ColliderRect;
}
