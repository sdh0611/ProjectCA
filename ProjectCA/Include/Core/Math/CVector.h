#pragma once

	/*
		NOTE:
			힘의 방향을 나타내기 위해 사용하는 Vector 클래스
	
	*/

#include "..\..\..\stdafx.h"

namespace CVector {

	class Vector2d {
	
	public:
		Vector2d(float fx = 0.f, float fy = 0.f);
		~Vector2d();


	public:
		Vector2d operator +(const Vector2d& other);
		Vector2d operator - (const Vector2d& other);
		//Vector2d operator * (const Vector2d& other);
		Vector2d& operator = (const Vector2d& other);
		bool operator == (const Vector2d& other);


	public:
		void SetVector(float fx, float fy);
		void Normalize();
		float GetVectorScale();
		Vector2d GetNormalVector();


	public:
		Types::Point GetVector() { return Types::Point(m_fX, m_fY); }
		
		
	public:
		static float DotProduct(const Vector2d&, const Vector2d&);	//두 벡터 좌표의 곱
		static float DotProduct(float, float, float);							//두 벡터의 크기, cos의 곱
		//static float CrossProduct(const Vector2d&, const Vector2d&);


	private:
		


	private:
		float m_fX;
		float m_fY;

	};


	class Vector3d {


	};



}