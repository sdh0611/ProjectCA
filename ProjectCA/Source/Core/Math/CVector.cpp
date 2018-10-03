#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Math\CVector.h"

namespace CVector {

	Vector2d::Vector2d(float fx, float fy)
		:m_fX(fx), m_fY(fy)
	{

	}

	Vector2d::~Vector2d()
	{

	}

	Vector2d Vector2d::operator+(const Vector2d & other)
	{
		m_fX += other.m_fX;
		m_fY += other.m_fY;

		return Vector2d(m_fX, m_fY);
	}

	Vector2d Vector2d::operator-(const Vector2d & other)
	{
		m_fX -= other.m_fX;
		m_fY -= other.m_fY;

		return Vector2d(m_fX, m_fY);
	}

	//Vector2d Vector2d::operator*(const Vector2d & other)
	//{
	//	m_fX *= other.m_fX;
	//	m_fY *= other.m_fY;

	//	return Vector2d(m_fX, m_fY);
	//}

	Vector2d & Vector2d::operator=(const Vector2d & other)
	{
		if (*this == other)
			return *this;

		this->m_fX = other.m_fX;
		this->m_fY = other.m_fY;

		return *this;
		
	}

	bool Vector2d::operator==(const Vector2d & other)
	{
		if(m_fX != other.m_fX || m_fY != other.m_fY)
			return false;

		return true;
	}

	void Vector2d::SetVector(float fx, float fy)
	{
		m_fX = fx;
		m_fY = fy;
	}

	void Vector2d::Normalize()
	{
		float scale = GetVectorScale();

		m_fX /= scale;
		m_fY /= scale;
	
	}

	float Vector2d::GetVectorScale()
	{
		float sum = m_fX * m_fX + m_fY * m_fY;

		return std::sqrtf(sum);
	}

	Vector2d Vector2d::GetNormalVector()
	{
		Vector2d normVec(*this);

		normVec.Normalize();

		return normVec;	
	}

	float Vector2d::DotProduct(const Vector2d & vector1, const Vector2d & vector2)
	{
		float result = (vector1.m_fX * vector2.m_fX) + (vector1.m_fY * vector2.m_fY);

		return result;
	}

	//float Modular(float value, float divider) {

	//	float result = value;

	//	while (result / divider) {
	//		result -= divider;
	//	}

	//	return result;
	//}

	float Vector2d::DotProduct(float vectorScale1, float vectorScale2, float angle)
	{
		//float radian = Modular(angle, 360);
		return (vectorScale1 * vectorScale2 * cos(angle));
	}

	//float Vector2d::CrossProduct(const Vector2d &, const Vector2d &)
	//{


	//	return 0.0f;
	//}

}