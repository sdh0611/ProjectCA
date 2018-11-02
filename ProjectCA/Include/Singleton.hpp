#pragma once

/*
	NOTE:
		Phoenix Singleton을 Template Class형태로 구현한 파일.
		Singleton으로 작성하고 싶은 Class는 이 Class를 상속받고,
		DECLARE_SINGLETON 매크로를 선언해준 뒤 작성하면됨.
*/

#include "..\stdafx.h"

//Singleton 선언 매크로. Singleton Template을 
//상속받은 class의 friend로 지정해줌.
#define DECLARE_SINGLETON(CLASS) \
		friend class Singleton<CLASS>;\
		private:\
			CLASS();\
			CLASS(const CLASS&);\
			~CLASS();


//Singleton 클래스들을 정의하기 위한 Singleton Template
template <class T>
class Singleton {
public:
	static T* GetInstance() {
		if (m_pInstance == nullptr)
			m_pInstance = new T;

		return m_pInstance;
	}
	static void Destroy() {
		if (m_pInstance != nullptr) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


protected:
	Singleton() { };
	virtual ~Singleton() { };


private:
	static T* m_pInstance;
	
};

template <class T> T* Singleton<T>::m_pInstance = nullptr;