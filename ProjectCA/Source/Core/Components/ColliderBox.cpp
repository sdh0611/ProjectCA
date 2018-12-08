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

bool ColliderBox::PostInit(CObject* pOwner, const Types::tstring& strTag)
{

	//m_ColliderPoint = point;
	if (!Collider::PostInit(pOwner, strTag))
		return false;

	//처음 Init할 때 기본값으로 Actor의 너비, 높이를 따라가도록 함.
	m_fWidth					= m_fCurWidth = m_pOwner->GetObjectWidth();
	m_fHeight					= m_fCurHeight =m_pOwner->GetObjectHeight();

	m_CurColliderPoint		= m_pOwner->GetObjectPosition();

	m_ColliderRect.left			= m_CurColliderPoint.x - m_fCurWidth / 2;
	m_ColliderRect.top		= m_CurColliderPoint.y - m_fCurHeight;
	m_ColliderRect.right		= m_CurColliderPoint.x + m_fCurWidth / 2;
	m_ColliderRect.bottom	= m_CurColliderPoint.y;

	m_bCollision				= false;
	m_bUseTrigger				= false;

	return true;
}

void ColliderBox::Init()
{
	m_CurColliderPoint = m_pOwner->GetObjectPosition();
	m_fCurWidth = m_fWidth;
	m_fCurHeight = m_fHeight;
	
	m_ColliderRect.left = m_ColliderPoint.x - m_fWidth * 0.5f;
	m_ColliderRect.top = m_ColliderPoint.y - m_fHeight;
	m_ColliderRect.right = m_ColliderPoint.x + m_fWidth * 0.5f;
	m_ColliderRect.bottom = m_ColliderPoint.y;
	m_bActive = true;
}


//물체가 움직임에 따라 CollisionBox의 좌표도 같이 이동해야함.
void ColliderBox::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		float fPivotWidthRatio = m_pOwner->GetComponent<TransformComponent>().lock()->GetPivotWidthRatio();
		float fPivotHeightRatio = m_pOwner->GetComponent<TransformComponent>().lock()->GetPivotHeightRatio();

		m_CurColliderPoint = m_pOwner->GetObjectPosition();

		m_ColliderRect.left = m_CurColliderPoint.x - m_fCurWidth * fPivotWidthRatio;
		m_ColliderRect.top = m_CurColliderPoint.y - m_fCurHeight * fPivotHeightRatio;
		m_ColliderRect.right = m_CurColliderPoint.x + m_fCurWidth * fPivotWidthRatio;
		m_ColliderRect.bottom = m_CurColliderPoint.y;
	}
}

void ColliderBox::LateUpdate()
{
	if (m_bActive)
	{
		float fPivotWidthRatio = m_pOwner->GetComponent<TransformComponent>().lock()->GetPivotWidthRatio();
		float fPivotHeightRatio = m_pOwner->GetComponent<TransformComponent>().lock()->GetPivotHeightRatio();

		m_CurColliderPoint = m_pOwner->GetObjectPosition();

		m_ColliderRect.left = m_CurColliderPoint.x - m_fCurWidth * fPivotWidthRatio;
		m_ColliderRect.top = m_CurColliderPoint.y - m_fCurHeight * fPivotHeightRatio;
		m_ColliderRect.right = m_CurColliderPoint.x + m_fCurWidth * fPivotWidthRatio;
		m_ColliderRect.bottom = m_CurColliderPoint.y;
	}
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

void ColliderBox::SetCurRectWidth(float fWidth)
{
	if (fWidth < 0.f)
		return;

	m_fCurWidth = fWidth;
}

void ColliderBox::SetCurRectHeight(float fHeight)
{
	if (fHeight < 0.f)
		return;

	m_fCurHeight = fHeight;
}

void ColliderBox::SetRectSize(float fWidth, float fHeight)
{
	if (fWidth < 0.f || fHeight < 0.f)
		return;

	m_fCurWidth	= fWidth;
	m_fCurHeight	= fHeight;

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

float ColliderBox::GetCurRectWidth() const
{
	return m_fCurWidth;
}

float ColliderBox::GetCurRectHeight() const
{
	return m_fCurHeight;
}

const Types::Rect & ColliderBox::GetRect() const
{
	return m_ColliderRect;
}
